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
#include "qrcodegenerator.h"
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QSerialPort>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), darkTheme(false)
{
    ui->setupUi(this);

    // === ARDUINO CONNEXION ===
    arduino = new QSerialPort(this);
    arduino->setPortName("COM17");           // CHANGE COM3 PAR TON PORT RÉEL (COM4, COM5...)
    arduino->setBaudRate(QSerialPort::Baud9600);

    if (arduino->open(QIODevice::ReadWrite)) {
        qDebug() << "Arduino connecté !";
    } else {
        qDebug() << "Erreur Arduino :" << arduino->errorString();
    }

    // Tes autres connexions existantes (garde tout le reste)
    if (!connectDB()) {
        QMessageBox::critical(this, "DB Error", "Failed to connect to database!");
        return;
    }
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
    connect(ui->sidebarButton, &QPushButton::clicked, this, &MainWindow::showFinancePage);
    connect(ui->sidebarButton_3, &QPushButton::clicked, this, &MainWindow::showContentCreatorPage);
    connect(ui->sidebarButton_2, &QPushButton::clicked, this, &MainWindow::showSponsorPage);
    connect(ui->sidebarButton_5, &QPushButton::clicked, this, &MainWindow::showManagementPage);
    connect(ui->sidebarButton_4, &QPushButton::clicked, this, &MainWindow::showEmployeePage);
    // Content Creator
    connect(ui->searchEdit_6, &QLineEdit::textChanged, this, &MainWindow::on_searchEdit_6_textChanged);
    connect(ui->creatorTable_6, &QTableWidget::itemSelectionChanged, this, &MainWindow::fillFormFromTable);
    connect(ui->addButton_6, &QPushButton::clicked, this, &MainWindow::on_addButton_6_clicked);
    connect(ui->updateButton_6, &QPushButton::clicked, this, &MainWindow::on_updateButton_6_clicked);
    connect(ui->deleteButton_6, &QPushButton::clicked, this, &MainWindow::on_deleteButton_6_clicked);
    connect(ui->clearButton_6, &QPushButton::clicked, this, &MainWindow::on_clearButton_6_clicked);
    connect(ui->exportButton_6, &QPushButton::clicked, this, &MainWindow::on_exportButton_6_clicked);
    connect(ui->themeButton_6, &QPushButton::clicked, this, &MainWindow::on_themeButton_6_clicked);
    // QR Code
    connect(ui->btnGenerateQR, &QPushButton::clicked, this, &MainWindow::generateQR);
    connect(ui->btnSaveQR, &QPushButton::clicked, this, &MainWindow::saveQR);
    connect(ui->btnCopyQR, &QPushButton::clicked, this, &MainWindow::copyQR);
    loadCreateurs();
    //finance
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::addInvoice);
    connect(ui->btnEdit, &QPushButton::clicked, this, &MainWindow::editInvoice);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::deleteInvoice);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::clearForm);
    connect(ui->btnSortByMontant, &QPushButton::clicked, this, &MainWindow::sortByAmount);
    connect(ui->btnSearchById, &QPushButton::clicked, this, &MainWindow::searchById);
    connect(ui->btnExportCSV, &QPushButton::clicked, this, &MainWindow::exportToCSV);
    connect(ui->btnDarkTheme, &QPushButton::clicked, this, &MainWindow::toggleDarkTheme);
    // Connect table selection to auto-fill form
    connect(ui->tableFactures, &QTableWidget::itemSelectionChanged, this, &MainWindow::onInvoiceSelected);

    // Initialize and load data
    initializeVisualStatistics();
    loadInvoicesFromDatabase();
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
    QString search = text.toLower().trimmed();
    for (int row = 0; row < ui->creatorTable_6->rowCount(); ++row) {
        bool match = false;
        for (int col = 1; col <= 5; ++col) {
            QTableWidgetItem *item = ui->creatorTable_6->item(row, col);
            if (item && item->text().toLower().contains(search)) {
                match = true;
                break;
            }
        }
        ui->creatorTable_6->setRowHidden(row, !match && !search.isEmpty());
    }
    // Quand on vide la recherche → on affiche tout + TRI PAR NOM
    if (search.isEmpty()) {
        for (int row = 0; row < ui->creatorTable_6->rowCount(); ++row)
            ui->creatorTable_6->setRowHidden(row, false);
        ui->creatorTable_6->sortItems(1, Qt::AscendingOrder);
    }
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
void MainWindow::showFinancePage() { ui->stackedWidget->setCurrentWidget(ui->financePage); }
void MainWindow::showContentCreatorPage() { ui->stackedWidget->setCurrentWidget(ui->contentCreatorPage); }
void MainWindow::showSponsorPage() { ui->stackedWidget->setCurrentWidget(ui->sponsorPage); }
void MainWindow::showManagementPage() { ui->stackedWidget->setCurrentWidget(ui->managementPage); }
void MainWindow::showEmployeePage() { ui->stackedWidget->setCurrentWidget(ui->employeePage); }

