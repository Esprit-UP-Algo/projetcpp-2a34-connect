#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include "employes.h"
#include "curvewidget.h"
#include "barchartwidget.h"
#include "fichedepaie.h"
#include "historymanager.h"
#include "smtp.h"
#include "serialmanager.h"
#include "qrcodegenerator.h"
#include "arduino.h"
#include "calendardialog.h"

#include <QClipboard>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QTableWidgetItem>
#include <QDebug>
#include <QRegularExpression>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFormLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QTimer>
#include <QStatusBar>
#include <QHeaderView>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegularExpressionValidator>
#include <QTextEdit>
#include <QInputDialog>
#include <QFileInfo>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QRandomGenerator>
#include <QLocale>
#include <QThread>

// ==================== FONCTION D'INITIALISATION BD ====================

void MainWindow::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non connectée";
        QMessageBox::critical(this, "Erreur BD", "La base de données n'est pas connectée!");
        return;
    }

    qDebug() << "🔧 Initialisation de la base de données...";
    qDebug() << "   Type:" << db.driverName();

    // Détecter le nom exact de la table
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkOracle("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkOracle.next()) {
            tableName = checkOracle.value(0).toString();
        }
    } else {
        // SQLite
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) {
        qDebug() << "⚠️ Table EMPOLYE non trouvée";
        qDebug() << "Tables disponibles:";
        QSqlQuery debugQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (debugQuery.next()) {
            qDebug() << "  -" << debugQuery.value(0).toString();
        }
        return;
    }

    qDebug() << "   ✓ Table trouvée:" << tableName;

    // Vérifier et ajouter la colonne CODE_BADGE si nécessaire
    bool hasCodeBadge = false;

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkCol(QString("SELECT COUNT(*) FROM USER_TAB_COLUMNS WHERE TABLE_NAME = '%1' AND COLUMN_NAME = 'CODE_BADGE'").arg(tableName.toUpper()));
        if (checkCol.next()) {
            hasCodeBadge = (checkCol.value(0).toInt() > 0);
        }
    } else {
        // SQLite
        QSqlQuery checkColumn(QString("PRAGMA table_info(%1)").arg(tableName));
        while (checkColumn.next()) {
            QString columnName = checkColumn.value(1).toString();
            if (columnName.toUpper() == "CODE_BADGE") {
                hasCodeBadge = true;
                break;
            }
        }
    }

    if (!hasCodeBadge) {
        qDebug() << "   ⚠️ Colonne CODE_BADGE manquante - Création...";
        QSqlQuery alterQuery;
        QString alterSql;

        if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
            alterSql = QString("ALTER TABLE %1 ADD CODE_BADGE VARCHAR2(20)").arg(tableName);
        } else {
            alterSql = QString("ALTER TABLE %1 ADD COLUMN CODE_BADGE TEXT").arg(tableName);
        }

        if (alterQuery.exec(alterSql)) {
            qDebug() << "   ✅ Colonne CODE_BADGE ajoutée avec succès";
        } else {
            qDebug() << "   ❌ Erreur ajout CODE_BADGE:" << alterQuery.lastError().text();
        }
    } else {
        qDebug() << "   ✓ Colonne CODE_BADGE existe";
    }

    // Vérifier et créer la table HISTORIQUE_ACCES si nécessaire
    bool hasHistory = false;

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkHist("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'HISTORIQUE_ACCES'");
        if (checkHist.next()) {
            hasHistory = (checkHist.value(0).toInt() > 0);
        }
    } else {
        QSqlQuery checkHistory("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='HISTORIQUE_ACCES'");
        if (checkHistory.next()) {
            hasHistory = (checkHistory.value(0).toInt() > 0);
        }
    }

    if (!hasHistory) {
        qDebug() << "   ⚠️ Table HISTORIQUE_ACCES manquante - Création...";
        QSqlQuery createHistory;

        if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
            createHistory.exec("CREATE TABLE HISTORIQUE_ACCES ("
                               "ID_ACCES NUMBER GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY, "
                               "ID_EMPLOYE NUMBER, "
                               "NOM_EMPLOYE VARCHAR2(100), "
                               "HEURE_ACCES TIMESTAMP, "
                               "TYPE_ACCES VARCHAR2(10))");
        } else {
            createHistory.exec("CREATE TABLE HISTORIQUE_ACCES ("
                               "ID_ACCES INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "ID_EMPLOYE INTEGER, "
                               "NOM_EMPLOYE TEXT, "
                               "HEURE_ACCES DATETIME, "
                               "TYPE_ACCES TEXT)");
        }

        if (createHistory.lastError().isValid()) {
            qDebug() << "   ❌ Erreur création HISTORIQUE_ACCES:" << createHistory.lastError().text();
        } else {
            qDebug() << "   ✅ Table HISTORIQUE_ACCES créée";
        }
    } else {
        qDebug() << "   ✓ Table HISTORIQUE_ACCES existe";
    }

    qDebug() << "✅ Initialisation de la base de données terminée\n";
}

// ==================== FONCTION POUR ASSIGNER DES BADGES PAR DÉFAUT ====================

void MainWindow::assignDefaultBadgesAutomatically()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    qDebug() << "🔄 Assignation automatique des codes badges...";

    // Vérifier combien d'employés n'ont pas de code badge
    QSqlQuery checkQuery("SELECT COUNT(*) FROM empolye WHERE CODE_BADGE IS NULL OR CODE_BADGE = '' OR CODE_BADGE = 'Non assigné'");
    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        qDebug() << "   " << checkQuery.value(0).toInt() << "employés sans code badge";

        // Assigner des codes par défaut
        QStringList defaultCodes = {"0888", "9999", "1234", "5678", "9012", "3456", "7890", "1111", "2222", "3333"};

        QSqlQuery selectQuery("SELECT IDEMPLOYE FROM empolye ORDER BY IDEMPLOYE");
        int index = 0;

        while (selectQuery.next() && index < defaultCodes.size()) {
            int id = selectQuery.value(0).toInt();

            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE empolye SET CODE_BADGE = ? WHERE IDEMPLOYE = ?");
            updateQuery.addBindValue(defaultCodes[index]);
            updateQuery.addBindValue(id);

            if (updateQuery.exec()) {
                qDebug() << "   ✅ ID" << id << "→ Code:" << defaultCodes[index];
            }

            index++;
        }

        qDebug() << "✅ Assignation automatique terminée";
    } else {
        qDebug() << "✅ Tous les employés ont déjà un code badge";
    }
}

// ==================== CONSTRUCTEUR ====================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , idEmployeSelectionne(-1)
    , idSponsorSelectionne(-1)
    , chatbot(new Chatbot())
    , smsManager(new SMSManager())
    , serialManager(new SerialManager(this))
    , arduino(new Arduino(this))
    , darkTheme(false)
    , editedRow(-1)
    , autoExportDone(false)
{
    ui->setupUi(this);

    // *** INITIALISATION DE LA BASE DE DONNÉES ***
    initDatabase();

    // *** ASSIGNATION AUTOMATIQUE DES BADGES ***
    assignDefaultBadgesAutomatically();

    qDebug() << "Arduino sera géré par SerialManager uniquement...";

    // === INITIALISATION ARDUINO ===
    // NE PAS connecter Arduino ici - SerialManager s'en occupera plus tard
    // Gardez la connexion pour la lecture des données seulement
    connect(arduino, &Arduino::dataReceived, this, [](const QByteArray &data) {
        qDebug() << "Arduino dit :" << data.trimmed();
    });

    // === CONNEXION CREATEUR ===
    ui->creatorTable_7->setAttribute(Qt::WA_StyledBackground, true);
    ui->tableFactures->setAttribute(Qt::WA_StyledBackground, true);

    // === FINANCE TABLE ===
    ui->tableFactures->setColumnCount(7);
    ui->tableFactures->setHorizontalHeaderLabels({"Invoice ID","Amount","Issue Date","Due Date","Status","Employee","Sponsor"});

    // === CONTENT CREATOR INIT ===
    ui->platformCombo_7->setCurrentText("YouTube");
    ui->comboBox_7->setCurrentText("male");
    ui->themeButton_7->setText("Dark Theme");

    // Content Creator connections
    connect(ui->searchEdit_9, &QLineEdit::textChanged, this, &MainWindow::on_searchEdit_9_textChanged);
    connect(ui->creatorTable_7, &QTableWidget::itemSelectionChanged, this, &MainWindow::fillFormFromTable);
    connect(ui->addButton_7, &QPushButton::clicked, this, &MainWindow::on_addButton_7_clicked);
    connect(ui->updateButton_7, &QPushButton::clicked, this, &MainWindow::on_updateButton_7_clicked);
    connect(ui->deleteButton_7, &QPushButton::clicked, this, &MainWindow::on_deleteButton_7_clicked);
    connect(ui->clearButton_7, &QPushButton::clicked, this, &MainWindow::on_clearButton_7_clicked);
    connect(ui->exportButton_7, &QPushButton::clicked, this, &MainWindow::on_exportButton_7_clicked);
    connect(ui->themeButton_7, &QPushButton::clicked, this, &MainWindow::on_themeButton_7_clicked);

    // QR Code connections
    connect(ui->btnGenerateQR_2, &QPushButton::clicked, this, &MainWindow::generateQR);
    connect(ui->btnSaveQR_2, &QPushButton::clicked, this, &MainWindow::saveQR);
    connect(ui->btnCopyQR_2, &QPushButton::clicked, this, &MainWindow::copyQR);
    loadCreateurs();

    // === FINANCE CONNECTIONS ===
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::addInvoice);
    connect(ui->btnEdit, &QPushButton::clicked, this, &MainWindow::editInvoice);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::deleteInvoice);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::clearForm);
    connect(ui->btnSortByMontant, &QPushButton::clicked, this, &MainWindow::sortByAmount);
    connect(ui->btnSearchById, &QPushButton::clicked, this, &MainWindow::searchById);
    connect(ui->btnExportCSV, &QPushButton::clicked, this, &MainWindow::exportToCSV);
    connect(ui->btnDarkTheme, &QPushButton::clicked, this, &MainWindow::toggleDarkTheme);
    connect(ui->tableFactures, &QTableWidget::itemSelectionChanged, this, &MainWindow::onInvoiceSelected);

    // === DEAL PAGE VALIDATORS ===
    ui->lineEditID_deal->setValidator(new QIntValidator(1, 999999, this));
    ui->lineEditID_deal->setMaxLength(6);

    // === STATS TABLE ===
    ui->tableStats_deal->setColumnCount(4);
    ui->tableStats_deal->setHorizontalHeaderLabels(QStringList() << "Material" << "In Stock" << "Booked" << "Usage %");
    ui->tableStats_deal->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableStats_deal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableStats_deal->setSelectionMode(QAbstractItemView::SingleSelection);
    updateStatsTable();

    // --- 1. NETTOYAGE & INITIALISATION ---
    ui->lineEdit_6->clear();

    // --- 2. CONNEXIONS MANUELLES ---
    connect(ui->lineEdit_2, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_25_clicked);
    connect(ui->lineEdit_4, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_26_clicked);

    // --- 3. PLACEHOLDERS ---
    ui->lineEdit_12->setPlaceholderText("Généré Auto");
    ui->lineEdit_13->setPlaceholderText("Lettres uniquement");
    ui->lineEdit_14->setPlaceholderText("Lettres uniquement");
    ui->lineEdit_15->setPlaceholderText("8 Chiffres exacts");
    ui->lineEdit_16->setPlaceholderText("Format: MM/JJ/AAAA");
    ui->lineEdit_17->setPlaceholderText("exemple@email.com");
    ui->lineEdit_18->setPlaceholderText("8 Chiffres exacts");
    ui->lineEdit_19->setPlaceholderText("M ou F");
    ui->lineEdit_20->setPlaceholderText("Ex: 1200.50");
    ui->lineEdit_21->setPlaceholderText("Chiffres uniquement");
    ui->lineEdit_6->setPlaceholderText("recherche par nom, prenom, cin");

    // --- 4. VALIDATORS ---
    QRegularExpression regexLettres("^[a-zA-ZÀ-ÿ\\s'-]*$");
    QValidator *validatorLettres = new QRegularExpressionValidator(regexLettres, this);
    ui->lineEdit_13->setValidator(validatorLettres);
    ui->lineEdit_14->setValidator(validatorLettres);

    QRegularExpression regexChiffres("^[0-9]*$");
    QValidator *validatorChiffres = new QRegularExpressionValidator(regexChiffres, this);
    ui->lineEdit_15->setValidator(validatorChiffres); ui->lineEdit_15->setMaxLength(8);
    ui->lineEdit_18->setValidator(validatorChiffres); ui->lineEdit_18->setMaxLength(8);

    ui->lineEdit_19->setMaxLength(1);
    ui->lineEdit_21->setValidator(new QIntValidator(0, 999999, this));

    QDoubleValidator *salaireVal = new QDoubleValidator(0.0, 999999.0, 2, this);
    salaireVal->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_20->setValidator(salaireVal);

    // --- 5. CONNEXION SERIAL MANAGER ---
    if (serialManager) {
        // Connexion pour recevoir les codes badges
        connect(serialManager, &SerialManager::dataReceived, this, &MainWindow::onArduinoDataReceived);

        // Ajoutez ces trois lignes :
        connect(serialManager, &SerialManager::codeReceived, this, &MainWindow::onCodeReceived);
        connect(serialManager, &SerialManager::accessGranted, this, &MainWindow::onAccessGranted);
        connect(serialManager, &SerialManager::accessDenied, this, &MainWindow::onAccessDenied);

        // Connexion pour les états de la porte
        connect(serialManager, &SerialManager::porteOuverte, this, &MainWindow::onPorteOuverte);
        connect(serialManager, &SerialManager::porteFermee, this, &MainWindow::onPorteFermee);

        // Connexion pour le statut
        connect(serialManager, &SerialManager::connected, this, &MainWindow::onArduinoConnected);
        connect(serialManager, &SerialManager::disconnected, this, &MainWindow::onArduinoDisconnected);
        connect(serialManager, &SerialManager::error, this, &MainWindow::onArduinoError);
    }
    QList<QPushButton*> allButtons = findChildren<QPushButton*>();
    for (QPushButton* btn : allButtons) {
        QString btnText = btn->text().toLower();
        QString objName = btn->objectName().toLower();

        if (btnText.contains("badge") || btnText.contains("assign") ||
            btnText.contains("utron") || objName.contains("badge") ||
            objName.contains("assign")) {

            qDebug() << "   ✅ Bouton trouvé:" << objName << "| Texte:" << btn->text();

            if (btnText.contains("assign") || objName.contains("assign")) {
                connect(btn, &QPushButton::clicked, this, &MainWindow::on_pushButton_assignBadge_clicked);
                qDebug() << "   🔗 Connecté à on_pushButton_assignBadge_clicked";
            }

            if (btnText.contains("remove") || btnText.contains("supprim") ||
                btnText.contains("zitor")) {
                connect(btn, &QPushButton::clicked, this, &MainWindow::on_pushButton_removeBadge_clicked);
                qDebug() << "   🔗 Connecté à on_pushButton_removeBadge_clicked";
            }
        }
    }

    // --- 7. CRÉATION MENU POUR LES BOUTONS ARDUINO ---
    QMenu *badgeMenu = menuBar()->addMenu("Badge Arduino");
    QAction *assignBadgeAction = badgeMenu->addAction("Assigner Code Badge");
    QAction *removeBadgeAction = badgeMenu->addAction("Supprimer Code Badge");
    QAction *showHistoryAction = badgeMenu->addAction("Historique Accès");
    QAction *testBadgeAction = badgeMenu->addAction("TEST: Scanner 0888");
    QAction *testServoAction = badgeMenu->addAction("TEST: Servo");

    connect(assignBadgeAction, &QAction::triggered, this, &MainWindow::on_pushButton_assignBadge_clicked);
    connect(removeBadgeAction, &QAction::triggered, this, &MainWindow::on_pushButton_removeBadge_clicked);
    connect(showHistoryAction, &QAction::triggered, this, &MainWindow::on_btnShowAccessHistory_clicked);
    connect(testBadgeAction, &QAction::triggered, [this]() {
        processBadgeCode("0888");
    });
    connect(testServoAction, &QAction::triggered, [this]() {
        if (serialManager && serialManager->isConnected()) {
            serialManager->sendCommand("AUTORISE:Test");
        }
    });

    // --- 8. CHARGEMENT DES DONNÉES ---
    initEmployesTable();
    initSponsorsTable();
    afficherEmployes();
    afficherSponsors();

    ui->lineEdit->setText("🤖 Bonjour ! Je suis prêt à vous aider.");
    ui->pushButton_stats->setText("📊 Stats Graphiques");

    // --- 9. CONNEXION ARDUINO DÉLAYÉE ---
    QTimer::singleShot(2000, this, [this]() {
        qDebug() << "Tentative de connexion Arduino via SerialManager...";

        // D'abord, déconnecter Arduino s'il est connecté
        if (arduino->isConnected()) {
            arduino->disconnectArduino();
            qDebug() << "Arduino déconnecté pour libérer le port...";
            QThread::msleep(500);
        }

        // Obtenir les ports disponibles
        QStringList ports = serialManager->getAvailablePorts();
        qDebug() << "Ports disponibles:" << ports;

        bool connected = false;
        for (const QString &port : ports) {
            qDebug() << "Essai du port:" << port;
            if (serialManager->connectToArduino(port)) {
                connected = true;
                ui->statusbar->showMessage("✅ Arduino connecté sur " + port, 3000);
                qDebug() << "✅ SerialManager connecté sur" << port;

                // Attendre un peu pour que l'Arduino s'initialise
                QTimer::singleShot(1000, [this]() {
                    if (serialManager->isConnected()) {
                        serialManager->sendCommand("INIT");
                    }
                });
                break;
            }
        }

        if (!connected) {
            ui->statusbar->showMessage("⚠️ Arduino non connecté - Vérifiez le port COM", 5000);
            qDebug() << "❌ Aucun port Arduino trouvé";
        }
    });

    // --- 10. INITIALISATION FINANCE ===
    initializeVisualStatistics();
    loadInvoicesFromDatabase();

    // --- 11. INITIALISATION SPONSOR VALIDATORS ===
    setupSponsorValidators();

    qDebug() << "--- CONSTRUCTEUR MAINWINDOW TERMINÉ ---";
}

