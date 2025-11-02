#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sponsor.h"
#include <QDate>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(S.afficher());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_23_clicked()
{
    QString nom =ui->nom->text();
    QString prenom=ui->prenom->text();
    QString email= ui->email->text();
    QString categorie=ui->categorie->text();
    QDate debutcontrat=ui->debut->date();
    QDate fincontrat=ui->fin->date();
    int budget= ui->budget->text().toInt();

    sponsor S(nom,prenom,categorie,email,debutcontrat,fincontrat,budget);


    bool test=S.ajouter();
    if (test)
    { //Actualiser
        ui->tableView->setModel(S.afficher());

        QMessageBox:: information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "click cancel to exit."),QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"
                                          "click Cancel to exit."),QMessageBox::Cancel);

}


void MainWindow::on_pushButton_38_clicked()
{
    int id = ui->ids->text().toInt();
    QString nom =ui->nom2->text();
    QString prenom=ui->prenom2->text();
    QString email= ui->email2->text();
    QString categorie= ui->categorie2->text();
    QDate debutcontrat=ui->debut2->date();
    QDate fincontrat=ui->fin2->date();
    int budget= ui->budget2->text().toInt();

    sponsor S(nom,prenom,categorie,email,debutcontrat,fincontrat,budget);
    bool test=S.modifier(id,nom,prenom,categorie,email,debutcontrat,fincontrat,budget);
    if(test)
    {

        ui->tableView->setModel(S.afficher());
        QMessageBox::information(nullptr, QObject::tr("modifier une agence"),
                                 QObject::tr("Agence modifié.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);

    }
    else
        QMessageBox::critical(nullptr, QObject::tr("modifier une agence"),
                              QObject::tr("Erreur !.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
}


void MainWindow::on_pushButton_40_clicked()
{

    // Vérifier qu'une ligne est sélectionnée
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Veuillez sélectionner un sponsor à supprimer."));
        return;
    }

    // Récupérer la première ligne sélectionnée
    int row = selection.at(0).row();

    // Extraire l'ID de l'employé depuis la colonne ID (supposons colonne 0)
    QString id = ui->tableView->model()->index(row, 0).data().toString();

    // Créer l'objet employee
    sponsor S;

    // Appeler la fonction supprimer avec l'ID sélectionné
    bool test = S.supprimer(id); // Ici supprimer attend un QString

    if (test) {
        // Actualiser la table
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée.\n"
                                             "Cliquez sur Cancel pour fermer."),
                                 QMessageBox::Cancel);
        ui->tableView->setModel(S.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Cliquez sur Cancel pour fermer."),
                              QMessageBox::Cancel);
    }
}


void MainWindow::on_refreshbtn_clicked()
{
    ui->tableView->setModel(S.afficher());

}