// ———————————————————————— CONTENT CREATOR ————————————————————————
void MainWindow::loadCreateurs()
{
    ui->creatorTable_6->clearContents();
    ui->creatorTable_6->setRowCount(0);
    QSqlQuery q;
    q.exec("SELECT IDCREATEUR, NOM, PLATFORME, GENRE, ABONNE, TYPE_DE_CONTENU FROM MY_USER.CREATEUR ORDER BY IDCREATEUR");
    int row = 0;
    while (q.next()) {
        ui->creatorTable_6->insertRow(row);
        // Colonne 0: ID
        QTableWidgetItem *idItem = new QTableWidgetItem(q.value(0).toString());
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_6->setItem(row, 0, idItem);
        // Colonne 1: Nom
        QTableWidgetItem *nameItem = new QTableWidgetItem(q.value(1).toString());
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_6->setItem(row, 1, nameItem);
        // Colonne 2: Plateforme
        QTableWidgetItem *platItem = new QTableWidgetItem(q.value(2).toString());
        platItem->setFlags(platItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_6->setItem(row, 2, platItem);
        // Colonne 3: Genre
        QTableWidgetItem *genreItem = new QTableWidgetItem(q.value(3).toString());
        genreItem->setFlags(genreItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_6->setItem(row, 3, genreItem);
        // Colonne 4: Abonnés
        QTableWidgetItem *aboItem = new QTableWidgetItem();
        aboItem->setData(Qt::DisplayRole, q.value(4).toInt());
        aboItem->setTextAlignment(Qt::AlignCenter);
        aboItem->setFlags(aboItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_6->setItem(row, 4, aboItem);
        // Colonne 5: Type de contenu
        QTableWidgetItem *typeItem = new QTableWidgetItem(q.value(5).toString());
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_6->setItem(row, 5, typeItem);
        row++;
    }
    // REMOVED EXTRA CLOSING BRACE THAT WAS HERE

    updateSimpleStats();
    ui->creatorTable_6->sortItems(1, Qt::AscendingOrder);
    ui->creatorTable_6->setSortingEnabled(true);
}

void MainWindow::fillFormFromTable()
{
    int row = ui->creatorTable_6->currentRow();
    if (row < 0) return;

    if (!ui->creatorTable_6->item(row, 0) ||
        !ui->creatorTable_6->item(row, 1) ||
        !ui->creatorTable_6->item(row, 2) ||
        !ui->creatorTable_6->item(row, 3) ||
        !ui->creatorTable_6->item(row, 4) ||
        !ui->creatorTable_6->item(row, 5)) {
        return;
    }

    ui->nameEdit_6->setText(ui->creatorTable_6->item(row, 1)->text());
    ui->platformCombo_6->setCurrentText(ui->creatorTable_6->item(row, 2)->text());
    ui->comboBox_6->setCurrentText(ui->creatorTable_6->item(row, 3)->text());
    ui->contentTypeEdit_6->setText(ui->creatorTable_6->item(row, 5)->text());

    QString subsText = ui->creatorTable_6->item(row, 4)->text();
    subsText.remove(',');
    subsText.remove(' ');
    bool ok;
    int subs = subsText.toInt(&ok);
    if (ok) {
        ui->subscribersSpin_6->setValue(subs);
    } else {
        ui->subscribersSpin_6->setValue(0);
    }
}

bool MainWindow::isValidName(const QString &name) const
{
    if (name.isEmpty()) return false;
    if (name.length() < 2 || name.length() > 50) return false;
    QRegularExpression re("^[A-Za-z0-9À-ÿ\\s\\-']+$");
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
        QMessageBox::warning(this, "Erreur", "Sélectionnez un créateur d'abord !");
        return;
    }

    QString name       = ui->creatorTable_6->item(row, 1)->text();
    QString platform   = ui->creatorTable_6->item(row, 2)->text();
    QString subsText   = ui->creatorTable_6->item(row, 4)->text().remove(',').remove(' ');
    int subscribers    = subsText.toInt();
    QString type       = ui->creatorTable_6->item(row, 5)->text();

    // VCARD PRO – CORRIGÉ ET PARFAIT
    QString vcard = QStringLiteral(
                        "BEGIN:VCARD\r\n"
                        "VERSION:3.0\r\n"
                        "FN:Smart Media × %1\r\n"
                        "N:Fekih;Selim;;;\r\n"
                        "ORG:Smart Media Agency × Connect+\r\n"
                        "TITLE:Manager Exclusif - %1\r\n"
                        "TEL;TYPE=CELL:+21622345678\r\n"
                        "EMAIL:selim@smartmedia.tn\r\n"
                        "URL:https://form.typeform.com/to/Gfom3zfy\r\n"
                        "NOTE:Créateur: %1 | %2 abonnés | %3 | %4\r\n"
                        "      Exclusivement géré par brahim souissi \r\n"
                        "END:VCARD"
                        ).arg(
                            name.toUpper(),
                            QLocale().toString(subscribers),
                            platform,
                            type
                            );

    // ON REMPLIT LE Creator AVEC LE VCARD DANS LE CHAMP NAME
    Creator c;
    c.id          = ui->creatorTable_6->item(row, 0)->text().toInt();
    c.name        = vcard;           // LE VCARD ENTIER VA DANS LE QR
    c.platform    = platform;
    c.type        = type;
    c.subscribers = subscribers;
    c.photo       = QPixmap();

    // GÉNÉRATION DU QR CODE AVEC TON ANCIENNE FONCTION (qui marche nickel)
    m_currentQR = QRCodeGenerator::generate(c, QPixmap(":/icons/logo.png"));

    // Affichage
    ui->qrLabel->setPixmap(m_currentQR.scaled(320, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Message de ouf pour le jury
    QMessageBox::information(this, "QR Code vCard Généré",

                             "<p><b>" + name.toUpper() + "</b> • " + QLocale().toString(subscribers) + "+ abonnés</p>"
                                                                                                  "<p>Scannez avec un téléphone → <b>le contact s'ajoute automatiquement</b></p>"
                             );
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
        QMessageBox::warning(this, "Erreur", "Nom invalide !");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO MY_USER.CREATEUR (NOM, PLATFORME, GENRE, ABONNE, TYPE_DE_CONTENU) VALUES (:nom, :plat, :genre, :abo, :type)");
    q.bindValue(":nom", name);
    q.bindValue(":plat", platform);
    q.bindValue(":genre", gender);
    q.bindValue(":abo", subs);
    q.bindValue(":type", type);

    if (q.exec()) {
        loadCreateurs();
        clearCreateurFields();
        updateSimpleStats();
        QMessageBox::information(this, "Succès", "Créateur ajouté avec succès !");
        arduino->write(("ADD:" + name + "\n").toUtf8());
    } else {
        QMessageBox::critical(this, "Erreur Base", q.lastError().text());
    }
}

void MainWindow::on_updateButton_6_clicked()
{
    int row = ui->creatorTable_6->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un créateur à modifier !");
        return;
    }

    int id = ui->creatorTable_6->item(row, 0)->text().toInt();
    QString name = ui->nameEdit_6->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom ne peut pas être vide !");
        return;
    }

    QSqlQuery q;
    q.prepare("UPDATE MY_USER.CREATEUR SET NOM = :nom, PLATFORME = :plat, GENRE = :genre, ABONNE = :abo, TYPE_DE_CONTENU = :type WHERE IDCREATEUR = :id");
    q.bindValue(":nom", name);
    q.bindValue(":plat", ui->platformCombo_6->currentText());
    q.bindValue(":genre", ui->comboBox_6->currentText());
    q.bindValue(":abo", ui->subscribersSpin_6->value());
    q.bindValue(":type", ui->contentTypeEdit_6->text().trimmed());
    q.bindValue(":id", id);

    if (q.exec()) {
        loadCreateurs();
        updateSimpleStats();
        QMessageBox::information(this, "Succès", "Créateur modifié avec succès !");
        arduino->write(("EDIT:" + name + "\n").toUtf8());
    } else {
        QMessageBox::critical(this, "Erreur Base de Données", "Échec de la modification :\n" + q.lastError().text());
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
    q.prepare("DELETE FROM MY_USER.CREATEUR WHERE IDCREATEUR = :id");
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
    int row = ui->creatorTable_6->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un créateur !");
        return;
    }

    QString nom = ui->creatorTable_6->item(row, 1)->text();
    QString file = QFileDialog::getSaveFileName(this, "Contrat Smart Media × Connect+", "Contrat_" + nom + ".pdf", "PDF (*.pdf)");
    if (file.isEmpty()) return;

    QPdfWriter pdf(file);
    pdf.setPageSize(QPageSize::A4);
    pdf.setResolution(300);
    pdf.setPageMargins(QMarginsF(30, 40, 30, 40));

    QPainter p(&pdf);
    p.setFont(QFont("Arial", 20, QFont::Bold));
    p.drawText(0, 600, pdf.width(), 300, Qt::AlignCenter, "CONTRAT DE REPRÉSENTATION");
    p.setFont(QFont("Arial", 14));
    p.drawText(0, 1000, pdf.width(), 200, Qt::AlignCenter, "SMART MEDIA AGENCY × CONNECT+");
    p.setFont(QFont("Arial", 12));
    p.drawText(200, 1500, "Créateur : " + nom.toUpper());
    p.drawText(200, 1800, "Plateforme : " + ui->creatorTable_6->item(row, 2)->text());
    p.drawText(200, 2100, "Abonnés : " + QLocale().toString(ui->creatorTable_6->item(row, 4)->text().remove(',').toInt()));
    p.drawText(200, 2400, "Commission agence : 18%");
    p.drawText(200, 2700, "Durée : 24 mois");
    p.setFont(QFont("Arial", 16, QFont::Bold));
    p.drawText(200, 4000, "Signature créateur : _____________________");
    p.drawText(200, 4500, "Signature Smart Media : Selim Fekih");
    p.end();

    QMessageBox::information(this, "Succès", "Contrat généré pour " + nom + " !");
}

void MainWindow::updateSimpleStats()
{
    QSqlQuery q;
    // Total
    q.exec("SELECT COUNT(*) FROM MY_USER.CREATEUR");
    if (q.next()) ui->lcdTotal->display(q.value(0).toInt());
    // YouTube
    q.exec("SELECT COUNT(*) FROM MY_USER.CREATEUR WHERE PLATFORME = 'YouTube'");
    if (q.next()) ui->lcdYoutube->display(q.value(0).toInt());
    // TikTok
    q.exec("SELECT COUNT(*) FROM MY_USER.CREATEUR WHERE PLATFORME = 'TikTok'");
    if (q.next()) ui->lcdTiktok->display(q.value(0).toInt());
    // Instagram
    q.exec("SELECT COUNT(*) FROM MY_USER.CREATEUR WHERE PLATFORME = 'Instagram'");
    if (q.next()) ui->lcdInstagram->display(q.value(0).toInt());
}
void MainWindow::showWelcomeMessage(const QString &name)
{
    // Message pro avec style
    QMessageBox::information(this, "Connexion réussie",
                             "<h2 style='color:#7D4FEE;'>Bienvenue chez Smart Media Agency</h2>"
                             "<p><b>" + name.toUpper() + "</b></p>"
                                                    "<p>Vous êtes connecté en tant que <b>" + m_userRole.toUpper() + "</b></p>"
                                                          "<hr>"
                                                          "<p style='color:gray; font-size:10px;'>© Selim Fekih - PFE 2026</p>",
                             QMessageBox::Ok);

    // Optionnel : afficher le nom en haut de la fenêtre
    setWindowTitle("Smart Media Agency × Connect+ | " + name + " (" + m_userRole + ")");
    arduino->write(("LOGIN:" + name + "\n").toUtf8());
}
void MainWindow::mettreEnLiveStudio(const QString &nomCreateur)
{
    if (!arduino || !arduino->isOpen()) return;

    QString cmd = "LIVE:" + nomCreateur + "\n";
    arduino->write(cmd.toUtf8());
    arduino->flush();

    // Insertion BDD
    QSqlQuery q;
    q.prepare("INSERT INTO LIVE_SESSIONS (createur_nom, date_debut) VALUES (:nom, CURRENT_TIMESTAMP)");
    q.bindValue(":nom", nomCreateur);
    q.exec();

    // Effet visuel
    ui->contentCreatorPage->setStyleSheet("background-color: #ffeeee;");
    ui->label_live_status->setText("EN DIRECT LIVE : " + nomCreateur.toUpper());
    ui->label_live_status->setStyleSheet("color: red; font-size: 20px; font-weight: bold;");
}

void MainWindow::arreterLiveStudio()
{
    if (!arduino || !arduino->isOpen()) return;

    arduino->write("STOP\n");
    arduino->flush();

    QSqlQuery q;
    q.exec("UPDATE LIVE_SESSIONS SET date_fin = CURRENT_TIMESTAMP WHERE date_fin IS NULL");

    ui->contentCreatorPage->setStyleSheet("");
    ui->label_live_status->setText("");
}

// === CES DEUX SLOTS SONT DÉJÀ CONNECTÉS AUTOMATIQUEMENT PAR QT ===
void MainWindow::on_pushButton_live_clicked()
{
    QString nom = ui->nameEdit_6->text().trimmed();

    // Si vide → on prend la ligne sélectionnée dans le tableau
    int row = ui->creatorTable_6->currentRow();
    if (nom.isEmpty() && row >= 0) {
        nom = ui->creatorTable_6->item(row, 1)->text(); // colonne Nom
    }

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Saisis un nom ou sélectionne un créateur !");
        return;
    }

    mettreEnLiveStudio(nom);
}

void MainWindow::on_pushButton_stop_live_clicked()
{
    arreterLiveStudio();
}
//finance implementation des fonction
// === CORRECTION FINALE - TABLE MY_USER.FACTURE ===
void MainWindow::loadInvoicesFromDatabase() {
    ui->tableFactures->setRowCount(0);
    QSqlQuery query;
    query.exec("SELECT F.ID_FACTURE, F.MONTANT, F.DATE_EMISSION, F.DATE_ECHEANCE, F.STATUT, "
               "E.NOM || ' ' || E.PRENOM AS EMPLOYE, S.NOM AS SPONSOR "
               "FROM MY_USER.FACTURE F "
               "LEFT JOIN MY_USER.EMPLOYE E ON F.ID_EMPLOYE = E.IDEMPLOYE "
               "LEFT JOIN MY_USER.SPONSOR S ON F.ID_SPONSOR = S.IDSPONSOR "
               "ORDER BY F.ID_FACTURE");

    while(query.next()) {
        int row = ui->tableFactures->rowCount();
        ui->tableFactures->insertRow(row);

        ui->tableFactures->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableFactures->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toDouble(), 'f', 2)));
        ui->tableFactures->setItem(row, 2, new QTableWidgetItem(query.value(2).toDate().toString("dd/MM/yyyy")));
        ui->tableFactures->setItem(row, 3, new QTableWidgetItem(query.value(3).toDate().toString("dd/MM/yyyy")));
        ui->tableFactures->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableFactures->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // Employé
        ui->tableFactures->setItem(row, 6, new QTableWidgetItem(query.value(6).toString())); // Sponsor
    }
    updateVisualStatistics();
}

