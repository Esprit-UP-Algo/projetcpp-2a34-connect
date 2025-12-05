#include "connection.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Connection::Connection() {}

bool Connection::createconnect()
{
    // Remove any existing connection
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }

    // Create Oracle ODBC connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    // ✅ OPTION 1: Using DSN (recommended)
    db.setDatabaseName("PROJET"); // Replace with your actual DSN name
    db.setUserName("Moutie");      // Your Oracle username
    db.setPassword("esprit2026"); // Your Oracle password

    /* ✅ OPTION 2: Using connection string (alternative)
    db.setDatabaseName(
        "DRIVER={Oracle in OraClient19Home1};"
        "DBQ=localhost:1521/XE;"  // Your Oracle connection string
        "UID=MY_USER;"
        "PWD=YOUR_PASSWORD;"
    );
    */

    if (!db.open()) {
        qDebug() << "❌ Database connection FAILED!";
        qDebug() << "Error:" << db.lastError().text();
        qDebug() << "Driver:" << db.lastError().driverText();
        qDebug() << "Database:" << db.lastError().databaseText();
        return false;
    }

    qDebug() << "✅ Database connection SUCCESS!";
    qDebug() << "Connected to:" << db.databaseName();

    // Test query to verify connection
    QSqlQuery testQuery;
    if (testQuery.exec("SELECT 1 FROM DUAL")) {
        qDebug() << "✅ Test query successful";
    } else {
        qDebug() << "⚠️ Test query failed:" << testQuery.lastError().text();
    }

    return true;
}

Connection::~Connection() {}
