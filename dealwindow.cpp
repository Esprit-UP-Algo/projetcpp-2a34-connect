#include "dealwindow.h"
#include "../../Atelier_Connexion/Atelier_Connexion/connection.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QPageSize>
#include <QDateTime>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QHeaderView>
#include <QSet>

DealWindow::DealWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DealWindow)
    , deliveryTimer(nullptr)
{
    ui->setupUi(this);

    ui->searchName_deal->setPlaceholderText("Put your name here...");

    ui->dateTimeEdit_deal->setDisplayFormat("dd/MM/yyyy hh:mm");
    ui->dateTimeEdit_deal->setCalendarPopup(true);
    ui->dateTimeEdit_deal->setDateTime(QDateTime::currentDateTime());

    if (!Connection::getInstance()->open()) {
        QMessageBox::critical(this, "DB Error", "Cannot connect to Oracle!");
        return;
    }

    stock["Camera"]        = 5;
    stock["Microphone"]    = 8;
    stock["Studio Light"]  = 10;
    stock["Cameraman"]     = 3;
    stock["Video Editor"]  = 2;
    stock["Studio"]        = 1;
    ordered.clear();

    ui->tableStats_deal->setRowCount(6);
    ui->tableStats_deal->setColumnCount(4);
    ui->tableStats_deal->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { background-color: #6a1b9a; color: white; font-weight: bold; }"
        );
    ui->tableStats_deal->setHorizontalHeaderLabels({"Item", "Total", "Used", "Usage"});

    QStringList items = {"Camera", "Microphone", "Studio Light", "Cameraman", "Video Editor", "Studio"};
    for (int i = 0; i < items.size(); ++i) {
        ui->tableStats_deal->setItem(i, 0, new QTableWidgetItem(items[i]));
        ui->tableStats_deal->setItem(i, 1, new QTableWidgetItem("0"));
        ui->tableStats_deal->setItem(i, 2, new QTableWidgetItem("0"));
        ui->tableStats_deal->setItem(i, 3, new QTableWidgetItem("0%"));
        for (int c = 0; c < 4; ++c)
            ui->tableStats_deal->item(i, c)->setFlags(Qt::ItemIsEnabled);
    }
    ui->tableStats_deal->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->lblWarning_deal->setStyleSheet("color:white;background:red;font-weight:bold;padding:10px;");
    ui->lblWarning_deal->setVisible(false);

    deliveryTimer = new QTimer(this);
    connect(deliveryTimer, &QTimer::timeout, this, &DealWindow::deliverPendingOrders);
    deliveryTimer->start(20000);

    QTimer *checkTimer = new QTimer(this);
    connect(checkTimer, &QTimer::timeout, this, &DealWindow::checkAutoOrder);
    checkTimer->start(5000);

    QTimer::singleShot(0, this, [this](){ updateMonthlyStats(); refreshTable(); });
}

DealWindow::~DealWindow()
{
    delete ui;
}

QDateTime DealWindow::findNextAvailableSlot(const QDateTime &requested, int requiredMask)
{
    QDateTime candidate = requested;
    QDateTime dayEnd = candidate.date().startOfDay().addSecs(20*3600 + 59*60);

    while (candidate <= dayEnd) {
        QSqlQuery q(*(Connection::getInstance()->database()));
        q.prepare("SELECT equip_mask FROM EQUIPMENT WHERE rent_date >= :start AND rent_date < :end");
        q.bindValue(":start", candidate);
        q.bindValue(":end", candidate.addSecs(3600));
        q.exec();

        bool conflict = false;
        while (q.next()) {
            if ((q.value(0).toInt() & requiredMask) != 0) {
                conflict = true;
                break;
            }
        }
        if (!conflict) return candidate;
        candidate = candidate.addSecs(1800);
    }

    QDateTime tomorrow = requested.addDays(1);
    tomorrow.setTime(QTime(8, 0));
    return tomorrow;
}

