#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "connection.h"

bool initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("gestion_sponsors.db");

    if (!db.open()) {
        qDebug() << "Erreur d'ouverture de la base de données:" << db.lastError().text();
        return false;
    }

    // Création de la table sponsor
    QSqlQuery query;
    QString createSponsorTable =
        "CREATE TABLE IF NOT EXISTS sponsor ("
        "IDSPONSOR INTEGER PRIMARY KEY AUTOINCREMENT, "
        "NOM TEXT NOT NULL, "
        "PRENOM TEXT NOT NULL, "
        "CATEGORIE TEXT, "
        "EMAIL TEXT, "
        "DEBUTCONTRAT DATE, "
        "FINCONTRAT DATE, "
        "BUDGET REAL)";

    if (!query.exec(createSponsorTable)) {
        qDebug() << "Erreur création table sponsor:" << query.lastError().text();
        return false;
    }

    // Création de la table employé
    QString createEmployeTable =
        "CREATE TABLE IF NOT EXISTS empolye ("
        "IDEMPLOYE INTEGER PRIMARY KEY AUTOINCREMENT, "
        "PRENOM TEXT NOT NULL, "
        "EMAIL TEXT, "
        "TELEPHONE TEXT, "
        "SEXE TEXT, "
        "POSTE TEXT, "
        "SALAIRE REAL, "
        "DATEDEMBAUCHE DATE, "
        "IDSUPERVISEUR INTEGER)";

    if (!query.exec(createEmployeTable)) {
        qDebug() << "Erreur création table empolye:" << query.lastError().text();
        return false;
    }

    // Création de la table historique chatbot
    QString createHistoriqueTable =
        "CREATE TABLE IF NOT EXISTS chat_historique ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "IDSPONSOR INTEGER, "
        "QUESTION TEXT, "
        "REPONSE TEXT, "
        "HORODATAGE DATETIME DEFAULT CURRENT_TIMESTAMP)";

    if (!query.exec(createHistoriqueTable)) {
        qDebug() << "Erreur création table historique:" << query.lastError().text();
    }

    // Création de la table SMS
    QString createSMSTable =
        "CREATE TABLE IF NOT EXISTS sms_historique ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "IDSPONSOR INTEGER, "
        "NUMERO TEXT, "
        "MESSAGE TEXT, "
        "SUCCES BOOLEAN, "
        "HORODATAGE DATETIME DEFAULT CURRENT_TIMESTAMP)";

    if (!query.exec(createSMSTable)) {
        qDebug() << "Erreur création table SMS:" << query.lastError().text();
    }

    qDebug() << "✅ Base de données initialisée avec succès";
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialiser la base de données
//if (!initializeDatabase()) {
       // QMessageBox::critical(nullptr, "Erreur", "Impossible d'initialiser la base de données !");
       // return -1;
   // }
    Connection conn;
    if (!conn.createconnect()) {
        QMessageBox::critical(nullptr, "DB Error", "Cannot connect to database.");
        return -1;
    }
    // Show Login
    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w;
        w.setUserRole(login.role());
        w.showWelcomeMessage(login.nomComplet());  // welcome with name + role
        w.show();
        return a.exec();
    } else {
        return 0;
    }
    return a.exec();
}
