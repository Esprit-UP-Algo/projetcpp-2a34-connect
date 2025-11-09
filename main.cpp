#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

bool creerTables()
{
    qDebug() << "=== CRÉATION DES TABLES ===";

    QSqlQuery query;

    // ==================== TABLE EMPOLYE ====================
    qDebug() << "Création table empolye...";

    QString createEmpolye =
        "CREATE TABLE IF NOT EXISTS empolye ("
        "IDEMPLOYE INTEGER PRIMARY KEY AUTOINCREMENT, "
        "PRENOM VARCHAR(20), "
        "DATEDEMBAUCHE DATE, "
        "EMAIL VARCHAR(20), "
        "TELEPHONE INTEGER, "
        "SEXE VARCHAR(20), "
        "POSTE VARCHAR(20), "
        "SALAIRE REAL, "
        "IDSUPERVISEUR VARCHAR(20))";

    if (!query.exec(createEmpolye)) {
        qDebug() << "❌ Erreur création table empolye:" << query.lastError().text();
        return false;
    }
    qDebug() << "✅ Table empolye créée/vérifiée";

    // ==================== TABLE SPONSOR ====================
    qDebug() << "Création table sponsor...";

    QString createSponsor =
        "CREATE TABLE IF NOT EXISTS sponsor ("
        "IDSPONSOR INTEGER PRIMARY KEY AUTOINCREMENT, "
        "NOM VARCHAR(50), "
        "PRENOM VARCHAR(50), "
        "CATEGORIE VARCHAR(50), "
        "EMAIL VARCHAR(100), "
        "DEBUTCONTRAT DATE, "
        "FINCONTRAT DATE, "
        "BUDGET INTEGER)";

    if (!query.exec(createSponsor)) {
        qDebug() << "❌ Erreur création table sponsor:" << query.lastError().text();
        return false;
    }
    qDebug() << "✅ Table sponsor créée/vérifiée";

    // ==================== VÉRIFIER SI LES TABLES SONT VIDES ====================
    query.exec("SELECT COUNT(*) FROM empolye");
    query.next();
    int countEmpolye = query.value(0).toInt();

    query.exec("SELECT COUNT(*) FROM sponsor");
    query.next();
    int countSponsor = query.value(0).toInt();

    qDebug() << "📊 Employés dans la base:" << countEmpolye;
    qDebug() << "📊 Sponsors dans la base:" << countSponsor;

    // ==================== AJOUTER DES DONNÉES DE TEST (OPTIONNEL) ====================
    if (countEmpolye == 0) {
        qDebug() << "Ajout d'un employé de test...";
        query.exec(
            "INSERT INTO empolye (PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE) "
            "VALUES ('Admin Test', DATE('now'), 'admin@test.com', 12345678, 'M', 'Manager', 5000)"
            );
        qDebug() << "✅ Employé de test ajouté";
    }

    if (countSponsor == 0) {
        qDebug() << "Ajout d'un sponsor de test...";
        query.exec(
            "INSERT INTO sponsor (NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET) "
            "VALUES ('TechCorp', 'Ahmed', 'Technologie', 'contact@techcorp.com', DATE('now'), DATE('now', '+1 year'), 50000)"
            );
        qDebug() << "✅ Sponsor de test ajouté";
    }

    qDebug() << "=== TABLES PRÊTES ===";
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "=== DÉMARRAGE APPLICATION ===";
    qDebug() << "Drivers disponibles:" << QSqlDatabase::drivers();

    // ==================== CONNEXION SQLITE ====================
    QSqlDatabase db;

    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        QMessageBox::critical(nullptr, "Erreur",
                              "❌ Driver SQLite non disponible!\n\nDrivers disponibles: " +
                                  QSqlDatabase::drivers().join(", "));
        return -1;
    }

    qDebug() << "🔄 Tentative de connexion SQLite...";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");

    if (!db.open()) {
        qDebug() << "❌ ERREUR SQLite:" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Base de Données",
                              "❌ Impossible d'ouvrir la base SQLite:\n" + db.lastError().text());
        return -1;
    }

    qDebug() << "✅ CONNEXION SQLite RÉUSSIE!";
    qDebug() << "📁 Base de données:" << db.databaseName();

    // ==================== CRÉER LES TABLES ====================
    if (!creerTables()) {
        QMessageBox::critical(nullptr, "Erreur",
                              "❌ Impossible de créer les tables de la base de données!");
        return -1;
    }

    // ==================== LANCER L'APPLICATION ====================
    MainWindow w;
    w.show();

    qDebug() << "=== APPLICATION PRÊTE ===";
    return a.exec();
}
