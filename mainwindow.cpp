// ==================== CORRECTION COMPLÈTE MAINWINDOW.CPP ====================

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sponsor.h"
#include "employes.h"
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , idEmployeSelectionne(-1)
    , idSponsorSelectionne(-1)
{
    ui->setupUi(this);
    qDebug() << "=== INITIALISATION MainWindow ===";

    // Connecter les signaux manuellement
    connecterSignaux();

    // Initialiser les tables
    initEmployesTable();
    initSponsorsTable();

    // Charger les données
    afficherEmployes();
    afficherSponsors();

    qDebug() << "=== INITIALISATION TERMINÉE ===";
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==================== CONNEXION DES SIGNAUX ====================
void MainWindow::connecterSignaux()
{
    qDebug() << "=== CONNEXION DES SIGNAUX ===";

    // Navigation
    connect(ui->sidebarButton, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_clicked);
    connect(ui->sidebarButton_2, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_2_clicked);
    connect(ui->sidebarButton_3, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_3_clicked);
    connect(ui->sidebarButton_4, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_4_clicked);
    connect(ui->sidebarButton_5, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_5_clicked);

    if (ui->sidebarButton12) {
        connect(ui->sidebarButton12, &QPushButton::clicked, this, &MainWindow::on_sidebarButton12_clicked);
        qDebug() << "✓ sidebarButton12 connecté";
    }

    // Employés
    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::on_pushButton_11_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::on_pushButton_8_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::on_pushButton_10_clicked);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &MainWindow::on_pushButton_12_clicked);
    connect(ui->lineEdit_6, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_6_textChanged);
    connect(ui->tableWidget, &QTableWidget::clicked, this, &MainWindow::on_tableWidget_clicked);

    // Sponsors
    connect(ui->pushButton_23, &QPushButton::clicked, this, &MainWindow::on_pushButton_23_clicked);
    connect(ui->pushButton_38, &QPushButton::clicked, this, &MainWindow::on_pushButton_38_clicked);
    connect(ui->pushButton_40, &QPushButton::clicked, this, &MainWindow::on_pushButton_40_clicked);
    connect(ui->sponsorTable_9, &QTableWidget::clicked, this, &MainWindow::on_sponsorTable_9_clicked);
    connect(ui->refreshbtn, &QPushButton::clicked, this, &MainWindow::on_refreshbtn_clicked);
    connect(ui->pushButton_stats, &QPushButton::clicked, this, &MainWindow::on_pushButton_stats_clicked);
}

// ==================== NAVIGATION ====================

void MainWindow::on_sidebarButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    qDebug() << "Navigation: Management Page";
}

void MainWindow::on_sidebarButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << "Navigation: Finance Page";
}

void MainWindow::on_sidebarButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "Navigation: Content Creator Page";
}

void MainWindow::on_sidebarButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    qDebug() << "Navigation: Sponsor Page";
    afficherSponsors(); // Rafraîchir les sponsors
}

void MainWindow::on_sidebarButton_5_clicked()
{
    qDebug() << "Navigation: Deal Page";
}

void MainWindow::on_sidebarButton12_clicked()
{
    qDebug() << "Bouton employés cliqué";
    ui->stackedWidget->setCurrentWidget(ui->employeePage);
    afficherEmployes(); // Rafraîchir les employés
}

// ==================== INITIALISATION ====================

void MainWindow::initEmployesTable()
{
    ui->tableWidget->setColumnCount(9);
    QStringList headers;
    headers << "ID" << "Prénom" << "Date" << "Email" << "Tél" << "Sexe" << "Poste" << "Salaire" << "Superviseur";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    qDebug() << "✓ Table employés initialisée";
}

void MainWindow::initSponsorsTable()
{
    ui->sponsorTable_9->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "Catégorie" << "Email" << "Début" << "Fin" << "Budget";
    ui->sponsorTable_9->setHorizontalHeaderLabels(headers);
    ui->sponsorTable_9->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sponsorTable_9->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->sponsorTable_9->horizontalHeader()->setStretchLastSection(true);
    qDebug() << "✓ Table sponsors initialisée";
}