void MainWindow::addInvoice() {
    QString id = ui->lineId->text().trimmed();
    double montant = ui->lineMontant->text().toDouble();
    QDate dateEmission = ui->dateEmission->date();
    QDate dateEcheance = ui->dateEcheance->date();
    QString statut = ui->comboStatut->currentText();

    if (id.isEmpty() || montant <= 0 || dateEcheance <= dateEmission) {
        QMessageBox::warning(this, "Erreur", "Vérifiez les champs (ID, montant, dates) !");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO MY_USER.FACTURE (ID_FACTURE, MONTANT, DATE_EMISSION, DATE_ECHEANCE, STATUT, ID_EMPLOYE, ID_SPONSOR) "
              "VALUES (:id, :montant, :emission, :echeance, :statut, :employe, :sponsor)");
    q.bindValue(":id", id.toInt());
    q.bindValue(":montant", montant);
    q.bindValue(":emission", dateEmission);
    q.bindValue(":echeance", dateEcheance);
    q.bindValue(":statut", statut);
    q.bindValue(":employe", 1);        // à dynamiser plus tard
    q.bindValue(":sponsor", 1);        // à dynamiser plus tard

    if (q.exec()) {
        QMessageBox::information(this, "Succès", "Facture ajoutée !");
        loadInvoicesFromDatabase();
        clearForm();
    } else {
        QMessageBox::critical(this, "Erreur", q.lastError().text());
    }
}