MainWindow::~MainWindow()
{
    delete chatbot;
    delete smsManager;
    delete serialManager;
    delete arduino;
    delete ui;
}

// ==================== NAVIGATION ====================
void MainWindow::on_sidebarButton_clicked() { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::on_sidebarButton_2_clicked() { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::on_sidebarButton_3_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_sidebarButton_4_clicked() { ui->stackedWidget->setCurrentIndex(3); afficherSponsors(); }
void MainWindow::on_sidebarButton_5_clicked() { ui->stackedWidget->setCurrentIndex(4); }
void MainWindow::on_sidebarButton12_clicked() { ui->stackedWidget->setCurrentWidget(ui->employeePage); afficherEmployes(); }

// ==================== TABLEAUX ====================
void MainWindow::initEmployesTable() {
    ui->tableWidget->setColumnCount(10);
    QStringList headers;
    headers << "ID" << "Nom Complet" << "Date Emb." << "Email" << "Tél"
            << "Sexe" << "Poste" << "Salaire" << "Superviseur" << "Code Badge";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::initSponsorsTable() {
    ui->sponsorTable_9->setColumnCount(8);
    QStringList headers; headers << "ID" << "Nom" << "Prénom" << "Catégorie" << "Email" << "Début" << "Fin" << "Budget";
    ui->sponsorTable_9->setHorizontalHeaderLabels(headers);
    ui->sponsorTable_9->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sponsorTable_9->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->sponsorTable_9->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

// ==================== SPONSOR VALIDATORS ====================
void MainWindow::setupSponsorValidators()
{
    QRegularExpression regexLettres("^[a-zA-ZÀ-ÿ\\s'-]*$");
    QValidator *validatorLettres = new QRegularExpressionValidator(regexLettres, this);

    // Page Ajout
    ui->nom->setValidator(validatorLettres);
    ui->prenom->setValidator(validatorLettres);

    // Page Modification
    ui->nom2->setValidator(validatorLettres);
    ui->prenom2->setValidator(validatorLettres);

    // Budget validators
    QIntValidator *validatorBudget = new QIntValidator(0, 999999999, this);
    ui->budget->setValidator(validatorBudget);
    ui->budget2->setValidator(validatorBudget);

    qDebug() << "✅ Validateurs sponsors configurés avec succès";
}

bool MainWindow::validerChampsSponsor()
{
    QStringList erreurs;

    // Validation Nom
    QString nom = ui->nom->text().trimmed();
    if (nom.isEmpty()) {
        erreurs << "❌ Le nom est obligatoire";
    } else if (nom.length() < 2) {
        erreurs << "❌ Le nom doit contenir au moins 2 caractères";
    } else if (!Sponsor::validerNom(nom)) {
        erreurs << "❌ Le nom ne doit contenir que des lettres";
    }

    // Validation Prénom
    QString prenom = ui->prenom->text().trimmed();
    if (prenom.isEmpty()) {
        erreurs << "❌ Le prénom est obligatoire";
    } else if (prenom.length() < 2) {
        erreurs << "❌ Le prénom doit contenir au moins 2 caractères";
    } else if (!Sponsor::validerNom(prenom)) {
        erreurs << "❌ Le prénom ne doit contenir que des lettres";
    }

    // Validation Email
    QString email = ui->email->text().trimmed();
    if (email.isEmpty()) {
        erreurs << "❌ L'email est obligatoire";
    } else if (!Sponsor::validerEmail(email)) {
        erreurs << "❌ Format d'email invalide (doit contenir @)";
    }

    // Validation Catégorie
    if (ui->categorie->text().trimmed().isEmpty()) {
        erreurs << "❌ La catégorie est obligatoire";
    }

    // Validation Budget
    bool ok;
    int budget = ui->budget->text().toInt(&ok);
    if (!ok) {
        erreurs << "❌ Le budget doit être un nombre";
    } else if (budget < 0) {
        erreurs << "❌ Le budget ne peut pas être négatif";
    }

    // Validation Dates
    if (!Sponsor::validerDates(ui->debut->date(), ui->fin->date())) {
        erreurs << "❌ La date de début doit être avant la date de fin";
    }

    if (!erreurs.isEmpty()) {
        QString message = "Erreurs de validation :\n\n" + erreurs.join("\n");
        QMessageBox::warning(this, "Erreurs de Saisie", message);
        return false;
    }

    return true;
}

void MainWindow::clearSponsorValidationErrors()
{
    // Page Ajout
    ui->nom->setStyleSheet("");
    ui->prenom->setStyleSheet("");
    ui->email->setStyleSheet("");
    ui->budget->setStyleSheet("");
    ui->debut->setStyleSheet("");
    ui->fin->setStyleSheet("");

    // Page Modification
    ui->nom2->setStyleSheet("");
    ui->prenom2->setStyleSheet("");
    ui->email2->setStyleSheet("");
    ui->budget2->setStyleSheet("");
    ui->debut2->setStyleSheet("");
    ui->fin2->setStyleSheet("");
}

// ==================== AFFICHAGE STANDARD ====================
void MainWindow::afficherEmployes() {
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non connectée!";
        return;
    }

    qDebug() << "🔍 Début afficherEmployes()...";
    qDebug() << "   Driver:" << db.driverName();
    qDebug() << "   Base ouverte:" << db.isOpen();

    QString actualTableName = "";

    // CORRECTION IMPORTANTE : détecter correctement Oracle
    bool isOracle = db.driverName().contains("QODBC", Qt::CaseInsensitive) ||
                    db.driverName().contains("ORACLE", Qt::CaseInsensitive);

    qDebug() << "   isOracle:" << isOracle;

    // Détecter le nom exact de la table
    if (isOracle) {
        qDebug() << "🔍 Recherche table Oracle...";

        // CORRECTION : utiliser la bonne requête pour Oracle
        QSqlQuery checkQuery;

        // Essayer d'abord EMPOLYE
        if (checkQuery.exec("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'EMPOLYE'")) {
            if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
                actualTableName = "EMPOLYE";
                qDebug() << "✅ Table EMPOLYE trouvée dans Oracle";
            } else {
                // Essayer EMPLOYE
                if (checkQuery.exec("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'EMPLOYE'")) {
                    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
                        actualTableName = "EMPLOYE";
                        qDebug() << "✅ Table EMPLOYE trouvée dans Oracle";
                    }
                }
            }
        }
    } else {
        // SQLite
        qDebug() << "🔍 Recherche table SQLite...";
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString tableName = listQuery.value(0).toString();
            qDebug() << "   Table disponible:" << tableName;
            if (tableName.toUpper() == "EMPOLYE" || tableName.toUpper() == "EMPLOYE") {
                actualTableName = tableName;
                break;
            }
        }
    }

    if (actualTableName.isEmpty()) {
        qDebug() << "❌ Table EMPOLYE/EMPLOYE non trouvée!";

        // Tester une requête directe pour voir ce qui existe
        QSqlQuery directQuery;
        if (directQuery.exec("SELECT 'EMPOLYE' AS table_name, COUNT(*) FROM EMPOLYE UNION ALL SELECT 'EMPLOYE', COUNT(*) FROM EMPLOYE")) {
            while (directQuery.next()) {
                qDebug() << "   Test direct:" << directQuery.value(0).toString()
                << "->" << directQuery.value(1).toInt() << "lignes";
            }
        } else {
            qDebug() << "   Erreur test direct:" << directQuery.lastError().text();
        }

        // Afficher toutes les tables Oracle
        if (isOracle) {
            QSqlQuery listTables("SELECT TABLE_NAME FROM USER_TABLES ORDER BY TABLE_NAME");
            qDebug() << "   Tables disponibles dans Oracle:";
            while (listTables.next()) {
                qDebug() << "   - " << listTables.value(0).toString();
            }
        }

        ui->tableWidget->setRowCount(0);
        return;
    }

    // Construire la requête
    QString queryStr;
    if (isOracle) {
        // CORRECTION : requête Oracle optimisée
        queryStr = QString(
                       "SELECT "
                       "IDEMPLOYE, "
                       "PRENOM, "
                       "TO_CHAR(DATEDEMBAUCHE, 'DD/MM/YYYY') AS DATE_EMBAUCHE, "
                       "EMAIL, "
                       "TELEPHONE, "
                       "SEXE, "
                       "POSTE, "
                       "SALAIRE, "
                       "NVL(TO_CHAR(IDSUPERVISEUR), 'Aucun') AS SUPERVISEUR, "
                       "NVL(CODE_BADGE, 'Non assigné') AS CODE_BADGE "
                       "FROM %1 "
                       "ORDER BY IDEMPLOYE"
                       ).arg(actualTableName);
    } else {
        // SQLite
        queryStr = QString(
                       "SELECT IDEMPLOYE, PRENOM, "
                       "strftime('%%d/%%m/%%Y', DATEDEMBAUCHE), "
                       "EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, "
                       "COALESCE(IDSUPERVISEUR, 'Aucun'), "
                       "COALESCE(CODE_BADGE, 'Non assigné') "
                       "FROM %1 ORDER BY IDEMPLOYE"
                       ).arg(actualTableName);
    }

    qDebug() << "📋 Exécution requête...";
    qDebug() << "   SQL:" << queryStr;

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "❌ Erreur exécution requête:" << query.lastError().text();
        ui->tableWidget->setRowCount(0);
        return;
    }

    // Préparer le tableau
    ui->tableWidget->setRowCount(0);

    // Définir les en-têtes de colonnes
    QStringList headers;
    headers << "ID" << "Nom Complet" << "Date Embauche" << "Email"
            << "Téléphone" << "Sexe" << "Poste" << "Salaire"
            << "Superviseur" << "Code Badge";
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    qDebug() << "📝 Remplissage tableau...";

    while (query.next()) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < headers.size(); ++col) {
            QVariant value = query.value(col);
            QString textValue = value.isNull() ? "" : value.toString();

            // Afficher les premières lignes pour debug
            if (row < 2 && col == 0) {
                qDebug() << "   Ligne" << row << "-> ID:" << textValue
                         << "Nom:" << query.value(1).toString();
            }

            QTableWidgetItem *item = new QTableWidgetItem(textValue);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

            // Colorier le code badge différemment s'il est assigné
            if (col == 9) {
                if (textValue != "Non assigné" && !textValue.isEmpty()) {
                    item->setBackground(QColor(220, 255, 220));
                    item->setForeground(Qt::darkGreen);
                    item->setFont(QFont("Arial", 10, QFont::Bold));
                }
            }

            // Colorier le salaire selon la valeur
            if (col == 7) {
                bool ok;
                double salaire = textValue.toDouble(&ok);
                if (ok) {
                    if (salaire > 3000) {
                        item->setBackground(QColor(255, 255, 200));
                    } else if (salaire < 1000) {
                        item->setBackground(QColor(255, 230, 230));
                    }
                }
            }

            ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }

    // Ajuster la largeur des colonnes
    ui->tableWidget->resizeColumnsToContents();

    qDebug() << "✅ Nombre d'employés affichés:" << row;

    if (row == 0) {
        qDebug() << "⚠️ Aucun employé trouvé";
    }
}
void MainWindow::afficherSponsors() {
    Sponsor s;
    QSqlQueryModel *model = s.afficher();
    ui->sponsorTable_9->setRowCount(0);

    if (model) {
        int rowCount = model->rowCount();
        int colCount = model->columnCount();

        for (int r = 0; r < rowCount; ++r) {
            ui->sponsorTable_9->insertRow(r);
            for (int c = 0; c < colCount && c < 8; ++c) {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(r, c)).toString());
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);

                // Colorier le budget selon la valeur
                if (c == 7) {
                    bool ok;
                    int budget = item->text().toInt(&ok);
                    if (ok) {
                        if (budget > 10000) {
                            item->setBackground(QColor(220, 255, 220));
                        } else if (budget < 1000) {
                            item->setBackground(QColor(255, 220, 220));
                        }
                    }
                }

                ui->sponsorTable_9->setItem(r, c, item);
            }
        }
        delete model;
    }
}