// ==================== AFFICHAGE EMPLOYÉS - CORRIGÉ ====================

void MainWindow::afficherEmployes()
{
    qDebug() << "=== AFFICHAGE EMPLOYÉS ===";

    QSqlQuery query;
    // ✅ CORRECTION: Utiliser "empolye" (sans S) et les bons noms de colonnes
    QString sql = "SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye";

    qDebug() << "Requête SQL:" << sql;

    if (!query.exec(sql)) {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur",
                              "❌ Impossible de charger les employés:\n" + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);

        for (int col = 0; col < 9; col++) {
            QString value = query.value(col).toString();
            QTableWidgetItem *item = new QTableWidgetItem(value);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }

    qDebug() << "✅ Employés affichés:" << row << "lignes";

    if (row == 0) {
        qDebug() << "⚠️ ATTENTION: Aucun employé dans la base!";
        QMessageBox::information(this, "Info", "📭 Aucun employé dans la base de données.\nAjoutez-en un avec le bouton 'Ajouter'.");
    }
}

// ==================== AFFICHAGE SPONSORS - CORRIGÉ ====================

void MainWindow::afficherSponsors()
{
    qDebug() << "=== AFFICHAGE SPONSORS ===";

    QSqlQuery query;
    QString sql = "SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor";

    qDebug() << "Requête SQL:" << sql;

    if (!query.exec(sql)) {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur",
                              "❌ Impossible de charger les sponsors:\n" + query.lastError().text());
        return;
    }

    ui->sponsorTable_9->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->sponsorTable_9->insertRow(row);

        for (int col = 0; col < 8; col++) {
            QString value = query.value(col).toString();
            QTableWidgetItem *item = new QTableWidgetItem(value);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->sponsorTable_9->setItem(row, col, item);
        }
        row++;
    }

    qDebug() << "✅ Sponsors affichés:" << row << "lignes";

    if (row == 0) {
        qDebug() << "⚠️ ATTENTION: Aucun sponsor dans la base!";
        QMessageBox::information(this, "Info", "📭 Aucun sponsor dans la base de données.\nAjoutez-en un avec le bouton 'Ajouter'.");
    }
}

// ==================== EMPLOYÉS - AJOUTER ====================

void MainWindow::on_pushButton_11_clicked()
{
    qDebug() << "=== AJOUTER EMPLOYÉ ===";

    QString prenom = ui->lineEdit_13->text().trimmed();
    QString email = ui->lineEdit_17->text().trimmed();
    QString telephone = ui->lineEdit_18->text().trimmed();
    QString sexe = ui->lineEdit_19->text().trimmed();
    QString poste = ui->lineEdit_14->text().trimmed();
    QString salaire_str = ui->lineEdit_20->text().trimmed();

    // VALIDATION
    if (!Employes::validerPrenom(prenom)) {
        QMessageBox::warning(this, "Erreur", "❌ Prénom invalide");
        return;
    }

    if (!email.isEmpty() && !Employes::validerEmail(email)) {
        QMessageBox::warning(this, "Erreur", "❌ Email invalide");
        return;
    }

    if (!telephone.isEmpty() && !Employes::validerTelephone(telephone)) {
        QMessageBox::warning(this, "Erreur", "❌ Téléphone invalide");
        return;
    }

    if (!Employes::validerPoste(poste)) {
        QMessageBox::warning(this, "Erreur", "❌ Poste invalide");
        return;
    }

    bool ok;
    double salaire = salaire_str.toDouble(&ok);
    if (!ok || !Employes::validerSalaire(salaire)) {
        QMessageBox::warning(this, "Erreur", "❌ Salaire invalide");
        return;
    }

    Employes emp;
    emp.setPrenom(prenom);
    emp.setEmail(email);
    emp.setTelephone(telephone);
    emp.setSexe(sexe);
    emp.setPoste(poste);
    emp.setSalaire(salaire);
    emp.setDateEmbauche(QDate::currentDate());

    if (emp.ajouter()) {
        afficherEmployes();
        clearEmployeFields();
        QMessageBox::information(this, "Succès", "✅ Employé ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Erreur lors de l'ajout");
    }
}