void DealWindow::on_btnBook_deal_clicked()
{
    QString name = ui->lineEditName_deal->text().trimmed();
    QDateTime requestedTime = ui->dateTimeEdit_deal->dateTime();
    int mask = getEquipMask();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name is required!");
        return;
    }
    if (requestedTime.time() < QTime(8,0) || requestedTime.time() > QTime(20,0)) {
        QMessageBox::critical(this, "Time Error", "Booking only allowed 08:00–20:00!");
        return;
    }
    if (mask == 0) {
        QMessageBox::warning(this, "Error", "Select at least one equipment!");
        return;
    }

    QSqlQuery q(*(Connection::getInstance()->database()));
    q.prepare("SELECT equip_mask FROM EQUIPMENT WHERE rent_date >= :start AND rent_date < :end");
    q.bindValue(":start", requestedTime);
    q.bindValue(":end", requestedTime.addSecs(3600));
    q.exec();

    int existingMask = 0;
    while (q.next()) {
        existingMask |= q.value(0).toInt();
    }

    if ((existingMask & mask) != 0) {
        QDateTime suggestion = findNextAvailableSlot(requestedTime, mask);
        QString suggestText = suggestion.date() == requestedTime.date()
                                  ? suggestion.toString("hh:mm")
                                  : "Tomorrow at " + suggestion.toString("hh:mm");

        QStringList conflicting;
        if ((existingMask & 1) && (mask & 1)) conflicting << "Camera";
        if ((existingMask & 2) && (mask & 2)) conflicting << "Microphone";
        if ((existingMask & 4) && (mask & 4)) conflicting << "Studio Light";
        if ((existingMask & 8) && (mask & 8)) conflicting << "Cameraman";
        if ((existingMask & 16) && (mask & 16)) conflicting << "Video Editor";
        if ((existingMask & 32) && (mask & 32)) conflicting << "Studio";

        int reply = QMessageBox::question(this, "Conflict Detected!",
                                          QString("<b>Conflict!</b><br><br>"
                                                  "These items are already booked at <b>%1</b>:<br>"
                                                  "• <font color='red'>%2</font><br><br>"
                                                  "<b>Next available time: %3</b><br><br>"
                                                  "Change booking to suggested time?")
                                              .arg(requestedTime.toString("hh:mm"), conflicting.join(", "), suggestText),
                                          QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            ui->dateTimeEdit_deal->setDateTime(suggestion);
            QMessageBox::information(this, "Time Updated", "Booking time changed to " + suggestText);
        }
        return;
    }

    QSqlQuery insertQ(*(Connection::getInstance()->database()));
    bool isUpdate = false;
    int id = ui->lineEditID_deal->text().toInt(&isUpdate);

    if (isUpdate && id > 0) {
        insertQ.prepare("UPDATE EQUIPMENT SET name=:n, rent_date=TO_DATE(:d,'DD/MM/YYYY HH24:MI:SS'), equip_mask=:m WHERE id=:id");
        insertQ.bindValue(":id", id);
    } else {
        insertQ.prepare("INSERT INTO EQUIPMENT (name, rent_date, equip_mask) VALUES (:n, TO_DATE(:d,'DD/MM/YYYY HH24:MI:SS'), :m)");
    }
    insertQ.bindValue(":n", name);
    insertQ.bindValue(":d", requestedTime.toString("dd/MM/yyyy hh:mm:ss"));
    insertQ.bindValue(":m", mask);

    if (insertQ.exec()) {
        refreshTable();
        ui->lineEditID_deal->clear();
        ui->lineEditName_deal->clear();
        ui->checkCamera_deal->setChecked(false);
        ui->checkMic_deal->setChecked(false);
        ui->checkLight_deal->setChecked(false);
        ui->checkCameraman_deal->setChecked(false);
        ui->checkEditor_deal->setChecked(false);
        ui->checkStudio_deal->setChecked(false);
        QMessageBox::information(this, "Success", "Booking confirmed!");
    } else {
        QMessageBox::critical(this, "DB Error", insertQ.lastError().text());
    }
}

void DealWindow::on_btnEdit_deal_clicked()
{
    int row = ui->tableWidget_deal->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Select", "Select a row first!"); return; }

    ui->lineEditID_deal->setText(ui->tableWidget_deal->item(row,0)->text());
    ui->lineEditName_deal->setText(ui->tableWidget_deal->item(row,1)->text());
    QDateTime dt = QDateTime::fromString(ui->tableWidget_deal->item(row,2)->text(), "dd/MM/yyyy hh:mm");
    if (dt.isValid()) ui->dateTimeEdit_deal->setDateTime(dt);

    int mask = 0;
    QString eq = ui->tableWidget_deal->item(row,3)->text();
    if (eq.contains("Camera"))       mask |= 1;
    if (eq.contains("Microphone"))   mask |= 2;
    if (eq.contains("Studio Light")) mask |= 4;
    if (eq.contains("Cameraman"))    mask |= 8;
    if (eq.contains("Video Editor")) mask |= 16;
    if (eq.contains("Studio"))       mask |= 32;

    ui->checkCamera_deal->setChecked(mask & 1);
    ui->checkMic_deal->setChecked(mask & 2);
    ui->checkLight_deal->setChecked(mask & 4);
    ui->checkCameraman_deal->setChecked(mask & 8);
    ui->checkEditor_deal->setChecked(mask & 16);
    ui->checkStudio_deal->setChecked(mask & 32);
}