// ==================== RECHERCHE VISUELLE (SURLIGNAGE JAUNE) ====================
void MainWindow::on_lineEdit_6_textChanged(const QString &text) {
    Employes e; QSqlQueryModel *model = text.trimmed().isEmpty() ? e.afficher() : e.rechercherMultiCriteres(text);
    ui->tableWidget->setRowCount(0);
    if (model) {
        for (int r = 0; r < model->rowCount(); ++r) {
            ui->tableWidget->insertRow(r);
            for (int c = 0; c < 10; ++c) {
                QString dataStr = model->data(model->index(r, c)).toString();
                QTableWidgetItem *item = new QTableWidgetItem(dataStr);

                // VISUEL : SURLIGNAGE EN JAUNE
                if (!text.isEmpty() && dataStr.contains(text, Qt::CaseInsensitive)) {
                    item->setBackground(QColor(255, 235, 59));
                    item->setForeground(Qt::black);
                    item->setFont(QFont("Arial", 10, QFont::Bold));
                }

                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->tableWidget->setItem(r, c, item);
            }
        }
        delete model;
    }
}

// ==================== EMPLOYÉS (CRUD & HISTORIQUE DÉTAILLÉ) ====================

void MainWindow::on_pushButton_11_clicked() // AJOUTER
{
    QString nom = ui->lineEdit_13->text().trimmed();
    QString prenom = ui->lineEdit_14->text().trimmed();
    QString cin = ui->lineEdit_15->text().trimmed();
    QString dateStr = ui->lineEdit_16->text().trimmed();
    QString email = ui->lineEdit_17->text().trimmed();
    QString tel = ui->lineEdit_18->text().trimmed();
    QString sexe = ui->lineEdit_19->text().trimmed();
    QString salStr = ui->lineEdit_20->text().trimmed();
    QString idSup = ui->lineEdit_21->text().trimmed();

    // Validations
    if (!Employes::validerLettres(nom)) {
        QMessageBox::warning(this, "Erreur", "❌ NOM invalide.");
        ui->lineEdit_13->setFocus();
        return;
    }
    if (!Employes::validerLettres(prenom)) {
        QMessageBox::warning(this, "Erreur", "❌ PRÉNOM invalide.");
        ui->lineEdit_14->setFocus();
        return;
    }
    if (!Employes::valider8Chiffres(cin)) {
        QMessageBox::warning(this, "Erreur", "❌ CIN invalide (8 chiffres requis).");
        ui->lineEdit_15->setFocus();
        return;
    }
    if (!Employes::validerFormatDate(dateStr)) {
        QMessageBox::warning(this, "Erreur", "❌ Date invalide. Format: MM/JJ/AAAA");
        ui->lineEdit_16->setFocus();
        return;
    }
    if (!Employes::validerEmail(email)) {
        QMessageBox::warning(this, "Erreur", "❌ Email invalide.");
        ui->lineEdit_17->setFocus();
        return;
    }
    if (!Employes::valider8Chiffres(tel)) {
        QMessageBox::warning(this, "Erreur", "❌ Téléphone invalide (8 chiffres requis).");
        ui->lineEdit_18->setFocus();
        return;
    }
    if (!Employes::validerSexe(sexe)) {
        QMessageBox::warning(this, "Erreur", "❌ Sexe invalide (M ou F uniquement).");
        ui->lineEdit_19->setFocus();
        return;
    }

    bool ok;
    double sal = salStr.toDouble(&ok);
    if (!ok || !Employes::validerSalaire(sal)) {
        QMessageBox::warning(this, "Erreur", "❌ Salaire invalide.");
        ui->lineEdit_20->setFocus();
        return;
    }

    if (!idSup.isEmpty() && !Employes::validerChiffres(idSup)) {
        QMessageBox::warning(this, "Erreur", "❌ ID Superviseur invalide.");
        ui->lineEdit_21->setFocus();
        return;
    }

    // Convertir la date
    QStringList dateParts = dateStr.split("/");
    if (dateParts.size() != 3) {
        QMessageBox::warning(this, "Erreur", "❌ Format de date invalide.");
        return;
    }

    int mois = dateParts[0].toInt();
    int jour = dateParts[1].toInt();
    int annee = dateParts[2].toInt();

    QDate date(annee, mois, jour);
    if (!date.isValid()) {
        QMessageBox::warning(this, "Erreur", "❌ Date invalide.");
        return;
    }

    QString nomComplet = nom + " " + prenom;
    QString poste = "Employé";

    Employes emp(0, nomComplet, date, email, tel, sexe.toUpper(), poste, sal, idSup, "");

    qDebug() << "Tentative d'ajout employé avec données:";
    qDebug() << "Nom complet:" << nomComplet;
    qDebug() << "Date:" << date.toString("yyyy-MM-dd");
    qDebug() << "Email:" << email;
    qDebug() << "Tél:" << tel;
    qDebug() << "Sexe:" << sexe.toUpper();
    qDebug() << "Poste:" << poste;
    qDebug() << "Salaire:" << sal;
    qDebug() << "ID Sup:" << idSup;

    if (emp.ajouter()) {
        afficherEmployes();
        clearEmployeFields();

        QString details = QString("Nom: %1 | Email: %2 | Tél: %3 | Salaire: %4").arg(nomComplet).arg(email).arg(tel).arg(sal);
        HistoryManager::ajouterLog("AJOUT", details);

        QMessageBox::information(this, "Succès", "✅ Employé ajouté !");
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Erreur lors de l'ajout. Vérifiez la console pour plus de détails.");
    }
}

void MainWindow::on_pushButton_8_clicked() // MODIFIER
{
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé.");
        return;
    }

    QString nom = ui->lineEdit_13->text().trimmed();
    QString prenom = ui->lineEdit_14->text().trimmed();
    QString email = ui->lineEdit_17->text().trimmed();
    QString tel = ui->lineEdit_18->text().trimmed();
    QString idSup = ui->lineEdit_21->text().trimmed();
    QString poste = "Employé";

    bool ok;
    double sal = ui->lineEdit_20->text().toDouble(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Erreur", "❌ Salaire invalide.");
        return;
    }

    if (!Employes::validerEmail(email)) {
        QMessageBox::warning(this, "Erreur", "Email invalide.");
        return;
    }

    // Récupérer le code badge existant
    QString codeBadge = "";
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (ui->tableWidget->item(i, 0)->text().toInt() == idEmployeSelectionne) {
            QString badge = ui->tableWidget->item(i, 9)->text();
            if (badge != "Non assigné" && !badge.isEmpty()) {
                codeBadge = badge;
            }
            break;
        }
    }

    Employes emp;
    if (emp.modifier(idEmployeSelectionne, email, tel, poste, sal, idSup, codeBadge)) {
        afficherEmployes();

        QString details = QString("ID: %1 | Nom: %2 %3 | Email: %4 | Nouv. Salaire: %5").arg(idEmployeSelectionne).arg(nom).arg(prenom).arg(email).arg(sal);
        HistoryManager::ajouterLog("MODIFICATION", details);

        QMessageBox::information(this, "Succès", "✅ Modifié !");
    } else {
        QMessageBox::warning(this, "Erreur", "❌ Erreur lors de la modification.");
    }
}

void MainWindow::on_pushButton_10_clicked() // SUPPRIMER
{
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé.");
        return;
    }

    QString details = QString("ID: %1 | Nom: %2 | Email: %3").arg(idEmployeSelectionne).arg(ui->lineEdit_13->text()).arg(ui->lineEdit_17->text());

    if (QMessageBox::question(this, "Confirmer", "Supprimer cet employé ?") == QMessageBox::Yes) {
        Employes emp;
        if (emp.supprimer(idEmployeSelectionne)) {
            afficherEmployes();
            clearEmployeFields();

            HistoryManager::ajouterLog("SUPPRESSION", details);

            idEmployeSelectionne = -1;
            QMessageBox::information(this, "Succès", "✅ Employé supprimé !");
        } else {
            QMessageBox::critical(this, "Erreur", "❌ Erreur lors de la suppression.");
        }
    }
}

// ==================== HISTORIQUE (TABLEAU) ====================

void MainWindow::on_HISTORY_clicked()
{
    QString content = HistoryManager::lireHistorique();
    QDialog *d = new QDialog(this);
    d->setWindowTitle("Historique Employés");
    d->setMinimumSize(900, 600);
    d->setStyleSheet("background:white;");

    QVBoxLayout *l = new QVBoxLayout(d);
    QLabel *t = new QLabel("Journal des Activités");
    t->setStyleSheet("font: bold 16pt 'Arial'; color: #333;");
    t->setAlignment(Qt::AlignCenter);
    l->addWidget(t);

    QTableWidget *table = new QTableWidget(d);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Date", "Action", "Détails"});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    QStringList lignes = content.split("\n", Qt::SkipEmptyParts);
    QRegularExpression regex("^\\[(.*?)\\] \\[(.*?)\\] (.*)$");

    for (const QString &ligne : lignes) {
        QRegularExpressionMatch match = regex.match(ligne);
        if (match.hasMatch()) {
            int r = table->rowCount();
            table->insertRow(r);
            QTableWidgetItem *type = new QTableWidgetItem(match.captured(2));
            type->setTextAlignment(Qt::AlignCenter);

            if (match.captured(2) == "AJOUT") {
                type->setBackground(QColor(220, 255, 220));
                type->setForeground(QColor(0, 100, 0));
            }
            else if (match.captured(2) == "SUPPRESSION") {
                type->setBackground(QColor(255, 220, 220));
                type->setForeground(QColor(139, 0, 0));
            }
            else {
                type->setBackground(QColor(220, 240, 255));
                type->setForeground(QColor(0, 0, 139));
            }

            table->setItem(r, 0, new QTableWidgetItem(match.captured(1)));
            table->setItem(r, 1, type);
            table->setItem(r, 2, new QTableWidgetItem(match.captured(3)));
        }
    }
    l->addWidget(table);
    QPushButton *btn = new QPushButton("Fermer");
    connect(btn, &QPushButton::clicked, d, &QDialog::accept);
    l->addWidget(btn);

    d->exec();
}