// ==================== EMPLOYÉS - MODIFIER ====================

void MainWindow::on_pushButton_8_clicked()
{
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé !");
        return;
    }

    QString email = ui->lineEdit_17->text().trimmed();
    QString telephone = ui->lineEdit_18->text().trimmed();
    QString poste = ui->lineEdit_14->text().trimmed();
    double salaire = ui->lineEdit_20->text().toDouble();

    Employes emp;
    if (emp.modifier(idEmployeSelectionne, email, telephone, poste, salaire, "")) {
        afficherEmployes();
        QMessageBox::information(this, "Succès", "✅ Employé modifié !");
    }
}

// ==================== EMPLOYÉS - SUPPRIMER ====================

void MainWindow::on_pushButton_10_clicked()
{
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cet employé ?") == QMessageBox::Yes) {
        Employes emp;
        if (emp.supprimer(idEmployeSelectionne)) {
            afficherEmployes();
            clearEmployeFields();
            idEmployeSelectionne = -1;
            QMessageBox::information(this, "Succès", "✅ Employé supprimé !");
        }
    }
}

// ==================== EMPLOYÉS - TRIER ====================

void MainWindow::on_pushButton_12_clicked()
{
    QSqlQuery query;
    query.exec("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye ORDER BY SALAIRE DESC");

    ui->tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
    QMessageBox::information(this, "Succès", "✅ Trié par salaire");
}

// ==================== EMPLOYÉS - RECHERCHE ====================

