#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include "employes.h"
#include "curvewidget.h"     // Graphiques Courbes (Sponsors)
#include "barchartwidget.h"  // Graphiques Barres (Salaires)
#include "fichedepaie.h"       // PDF Paie
#include "historymanager.h"  // Historique
#include "smtp.h"            // Mailing
#include <QClipboard> // <-- add this
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
#include <QTableWidgetItem>
#include <QDebug>
#include <QRegularExpression>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
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
#include "qrcodegenerator.h"
#include "arduino.h"


// ==================== CONSTRUCTEUR ====================

MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , idEmployeSelectionne(-1)
    , idSponsorSelectionne(-1)
    , chatbot(new Chatbot())
    , smsManager(new SMSManager())
{


    ui->setupUi(this);
    // === ARDUINO CONNEXION ===
    arduino = new Arduino(this);

    // Tentative de connexion (tu peux rendre le port configurable plus tard)
    if (arduino->connectArduino("COM17")) {
        qDebug() << "Arduino connecté avec succès !";
    } else {
        qDebug() << "Échec connexion Arduino (normal si pas branché)";
        // Pas grave en dev
    }

    // Connect Arduino dataReceived once
    connect(arduino, &Arduino::dataReceived, this, [](const QByteArray &data) {
        qDebug() << "Arduino dit :" << data.trimmed();
    });
    //connect createur
    ui->creatorTable_7->setAttribute(Qt::WA_StyledBackground, true);
    ui->tableFactures->setAttribute(Qt::WA_StyledBackground, true);
    //
    //finance table
    ui->tableFactures->setColumnCount(7);
    ui->tableFactures->setHorizontalHeaderLabels({"Invoice ID","Amount","Issue Date","Due Date","Status","Employee","Sponsor"});
    //
    ui->platformCombo_7->setCurrentText("YouTube");
    ui->comboBox_7->setCurrentText("male");
    // Set initial button text
    ui->themeButton_7->setText("Dark Theme");
    // Content Creator
    connect(ui->searchEdit_9, &QLineEdit::textChanged, this, &MainWindow::on_searchEdit_9_textChanged);
    connect(ui->creatorTable_7, &QTableWidget::itemSelectionChanged, this, &MainWindow::fillFormFromTable);
    connect(ui->addButton_7, &QPushButton::clicked, this, &MainWindow::on_addButton_7_clicked);
    connect(ui->updateButton_7, &QPushButton::clicked, this, &MainWindow::on_updateButton_7_clicked);
    connect(ui->deleteButton_7, &QPushButton::clicked, this, &MainWindow::on_deleteButton_7_clicked);
    connect(ui->clearButton_7, &QPushButton::clicked, this, &MainWindow::on_clearButton_7_clicked);
    connect(ui->exportButton_7, &QPushButton::clicked, this, &MainWindow::on_exportButton_7_clicked);
    connect(ui->themeButton_7, &QPushButton::clicked, this, &MainWindow::on_themeButton_7_clicked);
    // QR Code
    connect(ui->btnGenerateQR_2, &QPushButton::clicked, this, &MainWindow::generateQR);
    connect(ui->btnSaveQR_2, &QPushButton::clicked, this, &MainWindow::saveQR);
    connect(ui->btnCopyQR_2, &QPushButton::clicked, this, &MainWindow::copyQR);
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
    ui->lineEditID_deal->setValidator(new QIntValidator(1, 999999, this));
    ui->lineEditID_deal->setMaxLength(6);


    // Autoriser uniquement les chiffres dans l'ID
    ui->lineEditID_deal->setValidator(new QIntValidator(1, 999999, this));
    // Optionnel : max 6 chiffres
    ui->lineEditID_deal->setMaxLength(6);


    ui->tableStats_deal->setColumnCount(4);
    ui->tableStats_deal->setHorizontalHeaderLabels(QStringList() << "Material" << "In Stock" << "Booked" << "Usage %");
    ui->tableStats_deal->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableStats_deal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableStats_deal->setSelectionMode(QAbstractItemView::SingleSelection);
    updateStatsTable();  // Initial update to show empty stats



    // --- 1. NETTOYAGE & INITIALISATION ---
    ui->lineEdit_6->clear(); // Vide le texte par défaut de la recherche

    // --- 2. CONNEXIONS MANUELLES (Touche Entrée uniquement) ---
    // Les boutons (dont MAILING, HISTORY, STATS) sont connectés auto via on_..._clicked
    connect(ui->lineEdit_2, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_25_clicked);
    connect(ui->lineEdit_4, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_26_clicked);

    // --- 3. PLACEHOLDERS (INDICES VISUELS GRIS) ---
    ui->lineEdit_12->setPlaceholderText("Généré Auto");              // ID
    ui->lineEdit_13->setPlaceholderText("Lettres uniquement");       // Nom
    ui->lineEdit_14->setPlaceholderText("Lettres uniquement");       // Prénom
    ui->lineEdit_15->setPlaceholderText("8 Chiffres exacts");        // CIN
    ui->lineEdit_16->setPlaceholderText("Format: MM/JJ/AAAA");       // Date
    ui->lineEdit_17->setPlaceholderText("exemple@email.com");        // Email
    ui->lineEdit_18->setPlaceholderText("8 Chiffres exacts");        // Tél
    ui->lineEdit_19->setPlaceholderText("M ou F");                   // Sexe
    ui->lineEdit_14->setPlaceholderText("Lettres uniquement");       // Prénom
    ui->lineEdit_20->setPlaceholderText("Ex: 1200.50");              // Salaire
    ui->lineEdit_21->setPlaceholderText("Chiffres uniquement");      // ID Sup
    ui->lineEdit_6->setPlaceholderText("recherche par nom, prenom, cin"); // Barre Recherche

    // --- 4. VALIDATORS (BLOCAGE CLAVIER) ---

    // A. Lettres (Nom, Prénom)
    QRegularExpression regexLettres("^[a-zA-ZÀ-ÿ\\s'-]*$");
    QValidator *validatorLettres = new QRegularExpressionValidator(regexLettres, this);
    ui->lineEdit_13->setValidator(validatorLettres);
    ui->lineEdit_14->setValidator(validatorLettres);

    // B. Chiffres (CIN, Tél)
    QRegularExpression regexChiffres("^[0-9]*$");
    QValidator *validatorChiffres = new QRegularExpressionValidator(regexChiffres, this);
    ui->lineEdit_15->setValidator(validatorChiffres); ui->lineEdit_15->setMaxLength(8);
    ui->lineEdit_18->setValidator(validatorChiffres); ui->lineEdit_18->setMaxLength(8);

    // C. Divers
    ui->lineEdit_19->setMaxLength(1); // Sexe
    ui->lineEdit_21->setValidator(new QIntValidator(0, 999999, this)); // ID Sup

    // D. Salaire (Décimal)
    QDoubleValidator *salaireVal = new QDoubleValidator(0.0, 999999.0, 2, this);
    salaireVal->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_20->setValidator(salaireVal);

    // --- 5. CHARGEMENT DES DONNÉES ---
    initEmployesTable();
    initSponsorsTable();
    afficherEmployes();
    afficherSponsors();

    ui->lineEdit->setText("🤖 Bonjour ! Je suis prêt à vous aider.");
    ui->pushButton_stats->setText("📊 Stats Graphiques");
}