// ---------------------- TRI INTERACTIF ----------------------
void MainWindow::on_pushButton_12_clicked() // TRIER
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Option de Tri");
    msgBox.setText("Comment voulez-vous trier les salaires ?");
    QAbstractButton *desc = msgBox.addButton("Décroissant 📉", QMessageBox::YesRole);
    QAbstractButton *asc = msgBox.addButton("Croissant 📈", QMessageBox::NoRole);
    msgBox.addButton("Annuler", QMessageBox::RejectRole);
    msgBox.exec();

    bool croissant = (msgBox.clickedButton() == asc);
    if (msgBox.clickedButton() != desc && msgBox.clickedButton() != asc) return;

    Employes e;
    QSqlQueryModel *model = e.trierParSalaire(croissant);
    ui->tableWidget->setRowCount(0);
    if (model) {
        for (int r = 0; r < model->rowCount(); ++r) {
            ui->tableWidget->insertRow(r);
            for (int c = 0; c < 10; ++c) {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(r, c)).toString());
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);

                if (c == 7) {
                    bool ok;
                    double salaire = item->text().toDouble(&ok);
                    if (ok) {
                        if (salaire > 3000) {
                            item->setBackground(QColor(255, 255, 200));
                        } else if (salaire < 1000) {
                            item->setBackground(QColor(255, 230, 230));
                        }
                    }
                }

                ui->tableWidget->setItem(r, c, item);
            }
        }
        delete model;
    }
    QMessageBox::information(this, "Tri", croissant ? "✅ Tri Croissant" : "✅ Tri Décroissant");
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    idEmployeSelectionne = ui->tableWidget->item(row, 0)->text().toInt();

    qDebug() << "========================================";
    qDebug() << "👤 EMPLOYÉ SÉLECTIONNÉ:";
    qDebug() << "   Row:" << row;
    qDebug() << "   ID:" << idEmployeSelectionne;
    qDebug() << "   Nom:" << ui->tableWidget->item(row, 1)->text();
    qDebug() << "========================================";

    ui->lineEdit_12->setText(ui->tableWidget->item(row, 0)->text());
    QString nomComplet = ui->tableWidget->item(row, 1)->text();
    QStringList parts = nomComplet.split(" ");
    if (parts.size() >= 1) ui->lineEdit_13->setText(parts[0]);
    if (parts.size() >= 2) ui->lineEdit_14->setText(parts.mid(1).join(" "));
    else ui->lineEdit_14->clear();

    ui->lineEdit_15->clear();
    ui->lineEdit_16->setText(ui->tableWidget->item(row, 2)->text());
    ui->lineEdit_17->setText(ui->tableWidget->item(row, 3)->text());
    ui->lineEdit_18->setText(ui->tableWidget->item(row, 4)->text());
    ui->lineEdit_19->setText(ui->tableWidget->item(row, 5)->text());
    ui->lineEdit_20->setText(ui->tableWidget->item(row, 7)->text());
    ui->lineEdit_21->setText(ui->tableWidget->item(row, 8)->text());

    QString codeBadge = ui->tableWidget->item(row, 9)->text();
    if (codeBadge != "Non assigné" && !codeBadge.isEmpty()) {
        ui->statusbar->showMessage(QString("🏷️ Code badge: %1").arg(codeBadge), 3000);
        qDebug() << "   Code Badge:" << codeBadge;
    } else {
        qDebug() << "   Code Badge: Non assigné";
    }
}

// ==================== MAILING (SMTP) ====================

void MainWindow::on_MAILING_clicked()
{
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé.");
        return;
    }

    QString emailDest = ui->lineEdit_17->text().trimmed();
    QString nomEmp = ui->lineEdit_13->text().trimmed();

    if (!Employes::validerEmail(emailDest)) {
        QMessageBox::warning(this, "Erreur", "❌ Email invalide.");
        return;
    }

    if (QMessageBox::question(this, "Confirmer Envoi",
                              "Envoyer le mail de motivation à " + nomEmp + " ?") == QMessageBox::Yes) {

        QString user = "amenalaya57@gmail.com";
        QString pass = "sgnz xdto zonf jhsv";

        Smtp *smtp = new Smtp(user, pass, "smtp.gmail.com", 465);
        QString sujet = "Remerciements - CONNECT PLUS+";
        QString corps = "Bonjour " + nomEmp + ",\n\n"
                                              "Je tiens à vous remercier sincèrement pour les efforts que vous avez fournis ces derniers jours. "
                                              "Votre engagement, votre sérieux et votre esprit d'équipe ont eu un réel impact sur l'avancement de notre travail.\n\n"
                                              "Grâce à votre implication, nous avons pu atteindre nos objectifs dans de très bonnes conditions.\n\n"
                                              "Merci encore pour votre travail remarquable.\n"
                                              "Continuez ainsi — vous faites vraiment la différence.\n\n"
                                              "Bien cordialement,\n"
                                              "La Direction RH";

        smtp->sendMail(user, emailDest, sujet, corps);

        HistoryManager::ajouterLog("MAILING", "Mail motivation envoyé à " + emailDest);
        QMessageBox::information(this, "Succès", "✅ Email envoyé !");
    }
}

// ==================== FICHE DE PAIE & STATS SALAIRES ====================

void MainWindow::on_pushButton_13_clicked() // PDF
{
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un employé.");
        return;
    }
    QString f = QFileDialog::getSaveFileName(this, "Fiche Paie", "FichePaie.pdf", "PDF (*.pdf)");
    if (!f.isEmpty()) {
        FichePaie g;
        if (g.genererPDF(idEmployeSelectionne, f)) {
            QMessageBox::information(this, "Succès", "✅ PDF généré !");
            QDesktopServices::openUrl(QUrl::fromLocalFile(f));
        } else {
            QMessageBox::warning(this, "Erreur", "❌ Erreur lors de la génération du PDF.");
        }
    }
}

void MainWindow::on_STATS_clicked() // BAR CHART
{
    QVector<int> v;
    QStringList l;
    Employes::getStatistiquesSalaires(v, l);

    if (v.isEmpty() || l.isEmpty()) {
        QMessageBox::information(this, "Information", "Aucune donnée de salaire disponible.");
        return;
    }

    QDialog *d = new QDialog(this);
    d->setWindowTitle("📊 Statistiques Salaires");
    d->setMinimumSize(600, 400);
    d->setStyleSheet("background:white;");

    QVBoxLayout *lay = new QVBoxLayout(d);
    BarChartWidget *chart = new BarChartWidget();
    chart->setData(v, l);
    lay->addWidget(chart);

    d->exec();
}

// ==================== SPONSORS (CRUD) ====================
void MainWindow::on_pushButton_23_clicked()
{
    if (!validerChampsSponsor()) {
        return;
    }

    Sponsor s(ui->nom->text().trimmed(),
              ui->prenom->text().trimmed(),
              ui->categorie->text().trimmed(),
              ui->email->text().trimmed(),
              ui->debut->date(),
              ui->fin->date(),
              ui->budget->text().toInt());

    if (s.ajouter()) {
        afficherSponsors();
        clearSponsorFields();
        clearSponsorValidationErrors();
        QMessageBox::information(this, "Succès", "✅ Sponsor ajouté avec succès !");
    } else {
        QMessageBox::warning(this, "Erreur", "❌ Erreur lors de l'ajout du sponsor.");
    }
}


void MainWindow::on_pushButton_38_clicked()
{
    if (idSponsorSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un sponsor à modifier.");
        return;
    }

    QStringList erreurs;

    QString nom = ui->nom2->text().trimmed();
    QString prenom = ui->prenom2->text().trimmed();
    QString email = ui->email2->text().trimmed();

    if (nom.isEmpty() || nom.length() < 2 || !Sponsor::validerNom(nom)) {
        erreurs << "❌ Nom invalide";
    }

    if (prenom.isEmpty() || prenom.length() < 2 || !Sponsor::validerNom(prenom)) {
        erreurs << "❌ Prénom invalide";
    }

    if (!Sponsor::validerEmail(email)) {
        erreurs << "❌ Email invalide";
    }

    bool ok;
    int budget = ui->budget2->text().toInt(&ok);
    if (!ok || budget < 0) {
        erreurs << "❌ Budget invalide";
    }

    if (!Sponsor::validerDates(ui->debut2->date(), ui->fin2->date())) {
        erreurs << "❌ Dates invalides (début doit être avant fin)";
    }

    if (!erreurs.isEmpty()) {
        QMessageBox::warning(this, "Erreurs de Validation", erreurs.join("\n"));
        return;
    }

    Sponsor s;
    if (s.modifier(idSponsorSelectionne, nom, prenom,
                   ui->categorie2->text().trimmed(), email,
                   ui->debut2->date(), ui->fin2->date(), budget)) {
        afficherSponsors();
        clearSponsorValidationErrors();
        QMessageBox::information(this, "Succès", "✅ Sponsor modifié avec succès !");
    } else {
        QMessageBox::warning(this, "Erreur", "❌ Erreur lors de la modification.");
    }
}

void MainWindow::on_pushButton_40_clicked() {
    if (idSponsorSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un sponsor.");
        return;
    }

    if (QMessageBox::question(this, "Confirmer", "Supprimer ce sponsor ?") == QMessageBox::Yes) {
        Sponsor s;
        if (s.supprimer(idSponsorSelectionne)) {
            afficherSponsors();
            QMessageBox::information(this, "Succès", "✅ Sponsor supprimé !");
        } else {
            QMessageBox::warning(this, "Erreur", "❌ Erreur lors de la suppression.");
        }
    }
}

void MainWindow::on_sponsorTable_9_clicked(const QModelIndex &index)
{
    int r = index.row();
    idSponsorSelectionne = ui->sponsorTable_9->item(r, 0)->text().toInt();

    ui->ids->setText(ui->sponsorTable_9->item(r, 0)->text());
    ui->nom2->setText(ui->sponsorTable_9->item(r, 1)->text());
    ui->prenom2->setText(ui->sponsorTable_9->item(r, 2)->text());
    ui->categorie2->setText(ui->sponsorTable_9->item(r, 3)->text());
    ui->email2->setText(ui->sponsorTable_9->item(r, 4)->text());
    ui->debut2->setDate(QDate::fromString(ui->sponsorTable_9->item(r, 5)->text(), "yyyy-MM-dd"));
    ui->fin2->setDate(QDate::fromString(ui->sponsorTable_9->item(r, 6)->text(), "yyyy-MM-dd"));
    ui->budget2->setText(ui->sponsorTable_9->item(r, 7)->text());

    qDebug() << "📋 Sponsor sélectionné:" << idSponsorSelectionne;
    qDebug() << "   Nom:" << ui->nom2->text();
    qDebug() << "   Email:" << ui->email2->text();
}

void MainWindow::on_refreshbtn_clicked() {
    afficherSponsors();
    QMessageBox::information(this, "Rafraîchissement", "✅ Données actualisées !");
}

// ==================== STATS SPONSORS ====================
void MainWindow::on_pushButton_stats_clicked() {
    afficherStatistiquesCourbes();
}

