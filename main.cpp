#include "mainwindow.h"
#include "logindialog.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connect DB
    Connection conn;
    if (!conn.createconnect()) {
        QMessageBox::critical(nullptr, "DB Error", "Cannot connect to database.");
        return -1;
    }

    // Show Login
    LoginDialog login;
    if (login.exec() != QDialog::Accepted) return 0;

    // Open MainWindow with Role
    MainWindow w;
    w.setUserRole(login.role());
    w.show();

    return a.exec();
}
