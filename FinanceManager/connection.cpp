#include "connection.h"
#include <QApplication>
#include <QDir>

Connection::Connection() {
    // Use SQLite only
    db = QSqlDatabase::addDatabase("QSQLITE", "main_connection");
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
    qDebug() << "=== FINANCE MANAGER - DATABASE CONNECTION ===";
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    // SQLite configuration
    QString dbPath = QApplication::applicationDirPath() + "/finance_manager.db";
    db.setDatabaseName(dbPath);

    qDebug() << "SQLite database path:" << dbPath;

    if (db.open()) {
        qDebug() << "✅ DATABASE CONNECTION SUCCESSFUL!";

        // Create tables
        QSqlQuery query(db);

        // FACTURE table
        if (!query.exec("CREATE TABLE IF NOT EXISTS FACTURE ("
                        "id_facture VARCHAR(20) PRIMARY KEY, "
                        "montant DECIMAL(10,2) NOT NULL, "
                        "date_emission DATE NOT NULL, "
                        "date_echeance DATE NOT NULL, "
                        "statut VARCHAR(20) NOT NULL, "
                        "id_employe INTEGER)")) {
            qDebug() << "Error creating FACTURE table:" << query.lastError().text();
            return false;
        }

        // Insert sample data if empty
        query.exec("SELECT COUNT(*) FROM FACTURE");
        if (query.next() && query.value(0).toInt() == 0) {
            if (query.exec("INSERT INTO FACTURE (id_facture, montant, date_emission, date_echeance, statut, id_employe) VALUES "
                           "('FACT001', 1500.00, '2024-01-01', '2024-02-01', 'Paid', 1), "
                           "('FACT002', 2500.50, '2024-01-15', '2024-02-15', 'Pending', 1), "
                           "('FACT003', 1800.75, '2024-01-20', '2024-03-20', 'Pending', 1)")) {
                qDebug() << "✅ Sample data inserted successfully";
            }
        }

        qDebug() << "✅ Database initialized successfully";
        return true;
    } else {
        qDebug() << "❌ Database connection failed:" << db.lastError().text();
        return false;
    }
}

QSqlDatabase Connection::getDatabase() {
    return db;
}

bool Connection::testConnection() {
    return db.isOpen();
}

void Connection::closeConnection() {
    if (db.isOpen()) {
        db.close();
    }
}
