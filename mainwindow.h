#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void onInvoiceSelected();

private:
    Ui::MainWindow *ui;
    bool darkTheme = false;

    // Helper functions
    void insertInvoiceInTable(QString id, double amount, QDate issueDate, QDate dueDate, QString status);
    void loadInvoicesFromDatabase();
    void initializeVisualStatistics();
    void updateVisualStatistics();
    void applyLightTheme();
    void applyDarkTheme();
};

#endif // MAINWINDOW_H