void DealWindow::on_btnDelete_deal_clicked()
{
    int row = ui->tableWidget_deal->currentRow();
    if (row < 0) { QMessageBox::warning(this, "Select", "Select a row first!"); return; }
    if (QMessageBox::question(this, "Confirm", "Delete this booking?") != QMessageBox::Yes) return;

    int id = ui->tableWidget_deal->item(row,0)->text().toInt();
    QSqlQuery q(*(Connection::getInstance()->database()));
    q.prepare("DELETE FROM EQUIPMENT WHERE id = :id");
    q.bindValue(":id", id);
    if (q.exec()) {
        refreshTable();
        QMessageBox::information(this, "Deleted", "Booking removed!");
    } else {
        QMessageBox::critical(this, "Error", q.lastError().text());
    }
}

void DealWindow::on_btnOrder_deal_clicked()   { refreshTable(); }
void DealWindow::on_btnSearch_deal_clicked()  { refreshTable(ui->searchName_deal->text().trimmed()); }
void DealWindow::on_btnClear_deal_clicked()
{
    if (QMessageBox::question(this, "Clear All", "Delete ALL bookings?") == QMessageBox::Yes) {
        QSqlQuery q(*(Connection::getInstance()->database()));
        q.exec("DELETE FROM EQUIPMENT");
        refreshTable();
    }
}

void DealWindow::on_btnExportPDF_deal_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "Export to PDF",
                                                "equipment_" + QDate::currentDate().toString("yyyy-MM-dd") + ".pdf",
                                                "PDF Files (*.pdf)");
    if (file.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(file);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont f = painter.font();
    f.setPointSize(18); f.setBold(true);
    painter.setFont(f);
    painter.drawText(printer.pageRect(QPrinter::DevicePixel),
                     Qt::AlignTop | Qt::AlignHCenter, "EQUIPMENT RENTAL LIST");

    f.setPointSize(10); f.setBold(false);
    painter.setFont(f);

    double scale = qMin(printer.width() * 0.9 / ui->tableWidget_deal->width(),
                        (printer.height() - 200.0) / ui->tableWidget_deal->height());

    painter.translate(printer.width() * 0.05, 150);
    painter.scale(scale, scale);
    ui->tableWidget_deal->render(&painter);

    QMessageBox::information(this, "Success", "PDF exported successfully!");
}

void DealWindow::autoExportStatsWhenCritical()
{
    updateMonthlyStats(); // Ensure latest data

    QString downloads = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString fileName = "CRITICAL_USAGE_ALERT_" + timestamp + ".pdf";
    QString fullPath = QDir(downloads).filePath(fileName);

    // Build HTML report
    QString html;
    html += "<html><head>"
            "<meta charset='UTF-8'>"
            "<style>"
            "body { font-family: Arial; }"
            "h1 { color:#8B0000; text-align:center; font-size:34px; }"
            "h2 { color:#8B0000; text-align:center; font-size:18px; }"
            "table { width:100%; border-collapse:collapse; margin-top:20px; }"
            "th { background:#4A148C; color:white; padding:10px; font-size:18px; }"
            "td { border:1px solid #777; padding:14px; font-size:18px; text-align:center; }"
            ".crit { background:#FFCCCC; font-weight:bold; }"

            /* >>> NEW STYLE FOR BIG READABLE ITEM NAMES <<< */
            ".bigItem { "
            "   font-size:200px; "
            "   font-weight:bold; "
            "   color:#8B0000; "
            "   text-align:center; "
            "   margin-top:20px; "
            "}"
            "</style>"
            "</head><body>";

    html += "<h1>CRITICAL EQUIPMENT USAGE ALERT</h1>";
    html += "<h2>Generated: " +
            QDateTime::currentDateTime().toString("dddd, dd MMMM yyyy - hh:mm:ss") +
            "</h2>";

    html += "<table>";
    html += "<tr><th>Item</th><th>Total</th><th>Used</th><th>Usage %</th></tr>";

    QStringList criticalItems;

    for (int row = 0; row < ui->tableStats_deal->rowCount(); ++row)
    {
        QString item = ui->tableStats_deal->item(row, 0)->text();
        QString total = ui->tableStats_deal->item(row, 1)->text();
        QString used = ui->tableStats_deal->item(row, 2)->text();
        QString percentStr = ui->tableStats_deal->item(row, 3)->text();

        double percent = percentStr.remove('%').toDouble();

        if (percent >= 100)
        {
            criticalItems << item;

            html += "<tr class='crit'>"
                    "<td>" + item + "</td>"
                             "<td>" + total + "</td>"
                              "<td>" + used + "</td>"
                             "<td>" + QString::number(percent) + "%</td>"
                                                 "</tr>";
        }
    }

    html += "</table>";

    // ------------ BIG VISIBLE CRITICAL ITEMS SECTION ---------------
    html += "<br><br><h1>Items exceeding 100% usage:</h1>";

    if (criticalItems.isEmpty()) {
        html += "<p class='bigItem'>None</p>";
    } else {
        for (const QString &name : criticalItems) {
            html += "<p class='bigItem'>⚠ " + name + " ⚠</p>";
        }
    }

    html += "<h1>⚠ ACTION REQUIRED: " +
            QString::number(criticalItems.count()) +
            " item(s) above 100% usage ⚠</h1>";

    html += "</body></html>";

    // Convert HTML → PDF
    QTextDocument doc;
    doc.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fullPath);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    doc.print(&printer);

    QMessageBox::critical(this,
                          "CRITICAL REPORT GENERATED",
                          "PDF saved in your Downloads folder:\n\n" + fullPath);
}

