#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ✅ UTILISATION DU SINGLETON
    Connection& c = Connection::getInstance();
    bool test = c.createconnect();

    MainWindow w;

    if(test) {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("Base de données ouverte"),
                                 QObject::tr("Connexion Oracle réussie!\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Base de données fermée"),
                              QObject::tr("Échec de connexion Oracle.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }

    return a.exec();
}
