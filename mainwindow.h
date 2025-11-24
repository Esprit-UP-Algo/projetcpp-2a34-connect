#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUserRole(const QString &role);

private slots:
    // === NAVIGATION ===
    void showFinancePage();
    void showContentCreatorPage();
    void showSponsorPage();
    void showManagementPage();
    void showEmployeePage();
    void updateSimpleStats();


    // === THEME ===
    void on_themeButton_6_clicked();

    // === CONTENT CREATOR CRUD ===
    void on_addButton_6_clicked();
    void on_updateButton_6_clicked();
    void on_deleteButton_6_clicked();
    void on_clearButton_6_clicked();
    void on_exportButton_6_clicked();
    void on_searchEdit_6_textChanged(const QString &text);
    void fillFormFromTable();

    // === QR CODE ===
    void generateQR();
    void saveQR();
    void copyQR();

private:
    Ui::MainWindow *ui;
    bool darkTheme = false;
    QString m_userRole;
    QPixmap m_currentQR;

    // === DATABASE ===
    bool connectDB();

    // === THEME ===
    void applyLightTheme();
    void applyDarkTheme();

    // === CONTENT CREATOR ===
    void loadCreateurs();
    void clearCreateurFields();
    bool isValidName(const QString &name) const;

    // === ROLE SYSTEM ===
    void disableAllInputs();
    void applyRoleRestrictions();
};

#endif // MAINWINDOW_H