void MainWindow::afficherStatistiquesCourbes() {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Statistiques Sponsors");
    dialog->setMinimumSize(1200, 600);
    dialog->setStyleSheet("background-color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);

    QLabel *title = new QLabel("Statistiques Sponsors");
    title->setStyleSheet("font: bold 18pt 'Arial'; color: #7D4FEE; margin: 10px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QHBoxLayout *chartsLayout = new QHBoxLayout();

    QGroupBox *evolutionBox = new QGroupBox("📈 Évolution des Contrats");
    evolutionBox->setStyleSheet("QGroupBox { font: bold 14pt 'Arial'; color: #7D4FEE; }");
    QVBoxLayout *evolutionLayout = new QVBoxLayout(evolutionBox);

    CurveWidget *evolutionChart = new CurveWidget();
    QVector<QPointF> evolutionData = Sponsor::getDonneesEvolutionContrats();
    evolutionChart->setData(evolutionData, "Contrats", Qt::red);
    evolutionLayout->addWidget(evolutionChart);

    QLabel *evolutionSummary = new QLabel();
    evolutionSummary->setText(QString("Total: %1 contrats").arg(evolutionData.size()));
    evolutionSummary->setStyleSheet("font: 10pt 'Arial'; color: #555; padding: 5px;");
    evolutionLayout->addWidget(evolutionSummary);

    chartsLayout->addWidget(evolutionBox);

    QGroupBox *budgetBox = new QGroupBox("💰 Budgets par Catégorie");
    budgetBox->setStyleSheet("QGroupBox { font: bold 14pt 'Arial'; color: #7D4FEE; }");
    QVBoxLayout *budgetLayout = new QVBoxLayout(budgetBox);

    QVector<QPointF> budgetData;
    QStringList categories;
    Sponsor::getDonneesBudgetParCategorie(budgetData, categories);

    double totalBudget = 0;
    for (const QPointF &point : budgetData) {
        totalBudget += point.y();
    }

    CurveWidget *budgetChart = new CurveWidget();
    budgetChart->setData(budgetData, "Budget (€)", Qt::blue);
    budgetLayout->addWidget(budgetChart);

    QLabel *budgetLegend = new QLabel();
    QString legendText = "<b>Légende:</b><br>";
    for (int i = 0; i < qMin(categories.size(), 5); ++i) {
        legendText += QString("%1: %2€<br>").arg(categories[i]).arg(budgetData[i].y());
    }
    if (categories.size() > 5) {
        legendText += "...";
    }
    budgetLegend->setText(legendText);
    budgetLegend->setStyleSheet("font: 10pt 'Arial'; color: #555; padding: 5px;");
    budgetLayout->addWidget(budgetLegend);

    chartsLayout->addWidget(budgetBox);

    mainLayout->addLayout(chartsLayout);

    QGroupBox *statsBox = new QGroupBox("📊 Résumé Statistique");
    statsBox->setStyleSheet("QGroupBox { font: bold 14pt 'Arial'; color: #7D4FEE; }");
    QGridLayout *statsLayout = new QGridLayout(statsBox);

    int totalSponsors;
    double totalBudgetAll, avgBudget;
    Sponsor::getResumeStatistiques(totalSponsors, totalBudgetAll, avgBudget);

    statsLayout->addWidget(new QLabel("Total Sponsors:"), 0, 0);
    statsLayout->addWidget(new QLabel(QString::number(totalSponsors)), 0, 1);

    statsLayout->addWidget(new QLabel("Budget Total:"), 1, 0);
    statsLayout->addWidget(new QLabel(QString("%1 €").arg(totalBudgetAll, 0, 'f', 2)), 1, 1);

    statsLayout->addWidget(new QLabel("Budget Moyen:"), 2, 0);
    statsLayout->addWidget(new QLabel(QString("%1 €").arg(avgBudget, 0, 'f', 2)), 2, 1);

    statsLayout->addWidget(new QLabel("Contrats Actifs:"), 0, 2);
    statsLayout->addWidget(new QLabel(QString::number(evolutionData.size())), 0, 3);

    statsLayout->addWidget(new QLabel("Catégories:"), 1, 2);
    statsLayout->addWidget(new QLabel(QString::number(categories.size())), 1, 3);

    mainLayout->addWidget(statsBox);

    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setStyleSheet("QPushButton { background-color: #7D4FEE; color: white; padding: 10px; border-radius: 5px; }");
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    dialog->exec();
}

void MainWindow::creerCourbeEvolutionContrats(QVBoxLayout *l) {
    CurveWidget *w = new CurveWidget();
    w->setData(Sponsor::getDonneesEvolutionContrats(), "Contrats", Qt::red);
    l->addWidget(w);
}

void MainWindow::creerCourbeBudgets(QVBoxLayout *l) {
    QVector<QPointF> data;
    QStringList labels;
    Sponsor::getDonneesBudgetParCategorie(data, labels);
    CurveWidget *w = new CurveWidget();
    w->setData(data, "Budget", Qt::blue);
    l->addWidget(w);
}

void MainWindow::creerGraphiqueCategories(QVBoxLayout *l) {
    QVector<QPointF> data;
    QStringList labels;
    Sponsor::getDonneesRepartitionCategories(data, labels);
    CurveWidget *w = new CurveWidget();
    w->setData(data, "Catégories", Qt::green);
    l->addWidget(w);
}

void MainWindow::creerGraphiqueDureeContrats(QVBoxLayout *l) {
    QVector<QPointF> data;
    Sponsor::getDonneesDureeContrats(data);
    CurveWidget *w = new CurveWidget();
    w->setData(data, "Durée (jours)", Qt::yellow);
    l->addWidget(w);
}

// ==================== CHATBOT & SMS ====================
void MainWindow::on_pushButton_25_clicked() {
    QString q = ui->lineEdit_2->text().trimmed(); if(q.isEmpty()) return;
    ui->lineEdit->setText("👤 " + q + "\n🤖 " + chatbot->obtenirReponse(q, idSponsorSelectionne)); ui->lineEdit_2->clear();
}

void MainWindow::on_pushButton_26_clicked() {
    if(smsManager->envoyerSMS(ui->lineEdit_3->text(), ui->lineEdit_4->text()))
        QMessageBox::information(this, "OK", "✅ SMS envoyé !");
    else
        QMessageBox::warning(this, "Erreur", "❌ Échec d'envoi SMS");
}

void MainWindow::on_btnConfigSMS_clicked() {
    QMessageBox::information(this, "Info", "API SMS chargée");
}

void MainWindow::on_ConfigurerGemini_clicked() {
    QDialog d(this);
    QVBoxLayout *l = new QVBoxLayout(&d);
    QLineEdit *k = new QLineEdit(&d);
    l->addWidget(new QLabel("Clé API Gemini:"));
    l->addWidget(k);
    QDialogButtonBox *b = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    l->addWidget(b);
    connect(b, &QDialogButtonBox::accepted, &d, &QDialog::accept);
    if(d.exec() == QDialog::Accepted)
        chatbot->configurerAPI(k->text());
}

// ==================== OUTILS SPONSORS ====================
void MainWindow::exporterSponsorsPDFParContrat() {
    QString f = QFileDialog::getSaveFileName(this, "Export", "", "HTML (*.html)");
    if (!f.isEmpty())
        Sponsor::exporterPDFParContrat(f);
}

void MainWindow::trierSponsorsParDate() {
    QSqlQueryModel *model = Sponsor().trierParDate();
    ui->sponsorTable_9->setRowCount(0);
    if (model) {
        for(int row=0; row < model->rowCount(); ++row) {
            ui->sponsorTable_9->insertRow(row);
            for(int col=0; col < 8; ++col)
                ui->sponsorTable_9->setItem(row, col, new QTableWidgetItem(model->data(model->index(row, col)).toString()));
        }
        delete model;
    }
    QMessageBox::information(this, "Tri", "✅ Trié par date !");
}

void MainWindow::on_pushButton_35_clicked() { exporterSponsorsPDFParContrat(); }
void MainWindow::on_pushButton_37_clicked() { trierSponsorsParDate(); }
void MainWindow::on_pushButton_39_clicked() { exporterSponsorsPDFParContrat(); }
void MainWindow::on_pushButton_41_clicked() { trierSponsorsParDate(); }

void MainWindow::rechercherSponsors(const QString &t) {
    Sponsor s; QSqlQueryModel *m = t.isEmpty() ? s.afficher() : s.rechercherParNom(t);
    ui->sponsorTable_9->setRowCount(0);
    if(m) {
        for(int r=0; r<m->rowCount(); ++r) {
            ui->sponsorTable_9->insertRow(r);
            for(int c=0; c<8; ++c)
                ui->sponsorTable_9->setItem(r, c, new QTableWidgetItem(m->data(m->index(r, c)).toString()));
        }
        delete m;
    }
}

void MainWindow::on_searchEdit_7_textChanged(const QString &t) { rechercherSponsors(t); }
void MainWindow::on_searchEdit_8_textChanged(const QString &t) { rechercherSponsors(t); }
void MainWindow::on_pushButton_24_clicked() { rechercherSponsors(ui->searchEdit_7->text()); }

// ==================== CLEAR FIELDS ====================
void MainWindow::clearEmployeFields() {
    ui->lineEdit_12->clear(); ui->lineEdit_13->clear(); ui->lineEdit_14->clear();
    ui->lineEdit_15->clear(); ui->lineEdit_16->clear();
    ui->lineEdit_17->clear(); ui->lineEdit_18->clear(); ui->lineEdit_19->clear();
    ui->lineEdit_20->clear(); ui->lineEdit_21->clear();
}

void MainWindow::clearSponsorFields() {
    ui->nom->clear(); ui->prenom->clear(); ui->email->clear();
    ui->categorie->clear(); ui->budget->clear();
}

void MainWindow::on_lineEdit_textEdited(const QString &) {}
void MainWindow::on_lineEdit_cursorPositionChanged(int, int) {}

// ==================== DEAL PAGE CORE LOGIC ====================

QSet<QString> MainWindow::previouslyCriticalItems;

int MainWindow::getEquipMask() {
    int mask = 0;
    if (ui->checkCamera_deal->isChecked()) mask |= 1;
    if (ui->checkMic_deal->isChecked()) mask |= 2;
    if (ui->checkLight_deal->isChecked()) mask |= 4;
    if (ui->checkCameraman_deal->isChecked()) mask |= 8;
    if (ui->checkEditor_deal->isChecked()) mask |= 16;
    if (ui->checkStudio_deal->isChecked()) mask |= 32;
    return mask;
}

QDateTime MainWindow::findNextAvailableSlot(const QDateTime &requested, int requiredMask)
{
    QDateTime current = requested;
    QDate date = current.date();

    while (true) {
        QTime t = current.time();
        if (t < QTime(8,0) || t > QTime(20,0)) {
            current = current.addSecs(3600);
            continue;
        }

        bool conflict = false;
        for (int row = 0; row < ui->tableWidget_deal->rowCount(); ++row) {
            if (editedRow >= 0 && row == editedRow) continue;

            QString timeStr = ui->tableWidget_deal->item(row, 3)->text();
            QDateTime bookedTime = QDateTime::fromString(timeStr, "yyyy-MM-dd hh:mm");
            if (bookedTime == current) {
                int bookedMask = ui->tableWidget_deal->item(row, 2)->text().toInt();
                if (bookedMask & requiredMask) {
                    conflict = true;
                    break;
                }
            }
        }

        if (!conflict) {
            return current;
        }

        current = current.addSecs(3600);

        if (current.time() > QTime(20,0)) {
            current = QDateTime(date.addDays(1), QTime(8,0));
            date = current.date();
        }
    }
}

void MainWindow::refreshTable(const QString &filter) {
    for (int row = 0; row < ui->tableWidget_deal->rowCount(); row++) {
        bool match = ui->tableWidget_deal->item(row, 1)->text().contains(filter, Qt::CaseInsensitive);
        ui->tableWidget_deal->setRowHidden(row, !match);
    }
}

void MainWindow::on_btnBook_deal_clicked()
{
    QString idText = ui->lineEditID_deal->text().trimmed();
    QString name = ui->lineEditName_deal->text().trimmed();
    QDateTime selectedTime = ui->dateTimeEdit_deal->dateTime();
    int mask = getEquipMask();

    if (idText.isEmpty() || !idText.contains(QRegularExpression("^[0-9]+$"))) {
        QMessageBox::warning(this, "Erreur", "L'ID doit contenir uniquement des chiffres !");
        return;
    }
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom du client est obligatoire !");
        return;
    }

    QTime time = selectedTime.time();
    if (time < QTime(8,0) || time > QTime(20,0)) {
        QMessageBox::warning(this, "Horaire invalide",
                             "Réservations uniquement entre 08:00 et 20:00 !\nHeure choisie : " + time.toString("hh:mm"));
        return;
    }

    QDateTime finalTime = selectedTime;

    QDateTime conflictTime = findNextAvailableSlot(selectedTime, mask);

    if (conflictTime != selectedTime) {
        QString msg = QString(
                          "<b>Conflit détecté !</b><br><br>"
                          "Un ou plusieurs équipements sont déjà réservés à %1.<br><br>"
                          "<b>Prochaine heure disponible :</b> %2<br><br>"
                          "Voulez-vous réserver à cette heure ?")
                          .arg(selectedTime.toString("dddd dd MMMM yyyy à hh:mm"))
                          .arg(conflictTime.toString("dddd dd MMMM yyyy à hh:mm"));

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Conflit de réservation", msg,
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            QMessageBox::information(this, "Annulé", "Réservation annulée.");
            return;
        }
        finalTime = conflictTime;
    }

    if (editedRow >= 0) {
        ui->tableWidget_deal->item(editedRow, 0)->setText(idText);
        ui->tableWidget_deal->item(editedRow, 1)->setText(name);
        ui->tableWidget_deal->item(editedRow, 2)->setText(QString::number(mask));
        ui->tableWidget_deal->item(editedRow, 3)->setText(finalTime.toString("yyyy-MM-dd hh:mm"));

        ui->btnBook_deal->setText("Book Deal");
        ui->btnEdit_deal->setEnabled(true);
        ui->btnDelete_deal->setEnabled(true);
        editedRow = -1;

        QMessageBox::information(this, "Succès", "Réservation modifiée avec succès !");
    }
    else {
        int row = ui->tableWidget_deal->rowCount();
        ui->tableWidget_deal->insertRow(row);
        ui->tableWidget_deal->setItem(row, 0, new QTableWidgetItem(idText));
        ui->tableWidget_deal->setItem(row, 1, new QTableWidgetItem(name));
        ui->tableWidget_deal->setItem(row, 2, new QTableWidgetItem(equipmentMaskToText(mask)));
        ui->tableWidget_deal->setItem(row, 3, new QTableWidgetItem(finalTime.toString("yyyy-MM-dd hh:mm")));

        QMessageBox::information(this, "Succès",
                                 finalTime == selectedTime ?
                                     "Réservation ajoutée avec succès !" :
                                     "Réservation ajoutée à la prochaine heure disponible !");
    }

    ui->lineEditID_deal->clear();
    ui->lineEditName_deal->clear();
    ui->checkCamera_deal->setChecked(false);
    ui->checkMic_deal->setChecked(false);
    ui->checkLight_deal->setChecked(false);
    ui->checkCameraman_deal->setChecked(false);
    ui->checkEditor_deal->setChecked(false);
    ui->checkStudio_deal->setChecked(false);

    updateStatsTable();
}

