#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

class Connection
{
private:
    QSqlDatabase db;
    Connection(); // Constructeur privé
    ~Connection(); // Destructeur privé

    // Interdire la copie
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

public:
    static Connection& getInstance();
    bool createconnect();
    QSqlDatabase getDatabase();
    bool testConnection();
    void closeConnection();
};

#endif // CONNECTION_H
