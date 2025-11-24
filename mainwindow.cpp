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
#include <QPdfWriter>      // ← OBLIGATOIRE pour créer le PDF
#include <QPainter>        // ← OBLIGATOIRE pour dessiner dedans
#include <QPageSize>       // ← Pour A4, A3, etc.

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), darkTheme(false)
{
    ui->setupUi(this);

    // CRITICAL: Initialize darkTheme property
    for (QWidget *w : findChildren<QWidget*>()) {
        w->setProperty("darkTheme", false);
        w->style()->unpolish(w);
        w->style()->polish(w);
    }

    // Enable rounded corners
    ui->creatorTable_6->setAttribute(Qt::WA_StyledBackground, true);
    ui->tableFactures->setAttribute(Qt::WA_StyledBackground, true);

    if (!connectDB()) {
        QMessageBox::critical(this, "DB Error", "Failed to connect to database!");
        return;
    }

    ui->platformCombo_6->setCurrentText("YouTube");
    ui->comboBox_6->setCurrentText("male");

    // Set initial button text
    ui->themeButton_6->setText("Dark Theme");
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
    // Bouton "Stat" → passe à la page stackedWidget_3 (page des statistiques)

    // Connexion des boutons Stats et Retour



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

void MainWindow::on_searchEdit_6_textChanged(const QString &text)
{
    // Recherche en temps réel (ou tu peux laisser vide si tu veux faire plus tard)
    Q_UNUSED(text);

    // Exemple de recherche basique (optionnel)
    for (int row = 0; row < ui->creatorTable_6->rowCount(); ++row) {
        bool hide = true;
        for (int col = 0; col < ui->creatorTable_6->columnCount(); ++col) {
            QTableWidgetItem *item = ui->creatorTable_6->item(row, col);
            if (item && item->text().toLower().contains(text.toLower())) {
                hide = false;
                break;
            }
        }
        ui->creatorTable_6->setRowHidden(row, hide && !text.isEmpty());
    }

    // Si le champ est vide → on affiche tout
    if (text.isEmpty()) {
        for (int row = 0; row < ui->creatorTable_6->rowCount(); ++row) {
            ui->creatorTable_6->setRowHidden(row, false);
        }
    }

    // MET À JOUR LES STATS APRÈS RECHERCHE
    updateSimpleStats();
}
void MainWindow::on_themeButton_6_clicked()
{
    darkTheme = !darkTheme;

    for (QWidget *w : findChildren<QWidget*>()) {
        w->setProperty("darkTheme", darkTheme);
        w->style()->unpolish(w);
        w->style()->polish(w);
    }

    ui->themeButton_6->setText(darkTheme ? "Light Theme" : "Dark Theme");
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
    q.prepare("INSERT INTO \"MY_USER\".\"CREATEUR\" "
              "(\"NOM\", \"PLATFORME\", \"GENRE\", \"ABONNE\", \"TYPE_DE_CONTENU\", \"ID_EMPLOYEE\") "
              "VALUES (:nom, :plat, :genre, :abo, :type, :emp)");

    // Et tu mets une valeur (ex: 1 pour admin, ou l’ID de l’employé connecté)
    q.bindValue(":emp", 1);  // ou la variable de l’employé connecté
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
    updateSimpleStats(); // au démarrage
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
              "NOM = :name, PLATFORME = :plat, GENRE = :sex, "
              "ABONNE = :subs, TYPE_DE_CONTENU = :type "
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
    updateSimpleStats(); // au démarrage
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
    QString file = QFileDialog::getSaveFileName(this, "Exporter Rapport Officiel",
                                                "Rapport_Content_Creators_SmartMedia.pdf", "PDF (*.pdf)");
    if (file.isEmpty()) return;

    QPdfWriter pdf(file);
    pdf.setPageSize(QPageSize::A4);
    pdf.setResolution(300);
    pdf.setPageMargins(QMarginsF(20, 20, 20, 20));

    QPainter painter(&pdf);
    painter.setRenderHint(QPainter::Antialiasing);

    // Logo + Titre
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.setPen(QColor("#7D4FEE"));
    painter.drawText(0, 400, pdf.width(), 200, Qt::AlignCenter, "RAPPORT OFFICIEL CONTENT CREATORS");

    painter.setFont(QFont("Arial", 16));
    painter.drawText(0, 700, pdf.width(), 200, Qt::AlignCenter, "Smart Media Agency © Selim Fekih 2026");

    painter.setFont(QFont("Arial", 12));
    painter.drawText(100, 1100, "Date du rapport : " + QDate::currentDate().toString("dd/MM/yyyy"));

    // Stats LCD dans le PDF
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(100, 1500, "STATISTIQUES LIVE");
    painter.drawText(100, 1700, "Total Créateurs : " + QString::number(ui->lcdTotal->value()));
    painter.drawText(100, 1900, "YouTube : " + QString::number(ui->lcdYoutube->value()));
    painter.drawText(100, 2100, "TikTok : " + QString::number(ui->lcdTiktok->value()));
    painter.drawText(100, 2300, "Instagram : " + QString::number(ui->lcdInstagram->value()));

    // Tableau des créateurs
    painter.setFont(QFont("Arial", 10));
    int y = 2700;
    painter.drawText(100, y, "ID | Nom | Plateforme | Abonnés | Revenu");
    y += 300;

    QSqlQuery q("SELECT * FROM \"MY_USER\".\"CREATEUR\"");
    while (q.next() && y < pdf.height() - 1000) {
        painter.drawText(100, y, QString("%1 | %2 | %3 | %4 | %5 DT")
                             .arg(q.value(0).toInt())
                             .arg(q.value(1).toString())
                             .arg(q.value(2).toString())
                             .arg(q.value(3).toString())
                             .arg(q.value(4).toString()));
        y += 250;
    }

    // Signature
    painter.drawText(pdf.width() - 2500, pdf.height() - 800, "Signature Agence Smart Media");
    painter.drawLine(pdf.width() - 2500, pdf.height() - 700, pdf.width() - 800, pdf.height() - 700);

    painter.end();
    QMessageBox::information(this, "Succès", "Rapport PDF exporté comme un pro !");
}
void MainWindow::updateSimpleStats()
{
    QSqlQuery q;

    // Total créateurs
    q.exec("SELECT COUNT(*) FROM \"MY_USER\".\"CREATEUR\"");
    if (q.next()) ui->lcdTotal->display(q.value(0).toInt());

    // YouTube
    q.exec("SELECT COUNT(*) FROM \"MY_USER\".\"CREATEUR\" WHERE PLATEFORME = 'YouTube'");
    if (q.next()) ui->lcdYoutube->display(q.value(0).toInt());

    // TikTok
    q.exec("SELECT COUNT(*) FROM \"MY_USER\".\"CREATEUR\" WHERE PLATEFORME = 'TikTok'");
    if (q.next()) ui->lcdTiktok->display(q.value(0).toInt());

    // Instagram
    q.exec("SELECT COUNT(*) FROM \"MY_USER\".\"CREATEUR\" WHERE PLATEFORME = 'Instagram'");
    if (q.next()) ui->lcdInstagram->display(q.value(0).toInt());
}