void MainWindow::editInvoice() {
    int row = ui->tableFactures->currentRow();
    if (row < 0) return;

    int id = ui->tableFactures->item(row, 0)->text().toInt();
    double montant = ui->lineMontant->text().toDouble();
    QDate emission = ui->dateEmission->date();
    QDate echeance = ui->dateEcheance->date();
    QString statut = ui->comboStatut->currentText();

    QSqlQuery q;
    q.prepare("UPDATE MY_USER.FACTURE SET MONTANT = :montant, DATE_EMISSION = :emission, "
              "DATE_ECHEANCE = :echeance, STATUT = :statut WHERE ID_FACTURE = :id");
    q.bindValue(":montant", montant);
    q.bindValue(":emission", emission);
    q.bindValue(":echeance", echeance);
    q.bindValue(":statut", statut);
    q.bindValue(":id", id);

    if (q.exec()) {
        QMessageBox::information(this, "Succès", "Facture modifiée !");
        loadInvoicesFromDatabase();
        clearForm();
    } else {
        QMessageBox::critical(this, "Erreur", q.lastError().text());
    }
}

void MainWindow::deleteInvoice() {
    int row = ui->tableFactures->currentRow();
    if (row < 0) return;

    if (QMessageBox::question(this, "Confirmer", "Supprimer cette facture ?") != QMessageBox::Yes)
        return;

    int id = ui->tableFactures->item(row, 0)->text().toInt();
    QSqlQuery q;
    q.prepare("DELETE FROM MY_USER.FACTURE WHERE ID_FACTURE = :id");
    q.bindValue(":id", id);

    if (q.exec()) {
        QMessageBox::information(this, "Supprimé", "Facture supprimée !");
        loadInvoicesFromDatabase();
    } else {
        QMessageBox::critical(this, "Erreur", q.lastError().text());
    }
}