void MainWindow::on_lineEdit_6_textChanged(const QString &text)
{
    if (text.isEmpty()) {
        afficherEmployes();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye WHERE UPPER(PRENOM) LIKE UPPER(:search)");
    query.bindValue(":search", "%" + text + "%");
    query.exec();

    ui->tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

// ==================== EMPLOYÉS - SÉLECTION ====================

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    idEmployeSelectionne = ui->tableWidget->item(row, 0)->text().toInt();

    ui->lineEdit_13->setText(ui->tableWidget->item(row, 1)->text());
    ui->lineEdit_17->setText(ui->tableWidget->item(row, 3)->text());
    ui->lineEdit_18->setText(ui->tableWidget->item(row, 4)->text());
    ui->lineEdit_19->setText(ui->tableWidget->item(row, 5)->text());
    ui->lineEdit_14->setText(ui->tableWidget->item(row, 6)->text());
    ui->lineEdit_20->setText(ui->tableWidget->item(row, 7)->text());
}

// ==================== SPONSORS - AJOUTER ====================

// ==================== REMPLACEZ LA MÉTHODE on_pushButton_23_clicked DANS mainwindow.cpp ====================

void MainWindow::on_pushButton_23_clicked()
{
    qDebug() << "=== AJOUTER SPONSOR ===";

    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString categorie = ui->categorie->text().trimmed();
    QDate debut = ui->debut->date();
    QDate fin = ui->fin->date();
    QString budgetStr = ui->budget->text().trimmed();

    // ==================== VALIDATION NOM ====================
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "❌ Erreur - Nom",
                             "Le nom est obligatoire !\n\n"
                             "✓ Le nom doit contenir au moins 1 caractère");
        ui->nom->setFocus();
        return;
    }

    if (nom.length() < 2) {
        QMessageBox::warning(this, "❌ Erreur - Nom",
                             "Le nom est trop court !\n\n"
                             "✓ Le nom doit contenir au moins 2 caractères\n"
                             "✗ Vous avez entré: " + QString::number(nom.length()) + " caractère(s)");
        ui->nom->setFocus();
        return;
    }

    // Vérifier que le nom contient uniquement des lettres
    QRegularExpression regexNom("^[a-zA-ZÀ-ÿ\\s\\-']+$");
    if (!regexNom.match(nom).hasMatch()) {
        QMessageBox::warning(this, "❌ Erreur - Nom",
                             "Le nom contient des caractères invalides !\n\n"
                             "✓ Utilisez uniquement des lettres\n"
                             "✗ Caractères interdits: chiffres, symboles (#@$%...)");
        ui->nom->setFocus();
        return;
    }

    // ==================== VALIDATION PRÉNOM ====================
    if (prenom.isEmpty()) {
        QMessageBox::warning(this, "❌ Erreur - Prénom",
                             "Le prénom est obligatoire !\n\n"
                             "✓ Le prénom doit contenir au moins 1 caractère");
        ui->prenom->setFocus();
        return;
    }

    if (prenom.length() < 2) {
        QMessageBox::warning(this, "❌ Erreur - Prénom",
                             "Le prénom est trop court !\n\n"
                             "✓ Le prénom doit contenir au moins 2 caractères\n"
                             "✗ Vous avez entré: " + QString::number(prenom.length()) + " caractère(s)");
        ui->prenom->setFocus();
        return;
    }

    // Vérifier que le prénom contient uniquement des lettres
    QRegularExpression regexPrenom("^[a-zA-ZÀ-ÿ\\s\\-']+$");
    if (!regexPrenom.match(prenom).hasMatch()) {
        QMessageBox::warning(this, "❌ Erreur - Prénom",
                             "Le prénom contient des caractères invalides !\n\n"
                             "✓ Utilisez uniquement des lettres\n"
                             "✗ Caractères interdits: chiffres, symboles (#@$%...)");
        ui->prenom->setFocus();
        return;
    }

    // ==================== VALIDATION EMAIL ====================
    if (email.isEmpty()) {
        QMessageBox::warning(this, "❌ Erreur - Email",
                             "L'email est obligatoire !");
        ui->email->setFocus();
        return;
    }

    if (!email.contains("@")) {
        QMessageBox::warning(this, "❌ Erreur - Email",
                             "L'email doit contenir @\n\n"
                             "✓ Format correct: exemple@domaine.com\n"
                             "✗ Votre email: " + email);
        ui->email->setFocus();
        return;
    }

    if (!email.contains(".com") && !email.contains(".fr") && !email.contains(".tn") &&
        !email.contains(".net") && !email.contains(".org")) {
        QMessageBox::warning(this, "❌ Erreur - Email",
                             "L'email doit se terminer par .com, .fr, .tn, .net ou .org\n\n"
                             "✓ Format correct: exemple@domaine.com\n"
                             "✗ Votre email: " + email);
        ui->email->setFocus();
        return;
    }

    // Validation complète de l'email
    QRegularExpression regexEmail("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.(com|net|org|edu|gov|fr|tn|ma|dz)$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::warning(this, "❌ Erreur - Email",
                             "Format d'email invalide !\n\n"
                             "✓ Format correct: exemple@domaine.com\n"
                             "✗ Votre email: " + email);
        ui->email->setFocus();
        return;
    }

    // ==================== VALIDATION CATÉGORIE ====================
    if (categorie.isEmpty()) {
        QMessageBox::warning(this, "❌ Erreur - Catégorie",
                             "La catégorie est obligatoire !");
        ui->categorie->setFocus();
        return;
    }

    if (categorie.length() < 2) {
        QMessageBox::warning(this, "❌ Erreur - Catégorie",
                             "La catégorie est trop courte !\n\n"
                             "✓ La catégorie doit contenir au moins 2 caractères\n"
                             "✗ Vous avez entré: " + QString::number(categorie.length()) + " caractère(s)");
        ui->categorie->setFocus();
        return;
    }

    // Vérifier que la catégorie contient uniquement des lettres/chiffres
    QRegularExpression regexCategorie("^[a-zA-ZÀ-ÿ0-9\\s\\-&]+$");
    if (!regexCategorie.match(categorie).hasMatch()) {
        QMessageBox::warning(this, "❌ Erreur - Catégorie",
                             "La catégorie contient des caractères invalides !\n\n"
                             "✓ Utilisez uniquement des lettres et chiffres\n"
                             "✗ Caractères interdits: symboles spéciaux (#@$%...)");
        ui->categorie->setFocus();
        return;
    }

    // ==================== VALIDATION DATES ====================
    if (!debut.isValid()) {
        QMessageBox::warning(this, "❌ Erreur - Date de début",
                             "La date de début est invalide !");
        ui->debut->setFocus();
        return;
    }

    if (!fin.isValid()) {
        QMessageBox::warning(this, "❌ Erreur - Date de fin",
                             "La date de fin est invalide !");
        ui->fin->setFocus();
        return;
    }

    // VÉRIFIER QUE DATE DÉBUT < DATE FIN
    if (debut >= fin) {
        QMessageBox::warning(this, "❌ Erreur - Dates",
                             "La date de début doit être AVANT la date de fin !\n\n"
                             "✗ Date de début: " + debut.toString("dd/MM/yyyy") + "\n"
                                                                  "✗ Date de fin: " + fin.toString("dd/MM/yyyy") + "\n\n"
                                                                "✓ Corrigez les dates");
        ui->fin->setFocus();
        return;
    }

    // Vérifier durée max (10 ans)
    int dureeJours = debut.daysTo(fin);
    if (dureeJours > 365 * 10) {
        QMessageBox::warning(this, "❌ Erreur - Durée du contrat",
                             "La durée du contrat est trop longue !\n\n"
                             "✓ Maximum: 10 ans (3650 jours)\n"
                             "✗ Votre durée: " + QString::number(dureeJours) + " jours");
        ui->fin->setFocus();
        return;
    }

    // ==================== VALIDATION BUDGET ====================
    if (budgetStr.isEmpty()) {
        QMessageBox::warning(this, "❌ Erreur - Budget",
                             "Le budget est obligatoire !");
        ui->budget->setFocus();
        return;
    }

    // Vérifier que le budget est un NOMBRE
    bool ok;
    int budget = budgetStr.toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "❌ Erreur - Budget",
                             "Le budget doit être un NOMBRE !\n\n"
                             "✓ Exemples valides: 5000, 10000, 50000\n"
                             "✗ Votre saisie: " + budgetStr + "\n\n"
                                               "N'utilisez pas de lettres ou symboles");
        ui->budget->setFocus();
        ui->budget->clear();
        return;
    }

    if (budget < 0) {
        QMessageBox::warning(this, "❌ Erreur - Budget",
                             "Le budget ne peut pas être négatif !\n\n"
                             "✓ Le budget doit être >= 0\n"
                             "✗ Votre budget: " + QString::number(budget));
        ui->budget->setFocus();
        return;
    }

    if (budget > 10000000) {
        QMessageBox::warning(this, "❌ Erreur - Budget",
                             "Le budget est trop élevé !\n\n"
                             "✓ Maximum: 10,000,000\n"
                             "✗ Votre budget: " + QString::number(budget));
        ui->budget->setFocus();
        return;
    }

    // ==================== TOUTES LES VALIDATIONS RÉUSSIES ====================
    qDebug() << "✅ Toutes les validations réussies";
    qDebug() << "   Nom:" << nom;
    qDebug() << "   Prénom:" << prenom;
    qDebug() << "   Email:" << email;
    qDebug() << "   Catégorie:" << categorie;
    qDebug() << "   Début:" << debut.toString("yyyy-MM-dd");
    qDebug() << "   Fin:" << fin.toString("yyyy-MM-dd");
    qDebug() << "   Budget:" << budget;

    Sponsor s(nom, prenom, categorie, email, debut, fin, budget);

    if (s.ajouter()) {
        afficherSponsors();
        clearSponsorFields();
        QMessageBox::information(this, "✅ Succès",
                                 "Sponsor ajouté avec succès !\n\n"
                                 "📊 Détails:\n"
                                 "• Nom: " + nom + " " + prenom + "\n"
                                                            "• Email: " + email + "\n"
                                               "• Catégorie: " + categorie + "\n"
                                                   "• Budget: " + QString::number(budget) + " €\n"
                                                                 "• Durée: " + QString::number(dureeJours) + " jours");
        qDebug() << "✅ Sponsor ajouté avec succès";
    } else {
        QMessageBox::critical(this, "❌ Erreur",
                              "Erreur lors de l'ajout du sponsor dans la base de données.\n\n"
                              "Consultez les logs pour plus de détails.");
        qDebug() << "❌ Échec ajout sponsor";
    }
}
// ==================== SPONSORS - MODIFIER ====================

