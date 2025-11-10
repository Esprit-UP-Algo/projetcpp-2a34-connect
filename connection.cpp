#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

Connection::Connection() {}

bool Connection::createconnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setPort(1521);
    db.setDatabaseName("xe");  // Change if needed
    db.setUserName("MY_USER");
    db.setPassword("1234");  // ← PUT YOUR REAL PASSWORD

    if (!db.open()) {
        qDebug() << "DB ERROR:" << db.lastError().text();
        return false;
    }

    qDebug() << "DB CONNECTED SUCCESSFULLY!";
    return true;
}