void DealWindow::updateMonthlyStats()
{
    QDateTime now = QDateTime::currentDateTime();
    QDate monthStartDate = now.date().addDays(-now.date().day() + 1);  // 1st of current month
    QDateTime monthStart(monthStartDate, QTime(0,0));
    QDateTime monthEnd(now.date(), QTime(23,59,59));

    ui->lblCurrentMonth_deal->setText("Statistics for " + now.date().toString("MMMM yyyy"));

    // Count how many times each item was used this month
    QMap<QString, int> booked;
    QSqlQuery q(*(Connection::getInstance()->database()));
    q.prepare("SELECT equip_mask FROM EQUIPMENT WHERE rent_date BETWEEN :start AND :end");
    q.bindValue(":start", monthStart);
    q.bindValue(":end", monthEnd);
    q.exec();

    while (q.next()) {
        int mask = q.value(0).toInt();
        if (mask & 1)  booked["Camera"]++;
        if (mask & 2)  booked["Microphone"]++;
        if (mask & 4)  booked["Studio Light"]++;
        if (mask & 8)  booked["Cameraman"]++;
        if (mask & 16) booked["Video Editor"]++;
        if (mask & 32) booked["Studio"]++;
    }

    QSet<QString> currentlyCritical;
    QStringList items = {"Camera","Microphone","Studio Light","Cameraman","Video Editor","Studio"};

    for (int i = 0; i < items.size(); ++i) {
        QString item = items[i];
        int total = stock[item] + ordered.value(item, 0);
        int used = booked.value(item, 0);
        double percent = total > 0 ? (used * 100.0 / total) : 0.0;

        // Update table
        ui->tableStats_deal->item(i, 1)->setText(QString::number(total));
        ui->tableStats_deal->item(i, 2)->setText(QString::number(used));
        ui->tableStats_deal->item(i, 3)->setText(QString::number(percent, 'f', 1) + "%");

        QTableWidgetItem* cell = ui->tableStats_deal->item(i, 3);

        // Color logic
        if (percent >= 100.0) {
            cell->setBackground(QColor(139, 0, 0));      // Dark red
            cell->setForeground(Qt::white);
            cell->setFont(QFont("Arial", 10, QFont::Bold));
            currentlyCritical.insert(item);
        }
        else if (percent >= 90.0) {
            cell->setBackground(Qt::red);
            cell->setForeground(Qt::white);
        }
        else if (percent >= 70.0) {
            cell->setBackground(Qt::yellow);
            cell->setForeground(Qt::black);
        }
        else {
            cell->setBackground(Qt::white);
            cell->setForeground(Qt::black);
            cell->setFont(QFont("Arial", 9, QFont::Normal));
        }
    }

    // === TRIGGER CRITICAL ALERT ONLY WHEN A NEW ITEM HITS 100%+ ===
    bool hasNewCritical = false;
    for (const QString &item : currentlyCritical) {
        if (!previouslyCriticalItems.contains(item)) {
            hasNewCritical = true;
            break;
        }
    }

    if (hasNewCritical) {
        // Small delay to ensure DB is fully updated after booking
        QTimer::singleShot(300, this, [this]() {
            updateMonthlyStats();  // Final refresh
            autoExportStatsWhenCritical();  // NOW IT WILL DETECT 100% CORRECTLY
        });
    }

    // Update previous state
    previouslyCriticalItems = currentlyCritical;
}

