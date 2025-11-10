#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QClipboard>
#include <QApplication>
#include <QSqlQuery>
#include <QRegularExpression>
#include <QFont>
#include "qrcodegenerator.h"  // ← ALREADY THERE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), darkTheme(false)
{
    ui->setupUi(this);

    if (!connectDB()) {
        QMessageBox::critical(this, "DB Error", "Failed to connect to database!");
        return;
    }

    ui->platformCombo_6->setCurrentText("YouTube");
    ui->comboBox_6->setCurrentText("male");
    applyLightTheme();

    // Navigation
    connect(ui->sidebarButton,    &QPushButton::clicked, this, &MainWindow::showFinancePage);
    connect(ui->sidebarButton_3,  &QPushButton::clicked, this, &MainWindow::showContentCreatorPage);
    connect(ui->sidebarButton_2,  &QPushButton::clicked, this, &MainWindow::showSponsorPage);
    connect(ui->sidebarButton_5,  &QPushButton::clicked, this, &MainWindow::showManagementPage);
    connect(ui->sidebarButton_4,  &QPushButton::clicked, this, &MainWindow::showEmployeePage);

    // Content Creator
    connect(ui->searchEdit_6,    &QLineEdit::textChanged, this, &MainWindow::on_searchEdit_6_textChanged);
    connect(ui->creatorTable_6,  &QTableWidget::itemSelectionChanged, this, &MainWindow::fillFormFromTable);
    connect(ui->addButton_6,     &QPushButton::clicked, this, &MainWindow::on_addButton_6_clicked);
    connect(ui->updateButton_6,  &QPushButton::clicked, this, &MainWindow::on_updateButton_6_clicked);
    connect(ui->deleteButton_6,  &QPushButton::clicked, this, &MainWindow::on_deleteButton_6_clicked);
    connect(ui->clearButton_6,   &QPushButton::clicked, this, &MainWindow::on_clearButton_6_clicked);
    connect(ui->exportButton_6,  &QPushButton::clicked, this, &MainWindow::on_exportButton_6_clicked);
    connect(ui->themeButton_6,   &QPushButton::clicked, this, &MainWindow::on_themeButton_6_clicked);

    // QR Code
    connect(ui->btnGenerateQR, &QPushButton::clicked, this, &MainWindow::generateQR);
    connect(ui->btnSaveQR,     &QPushButton::clicked, this, &MainWindow::saveQR);
    connect(ui->btnCopyQR,     &QPushButton::clicked, this, &MainWindow::copyQR);

    loadCreateurs();
}

MainWindow::~MainWindow() { delete ui; }

// ———————————————————————— DATABASE ————————————————————————
bool MainWindow::connectDB()
{
    Connection conn;
    return conn.createconnect();
}

// ———————————————————————— THEME ————————————————————————
void MainWindow::applyLightTheme()
{
    setStyleSheet("background-color: #f0f0f0; color: black;");
    ui->themeButton_6->setText("Dark Theme");
}

void MainWindow::applyDarkTheme()
{
    setStyleSheet("background-color: #2b2b2b; color: white;");
    ui->themeButton_6->setText("Light Theme");
}

void MainWindow::on_themeButton_6_clicked()
{
    darkTheme = !darkTheme;
    if (darkTheme) applyDarkTheme();
    else applyLightTheme();
}

// ———————————————————————— NAVIGATION ————————————————————————
void MainWindow::showFinancePage()        { ui->stackedWidget->setCurrentWidget(ui->financePage); }
void MainWindow::showContentCreatorPage() { ui->stackedWidget->setCurrentWidget(ui->contentCreatorPage); }
void MainWindow::showSponsorPage()        { ui->stackedWidget->setCurrentWidget(ui->sponsorPage); }
void MainWindow::showManagementPage()     { ui->stackedWidget->setCurrentWidget(ui->managementPage); }
void MainWindow::showEmployeePage()       { ui->stackedWidget->setCurrentWidget(ui->employeePage); }

