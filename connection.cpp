#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("PROJET");//inserer le nom de la source de données
    db.setUserName("Moutie");//inserer nom de l'utilisateur
    db.setPassword("esprit2026");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;





    return  test;
}