void DealWindow::checkAutoOrder()
{
    QDateTime now = QDateTime::currentDateTime();
    QDate monthStartDate = now.date().addDays(-now.date().day() + 1);
    QDateTime monthStart(monthStartDate, QTime(0,0));
    QDateTime monthEnd(now.date(), QTime(23,59));

    QStringList items = {"Camera","Microphone","Studio Light","Cameraman","Video Editor","Studio"};
    for (const QString &item : items) {
        int bit = (item=="Camera")?1:(item=="Microphone")?2:(item=="Studio Light")?4:
                                           (item=="Cameraman")?8:(item=="Video Editor")?16:32;

        QSqlQuery q(*(Connection::getInstance()->database()));
        q.prepare("SELECT COUNT(*) FROM EQUIPMENT WHERE rent_date BETWEEN :s AND :e AND (equip_mask & :b)<>0");
        q.bindValue(":s", monthStart);
        q.bindValue(":e", monthEnd);
        q.bindValue(":b", bit);
        q.exec(); q.next();
        int used = q.value(0).toInt();

        int total = stock[item] + ordered.value(item,0);
        if (total > 0 && (used*100.0/total) >= 90 && ordered.value(item,0)==0) {
            ordered[item] = 1;
            ui->lblWarning_deal->setText("90% usage! 1 " + item + " auto-ordered!");
            ui->lblWarning_deal->setVisible(true);
            QMessageBox::warning(this, "AUTO ORDER", "90% usage → 1 " + item + " ordered!");
        }
    }
    updateMonthlyStats();
}

void DealWindow::deliverPendingOrders()
{
    bool delivered = false;
    for (auto it = ordered.begin(); it != ordered.end(); ++it) {
        if (it.value() > 0) {
            stock[it.key()] += it.value();
            it.value() = 0;
            delivered = true;
        }
    }
    if (delivered) {
        ui->lblWarning_deal->setText("New items delivered!");
        ui->lblWarning_deal->setVisible(true);
        QTimer::singleShot(3000, [this](){ ui->lblWarning_deal->setVisible(false); });
        QMessageBox::information(this, "DELIVERY", "New equipment arrived!");
        previouslyCriticalItems.clear();
    }
    updateMonthlyStats();
}

void DealWindow::refreshTable(const QString &filter)
{
    ui->tableWidget_deal->setRowCount(0);
    QSqlQuery q(*(Connection::getInstance()->database()));

    QString sql = "SELECT id, name, TO_CHAR(rent_date,'DD/MM/YYYY HH24:MI'), equip_mask FROM EQUIPMENT";
    if (!filter.isEmpty()) sql += " WHERE LOWER(name) LIKE LOWER(:f)";
    sql += " ORDER BY rent_date DESC";

    q.prepare(sql);
    if (!filter.isEmpty()) q.bindValue(":f", "%" + filter + "%");
    q.exec();

    int row = 0;
    while (q.next()) {
        ui->tableWidget_deal->insertRow(row);
        ui->tableWidget_deal->setItem(row,0,new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidget_deal->setItem(row,1,new QTableWidgetItem(q.value(1).toString()));
        ui->tableWidget_deal->setItem(row,2,new QTableWidgetItem(q.value(2).toString()));

        int mask = q.value(3).toInt();
        QStringList eq;
        if (mask & 1)  eq << "Camera";
        if (mask & 2)  eq << "Microphone";
        if (mask & 4)  eq << "Studio Light";
        if (mask & 8)  eq << "Cameraman";
        if (mask & 16) eq << "Video Editor";
        if (mask & 32) eq << "Studio";

        ui->tableWidget_deal->setItem(row,3,new QTableWidgetItem(eq.join(", ")));
        ++row;
    }
    ui->tableWidget_deal->resizeColumnsToContents();
    updateMonthlyStats();
}

int DealWindow::getEquipMask()
{
    int mask = 0;
    if (ui->checkCamera_deal->isChecked())     mask |= 1;
    if (ui->checkMic_deal->isChecked())        mask |= 2;
    if (ui->checkLight_deal->isChecked())      mask |= 4;
    if (ui->checkCameraman_deal->isChecked()) mask |= 8;
    if (ui->checkEditor_deal->isChecked())     mask |= 16;
    if (ui->checkStudio_deal->isChecked())     mask |= 32;
    return mask;
}

QSet<QString> DealWindow::previouslyCriticalItems;