// ———————————————————————— CONTENT CREATOR ————————————————————————
void MainWindow::loadCreateurs()
{
    ui->creatorTable_6->blockSignals(true);
    ui->creatorTable_6->clearContents();
    ui->creatorTable_6->setRowCount(0);
    ui->creatorTable_6->setColumnCount(7);
    ui->creatorTable_6->setHorizontalHeaderLabels({
        "ID", "Name", "Platform", "Gender", "Subscribers", "Content Type", "ID Employee"
    });

    QSqlQuery query("SELECT * FROM \"MY_USER\".\"CREATEUR\" ORDER BY IDCREATEUR");
    int row = 0;
    while (query.next()) {
        ui->creatorTable_6->insertRow(row);
        for (int col = 0; col < 7; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();
            QVariant val = query.value(col);
            item->setText(val.isNull() ? "" : val.toString());
            if (col == 4) {
                item->setData(Qt::UserRole, val.toInt());
                item->setTextAlignment(Qt::AlignCenter);
            }
            ui->creatorTable_6->setItem(row, col, item);
        }
        ++row;
    }

    ui->creatorTable_6->resizeColumnsToContents();
    ui->creatorTable_6->setSortingEnabled(true);
    ui->creatorTable_6->sortItems(4, Qt::DescendingOrder);
    ui->creatorTable_6->blockSignals(false);
}

void MainWindow::on_searchEdit_6_textChanged(const QString &text)
{
    QString search = text.toLower().trimmed();
    for (int i = 0; i < ui->creatorTable_6->rowCount(); ++i) {
        auto *name = ui->creatorTable_6->item(i, 1);
        auto *plat = ui->creatorTable_6->item(i, 2);
        if (!name || !plat) continue;
        bool match = name->text().toLower().contains(search) || plat->text().toLower().contains(search);
        ui->creatorTable_6->setRowHidden(i, !match && !search.isEmpty());
    }
    if (!search.isEmpty()) {
        ui->creatorTable_6->sortItems(4, Qt::DescendingOrder);
    }
}

void MainWindow::fillFormFromTable()
{
    int row = ui->creatorTable_6->currentRow();
    if (row < 0) return;
    auto get = [this, row](int col) { return ui->creatorTable_6->item(row, col); };
    if (!get(1) || !get(2) || !get(3) || !get(4) || !get(5)) return;

    ui->nameEdit_6->setText(get(1)->text());
    ui->platformCombo_6->setCurrentText(get(2)->text());
    ui->comboBox_6->setCurrentText(get(3)->text());
    ui->subscribersSpin_6->setValue(get(4)->text().toInt());
    ui->contentTypeEdit_6->setText(get(5)->text());
}

bool MainWindow::isValidName(const QString &name) const
{
    if (name.length() < 2 || name.length() > 50) return false;
    QRegularExpression re("^[a-zA-ZÀ-ÿ\\s'-]+$");
    return re.match(name).hasMatch();
}

void MainWindow::clearCreateurFields()
{
    ui->nameEdit_6->clear();
    ui->platformCombo_6->setCurrentIndex(0);
    ui->comboBox_6->setCurrentIndex(0);
    ui->subscribersSpin_6->setValue(0);
    ui->contentTypeEdit_6->clear();
    ui->nameEdit_6->setFocus();
}

// ———————————————————————— ROLE SYSTEM ————————————————————————
void MainWindow::disableAllInputs()
{
    auto disable = [this](auto w) { if (w) w->setEnabled(false); };
    disable(ui->addButton_6); disable(ui->updateButton_6); disable(ui->deleteButton_6);
    disable(ui->clearButton_6); disable(ui->exportButton_6);
    disable(ui->nameEdit_6); disable(ui->platformCombo_6); disable(ui->comboBox_6);
    disable(ui->subscribersSpin_6); disable(ui->contentTypeEdit_6);
    disable(ui->searchEdit_6); disable(ui->btnGenerateQR); disable(ui->btnSaveQR); disable(ui->btnCopyQR);
}

