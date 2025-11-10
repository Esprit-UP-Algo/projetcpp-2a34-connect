#include "connection.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

Connection::Connection() {
    // Use an available driver - QODBC for Oracle
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
    qDebug() << "=== ORACLE CONNECTION ATTEMPT VIA ODBC ===";
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    // ODBC configuration for Oracle
    QString connectionString = "DRIVER={Oracle dans OraClient11g_home1};SERVER=DESKTOP-BUIGG3M:1521/XE;UID=system;PWD=05032005";
    db.setDatabaseName(connectionString);

    qDebug() << "Using ODBC configuration...";

    if (db.open()) {
        qDebug() << "✅ ORACLE CONNECTION VIA ODBC SUCCESSFUL!";

        // Test a simple query
        QSqlQuery query;
        if (query.exec("SELECT sysdate FROM dual")) {
            if (query.next()) {
                qDebug() << "✅ Query test successful. System date:" << query.value(0).toString();
            }
            return true;
        } else {
            qDebug() << "⚠️ Connection OK but query error:" << query.lastError().text();
            return true; // Connection is good even if query fails
        }
    } else {
        qDebug() << "❌ ODBC connection failed:" << db.lastError().text();

        // SQLite fallback
        qDebug() << "=== USING SQLITE FALLBACK ===";
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("project_manager.db");

        if (db.open()) {
            qDebug() << "✅ SQLITE CONNECTION SUCCESSFUL!";

            // Create necessary tables
            QSqlQuery query;

            // Facture table
            query.exec("CREATE TABLE IF NOT EXISTS Facture ("
                       "id_facture VARCHAR(20) PRIMARY KEY, "
                       "montant DECIMAL(10,2), "
                       "date_emission DATE, "
                       "date_echeance DATE, "
                       "statut VARCHAR(20), "
                       "id_employe INTEGER)");

            // Insert test data
            query.exec("INSERT OR IGNORE INTO Facture VALUES ('FACT001', 1500.00, '2025-01-01', '2025-02-01', 'Paid', 1)");
            query.exec("INSERT OR IGNORE INTO Facture VALUES ('FACT002', 2500.50, '2025-01-15', '2025-02-15', 'Pending', 1)");

            qDebug() << "✅ SQLite database initialized with test data";
            return true;
        } else {
            qDebug() << "❌ SQLite failed:" << db.lastError().text();
            return false;
        }
    }
}