void MainWindow::on_btnEdit_deal_clicked()
{
    int row = ui->tableWidget_deal->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a booking to edit!");
        return;
    }

    QString id = ui->tableWidget_deal->item(row, 0)->text();
    QString name = ui->tableWidget_deal->item(row, 1)->text();
    QString maskText = ui->tableWidget_deal->item(row, 2)->text();
    QString dateTimeStr = ui->tableWidget_deal->item(row, 3)->text();

    ui->lineEditID_deal->setText(id);
    ui->lineEditName_deal->setText(name);

    QDateTime dt = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm");
    if (dt.isValid()) {
        ui->dateTimeEdit_deal->setDateTime(dt);
    }

    int mask = maskText.toInt();

    ui->checkCamera_deal->setChecked(mask & 1);
    ui->checkMic_deal->setChecked(mask & 2);
    ui->checkLight_deal->setChecked(mask & 4);
    ui->checkCameraman_deal->setChecked(mask & 8);
    ui->checkEditor_deal->setChecked(mask & 16);
    ui->checkStudio_deal->setChecked(mask & 32);

    ui->btnBook_deal->setText("Update Booking");
    ui->btnEdit_deal->setEnabled(false);
    ui->btnDelete_deal->setEnabled(false);

    editedRow = row;

    QMessageBox::information(this, "Edit Mode", "Now modify the fields and click 'Update Booking' to save changes.");
}

void MainWindow::on_btnDelete_deal_clicked() {
    int row = ui->tableWidget_deal->currentRow();
    if (row >= 0) ui->tableWidget_deal->removeRow(row);
    updateStatsTable();
}

void MainWindow::on_btnClear_deal_clicked()
{
    ui->tableWidget_deal->setRowCount(0);
    ui->lineEditID_deal->clear();
    ui->lineEditName_deal->clear();
    ui->searchName_deal->clear();
    ui->dateTimeEdit_deal->setDateTime(QDateTime::currentDateTime());
    ui->checkCamera_deal->setChecked(false);
    ui->checkMic_deal->setChecked(false);
    ui->checkLight_deal->setChecked(false);
    ui->checkCameraman_deal->setChecked(false);
    ui->checkEditor_deal->setChecked(false);
    ui->checkStudio_deal->setChecked(false);
    updateStatsTable();
    QMessageBox::information(this, "Cleared", "All bookings have been removed!");
}

void MainWindow::on_btnOrder_deal_clicked()
{
    ui->tableWidget_deal->sortItems(3, Qt::AscendingOrder);
    QMessageBox::information(this, "Sorted",
                             "Bookings have been sorted by date and time (oldest first).");
    updateStatsTable();
}

void MainWindow::on_btnSearch_deal_clicked() {
    refreshTable(ui->searchName_deal->text());
}

void MainWindow::on_btnExportPDF_deal_clicked()
{
    QString downloads = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (downloads.isEmpty()) return;

    QString fileName = QString("Bookings_Report_%1.pdf")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));
    QString filePath = QDir(downloads).filePath(fileName);

    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);
    pdf.setPageMargins(QMarginsF(10, 10, 10, 10));

    QPainter painter(&pdf);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont titleFont("Arial", 22, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(0, 100, pdf.width(), 100, Qt::AlignCenter,
                     "Equipment Booking Report");

    QFont dateFont("Arial", 12);
    painter.setFont(dateFont);
    painter.drawText(0, 180, pdf.width(), 50, Qt::AlignCenter,
                     "Generated: " + QDateTime::currentDateTime()
                                         .toString("dddd, dd MMMM yyyy - hh:mm"));

    ui->tableWidget_deal->resizeColumnsToContents();
    ui->tableWidget_deal->resizeRowsToContents();

    int w = ui->tableWidget_deal->width();
    int h = ui->tableWidget_deal->height();

    QImage img(w * 3, h * 3, QImage::Format_ARGB32);
    img.fill(Qt::white);

    QPainter imgPainter(&img);
    imgPainter.scale(3.0, 3.0);
    ui->tableWidget_deal->render(&imgPainter);
    imgPainter.end();

    QRect target(50, 250, pdf.width() - 100, (pdf.width() - 100) * (float(h) / w));
    painter.drawImage(target, img);

    painter.end();

    QMessageBox::information(this, "Success", "PDF exported:\n" + fileName);
    QDesktopServices::openUrl(QUrl::fromLocalFile(downloads));
}

void MainWindow::updateMonthlyStats() {}
void MainWindow::checkAutoOrder() {}
void MainWindow::deliverPendingOrders() {}
void MainWindow::autoExportStatsWhenCritical() {}

void MainWindow::updateStatsTable() {
    QStringList materials = {"camera", "Microphone", "Studio light", "cameraman", "video editor", "studio"};
    QList<int> totals = {5, 5, 3, 2, 2, 1};
    QList<int> bits = {1, 2, 4, 8, 16, 32};

    ui->tableStats_deal->setRowCount(0);

    for (int i = 0; i < materials.size(); ++i) {
        int booked = 0;
        for (int row = 0; row < ui->tableWidget_deal->rowCount(); ++row) {
            int mask = ui->tableWidget_deal->item(row, 2)->text().toInt();
            if (mask & bits[i]) {
                booked++;
            }
        }
        int in_stock = totals[i] - booked;
        double usage = (totals[i] > 0) ? (booked * 100.0 / totals[i]) : 0.0;

        ui->tableStats_deal->insertRow(i);
        ui->tableStats_deal->setItem(i, 0, new QTableWidgetItem(materials[i]));
        ui->tableStats_deal->setItem(i, 1, new QTableWidgetItem(QString::number(in_stock)));
        ui->tableStats_deal->setItem(i, 2, new QTableWidgetItem(QString::number(booked)));
        ui->tableStats_deal->setItem(i, 3, new QTableWidgetItem(QString::number(usage, 'f', 2) + "%"));
    }
}

QString MainWindow::equipmentMaskToText(int mask)
{
    QStringList equip;
    if (mask & 1)  equip << "Camera";
    if (mask & 2)  equip << "Microphone";
    if (mask & 4)  equip << "Studio Light";
    if (mask & 8)  equip << "Cameraman";
    if (mask & 16) equip << "Video Editor";
    if (mask & 32) equip << "Studio";
    return equip.join(", ");
}

void MainWindow::on_btnCalendar_deal_clicked()
{
    CalendarDialog dlg(this);
    dlg.exec();
}

// ==================== CONTENT CREATOR ====================

void MainWindow::on_searchEdit_9_textChanged(const QString &text)
{
    QString search = text.toLower().trimmed();
    for (int row = 0; row < ui->creatorTable_7->rowCount(); ++row) {
        bool match = false;
        for (int col = 1; col <= 5; ++col) {
            QTableWidgetItem *item = ui->creatorTable_7->item(row, col);
            if (item && item->text().toLower().contains(search)) {
                match = true;
                break;
            }
        }
        ui->creatorTable_7->setRowHidden(row, !match && !search.isEmpty());
    }
    if (search.isEmpty()) {
        for (int row = 0; row < ui->creatorTable_7->rowCount(); ++row)
            ui->creatorTable_7->setRowHidden(row, false);
        ui->creatorTable_7->sortItems(1, Qt::AscendingOrder);
    }
    updateSimpleStats();
}

void MainWindow::on_themeButton_7_clicked()
{
    darkTheme = !darkTheme;
    for (QWidget *w : findChildren<QWidget*>()) {
        w->setProperty("darkTheme", darkTheme);
        w->style()->unpolish(w);
        w->style()->polish(w);
    }
    ui->themeButton_7->setText(darkTheme ? "Light Theme" : "Dark Theme");
}

void MainWindow::loadCreateurs()
{
    ui->creatorTable_7->clearContents();
    ui->creatorTable_7->setRowCount(0);
    QSqlQuery q;
    q.exec("SELECT IDCREATEUR, NOM, PLATFORME, GENRE, ABONNE, TYPE_DE_CONTENU FROM MY_USER.CREATEUR ORDER BY IDCREATEUR");
    int row = 0;
    while (q.next()) {
        ui->creatorTable_7->insertRow(row);

        QTableWidgetItem *idItem = new QTableWidgetItem(q.value(0).toString());
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 0, idItem);

        QTableWidgetItem *nameItem = new QTableWidgetItem(q.value(1).toString());
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 1, nameItem);

        QTableWidgetItem *platItem = new QTableWidgetItem(q.value(2).toString());
        platItem->setFlags(platItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 2, platItem);

        QTableWidgetItem *genreItem = new QTableWidgetItem(q.value(3).toString());
        genreItem->setFlags(genreItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 3, genreItem);

        QTableWidgetItem *aboItem = new QTableWidgetItem();
        aboItem->setData(Qt::DisplayRole, q.value(4).toInt());
        aboItem->setTextAlignment(Qt::AlignCenter);
        aboItem->setFlags(aboItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 4, aboItem);

        QTableWidgetItem *typeItem = new QTableWidgetItem(q.value(5).toString());
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 5, typeItem);
        row++;
    }
    updateSimpleStats();
    ui->creatorTable_7->sortItems(1, Qt::AscendingOrder);
    ui->creatorTable_7->setSortingEnabled(true);
}

void MainWindow::fillFormFromTable()
{
    int row = ui->creatorTable_7->currentRow();
    if (row < 0) return;

    if (!ui->creatorTable_7->item(row, 0) ||
        !ui->creatorTable_7->item(row, 1) ||
        !ui->creatorTable_7->item(row, 2) ||
        !ui->creatorTable_7->item(row, 3) ||
        !ui->creatorTable_7->item(row, 4) ||
        !ui->creatorTable_7->item(row, 5)) {
        return;
    }

    ui->nameEdit_7->setText(ui->creatorTable_7->item(row, 1)->text());
    ui->platformCombo_7->setCurrentText(ui->creatorTable_7->item(row, 2)->text());
    ui->comboBox_7->setCurrentText(ui->creatorTable_7->item(row, 3)->text());
    ui->contentTypeEdit_7->setText(ui->creatorTable_7->item(row, 5)->text());

    QString subsText = ui->creatorTable_7->item(row, 4)->text();
    subsText.remove(',');
    subsText.remove(' ');
    bool ok;
    int subs = subsText.toInt(&ok);
    if (ok) {
        ui->subscribersSpin_7->setValue(subs);
    } else {
        ui->subscribersSpin_7->setValue(0);
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
    ui->nameEdit_7->clear();
    ui->platformCombo_7->setCurrentIndex(0);
    ui->comboBox_7->setCurrentIndex(0);
    ui->subscribersSpin_7->setValue(0);
    ui->contentTypeEdit_7->clear();
    ui->nameEdit_7->setFocus();
}

// ==================== ROLE SYSTEM ====================
void MainWindow::disableAllInputs()
{
    auto disable = [this](auto w) { if (w) w->setEnabled(false); };
    disable(ui->addButton_7); disable(ui->updateButton_7); disable(ui->deleteButton_7);
    disable(ui->clearButton_7); disable(ui->exportButton_7);
    disable(ui->nameEdit_7); disable(ui->platformCombo_7); disable(ui->comboBox_7);
    disable(ui->subscribersSpin_7); disable(ui->contentTypeEdit_7);
    disable(ui->searchEdit_7); disable(ui->btnGenerateQR_2); disable(ui->btnSaveQR_2); disable(ui->btnCopyQR_2);
}

void MainWindow::applyRoleRestrictions()
{
    if (m_userRole == "viewer") {
        disableAllInputs();
        ui->creatorTable_7->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

// ==================== QR CODE ====================
void MainWindow::generateQR()
{
    int row = ui->creatorTable_7->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un créateur d'abord !");
        return;
    }

    QString name       = ui->creatorTable_7->item(row, 1)->text();
    QString platform   = ui->creatorTable_7->item(row, 2)->text();
    QString subsText   = ui->creatorTable_7->item(row, 4)->text().remove(',').remove(' ');
    int subscribers    = subsText.toInt();
    QString type       = ui->creatorTable_7->item(row, 5)->text();

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

    Createur c;
    c.id          = ui->creatorTable_7->item(row, 0)->text().toInt();
    c.name        = vcard;
    c.platform    = platform;
    c.type        = type;
    c.subscribers = subscribers;
    c.photo       = QPixmap();

    m_currentQR = QRCodeGenerator::generate(c, QPixmap(":/icons/logo.png"));

    ui->qrLabel->setPixmap(m_currentQR.scaled(320, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));

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

void MainWindow::on_addButton_7_clicked()
{
    QString name = ui->nameEdit_7->text().trimmed();
    QString platform = ui->platformCombo_7->currentText();
    QString gender = ui->comboBox_7->currentText();
    int subs = ui->subscribersSpin_7->value();
    QString type = ui->contentTypeEdit_7->text().trimmed();

    if (name.isEmpty() || !isValidName(name)) {
        QMessageBox::warning(this, "Erreur", "Nom invalide !");
        return;
    }

    QSqlQuery qmax;
    int nextId = 1;
    if (qmax.exec("SELECT MAX(IDCREATEUR) FROM MY_USER.CREATEUR") && qmax.next()) {
        nextId = qmax.value(0).toInt() + 1;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO MY_USER.CREATEUR (IDCREATEUR, NOM, PLATFORME, GENRE, ABONNE, TYPE_DE_CONTENU) "
              "VALUES (:id, :nom, :plat, :genre, :abo, :type)");
    q.bindValue(":id", nextId);
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
        // Utiliser SerialManager au lieu de Arduino
        if (serialManager && serialManager->isConnected()) {
            serialManager->sendCommand("ADD:" + name);
        }
    } else {
        QMessageBox::critical(this, "Erreur Base", q.lastError().text());
    }
}

void MainWindow::on_updateButton_7_clicked()
{
    int row = ui->creatorTable_7->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un créateur à modifier !");
        return;
    }

    int id = ui->creatorTable_7->item(row, 0)->text().toInt();
    QString name = ui->nameEdit_7->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom ne peut pas être vide !");
        return;
    }

    QSqlQuery q;
    q.prepare("UPDATE MY_USER.CREATEUR SET NOM = :nom, PLATFORME = :plat, GENRE = :genre, ABONNE = :abo, TYPE_DE_CONTENU = :type WHERE IDCREATEUR = :id");
    q.bindValue(":nom", name);
    q.bindValue(":plat", ui->platformCombo_7->currentText());
    q.bindValue(":genre", ui->comboBox_7->currentText());
    q.bindValue(":abo", ui->subscribersSpin_7->value());
    q.bindValue(":type", ui->contentTypeEdit_7->text().trimmed());
    q.bindValue(":id", id);

    if (q.exec()) {
        loadCreateurs();
        updateSimpleStats();
        QMessageBox::information(this, "Succès", "Créateur modifié avec succès !");
        // Utiliser SerialManager au lieu de Arduino
        if (serialManager && serialManager->isConnected()) {
            serialManager->sendCommand("EDIT:" + name);
        }
    } else {
        QMessageBox::critical(this, "Erreur Base de Données", "Échec de la modification :\n" + q.lastError().text());
    }
}