void MainWindow::applyRoleRestrictions()
{
    if (m_userRole == "viewer") {
        disableAllInputs();
        ui->creatorTable_6->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
    else if (m_userRole == "editor") {
        ui->sidebarButton->setEnabled(false);
    }
    else if (m_userRole == "manager") {
        ui->sidebarButton_4->setEnabled(false);
    }
}

void MainWindow::setUserRole(const QString &role)
{
    m_userRole = role;
    applyRoleRestrictions();
}

// ———————————————————————— QR CODE ————————————————————————
void MainWindow::generateQR()
{
    int row = ui->creatorTable_6->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Select a creator.");
        return;
    }

    Creator c;
    c.id = ui->creatorTable_6->item(row, 0)->text().toInt();
    c.name = ui->creatorTable_6->item(row, 1)->text();
    c.platform = ui->creatorTable_6->item(row, 2)->text();
    c.subscribers = ui->creatorTable_6->item(row, 4)->text().toInt();
    c.type = ui->creatorTable_6->item(row, 5)->text();
    c.photo = QPixmap(":/icons/default_avatar.png");

    QPixmap logo(":/icons/logo.png");
    m_currentQR = QRCodeGenerator::generate(c, logo);
    ui->qrLabel->setPixmap(m_currentQR.scaled(300, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::saveQR()
{
    if (m_currentQR.isNull()) return;
    QString file = QFileDialog::getSaveFileName(this, "Save QR", "", "PNG (*.png)");
    if (!file.isEmpty()) m_currentQR.save(file, "PNG");
}

void MainWindow::copyQR()
{
    if (m_currentQR.isNull()) return;
    QApplication::clipboard()->setPixmap(m_currentQR);
    QMessageBox::information(this, "Copied", "QR copied to clipboard!");
}
void MainWindow::on_addButton_6_clicked()
{
    QString name = ui->nameEdit_6->text().trimmed();
    QString platform = ui->platformCombo_6->currentText();
    QString gender = ui->comboBox_6->currentText();
    int subs = ui->subscribersSpin_6->value();
    QString type = ui->contentTypeEdit_6->text().trimmed();

    if (name.isEmpty() || !isValidName(name)) {
        QMessageBox::warning(this, "Error", "Invalid name!");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO \"MY_USER\".\"CREATEUR\" (NOM, PLATEFORME, SEXE, ABONNES, TYPE_CONTENU) "
              "VALUES (:name, :plat, :sex, :subs, :type)");
    q.bindValue(":name", name);
    q.bindValue(":plat", platform);
    q.bindValue(":sex", gender);
    q.bindValue(":subs", subs);
    q.bindValue(":type", type);

    if (q.exec()) {
        loadCreateurs();
        clearCreateurFields();
        QMessageBox::information(this, "Success", "Creator added!");
    } else {
        QMessageBox::critical(this, "Error", q.lastError().text());
    }
}

void MainWindow::on_updateButton_6_clicked()
{
    int row = ui->creatorTable_6->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Select a creator!");
        return;
    }

    int id = ui->creatorTable_6->item(row, 0)->text().toInt();
    QString name = ui->nameEdit_6->text().trimmed();
    if (!isValidName(name)) {
        QMessageBox::warning(this, "Error", "Invalid name!");
        return;
    }

    QSqlQuery q;
    q.prepare("UPDATE \"MY_USER\".\"CREATEUR\" SET "
              "NOM = :name, PLATEFORME = :plat, SEXE = :sex, "
              "ABONNES = :subs, TYPE_CONTENU = :type "
              "WHERE IDCREATEUR = :id");
    q.bindValue(":name", name);
    q.bindValue(":plat", ui->platformCombo_6->currentText());
    q.bindValue(":sex", ui->comboBox_6->currentText());
    q.bindValue(":subs", ui->subscribersSpin_6->value());
    q.bindValue(":type", ui->contentTypeEdit_6->text().trimmed());
    q.bindValue(":id", id);

    if (q.exec()) {
        loadCreateurs();
        QMessageBox::information(this, "Success", "Updated!");
    } else {
        QMessageBox::critical(this, "Error", q.lastError().text());
    }
}

void MainWindow::on_deleteButton_6_clicked()
{
    int row = ui->creatorTable_6->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Select a creator!");
        return;
    }

    if (QMessageBox::question(this, "Confirm", "Delete this creator?") != QMessageBox::Yes)
        return;

    int id = ui->creatorTable_6->item(row, 0)->text().toInt();
    QSqlQuery q;
    q.prepare("DELETE FROM \"MY_USER\".\"CREATEUR\" WHERE IDCREATEUR = :id");
    q.bindValue(":id", id);

    if (q.exec()) {
        loadCreateurs();
        clearCreateurFields();
        QMessageBox::information(this, "Success", "Deleted!");
    } else {
        QMessageBox::critical(this, "Error", q.lastError().text());
    }
}

void MainWindow::on_clearButton_6_clicked()
{
    clearCreateurFields();
}

void MainWindow::on_exportButton_6_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "Export CSV", "", "CSV (*.csv)");
    if (file.isEmpty()) return;

    QFile f(file);
    if (!f.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Error", "Cannot write file!");
        return;
    }

    QTextStream out(&f);
    out << "ID,Name,Platform,Gender,Subscribers,Type\n";
    for (int i = 0; i < ui->creatorTable_6->rowCount(); ++i) {
        if (ui->creatorTable_6->isRowHidden(i)) continue;
        for (int c = 0; c < 6; ++c) {
            out << ui->creatorTable_6->item(i, c)->text();
            if (c < 5) out << ",";
        }
        out << "\n";
    }
    f.close();
    QMessageBox::information(this, "Success", "Exported to " + file);
}