MainWindow::~MainWindow()
{
    delete chatbot;
    delete smsManager;
    delete ui;
}

// ==================== NAVIGATION ====================
void MainWindow::on_sidebarButton_clicked() { ui->stackedWidget->setCurrentIndex(0); }
void MainWindow::on_sidebarButton_2_clicked() { ui->stackedWidget->setCurrentIndex(1); }
void MainWindow::on_sidebarButton_3_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_sidebarButton_4_clicked() { ui->stackedWidget->setCurrentIndex(3); afficherSponsors(); }
void MainWindow::on_sidebarButton12_clicked() { ui->stackedWidget->setCurrentWidget(ui->employeePage); afficherEmployes(); }

// ==================== TABLEAUX ====================
void MainWindow::initEmployesTable() {
    ui->tableWidget->setColumnCount(9);
    QStringList headers; headers << "ID" << "Prénom (Nom)" << "Date Emb." << "Email" << "Tél" << "Sexe" << "Poste" << "Salaire" << "Superviseur";
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

// ==================== AFFICHAGE STANDARD ====================
void MainWindow::afficherEmployes() {
    Employes e; QSqlQueryModel *model = e.afficher();
    ui->tableWidget->setRowCount(0);
    if (model) {
        for (int r = 0; r < model->rowCount(); ++r) {
            ui->tableWidget->insertRow(r);
            for (int c = 0; c < 9; ++c) {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(r, c)).toString());
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                ui->tableWidget->setItem(r, c, item);
            }
        }
        delete model;
    }
}