void MainWindow::on_deleteButton_7_clicked()
{
    int row = ui->creatorTable_7->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Select a creator!");
        return;
    }

    if (QMessageBox::question(this, "Confirm", "Delete this creator?") != QMessageBox::Yes)
        return;

    int id = ui->creatorTable_7->item(row, 0)->text().toInt();
    QSqlQuery q;
    q.prepare("DELETE FROM MY_USER.CREATEUR WHERE IDCREATEUR = :id");
    q.bindValue(":id", id);

    if (q.exec()) {
        loadCreateurs();
        clearCreateurFields();
        updateSimpleStats();
        QMessageBox::information(this, "Success", "Deleted!");
    } else {
        QMessageBox::critical(this, "Error", q.lastError().text());
    }
}

void MainWindow::on_clearButton_7_clicked()
{
    clearCreateurFields();
}

void MainWindow::on_exportButton_7_clicked()
{
    int row = ui->creatorTable_7->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un créateur !");
        return;
    }

    QString nom = ui->creatorTable_7->item(row, 1)->text();
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
    p.drawText(200, 1800, "Plateforme : " + ui->creatorTable_7->item(row, 2)->text());
    p.drawText(200, 2100, "Abonnés : " + QLocale().toString(ui->creatorTable_7->item(row, 4)->text().remove(',').toInt()));
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
    if (q.next()) ui->lcdTotal_2->display(q.value(0).toInt());

    // YouTube
    q.exec("SELECT COUNT(*) FROM MY_USER.CREATEUR WHERE PLATFORME = 'YouTube'");
    if (q.next()) ui->lcdYoutube_2->display(q.value(0).toInt());

    // TikTok
    q.exec("SELECT COUNT(*) FROM MY_USER.CREATEUR WHERE PLATFORME = 'TikTok'");
    if (q.next()) ui->lcdTiktok_2->display(q.value(0).toInt());

    // Instagram
    q.exec("SELECT COUNT(*) FROM MY_USER.CREATEUR WHERE PLATFORME = 'Instagram'");
    if (q.next()) ui->lcdInstagram_2->display(q.value(0).toInt());
}

void MainWindow::showWelcomeMessage(const QString &name)
{
    QMessageBox::information(this, "Connexion réussie",
                             "<h2 style='color:#7D4FEE;'>Bienvenue chez Smart Media Agency</h2>"
                             "<p><b>" + name.toUpper() + "</b></p>"
                                                    "<p>Vous êtes connecté en tant que <b>" + m_userRole.toUpper() + "</b></p>"
                                                          "<hr>"
                                                          "<p style='color:gray; font-size:10px;'>© Selim Fekih - PFE 2026</p>",
                             QMessageBox::Ok);

    setWindowTitle("Smart Media Agency × Connect+ | " + name + " (" + m_userRole + ")");
    // Utiliser SerialManager au lieu de Arduino
    if (serialManager && serialManager->isConnected()) {
        serialManager->sendCommand("LOGIN:" + name);
    }
}

void MainWindow::mettreEnLiveStudio(const QString &nomCreateur) {
    // Utiliser SerialManager au lieu de Arduino
    if (serialManager && serialManager->isConnected()) {
        serialManager->sendCommand("LIVE_ON:" + nomCreateur);
    }
}

void MainWindow::arreterLiveStudio() {
    // Utiliser SerialManager au lieu de Arduino
    if (serialManager && serialManager->isConnected()) {
        serialManager->sendCommand("LIVE_OFF");
    }
}

void MainWindow::on_pushButton_live_clicked()
{
    QString nom = ui->nameEdit_7->text().trimmed();
    int row = ui->creatorTable_7->currentRow();
    if (nom.isEmpty() && row >= 0) {
        nom = ui->creatorTable_7->item(row, 1)->text();
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

// ==================== FINANCE ====================
void MainWindow::addInvoice() {
    int id = ui->lineId->text().toInt();
    double montant = ui->lineMontant->text().toDouble();
    QDate emission = ui->dateEmission->date();
    QDate echeance = ui->dateEcheance->date();
    QString status = ui->comboStatut->currentText();

    if (id <= 0 || montant <= 0 || echeance <= emission) {
        QMessageBox::warning(this, "Erreur", "Vérifiez les champs !");
        return;
    }

    QSqlQuery q;
    q.prepare("INSERT INTO MY_USER.FACTURE "
              "(ID_FACTURE, MONTANT, DATE_D_EMMISSION, DATE_D_ECHAENCE, ID_EMPLOYEE, ID_SPONSOR, STATUS) "
              "VALUES (:id, :montant, :emission, :echeance, :idemploye, :idsponsor, :status)");

    q.bindValue(":id", id);
    q.bindValue(":montant", montant);
    q.bindValue(":emission", emission);
    q.bindValue(":echeance", echeance);
    q.bindValue(":idemploye", 1);
    q.bindValue(":idsponsor", 1);
    q.bindValue(":status", status);

    if (q.exec()) {
        QMessageBox::information(this, "Succès", "Facture ajoutée !");
        loadInvoicesFromDatabase();
        clearForm();
    } else {
        QMessageBox::critical(this, "Erreur DB", q.lastError().text());
    }
}

void MainWindow::loadInvoicesFromDatabase() {
    ui->tableFactures->setRowCount(0);
    QSqlQuery query;
    QString sql =
        "SELECT F.ID_FACTURE, F.MONTANT, F.DATE_D_EMMISSION, F.DATE_D_ECHAENCE, "
        "       F.STATUS, E.NOM || ' ' || E.PRENOM AS EMPLOYEE_NAME, S.NOM AS SPONSOR_NAME "
        "FROM MY_USER.FACTURE F "
        "LEFT JOIN MY_USER.EMPLOYES E ON F.ID_EMPLOYEE = E.ID "
        "LEFT JOIN MY_USER.SPONSOR S ON F.ID_SPONSOR = S.ID "
        "ORDER BY F.ID_FACTURE";

    qDebug() << "Executing SQL:" << sql;
    if (!query.exec(sql)) {
        qDebug() << "SQL exec failed:" << query.lastError().text();
        // Créer la table si elle n'existe pas
        QSqlQuery createQuery;
        createQuery.exec("CREATE TABLE IF NOT EXISTS MY_USER.FACTURE ("
                         "ID_FACTURE INTEGER PRIMARY KEY, "
                         "MONTANT REAL, "
                         "DATE_D_EMMISSION TEXT, "
                         "DATE_D_ECHAENCE TEXT, "
                         "STATUS TEXT, "
                         "ID_EMPLOYEE INTEGER, "
                         "ID_SPONSOR INTEGER)");
        qDebug() << "Table FACTURE créée ou existe déjà";
        return;
    }

    int rows = 0;
    while (query.next()) {
        ++rows;
        int row = ui->tableFactures->rowCount();
        ui->tableFactures->insertRow(row);

        ui->tableFactures->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableFactures->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toDouble(), 'f', 2)));

        QDate d1 = query.value(2).toDate();
        QDate d2 = query.value(3).toDate();
        ui->tableFactures->setItem(row, 2, new QTableWidgetItem(d1.isValid() ? d1.toString("dd/MM/yyyy") : QString()));
        ui->tableFactures->setItem(row, 3, new QTableWidgetItem(d2.isValid() ? d2.toString("dd/MM/yyyy") : QString()));

        ui->tableFactures->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->tableFactures->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ui->tableFactures->setItem(row, 6, new QTableWidgetItem(query.value(6).toString()));
    }
    qDebug() << "Rows returned:" << rows;

    updateVisualStatistics();
}

void MainWindow::editInvoice() {
    int row = ui->tableFactures->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une facture !");
        return;
    }

    int id = ui->tableFactures->item(row, 0)->text().toInt();
    double montant = ui->lineMontant->text().toDouble();
    QDate emission = ui->dateEmission->date();
    QDate echeance = ui->dateEcheance->date();
    QString statut = ui->comboStatut->currentText();

    QSqlQuery q;
    q.prepare("UPDATE MY_USER.FACTURE SET MONTANT = :montant, DATE_D_EMMISSION = :emission, "
              "DATE_D_ECHAENCE = :echeance, STATUS = :status WHERE ID_FACTURE = :id");
    q.bindValue(":montant", montant);
    q.bindValue(":emission", emission);
    q.bindValue(":echeance", echeance);
    q.bindValue(":status", statut);
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

