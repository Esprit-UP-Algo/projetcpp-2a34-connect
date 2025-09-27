#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Menu
    void showManagementPage();
    void showFinancePage();
    void showContentCreatorPage();
    void showSponsorPage();
    void showEmployeePage();

    // Finance Page
    void addFacture();
    void editFacture();
    void deleteFacture();
    void clearForm();
    void sortByMontant();
    void searchById();

private:
    Ui::MainWindow *ui;

    // Helper function pour ajouter une facture au tableau
    void insertFactureInTable(QString id, double montant, QDate dateEmission, QDate dateEcheance, QString statut);
};

#endif // MAINWINDOW_H