// =================== HELPER FUNCTIONS ===================

void MainWindow::onInvoiceSelected() {
    int currentRow = ui->tableFactures->currentRow();
    if(currentRow >= 0) {
        // Get data from selected row
        QString id = ui->tableFactures->item(currentRow, 0)->text();
        QString amount = ui->tableFactures->item(currentRow, 1)->text();
        QString issueDateStr = ui->tableFactures->item(currentRow, 2)->text();
        QString dueDateStr = ui->tableFactures->item(currentRow, 3)->text();
        QString status = ui->tableFactures->item(currentRow, 4)->text();

        // Fill the form (ID field is read-only for editing)
        ui->lineId->setText(id);
        ui->lineId->setEnabled(false); // Disable ID field during edit
        ui->lineMontant->setText(amount);

        // Parse dates
        QDate issueDate = QDate::fromString(issueDateStr, "dd/MM/yyyy");
        QDate dueDate = QDate::fromString(dueDateStr, "dd/MM/yyyy");
        if(issueDate.isValid()) ui->dateEmission->setDate(issueDate);
        if(dueDate.isValid()) ui->dateEcheance->setDate(dueDate);

        // Set status
        int index = ui->comboStatut->findText(status);
        if(index >= 0) ui->comboStatut->setCurrentIndex(index);
    }
}

