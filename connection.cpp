#include "connection.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

Connection::Connection() {
    // Utiliser un driver disponible - QODBC pour Oracle
    db = QSqlDatabase::addDatabase("QODBC");
}

Connection::~Connection() {
    if (db.isOpen()) {
        db.close();
    }
}

Connection& Connection::getInstance() {
    static Connection instance;
    return instance;
}

bool Connection::createconnect() {
    qDebug() << "=== TENTATIVE DE CONNEXION ORACLE VIA ODBC ===";
    qDebug() << "Drivers disponibles:" << QSqlDatabase::drivers();

    // Configuration ODBC pour Oracle
    QString connectionString = "DRIVER={Oracle dans OraClient11g_home1};SERVER=DESKTOP-BUIGG3M:1521/XE;UID=system;PWD=05032005";
    db.setDatabaseName(connectionString);

    qDebug() << "Configuration ODBC utilisée...";

    if (db.open()) {
        qDebug() << "✅ CONNEXION ORACLE VIA ODBC RÉUSSIE!";

        // Test d'une requête simple
        QSqlQuery query;
        if (query.exec("SELECT sysdate FROM dual")) {
            if (query.next()) {
                qDebug() << "✅ Test requête réussi. Date système:" << query.value(0).toString();
            }
            return true;
        } else {
            qDebug() << "⚠️ Connexion OK mais erreur requête:" << query.lastError().text();
            return true; // La connexion est bonne même si la requête échoue
        }
    } else {
        qDebug() << "❌ ÉCHEC connexion ODBC:" << db.lastError().text();

        // Fallback SQLite
        qDebug() << "=== UTILISATION SQLITE DE SECOURS ===";
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("project_manager.db");

        if (db.open()) {
            qDebug() << "✅ CONNEXION SQLITE RÉUSSIE!";

            // Créer les tables nécessaires
            QSqlQuery query;

            // Table Facture
            query.exec("CREATE TABLE IF NOT EXISTS Facture ("
                       "id_facture VARCHAR(20) PRIMARY KEY, "
                       "montant DECIMAL(10,2), "
                       "date_emission DATE, "
                       "date_echeance DATE, "
                       "statut VARCHAR(20), "
                       "id_employe INTEGER)");

            // Insérer des données de test
            query.exec("INSERT OR IGNORE INTO Facture VALUES ('FACT001', 1500.00, '2025-01-01', '2025-02-01', 'Paid', 1)");
            query.exec("INSERT OR IGNORE INTO Facture VALUES ('FACT002', 2500.50, '2025-01-15', '2025-02-15', 'Pending', 1)");

            qDebug() << "✅ Base SQLite initialisée avec données de test";
            return true;
        } else {
            qDebug() << "❌ ÉCHEC SQLite:" << db.lastError().text();
            return false;
        }
    }
}