void MainWindow::onInvoiceSelected() {
    int currentRow = ui->tableFactures->currentRow();
    if(currentRow >= 0) {
        QString id = ui->tableFactures->item(currentRow, 0)->text();
        QString amount = ui->tableFactures->item(currentRow, 1)->text();
        QString issueDateStr = ui->tableFactures->item(currentRow, 2)->text();
        QString dueDateStr = ui->tableFactures->item(currentRow, 3)->text();
        QString status = ui->tableFactures->item(currentRow, 4)->text();

        ui->lineId->setText(id);
        ui->lineId->setEnabled(false);
        ui->lineMontant->setText(amount);

        QDate issueDate = QDate::fromString(issueDateStr, "dd/MM/yyyy");
        QDate dueDate = QDate::fromString(dueDateStr, "dd/MM/yyyy");
        if(issueDate.isValid()) ui->dateEmission->setDate(issueDate);
        if(dueDate.isValid()) ui->dateEcheance->setDate(dueDate);

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
    ui->lineId->setEnabled(true);
    ui->lineMontant->clear();
    ui->dateEmission->setDate(QDate::currentDate());
    ui->dateEcheance->setDate(QDate::currentDate().addDays(30));
    ui->comboStatut->setCurrentIndex(0);
}

void MainWindow::sortByAmount() {
    ui->tableFactures->sortItems(1, Qt::AscendingOrder);
}

void MainWindow::searchById() {
    QString searchId = ui->searchBox->text().trimmed();
    if(searchId.isEmpty()) {
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
        stream << "Invoice ID;Amount;Issue Date;Due Date;Status;Employee;Sponsor\n";

        for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
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
    ui->labelTotalValue->setText("0");
    ui->labelAmountValue->setText("$0.00");
    ui->labelPaidValue->setText("0");
    ui->labelPendingValue->setText("0");
    ui->labelCancelledValue->setText("0");
    ui->labelAverageValue->setText("$0.00");
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

    ui->labelTotalValue->setText(QString::number(totalInvoices));
    ui->labelAmountValue->setText(QString("$%1").arg(QString::number(totalAmount, 'f', 2)));
    ui->labelPaidValue->setText(QString::number(paidCount));
    ui->labelPendingValue->setText(QString::number(pendingCount));
    ui->labelCancelledValue->setText(QString::number(cancelledCount));
    ui->labelAverageValue->setText(QString("$%1").arg(QString::number(averageAmount, 'f', 2)));

    if(totalInvoices > 0) {
        ui->progressPaid->setValue((paidCount * 100) / totalInvoices);
        ui->progressPending->setValue((pendingCount * 100) / totalInvoices);
        ui->progressCancelled->setValue((cancelledCount * 100) / totalInvoices);
    } else {
        ui->progressPaid->setValue(0);
        ui->progressPending->setValue(0);
        ui->progressCancelled->setValue(0);
    }
}

void MainWindow::applyLightTheme() {
    // style application
}

void MainWindow::applyDarkTheme() {
    // style application
}

// ==================== ARDUINO/BADGE FUNCTIONS ====================

// Nouvelle fonction pour traiter les données Arduino
void MainWindow::onArduinoDataReceived(const QString &data)
{
    qDebug() << "Données reçues de l'Arduino:" << data;

    if (data.startsWith("CODE:")) {
        QString code = data.mid(5, 4);
        qDebug() << "Code badge extrait:" << code;
        processBadgeCode(code);
    } else if (data.contains("=== ACCES AUTORISE ===")) {
        ui->statusbar->showMessage("✅ Porte ouverte", 3000);
    } else if (data.contains("=== ACCES REFUSE ===")) {
        ui->statusbar->showMessage("❌ Accès refusé", 3000);
    } else if (data.contains("=== SYSTEME BADGEUSE ARDUINO ===")) {
        ui->statusbar->showMessage("🔄 Système badgeuse initialisé", 3000);
    } else if (data.contains("PORTE: Ouverte")) {
        onPorteOuverte();
    } else if (data.contains("PORTE: Fermee")) {
        onPorteFermee();
    }
}

void MainWindow::processBadgeCode(const QString &code)
{
    qDebug() << "Traitement du code badge:" << code;

    // Vérifier le code dans la base de données
    QSqlQuery query;
    query.prepare("SELECT IDEMPLOYE, PRENOM FROM empolye WHERE CODE_BADGE = ?");
    query.addBindValue(code);

    if (query.exec() && query.next()) {
        int idEmploye = query.value(0).toInt();
        QString nomEmploye = query.value(1).toString();

        // Enregistrer dans l'historique
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO HISTORIQUE_ACCES (ID_EMPLOYE, NOM_EMPLOYE, HEURE_ACCES, TYPE_ACCES) "
                            "VALUES (?, ?, ?, ?)");
        insertQuery.addBindValue(idEmploye);
        insertQuery.addBindValue(nomEmploye);
        insertQuery.addBindValue(QDateTime::currentDateTime());
        insertQuery.addBindValue("ENTREE");
        insertQuery.exec();

        qDebug() << "✅ Accès autorisé pour:" << nomEmploye;

        // Afficher dans l'interface
        ui->statusbar->showMessage("✅ Accès autorisé pour " + nomEmploye, 5000);

        // Envoyer la commande d'autorisation à l'Arduino
        if (serialManager && serialManager->isConnected()) {
            QString commande = "AUTORISE:" + nomEmploye;
            serialManager->sendCommand(commande);
            qDebug() << "📤 Commande envoyée à Arduino:" << commande;
        }

        // Afficher une notification
        QMessageBox::information(this, "Accès Autorisé",
                                 "<h3>✅ Accès Autorisé</h3>"
                                 "<p><b>Nom:</b> " + nomEmploye + "</p>"
                                                    "<p><b>Code:</b> " + code + "</p>"
                                              "<p><b>Heure:</b> " + QDateTime::currentDateTime().toString("HH:mm:ss") + "</p>");

        // Mettre à jour l'affichage si un employé est sélectionné
        if (idEmployeSelectionne == idEmploye) {
            ui->statusbar->showMessage("🏷️ Votre code: " + code, 3000);
        }

    } else {
        qDebug() << "❌ Code invalide:" << code;

        // Afficher dans l'interface
        ui->statusbar->showMessage("❌ Accès refusé - Code invalide", 5000);

        // Envoyer la commande de refus
        if (serialManager && serialManager->isConnected()) {
            serialManager->sendCommand("REFUSE");
            qDebug() << "📤 Commande REFUSE envoyée à Arduino";
        }

        // Afficher une notification d'erreur
        QMessageBox::warning(this, "Accès Refusé",
                             "<h3>❌ Accès Refusé</h3>"
                             "<p>Code badge invalide: <b>" + code + "</b></p>"
                                          "<p>Veuillez contacter l'administration.</p>");
    }
}

void MainWindow::onPorteOuverte() {
    ui->statusbar->showMessage("🚪 Porte ouverte", 3000);
}

void MainWindow::onPorteFermee() {
    ui->statusbar->showMessage("🚪 Porte fermée", 3000);
}

void MainWindow::onArduinoConnected() {
    ui->statusbar->showMessage("✅ Arduino connecté", 5000);
}

void MainWindow::onArduinoDisconnected() {
    ui->statusbar->showMessage("⚠️ Arduino déconnecté", 5000);
}

void MainWindow::onArduinoError(const QString &error) {
    QMessageBox::warning(this, "Erreur Arduino", error);
}

void MainWindow::on_pushButton_assignBadge_clicked() {
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez d'abord un employé !");
        return;
    }

    bool ok;
    QString code = QInputDialog::getText(this, "Assigner Code Badge",
                                         "Entrez le code badge (4 chiffres):",
                                         QLineEdit::Normal, "", &ok);

    if (ok && !code.isEmpty()) {
        if (!code.contains(QRegularExpression("^[0-9]{4}$"))) {
            QMessageBox::warning(this, "Erreur", "❌ Code invalide !\nDoit contenir exactement 4 chiffres.");
            return;
        }

        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM empolye WHERE CODE_BADGE = ? AND IDEMPLOYE != ?");
        checkQuery.addBindValue(code);
        checkQuery.addBindValue(idEmployeSelectionne);

        if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Erreur", "❌ Ce code est déjà utilisé par un autre employé !");
            return;
        }

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE empolye SET CODE_BADGE = ? WHERE IDEMPLOYE = ?");
        updateQuery.addBindValue(code);
        updateQuery.addBindValue(idEmployeSelectionne);

        if (updateQuery.exec()) {
            afficherEmployes();
            QMessageBox::information(this, "Succès",
                                     "✅ Code badge assigné avec succès !\n"
                                     "Code: " + code + "\n"
                                                  "Employé: " + ui->lineEdit_13->text());

            HistoryManager::ajouterLog("BADGE",
                                       QString("ID %1 → Code %2").arg(idEmployeSelectionne).arg(code));
        } else {
            QMessageBox::critical(this, "Erreur", "❌ Erreur lors de l'assignation du code.");
        }
    }
}

void MainWindow::on_pushButton_removeBadge_clicked() {
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez d'abord un employé !");
        return;
    }

    if (QMessageBox::question(this, "Confirmer",
                              "Supprimer le code badge de cet employé ?") == QMessageBox::Yes) {

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE empolye SET CODE_BADGE = NULL WHERE IDEMPLOYE = ?");
        updateQuery.addBindValue(idEmployeSelectionne);

        if (updateQuery.exec()) {
            afficherEmployes();
            QMessageBox::information(this, "Succès",
                                     "✅ Code badge supprimé avec succès !\n"
                                     "Employé: " + ui->lineEdit_13->text());

            HistoryManager::ajouterLog("BADGE_SUPPRIME",
                                       QString("ID %1 → Code supprimé").arg(idEmployeSelectionne));
        } else {
            QMessageBox::critical(this, "Erreur", "❌ Erreur lors de la suppression du code.");
        }
    }
}

// ==================== HISTORIQUE DES ACCÈS ====================

void MainWindow::on_btnShowAccessHistory_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Historique des Accès");
    dialog->setMinimumSize(900, 500);
    dialog->setStyleSheet("QDialog { background-color: white; }");

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *title = new QLabel("Historique des Accès Badge");
    title->setStyleSheet("font: bold 18pt 'Arial'; color: #7D4FEE; padding: 10px;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QTableWidget *table = new QTableWidget(dialog);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "ID Employé", "Nom", "Date/Heure", "Type"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Charger les données depuis la base de données
    QSqlQuery query;
    query.exec("SELECT ID_ACCES, ID_EMPLOYE, NOM_EMPLOYE, HEURE_ACCES, TYPE_ACCES "
               "FROM HISTORIQUE_ACCES ORDER BY HEURE_ACCES DESC LIMIT 100");

    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        table->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));

        // Formater la date/heure
        QDateTime dt = query.value(3).toDateTime();
        table->setItem(row, 3, new QTableWidgetItem(dt.toString("dd/MM/yyyy HH:mm:ss")));

        QString type = query.value(4).toString();
        QTableWidgetItem *typeItem = new QTableWidgetItem(type);

        // Colorier selon le type
        if (type == "ENTREE") {
            typeItem->setBackground(QColor(220, 255, 220));
            typeItem->setForeground(Qt::darkGreen);
        } else if (type == "SORTIE") {
            typeItem->setBackground(QColor(255, 220, 220));
            typeItem->setForeground(Qt::darkRed);
        }

        table->setItem(row, 4, typeItem);
        row++;
    }

    if (row == 0) {
        table->setRowCount(1);
        table->setItem(0, 0, new QTableWidgetItem("Aucun historique trouvé"));
        table->setSpan(0, 0, 1, 5);
    }

    layout->addWidget(table);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *exportBtn = new QPushButton("📄 Exporter CSV");
    connect(exportBtn, &QPushButton::clicked, [this, table]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Exporter Historique",
                                                        "historique_acces.csv", "CSV (*.csv)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream stream(&file);

                // En-têtes
                for (int col = 0; col < table->columnCount(); ++col) {
                    stream << table->horizontalHeaderItem(col)->text();
                    if (col < table->columnCount() - 1) stream << ";";
                }
                stream << "\n";

                // Données
                for (int row = 0; row < table->rowCount(); ++row) {
                    for (int col = 0; col < table->columnCount(); ++col) {
                        QTableWidgetItem *item = table->item(row, col);
                        if (item) {
                            stream << item->text();
                        }
                        if (col < table->columnCount() - 1) stream << ";";
                    }
                    stream << "\n";
                }

                file.close();
                QMessageBox::information(this, "Succès", "Historique exporté avec succès !");
            }
        }
    });

    QPushButton *refreshBtn = new QPushButton("🔄 Actualiser");
    connect(refreshBtn, &QPushButton::clicked, [this, dialog]() {
        dialog->accept();
        on_btnShowAccessHistory_clicked(); // Rappeler la fonction pour rafraîchir
    });

    QPushButton *closeBtn = new QPushButton("Fermer");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);

    buttonLayout->addWidget(exportBtn);
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addWidget(closeBtn);
    layout->addLayout(buttonLayout);

    dialog->exec();
}

// ==================== FONCTION DE RECONNEXION ARDUINO ====================

void MainWindow::reconnectArduino()
{
    if (serialManager) {
        // Fermer toutes les connexions série
        serialManager->disconnectArduino();

        // Attendre 1 seconde
        QTimer::singleShot(1000, [this]() {
            QStringList ports = serialManager->getAvailablePorts();
            qDebug() << "Reconnexion - Ports disponibles:" << ports;

            for (const QString &port : ports) {
                if (serialManager->connectToArduino(port)) {
                    ui->statusbar->showMessage("✅ Arduino reconnecté sur " + port, 5000);
                    return;
                }
            }
            ui->statusbar->showMessage("❌ Échec de reconnexion", 5000);
        });
    }
}
// ==================== ARDUINO/BADGE SLOTS ====================

void MainWindow::onCodeReceived(const QString &code)
{
    qDebug() << "Code reçu, traitement en cours...";
    processBadgeCode(code);
}

void MainWindow::onAccessGranted(const QString &nomEmploye)
{
    if (!nomEmploye.isEmpty()) {
        ui->statusbar->showMessage("✅ Accès autorisé pour " + nomEmploye, 5000);

        // Afficher une notification
        QMessageBox::information(this, "Accès Autorisé",
                                 "Bienvenue " + nomEmploye + " !\n"
                                                             "Porte ouverte pendant 5 secondes.");

        // Enregistrer dans l'historique
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO HISTORIQUE_ACCES (NOM_EMPLOYE, HEURE_ACCES, TYPE_ACCES) "
                            "VALUES (?, ?, ?)");
        insertQuery.addBindValue(nomEmploye);
        insertQuery.addBindValue(QDateTime::currentDateTime());
        insertQuery.addBindValue("ENTREE");
        insertQuery.exec();
    } else {
        ui->statusbar->showMessage("✅ Porte ouverte", 5000);
    }
}


void MainWindow::onAccessDenied(const QString &code)
{
    ui->statusbar->showMessage("❌ Accès refusé", 5000);
    QMessageBox::warning(this, "Accès Refusé", "Code invalide ou non reconnu.");
}
