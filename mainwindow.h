#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sponsor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_23_clicked();
    void on_pushButton_38_clicked();
    void on_pushButton_40_clicked();
    void on_refreshbtn_clicked();
    void on_tableView_selectionChanged();

private:
    Ui::MainWindow *ui;
    sponsor S;

    bool isValidName(const QString &name);
    bool isValidEmail(const QString &email);
    bool isValidBudget(const QString &budgetStr);
    bool isFieldEmpty(const QString &field, const QString &fieldName);
};

#endif // MAINWINDOW_H
