#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("PROJET");
    db.setUserName("Moutie");
    db.setPassword("esprit2026");

    if (db.open())
        test=true;

    return test;
}
