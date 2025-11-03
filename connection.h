#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

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
};

#endif // CONNECTION_H