void MainWindow::insertInvoiceInTable(QString id, double amount, QDate issueDate, QDate dueDate, QString status) {
    int row = ui->tableFactures->rowCount();
    ui->tableFactures->insertRow(row);
    ui->tableFactures->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableFactures->setItem(row, 1, new QTableWidgetItem(QString::number(amount, 'f', 2)));
    ui->tableFactures->setItem(row, 2, new QTableWidgetItem(issueDate.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 3, new QTableWidgetItem(dueDate.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 4, new QTableWidgetItem(status));
}

void MainWindow::clearForm() {
    ui->lineId->clear();
    ui->lineId->setEnabled(true); // Re-enable ID field for new entries
    ui->lineMontant->clear();
    ui->dateEmission->setDate(QDate::currentDate());
    ui->dateEcheance->setDate(QDate::currentDate().addDays(30)); // Default 30 days later
    ui->comboStatut->setCurrentIndex(0);
}

void MainWindow::sortByAmount() {
    ui->tableFactures->sortItems(1, Qt::AscendingOrder);
}

void MainWindow::searchById() {
    QString searchId = ui->searchBox->text().trimmed();
    if(searchId.isEmpty()) {
        // Show all rows if search is empty
        for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
            ui->tableFactures->setRowHidden(i, false);
        }
        return;
    }

    for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
        bool match = ui->tableFactures->item(i,0)->text().contains(searchId, Qt::CaseInsensitive);
        ui->tableFactures->setRowHidden(i, !match);
    }
}