void MainWindow::afficherSponsors() {
    Sponsor s; QSqlQueryModel *model = s.afficher();
    ui->sponsorTable_9->setRowCount(0);
    if (model) {
        for (int r = 0; r < model->rowCount(); ++r) {
            ui->sponsorTable_9->insertRow(r);
            for (int c = 0; c < 8; ++c) {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(r, c)).toString());
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
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
            for (int c = 0; c < 9; ++c) {
                QString dataStr = model->data(model->index(r, c)).toString();
                QTableWidgetItem *item = new QTableWidgetItem(dataStr);

                // VISUEL : SURLIGNAGE EN JAUNE
                if (!text.isEmpty() && dataStr.contains(text, Qt::CaseInsensitive)) {
                    item->setBackground(QColor(255, 235, 59)); // Jaune vif
                    item->setForeground(Qt::black);            // Texte noir
                    item->setFont(QFont("Arial", 10, QFont::Bold)); // Gras
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
    QString nom=ui->lineEdit_13->text().trimmed(), prenom=ui->lineEdit_14->text().trimmed(), cin=ui->lineEdit_15->text().trimmed();
    QString dateStr=ui->lineEdit_16->text().trimmed(), email=ui->lineEdit_17->text().trimmed(), tel=ui->lineEdit_18->text().trimmed();
    QString sexe=ui->lineEdit_19->text().trimmed(), salStr=ui->lineEdit_20->text().trimmed(), idSup=ui->lineEdit_21->text().trimmed();

    // Validations
    if (!Employes::validerLettres(nom)) { QMessageBox::warning(this, "Erreur", "❌ NOM invalide."); ui->lineEdit_13->setFocus(); return; }
    if (!Employes::validerLettres(prenom)) { QMessageBox::warning(this, "Erreur", "❌ PRÉNOM invalide."); ui->lineEdit_14->setFocus(); return; }
    if (!Employes::valider8Chiffres(cin)) { QMessageBox::warning(this, "Erreur", "❌ CIN invalide."); ui->lineEdit_15->setFocus(); return; }
    if (!Employes::validerFormatDate(dateStr)) { QMessageBox::warning(this, "Erreur", "❌ Date invalide."); ui->lineEdit_16->setFocus(); return; }
    if (!Employes::validerEmail(email)) { QMessageBox::warning(this, "Erreur", "❌ Email invalide."); ui->lineEdit_17->setFocus(); return; }
    if (!Employes::valider8Chiffres(tel)) { QMessageBox::warning(this, "Erreur", "❌ Téléphone invalide."); ui->lineEdit_18->setFocus(); return; }
    if (!Employes::validerSexe(sexe)) { QMessageBox::warning(this, "Erreur", "❌ Sexe invalide."); ui->lineEdit_19->setFocus(); return; }

    bool ok; double sal = salStr.toDouble(&ok);
    if (!ok || !Employes::validerSalaire(sal)) { QMessageBox::warning(this, "Erreur", "❌ Salaire invalide."); ui->lineEdit_20->setFocus(); return; }
    if (!idSup.isEmpty() && !Employes::validerChiffres(idSup)) { QMessageBox::warning(this, "Erreur", "❌ ID Superviseur invalide."); ui->lineEdit_21->setFocus(); return; }

    QString nomComplet = nom + " " + prenom;
    Employes emp(0, nomComplet, QDate::fromString(dateStr, "MM/dd/yyyy"), email, tel, sexe, "Poste", sal, idSup);

    if (emp.ajouter()) {
        afficherEmployes();
        clearEmployeFields();

        // ✅ HISTORIQUE DÉTAILLÉ
        QString details = QString("Nom: %1 | Email: %2 | Tél: %3").arg(nomComplet).arg(email).arg(tel);
        HistoryManager::ajouterLog("AJOUT", details);

        QMessageBox::information(this, "Succès", "✅ Employé ajouté !");
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Erreur base de données.");
    }
}

void MainWindow::on_pushButton_8_clicked() // MODIFIER
{
    if (idEmployeSelectionne == -1) { QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé."); return; }

    QString nom=ui->lineEdit_13->text().trimmed();
    QString email=ui->lineEdit_17->text().trimmed(), tel=ui->lineEdit_18->text().trimmed();
    QString idSup=ui->lineEdit_21->text().trimmed();
    double sal = ui->lineEdit_20->text().toDouble();

    if (!Employes::validerEmail(email)) { QMessageBox::warning(this, "Erreur", "Email invalide."); return; }

    Employes emp;
    if (emp.modifier(idEmployeSelectionne, email, tel, "Poste", sal, idSup)) {
        afficherEmployes();

        // ✅ HISTORIQUE DÉTAILLÉ
        QString details = QString("ID: %1 | Nom: %2 | Email: %3 | Nouv. Salaire: %4").arg(idEmployeSelectionne).arg(nom).arg(email).arg(sal);
        HistoryManager::ajouterLog("MODIFICATION", details);

        QMessageBox::information(this, "Succès", "✅ Modifié !");
    }
}

void MainWindow::on_pushButton_10_clicked() // SUPPRIMER
{
    if (idEmployeSelectionne == -1) { QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé."); return; }

    // Capture des données pour l'historique
    QString details = QString("ID: %1 | Nom: %2 | Email: %3").arg(idEmployeSelectionne).arg(ui->lineEdit_13->text()).arg(ui->lineEdit_17->text());

    if (QMessageBox::question(this, "Confirmer", "Supprimer ?") == QMessageBox::Yes) {
        Employes emp;
        if (emp.supprimer(idEmployeSelectionne)) {
            afficherEmployes();
            clearEmployeFields();

            // ✅ HISTORIQUE DÉTAILLÉ
            HistoryManager::ajouterLog("SUPPRESSION", details);

            idEmployeSelectionne = -1;
            QMessageBox::information(this, "Succès", "Supprimé !");
        }
    }
}

// ==================== HISTORIQUE (TABLEAU) ====================

void MainWindow::on_HISTORY_clicked()
{
    QString content = HistoryManager::lireHistorique();
    QDialog *d = new QDialog(this); d->setWindowTitle("Historique Employés"); d->setMinimumSize(900, 600); d->setStyleSheet("background:white;");
    QVBoxLayout *l = new QVBoxLayout(d);
    QLabel *t = new QLabel("Journal des Activités"); t->setStyleSheet("font: bold 16pt 'Arial'; color: #333;"); t->setAlignment(Qt::AlignCenter); l->addWidget(t);

    QTableWidget *table = new QTableWidget(d); table->setColumnCount(3); table->setHorizontalHeaderLabels({"Date", "Action", "Détails"});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    QStringList lignes = content.split("\n", Qt::SkipEmptyParts);
    QRegularExpression regex("^\\[(.*?)\\] \\[(.*?)\\] (.*)$");

    for (const QString &ligne : lignes) {
        QRegularExpressionMatch match = regex.match(ligne);
        if (match.hasMatch()) {
            int r = table->rowCount(); table->insertRow(r);
            QTableWidgetItem *type = new QTableWidgetItem(match.captured(2));
            type->setTextAlignment(Qt::AlignCenter);

            if (match.captured(2) == "AJOUT") { type->setBackground(QColor(220, 255, 220)); type->setForeground(QColor(0, 100, 0)); }
            else if (match.captured(2) == "SUPPRESSION") { type->setBackground(QColor(255, 220, 220)); type->setForeground(QColor(139, 0, 0)); }
            else { type->setBackground(QColor(220, 240, 255)); type->setForeground(QColor(0, 0, 139)); }

            table->setItem(r, 0, new QTableWidgetItem(match.captured(1)));
            table->setItem(r, 1, type);
            table->setItem(r, 2, new QTableWidgetItem(match.captured(3)));
        }
    }
    l->addWidget(table);
    QPushButton *btn = new QPushButton("Fermer"); connect(btn, &QPushButton::clicked, d, &QDialog::accept); l->addWidget(btn);
    d->exec();
}

// ---------------------- TRI INTERACTIF ----------------------
void MainWindow::on_pushButton_12_clicked() // TRIER
{
    QMessageBox msgBox; msgBox.setWindowTitle("Option de Tri"); msgBox.setText("Comment voulez-vous trier les salaires ?");
    QAbstractButton *desc = msgBox.addButton("Décroissant 📉", QMessageBox::YesRole);
    QAbstractButton *asc = msgBox.addButton("Croissant 📈", QMessageBox::NoRole);
    msgBox.addButton("Annuler", QMessageBox::RejectRole);
    msgBox.exec();

    bool croissant = (msgBox.clickedButton() == asc);
    if (msgBox.clickedButton() != desc && msgBox.clickedButton() != asc) return;

    Employes e; QSqlQueryModel *model = e.trierParSalaire(croissant);
    ui->tableWidget->setRowCount(0);
    if (model) {
        for (int r = 0; r < model->rowCount(); ++r) {
            ui->tableWidget->insertRow(r);
            for (int c = 0; c < 9; ++c) ui->tableWidget->setItem(r, c, new QTableWidgetItem(model->data(model->index(r, c)).toString()));
        }
        delete model;
    }
    QMessageBox::information(this, "Tri", croissant ? "Tri Croissant" : "Tri Décroissant");
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    idEmployeSelectionne = ui->tableWidget->item(row, 0)->text().toInt();

    ui->lineEdit_12->setText(ui->tableWidget->item(row, 0)->text()); // ID
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
}

// ==================== MAILING (SMTP) ====================

void MainWindow::on_MAILING_clicked()
{
    if (idEmployeSelectionne == -1) { QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé."); return; }

    QString emailDest = ui->lineEdit_17->text().trimmed();
    QString nomEmp = ui->lineEdit_13->text().trimmed();

    if (!Employes::validerEmail(emailDest)) { QMessageBox::warning(this, "Erreur", "❌ Email invalide."); return; }

    if (QMessageBox::question(this, "Confirmer Envoi", "Envoyer le mail de motivation à " + nomEmp + " ?") == QMessageBox::Yes) {

        // CONFIGURATION GMAIL (A REMPLIR)
        QString user = "amenalaya57@gmail.com";
        QString pass = "sgnz xdto zonf jhsv"; // App Password

        Smtp *smtp = new Smtp(user, pass, "smtp.gmail.com", 465);
        QString sujet = "Remerciements - CONNECT PLUS+";
        QString corps = "Bonjour " + nomEmp + ",\n\n"
                                              "Je tiens à vous remercier sincèrement pour les efforts que vous avez fournis ces derniers jours. "
                                              "Votre engagement, votre sérieux et votre esprit d’équipe ont eu un réel impact sur l’avancement de notre travail.\n\n"
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
    if (idEmployeSelectionne == -1) { QMessageBox::warning(this, "Erreur", "Sélectionnez un employé."); return; }
    QString f = QFileDialog::getSaveFileName(this, "Fiche Paie", "FichePaie.pdf", "PDF (*.pdf)");
    if (!f.isEmpty()) { FichePaie g; if (g.genererPDF(idEmployeSelectionne, f)) { QMessageBox::information(this, "Succès", "PDF généré !"); QDesktopServices::openUrl(QUrl::fromLocalFile(f)); } }
}

void MainWindow::on_STATS_clicked() // BAR CHART
{
    QVector<int> v; QStringList l; Employes::getStatistiquesSalaires(v, l);
    QDialog *d = new QDialog(this); d->setWindowTitle("Stats Salaires"); d->setMinimumSize(600, 400); d->setStyleSheet("background:white;");
    QVBoxLayout *lay = new QVBoxLayout(d); BarChartWidget *chart = new BarChartWidget(); chart->setData(v, l); lay->addWidget(chart);
    d->exec();
}

// ==================== SPONSORS (CRUD) ====================
void MainWindow::on_pushButton_23_clicked() {
    if (!Sponsor::validerNom(ui->nom->text())) return;
    Sponsor s(ui->nom->text(), ui->prenom->text(), ui->categorie->text(), ui->email->text(), ui->debut->date(), ui->fin->date(), ui->budget->text().toInt());
    if (s.ajouter()) { afficherSponsors(); clearSponsorFields(); }
}
void MainWindow::on_pushButton_38_clicked() {
    if (idSponsorSelectionne == -1) return;
    Sponsor s;
    if (s.modifier(idSponsorSelectionne, ui->nom2->text(), ui->prenom2->text(), ui->categorie2->text(), ui->email2->text(), ui->debut2->date(), ui->fin2->date(), ui->budget2->text().toInt())) afficherSponsors();
}
void MainWindow::on_pushButton_40_clicked() {
    if (idSponsorSelectionne == -1) return;
    if (QMessageBox::question(this, "Confirmer", "Supprimer ?") == QMessageBox::Yes) { Sponsor s; if (s.supprimer(idSponsorSelectionne)) afficherSponsors(); }
}
void MainWindow::on_sponsorTable_9_clicked(const QModelIndex &index) {
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
}
void MainWindow::on_refreshbtn_clicked() { afficherSponsors(); }

// ==================== STATS SPONSORS ====================
void MainWindow::on_pushButton_stats_clicked() { afficherStatistiquesCourbes(); }
void MainWindow::afficherStatistiquesCourbes() {
    QDialog *d = new QDialog(this); d->setMinimumSize(1200, 800); QVBoxLayout *l = new QVBoxLayout(d); QGridLayout *g = new QGridLayout();
    QGroupBox *b1 = new QGroupBox("Évolution"); QVBoxLayout *l1 = new QVBoxLayout(b1); creerCourbeEvolutionContrats(l1); g->addWidget(b1, 0, 0);
    QGroupBox *b2 = new QGroupBox("Budgets"); QVBoxLayout *l2 = new QVBoxLayout(b2); creerCourbeBudgets(l2); g->addWidget(b2, 0, 1);
    l->addLayout(g); QPushButton *c = new QPushButton("Fermer"); connect(c, &QPushButton::clicked, d, &QDialog::accept); l->addWidget(c); d->exec();
}
void MainWindow::creerCourbeEvolutionContrats(QVBoxLayout *l) { CurveWidget *w = new CurveWidget(); w->setData(Sponsor::getDonneesEvolutionContrats(), "Contrats", Qt::red); l->addWidget(w); }
void MainWindow::creerCourbeBudgets(QVBoxLayout *l) { QVector<QPointF> d; QStringList s; Sponsor::getDonneesBudgetParCategorie(d, s); CurveWidget *w = new CurveWidget(); w->setData(d, "Budget", Qt::blue); l->addWidget(w); }
void MainWindow::creerGraphiqueCategories(QVBoxLayout *l) {}
void MainWindow::creerGraphiqueDureeContrats(QVBoxLayout *l) {}
void MainWindow::exporterStatistiquesPDF() {}

// ==================== CHATBOT & SMS ====================
void MainWindow::on_pushButton_25_clicked() {
    QString q = ui->lineEdit_2->text().trimmed(); if(q.isEmpty()) return;
    ui->lineEdit->setText("👤 " + q + "\n🤖 " + chatbot->obtenirReponse(q, idSponsorSelectionne)); ui->lineEdit_2->clear();
}
void MainWindow::on_pushButton_26_clicked() {
    if(smsManager->envoyerSMS(ui->lineEdit_3->text(), ui->lineEdit_4->text())) QMessageBox::information(this, "OK", "Envoyé !");
}
void MainWindow::on_btnConfigSMS_clicked() { QMessageBox::information(this, "Info", "API chargée"); }
void MainWindow::on_ConfigurerGemini_clicked() {
    QDialog d(this); QVBoxLayout *l = new QVBoxLayout(&d); QLineEdit *k = new QLineEdit(&d);
    l->addWidget(new QLabel("Clé API:")); l->addWidget(k); QDialogButtonBox *b = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel); l->addWidget(b);
    connect(b, &QDialogButtonBox::accepted, &d, &QDialog::accept);
    if(d.exec() == QDialog::Accepted) chatbot->configurerAPI(k->text());
}

// ==================== OUTILS ====================
void MainWindow::exporterSponsorsPDFParContrat() { QString f = QFileDialog::getSaveFileName(this, "Export", "", "HTML (*.html)"); if (!f.isEmpty()) Sponsor::exporterPDFParContrat(f); }
void MainWindow::trierSponsorsParDate() {
    QSqlQueryModel *model = Sponsor().trierParDate();
    ui->sponsorTable_9->setRowCount(0);
    if (model) { for(int row=0; row < model->rowCount(); ++row) { ui->sponsorTable_9->insertRow(row); for(int col=0; col < 8; ++col) ui->sponsorTable_9->setItem(row, col, new QTableWidgetItem(model->data(model->index(row, col)).toString())); } delete model; }
    QMessageBox::information(this, "Tri", "✅ Trié par date !");
}
void MainWindow::on_pushButton_35_clicked() { exporterSponsorsPDFParContrat(); }
void MainWindow::on_pushButton_37_clicked() { trierSponsorsParDate(); }
void MainWindow::on_pushButton_39_clicked() { exporterSponsorsPDFParContrat(); }
void MainWindow::on_pushButton_41_clicked() { trierSponsorsParDate(); }
void MainWindow::rechercherSponsors(const QString &t) {
    Sponsor s; QSqlQueryModel *m = t.isEmpty() ? s.afficher() : s.rechercherParNom(t);
    ui->sponsorTable_9->setRowCount(0);
    if(m) { for(int r=0; r<m->rowCount(); ++r) { ui->sponsorTable_9->insertRow(r); for(int c=0; c<8; ++c) ui->sponsorTable_9->setItem(r, c, new QTableWidgetItem(m->data(m->index(r, c)).toString())); } delete m; }
}
void MainWindow::on_searchEdit_7_textChanged(const QString &t) { rechercherSponsors(t); }
void MainWindow::on_searchEdit_8_textChanged(const QString &t) { rechercherSponsors(t); }
void MainWindow::on_pushButton_24_clicked() { rechercherSponsors(ui->searchEdit_7->text()); }

void MainWindow::clearEmployeFields() {
    ui->lineEdit_12->clear(); ui->lineEdit_13->clear(); ui->lineEdit_14->clear(); ui->lineEdit_15->clear(); ui->lineEdit_16->clear();
    ui->lineEdit_17->clear(); ui->lineEdit_18->clear(); ui->lineEdit_19->clear(); ui->lineEdit_20->clear(); ui->lineEdit_21->clear();
}
void MainWindow::clearSponsorFields() { ui->nom->clear(); ui->prenom->clear(); ui->email->clear(); ui->categorie->clear(); ui->budget->clear(); }
void MainWindow::on_lineEdit_textEdited(const QString &) {}
void MainWindow::on_lineEdit_cursorPositionChanged(int, int) {}
// ================== DEAL PAGE CORE LOGIC ==================

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
        // Vérifier que l'heure est entre 8h et 20h
        QTime t = current.time();
        if (t < QTime(8,0) || t > QTime(20,0)) {
            current = current.addSecs(3600); // passe à l'heure suivante
            continue;
        }

        bool conflict = false;
        for (int row = 0; row < ui->tableWidget_deal->rowCount(); ++row) {
            if (editedRow >= 0 && row == editedRow) continue; // ignorer la ligne en cours d'édition

            QString timeStr = ui->tableWidget_deal->item(row, 3)->text();
            QDateTime bookedTime = QDateTime::fromString(timeStr, "yyyy-MM-dd hh:mm");
            if (bookedTime == current) {
                int bookedMask = ui->tableWidget_deal->item(row, 2)->text().toInt();
                if (bookedMask & requiredMask) { // conflit sur au moins un équipement
                    conflict = true;
                    break;
                }
            }
        }

        if (!conflict) {
            return current;
        }

        // Passer à l'heure suivante
        current = current.addSecs(3600);

        // Si on dépasse 20h, aller au lendemain à 8h
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
    // === 1. Récupération et contrôles de base ===
    QString idText = ui->lineEditID_deal->text().trimmed();
    QString name = ui->lineEditName_deal->text().trimmed();
    QDateTime selectedTime = ui->dateTimeEdit_deal->dateTime();
    int mask = getEquipMask();

    // ID uniquement chiffres
    if (idText.isEmpty() || !idText.contains(QRegularExpression("^[0-9]+$"))) {
        QMessageBox::warning(this, "Erreur", "L'ID doit contenir uniquement des chiffres !");
        return;
    }
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom du client est obligatoire !");
        return;
    }

    // Horaire entre 8h et 20h
    QTime time = selectedTime.time();
    if (time < QTime(8,0) || time > QTime(20,0)) {
        QMessageBox::warning(this, "Horaire invalide",
                             "Réservations uniquement entre 08:00 et 20:00 !\nHeure choisie : " + time.toString("hh:mm"));
        return;
    }

    QDateTime finalTime = selectedTime;

    // === 2. Vérifier les conflits ===
    QDateTime conflictTime = findNextAvailableSlot(selectedTime, mask);

    if (conflictTime != selectedTime) {
        // Il y a un conflit → proposer l'heure suivante
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
        finalTime = conflictTime; // Utiliser l'heure proposée
    }

    // === 3. Ajout ou Modification ===
    if (editedRow >= 0) {
        // --- MODIFICATION ---
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
        // --- NOUVELLE RÉSERVATION ---
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

    // === 4. Nettoyage ===
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

    // --- Get current values from the selected row ---
    QString id = ui->tableWidget_deal->item(row, 0)->text();
    QString name = ui->tableWidget_deal->item(row, 1)->text();
    QString maskText = ui->tableWidget_deal->item(row, 2)->text();
    QString dateTimeStr = ui->tableWidget_deal->item(row, 3)->text();

    // --- Fill the input fields with current values ---
    ui->lineEditID_deal->setText(id);
    ui->lineEditName_deal->setText(name);

    // Parse date/time and set in QDateTimeEdit
    QDateTime dt = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm");
    if (dt.isValid()) {
        ui->dateTimeEdit_deal->setDateTime(dt);
    }

    // --- Set checkboxes based on equipment mask ---
    int mask = maskText.toInt();

    ui->checkCamera_deal->setChecked(mask & 1);
    ui->checkMic_deal->setChecked(mask & 2);
    ui->checkLight_deal->setChecked(mask & 4);
    ui->checkCameraman_deal->setChecked(mask & 8);
    ui->checkEditor_deal->setChecked(mask & 16);
    ui->checkStudio_deal->setChecked(mask & 32);

    // Optional: Change button text or give feedback
    ui->btnBook_deal->setText("Update Booking");
    ui->btnEdit_deal->setEnabled(false);  // Prevent double-click issues
    ui->btnDelete_deal->setEnabled(false);

    // Store the row being edited (we'll use it when saving)
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
    // Clear all rows from the bookings table
    ui->tableWidget_deal->setRowCount(0);

    // Also clear the input fields (optional but recommended for clean UI)
    ui->lineEditID_deal->clear();
    ui->lineEditName_deal->clear();
    ui->searchName_deal->clear();
    ui->dateTimeEdit_deal->setDateTime(QDateTime::currentDateTime());

    // Uncheck all equipment checkboxes
    ui->checkCamera_deal->setChecked(false);
    ui->checkMic_deal->setChecked(false);
    ui->checkLight_deal->setChecked(false);
    ui->checkCameraman_deal->setChecked(false);
    ui->checkEditor_deal->setChecked(false);
    ui->checkStudio_deal->setChecked(false);

    // Update the stats table to show full stock again
    updateStatsTable();

    // Optional: Nice user feedback
    QMessageBox::information(this, "Cleared", "All bookings have been removed!");
}

void MainWindow::on_btnOrder_deal_clicked()
{
    // Sort tableWidget_deal by column 3 (Booking Time) → DateTime format: "yyyy-MM-dd hh:mm"
    ui->tableWidget_deal->sortItems(3, Qt::AscendingOrder);

    // Optional: Visual feedback
    QMessageBox::information(this, "Sorted",
                             "Bookings have been sorted by date and time (oldest first).");

    // Update stats after sorting (optional but recommended)
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

    // Create the PDF
    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);
    pdf.setPageMargins(QMarginsF(10, 10, 10, 10));

    QPainter painter(&pdf);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // -------------------------
    // TITLE
    // -------------------------
    QFont titleFont("Arial", 22, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(0, 100, pdf.width(), 100, Qt::AlignCenter,
                     "Equipment Booking Report");

    QFont dateFont("Arial", 12);
    painter.setFont(dateFont);
    painter.drawText(0, 180, pdf.width(), 50, Qt::AlignCenter,
                     "Generated: " + QDateTime::currentDateTime()
                                         .toString("dddd, dd MMMM yyyy - hh:mm"));

    // -------------------------
    // RENDER THE TABLE DIRECTLY
    // -------------------------

    // 1. Resize table to fit contents
    ui->tableWidget_deal->resizeColumnsToContents();
    ui->tableWidget_deal->resizeRowsToContents();

    // 2. Create a high-resolution image of the table
    int w = ui->tableWidget_deal->width();
    int h = ui->tableWidget_deal->height();

    QImage img(w * 3, h * 3, QImage::Format_ARGB32); // 3x resolution → ultra clear
    img.fill(Qt::white);

    QPainter imgPainter(&img);
    imgPainter.scale(3.0, 3.0);  // HD scaling
    ui->tableWidget_deal->render(&imgPainter);
    imgPainter.end();

    // 3. Draw the image inside the PDF
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
    dlg.exec();          // open popup AFTER loading colors
}
// ———————————————————————— THEME ————————————————————————
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
    // Quand on vide la recherche → on affiche tout + TRI PAR NOM
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
// ———————————————————————— CONTENT CREATOR ————————————————————————
void MainWindow::loadCreateurs()
{
    ui->creatorTable_7->clearContents();
    ui->creatorTable_7->setRowCount(0);
    QSqlQuery q;
    q.exec("SELECT IDCREATEUR, NOM, PLATFORME, GENRE, ABONNE, TYPE_DE_CONTENU FROM MY_USER.CREATEUR ORDER BY IDCREATEUR");
    int row = 0;
    while (q.next()) {
        ui->creatorTable_7->insertRow(row);
        // Colonne 0: ID
        QTableWidgetItem *idItem = new QTableWidgetItem(q.value(0).toString());
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 0, idItem);
        // Colonne 1: Nom
        QTableWidgetItem *nameItem = new QTableWidgetItem(q.value(1).toString());
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 1, nameItem);
        // Colonne 2: Plateforme
        QTableWidgetItem *platItem = new QTableWidgetItem(q.value(2).toString());
        platItem->setFlags(platItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 2, platItem);
        // Colonne 3: Genre
        QTableWidgetItem *genreItem = new QTableWidgetItem(q.value(3).toString());
        genreItem->setFlags(genreItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 3, genreItem);
        // Colonne 4: Abonnés
        QTableWidgetItem *aboItem = new QTableWidgetItem();
        aboItem->setData(Qt::DisplayRole, q.value(4).toInt());
        aboItem->setTextAlignment(Qt::AlignCenter);
        aboItem->setFlags(aboItem->flags() & ~Qt::ItemIsEditable);
        ui->creatorTable_7->setItem(row, 4, aboItem);
        // Colonne 5: Type de contenu
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

// ———————————————————————— ROLE SYSTEM ————————————————————————
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

// ———————————————————————— QR CODE ————————————————————————
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
    Createur c;
    c.id          = ui->creatorTable_7->item(row, 0)->text().toInt();
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

    // fallback: compute next id in code (use only if you cannot change the DB schema)
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
        arduino->sendCommand("ADD:" + name);
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
        arduino->sendCommand("EDIT:" + name);
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
    arduino->sendCommand("LOGIN:" + name);
}
void MainWindow::mettreEnLiveStudio(const QString &nomCreateur) {
    arduino->sendCommand("LIVE_ON:" + nomCreateur);
}

void MainWindow::arreterLiveStudio() {
    arduino->sendCommand("LIVE_OFF");
}

// === CES DEUX SLOTS SONT DÉJÀ CONNECTÉS AUTOMATIQUEMENT PAR QT ===
void MainWindow::on_pushButton_live_clicked()
{
    QString nom = ui->nameEdit_7->text().trimmed();

    // Si vide → on prend la ligne sélectionnée dans le tableau
    int row = ui->creatorTable_7->currentRow();
    if (nom.isEmpty() && row >= 0) {
        nom = ui->creatorTable_7->item(row, 1)->text(); // colonne Nom
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
//finance crud and stat
void MainWindow::addInvoice() {
    int id = ui->lineId->text().toInt();
    double montant = ui->lineMontant->text().toDouble();
    QDate emission = ui->dateEmission->date();
    QDate echeance = ui->dateEcheance->date();
    QString status = ui->comboStatut->currentText(); // requires STATUS column in DB

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
    // Bind QDate; if Oracle/ODBC requires string use emission.toString("yyyy-MM-dd")
    q.bindValue(":emission", emission);
    q.bindValue(":echeance", echeance);
    // Provide or ask user to select EMPLOYE/SPONSOR in UI — for now we use defaults 1/1 (change as needed)
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
        return;
    }

    int rows = 0;
    while (query.next()) {
        ++rows;
        // populate table as before...
        int row = ui->tableFactures->rowCount();
        ui->tableFactures->insertRow(row);

        ui->tableFactures->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // ID_FACTURE
        ui->tableFactures->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toDouble(), 'f', 2))); // MONTANT

        QDate d1 = query.value(2).toDate();
        QDate d2 = query.value(3).toDate();
        ui->tableFactures->setItem(row, 2, new QTableWidgetItem(d1.isValid() ? d1.toString("dd/MM/yyyy") : QString()));
        ui->tableFactures->setItem(row, 3, new QTableWidgetItem(d2.isValid() ? d2.toString("dd/MM/yyyy") : QString()));

        ui->tableFactures->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // STATUS
        ui->tableFactures->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // EMPLOYEE_NAME
        ui->tableFactures->setItem(row, 6, new QTableWidgetItem(query.value(6).toString())); // SPONSOR_NAME
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
    // style application left as-is
}

void MainWindow::applyDarkTheme() {
    // style application left as-is
}