void MainWindow::on_pushButton_38_clicked()
{
    if (idSponsorSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un sponsor !");
        return;
    }

    Sponsor s;
    if (s.modifier(idSponsorSelectionne, ui->nom2->text(), ui->prenom2->text(),
                   ui->categorie2->text(), ui->email2->text(), ui->debut2->date(),
                   ui->fin2->date(), ui->budget2->text().toInt())) {
        afficherSponsors();
        QMessageBox::information(this, "Succès", "✅ Sponsor modifié !");
    }
}

// ==================== SPONSORS - SUPPRIMER ====================

void MainWindow::on_pushButton_40_clicked()
{
    if (idSponsorSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un sponsor !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer ce sponsor ?") == QMessageBox::Yes) {
        Sponsor s;
        if (s.supprimer(idSponsorSelectionne)) {
            afficherSponsors();
            clearSponsorFields();
            idSponsorSelectionne = -1;
            QMessageBox::information(this, "Succès", "✅ Sponsor supprimé !");
        }
    }
}

// ==================== SPONSORS - SÉLECTION ====================

void MainWindow::on_sponsorTable_9_clicked(const QModelIndex &index)
{
    int row = index.row();
    idSponsorSelectionne = ui->sponsorTable_9->item(row, 0)->text().toInt();

    ui->ids->setText(ui->sponsorTable_9->item(row, 0)->text());
    ui->nom2->setText(ui->sponsorTable_9->item(row, 1)->text());
    ui->prenom2->setText(ui->sponsorTable_9->item(row, 2)->text());
    ui->categorie2->setText(ui->sponsorTable_9->item(row, 3)->text());
    ui->email2->setText(ui->sponsorTable_9->item(row, 4)->text());
    ui->debut2->setDate(QDate::fromString(ui->sponsorTable_9->item(row, 5)->text(), "yyyy-MM-dd"));
    ui->fin2->setDate(QDate::fromString(ui->sponsorTable_9->item(row, 6)->text(), "yyyy-MM-dd"));
    ui->budget2->setText(ui->sponsorTable_9->item(row, 7)->text());
}

