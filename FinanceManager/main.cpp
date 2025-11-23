#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Finance Manager");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("FinanceCorp");

    Connection& c = Connection::getInstance();
    bool test = c.createconnect();

    MainWindow w;
    w.setWindowTitle("Finance Manager - Financial Manager");

    if(test) {
        w.show();
        QMessageBox::information(nullptr, "Database Connection",
                                 "Database connection successful!\n"
                                 "Finance Manager is ready to use.");
    } else {
        QMessageBox::critical(nullptr, "Connection Error",
                              "Failed to connect to the database.\n"
                              "The application is using local SQLite mode.");
        w.show();
    }

    return a.exec();
}
