#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    static bool connectToDatabase();
    static QSqlDatabase getDatabase();
};

#endif // DATABASE_H