// ==================== SPONSORS - RAFRAÎCHIR ====================

void MainWindow::on_refreshbtn_clicked()
{
    afficherSponsors();
    QMessageBox::information(this, "Succès", "✅ Liste rafraîchie !");
}

// ==================== SPONSORS - STATISTIQUES ====================

void MainWindow::on_pushButton_stats_clicked()
{
    Sponsor s;
    QSqlQueryModel *model = s.statistiques();

    QString statsText = "📊 Statistiques:\n\n";
    for (int row = 0; row < model->rowCount(); ++row) {
        statsText += "🏷️ " + model->data(model->index(row, 0)).toString() + ":\n";
        statsText += "   Nombre: " + model->data(model->index(row, 1)).toString() + "\n";
        statsText += "   Budget total: " + model->data(model->index(row, 2)).toString() + " €\n\n";
    }

    QMessageBox::information(this, "Statistiques", statsText);
    delete model;
}

// ==================== MÉTHODES AUXILIAIRES ====================

void MainWindow::clearEmployeFields()
{
    ui->lineEdit_13->clear();
    ui->lineEdit_14->clear();
    ui->lineEdit_17->clear();
    ui->lineEdit_18->clear();
    ui->lineEdit_19->clear();
    ui->lineEdit_20->clear();
}

void MainWindow::clearSponsorFields()
{
    ui->nom->clear();
    ui->prenom->clear();
    ui->email->clear();
    ui->categorie->clear();
    ui->budget->clear();
    ui->nom2->clear();
    ui->prenom2->clear();
    ui->email2->clear();
    ui->categorie2->clear();
    ui->budget2->clear();
}
