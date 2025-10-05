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
    void addInvoice();
    void editInvoice();
    void deleteInvoice();
    void clearForm();
    void sortByAmount();
    void searchById();
    void exportToCSV();
    void toggleDarkTheme();
    void updateStatistics();

private:
    Ui::MainWindow *ui;
    bool darkTheme = false;

    // Helper function to add invoice to table
    void insertInvoiceInTable(QString id, double amount, QDate issueDate, QDate dueDate, QString status);
    void applyLightTheme();
    void applyDarkTheme();
    void initializeStatisticsTable();
};

#endif // MAINWINDOW_H
