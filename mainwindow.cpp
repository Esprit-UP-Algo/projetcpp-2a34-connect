#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sponsor.h"
#include <QDate>
#include <QMessageBox>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(S.afficher());

    // Connecter le signal de sélection pour charger les données
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::on_tableView_selectionChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Fonction de validation pour les noms et prénoms (lettres et espaces uniquement)
bool MainWindow::isValidName(const QString &name)
{
    if (name.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Le nom/prénom ne peut pas être vide."));
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s'-]+$");
    if (!regex.match(name).hasMatch()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Le nom/prénom ne peut contenir que des lettres."));
        return false;
    }
    return true;
}

// Fonction de validation pour l'email
bool MainWindow::isValidEmail(const QString &email)
{
    if (email.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("L'email ne peut pas être vide."));
        return false;
    }

    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!regex.match(email).hasMatch()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("L'email n'est pas valide. Exemple: user@example.com"));
        return false;
    }
    return true;
}

// Fonction de validation pour le budget
bool MainWindow::isValidBudget(const QString &budgetStr)
{
    if (budgetStr.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Le budget ne peut pas être vide."));
        return false;
    }

    bool ok;
    int budget = budgetStr.toInt(&ok);
    if (!ok || budget < 0) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Le budget doit être un nombre positif."));
        return false;
    }
    return true;
}

// Fonction de validation pour les champs requis
bool MainWindow::isFieldEmpty(const QString &field, const QString &fieldName)
{
    if (field.isEmpty()) {
        QString message = fieldName + " ne peut pas être vide.";
        QMessageBox::warning(nullptr, QObject::tr("Erreur"), message);
        return true;
    }
    return false;
}

void MainWindow::on_pushButton_23_clicked()
{
    // Récupérer les valeurs
    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString categorie = ui->categorie->text().trimmed();
    QDate debutcontrat = ui->debut->date();
    QDate fincontrat = ui->fin->date();
    QString budgetStr = ui->budget->text().trimmed();

    // Validations
    if (isFieldEmpty(nom, "Nom") || !isValidName(nom)) return;
    if (isFieldEmpty(prenom, "Prénom") || !isValidName(prenom)) return;
    if (isFieldEmpty(email, "Email") || !isValidEmail(email)) return;
    if (isFieldEmpty(categorie, "Catégorie")) return;
    if (!isValidBudget(budgetStr)) return;

    // Vérifier les dates
    if (debutcontrat >= fincontrat) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("La date de début doit être antérieure à la date de fin."));
        return;
    }

    int budget = budgetStr.toInt();

    sponsor S(nom, prenom, categorie, email, debutcontrat, fincontrat, budget);

    bool test = S.ajouter();
    if (test) {
        ui->tableView->setModel(S.afficher());

        // Effacer les champs
        ui->nom->clear();
        ui->prenom->clear();
        ui->email->clear();
        ui->categorie->clear();
        ui->budget->clear();

        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Sponsor ajouté avec succès.\n"
                                             "Cliquez sur OK pour continuer."), QMessageBox::Ok);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Erreur lors de l'ajout du sponsor.\n"
                                          "Cliquez sur OK pour continuer."), QMessageBox::Ok);
}


void MainWindow::on_pushButton_38_clicked()
{
    // Récupérer les valeurs
    QString idStr = ui->ids->text().trimmed();
    QString nom = ui->nom2->text().trimmed();
    QString prenom = ui->prenom2->text().trimmed();
    QString email = ui->email2->text().trimmed();
    QString categorie = ui->categorie2->text().trimmed();
    QDate debutcontrat = ui->debut2->date();
    QDate fincontrat = ui->fin2->date();
    QString budgetStr = ui->budget2->text().trimmed();

    // Validations
    if (idStr.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Veuillez sélectionner un sponsor à modifier."));
        return;
    }

    if (isFieldEmpty(nom, "Nom") || !isValidName(nom)) return;
    if (isFieldEmpty(prenom, "Prénom") || !isValidName(prenom)) return;
    if (isFieldEmpty(email, "Email") || !isValidEmail(email)) return;
    if (isFieldEmpty(categorie, "Catégorie")) return;
    if (!isValidBudget(budgetStr)) return;

    if (debutcontrat >= fincontrat) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("La date de début doit être antérieure à la date de fin."));
        return;
    }

    bool ok;
    int id = idStr.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("ID invalide."));
        return;
    }

    int budget = budgetStr.toInt();

    sponsor S;
    bool test = S.modifier(id, nom, prenom, categorie, email, debutcontrat, fincontrat, budget);

    if(test) {
        ui->tableView->setModel(S.afficher());

        // Effacer les champs
        ui->ids->clear();
        ui->nom2->clear();
        ui->prenom2->clear();
        ui->email2->clear();
        ui->categorie2->clear();
        ui->budget2->clear();

        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Sponsor modifié avec succès.\n"
                                             "Cliquez sur OK pour continuer."), QMessageBox::Ok);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Erreur lors de la modification.\n"
                                          "Cliquez sur OK pour continuer."), QMessageBox::Ok);
}


void MainWindow::on_pushButton_40_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Veuillez sélectionner un sponsor à supprimer."));
        return;
    }

    int row = selection.at(0).row();
    QString id = ui->tableView->model()->index(row, 0).data().toString();

    // Confirmation avant suppression
    QMessageBox::StandardButton reply = QMessageBox::question(nullptr,
                                                              QObject::tr("Confirmation"),
                                                              QObject::tr("Êtes-vous sûr de vouloir supprimer ce sponsor ?"),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) return;

    sponsor S;
    bool test = S.supprimer(id);

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Sponsor supprimé avec succès.\n"
                                             "Cliquez sur OK pour continuer."),
                                 QMessageBox::Ok);
        ui->tableView->setModel(S.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Erreur lors de la suppression.\n"
                                          "Cliquez sur OK pour continuer."),
                              QMessageBox::Ok);
    }
}


void MainWindow::on_refreshbtn_clicked()
{
    ui->tableView->setModel(S.afficher());
}

// Charger les données automatiquement lors de la sélection
void MainWindow::on_tableView_selectionChanged()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) return;

    int row = selection.at(0).row();

    ui->ids->setText(ui->tableView->model()->index(row, 0).data().toString());
    ui->nom2->setText(ui->tableView->model()->index(row, 1).data().toString());
    ui->prenom2->setText(ui->tableView->model()->index(row, 2).data().toString());
    ui->categorie2->setText(ui->tableView->model()->index(row, 3).data().toString());
    ui->email2->setText(ui->tableView->model()->index(row, 4).data().toString());
    ui->debut2->setDate(ui->tableView->model()->index(row, 5).data().toDate());
    ui->fin2->setDate(ui->tableView->model()->index(row, 6).data().toDate());
    ui->budget2->setText(ui->tableView->model()->index(row, 7).data().toString());
}