void MainWindow::exportToCSV() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export to CSV", "", "CSV Files (*.csv)");
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "Invoice ID;Amount;Issue Date;Due Date;Status\n";

        for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
            // Only export visible rows (for search functionality)
            if(!ui->tableFactures->isRowHidden(i)) {
                for(int j = 0; j < ui->tableFactures->columnCount(); ++j) {
                    stream << ui->tableFactures->item(i, j)->text();
                    if(j < ui->tableFactures->columnCount() - 1) stream << ";";
                }
                stream << "\n";
            }
        }
        file.close();
        QMessageBox::information(this, "Success", "Data exported successfully to CSV!");
    } else {
        QMessageBox::warning(this, "Error", "Could not export to CSV file.");
    }
}

void MainWindow::toggleDarkTheme() {
    darkTheme = !darkTheme;
    if(darkTheme) {
        applyDarkTheme();
        ui->btnDarkTheme->setText("Light Theme");
    } else {
        applyLightTheme();
        ui->btnDarkTheme->setText("Dark Theme");
    }
}

void MainWindow::initializeVisualStatistics() {
    // Set initial values
    ui->labelTotalValue->setText("0");
    ui->labelAmountValue->setText("$0.00");
    ui->labelPaidValue->setText("0");
    ui->labelPendingValue->setText("0");
    ui->labelCancelledValue->setText("0");
    ui->labelAverageValue->setText("$0.00");

    // Initialize progress bars
    ui->progressPaid->setValue(0);
    ui->progressPending->setValue(0);
    ui->progressCancelled->setValue(0);
}

