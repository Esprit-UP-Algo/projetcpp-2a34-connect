#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sponsor.h"
#include "employes.h"
#include "curvewidget.h"     // Graphiques Courbes (Sponsors)
#include "barchartwidget.h"  // Graphiques Barres (Salaires)
#include "fichedepaie.h"       // PDF Paie
#include "historymanager.h"  // Historique
#include "smtp.h"            // Mailing

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
void MainWindow::on_sidebarButton_5_clicked() { ui->stackedWidget->setCurrentIndex(4); }
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
        QString user = "xxxxxxxxxxxxxxxxxxx";
        QString pass = "xxxxxxxxxxxxxxxxxx"; // App Password

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
