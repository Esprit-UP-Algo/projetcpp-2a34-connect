#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_pushButton_11_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_12_clicked();
    void on_lineEdit_6_textChanged(const QString &arg1);
    void on_tableWidget_clicked(const QModelIndex &index);
    void on_sidebarButton_clicked();
    void on_sidebarButton_2_clicked();
    void on_sidebarButton_3_clicked();
    void on_sidebarButton_4_clicked();
    void on_sidebarButton_5_clicked();
    void on_sidebarButton12_clicked();
    void on_pushButton_23_clicked();
    void on_pushButton_38_clicked();
    void on_pushButton_40_clicked();
    void on_sponsorTable_9_clicked(const QModelIndex &index);
    void on_refreshbtn_clicked();
    void on_pushButton_stats_clicked();

private:
    Ui::MainWindow *ui;
    int idEmployeSelectionne;
    int idSponsorSelectionne;

    void checkDrivers();
    void testSimpleConnection();
    bool connecterOracle();
    void connecterSignaux();
    void initEmployesTable();
    void initSponsorsTable();
    void afficherEmployes();
    void afficherSponsors();
    void clearEmployeFields();
    void clearSponsorFields();
};

#endif // MAINWINDOW_H