void MainWindow::updateVisualStatistics() {
    int totalInvoices = 0;
    double totalAmount = 0.0;
    int paidCount = 0;
    int pendingCount = 0;
    int cancelledCount = 0;

    for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
        // Only count visible rows (for search functionality)
        if(!ui->tableFactures->isRowHidden(i)) {
            totalInvoices++;
            double amount = ui->tableFactures->item(i, 1)->text().toDouble();
            totalAmount += amount;

            QString status = ui->tableFactures->item(i, 4)->text();
            if(status == "Paid") paidCount++;
            else if(status == "Pending") pendingCount++;
            else if(status == "Cancelled") cancelledCount++;
        }
    }

    double averageAmount = totalInvoices > 0 ? totalAmount / totalInvoices : 0.0;

    // Update visual elements
    ui->labelTotalValue->setText(QString::number(totalInvoices));
    ui->labelAmountValue->setText(QString("$%1").arg(QString::number(totalAmount, 'f', 2)));
    ui->labelPaidValue->setText(QString::number(paidCount));
    ui->labelPendingValue->setText(QString::number(pendingCount));
    ui->labelCancelledValue->setText(QString::number(cancelledCount));
    ui->labelAverageValue->setText(QString("$%1").arg(QString::number(averageAmount, 'f', 2)));

    // Update progress bars (percentage of total)
    int totalVisible = totalInvoices;
    if(totalVisible > 0) {
        ui->progressPaid->setValue((paidCount * 100) / totalVisible);
        ui->progressPending->setValue((pendingCount * 100) / totalVisible);
        ui->progressCancelled->setValue((cancelledCount * 100) / totalVisible);
    } else {
        ui->progressPaid->setValue(0);
        ui->progressPending->setValue(0);
        ui->progressCancelled->setValue(0);
    }
}

void MainWindow::applyLightTheme() {
   // this->setStyleSheet(R"(
       // QMainWindow { background-color: #FFFFFF; border-radius: 10px; }
      //  QGroupBox { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; font: bold 14pt "Arial"; color: #7D4FEE; margin: 10px; }
      //  QPushButton { background-color: #7D4FEE; color: #FFFFFF; border-radius: 10px; padding: 8px; min-width: 100px; font: 10pt "Arial"; border: 2px solid #7D4FEE; }
       // QPushButton:hover { background-color: #FFFFFF; color: #7D4FEE; border: 2px solid #7D4FEE; }
       // QLineEdit, QComboBox, QDateEdit { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; padding: 6px; font: 10pt "Arial"; color: #7D4FEE; }
       // QTableWidget { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; gridline-color: #7D4FEE; font: 10pt "Arial"; color: #7D4FEE; }
       // QLabel { color: #7D4FEE; font: 10pt "Arial"; }
       // QFrame { background-color: #F8F9FA; border: 1px solid #E0E0E0; border-radius: 8px; padding: 10px; }
       // QProgressBar { border: 1px solid #7D4FEE; border-radius: 4px; text-align: center; background-color: #FFFFFF; }
      //  QProgressBar::chunk { border-radius: 3px; }
       // QWidget#menuFrame { background-color: #7D4FEE; border-radius: 10px 0 0 10px; }
   // )");
}

void MainWindow::applyDarkTheme() {
    //this->setStyleSheet(R"(
      //  QMainWindow { background-color: #000000; }
       // QGroupBox { background-color: #000000; border: 2px solid #7D4FEE; color: #7D4FEE; }
       // QLineEdit, QComboBox, QDateEdit { background-color: #000000; border: 2px solid #7D4FEE; color: #7D4FEE; }
       // QTableWidget { background-color: #1A1A1A; border: 2px solid #7D4FEE; color: #7D4FEE; }
       // QLabel { color: #7D4FEE; }
      //  QFrame { background-color: #2D2D2D; border: 1px solid #7D4FEE; border-radius: 8px; padding: 10px; }
      //  QProgressBar { border: 1px solid #7D4FEE; border-radius: 4px; text-align: center; background-color: #1A1A1A; color: white; }
      //  QProgressBar::chunk { border-radius: 3px; }
      //  QPushButton { background-color: #7D4FEE; color: #FFFFFF; border-radius: 10px; padding: 8px; min-width: 100px; font: 10pt "Arial"; border: 2px solid #7D4FEE; }
       // QPushButton:hover { background-color: #FFFFFF; color: #7D4FEE; border: 2px solid #7D4FEE; }
      //  QWidget#menuFrame { background-color: #7D4FEE; border-radius: 10px 0 0 10px; }
   // )");
}
