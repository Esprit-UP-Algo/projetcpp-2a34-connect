#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTableWidget>
#include <QSqlQuery>
#include <QWidget>
#include <QPainter>
#include <QLinearGradient>
#include <QComboBox>

#include "chatbot.h"
#include "smsmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CurveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurveWidget(QWidget *parent = nullptr);
    void setData(const QVector<QPointF> &data, const QString &title, const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPointF> m_data;
    QString m_title;
    QColor m_color;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation
    void on_sidebarButton_clicked();
    void on_sidebarButton_2_clicked();
    void on_sidebarButton_3_clicked();
    void on_sidebarButton_4_clicked();
    void on_sidebarButton_5_clicked();
    void on_sidebarButton12_clicked();

    // Employés
    void on_pushButton_11_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_12_clicked();
    void on_lineEdit_6_textChanged(const QString &arg1);
    void on_tableWidget_clicked(const QModelIndex &index);

    // Sponsors - CRUD
    void on_pushButton_23_clicked();
    void on_pushButton_38_clicked();
    void on_pushButton_40_clicked();
    void on_sponsorTable_9_clicked(const QModelIndex &index);
    void on_refreshbtn_clicked();
    void on_pushButton_stats_clicked();

    // Sponsors - FONCTIONNALITÉS
    void on_pushButton_35_clicked();
    void on_pushButton_37_clicked();
    void on_searchEdit_7_textChanged(const QString &arg1);
    void on_pushButton_39_clicked();
    void on_pushButton_41_clicked();
    void on_searchEdit_8_textChanged(const QString &arg1);

    // Chatbot & SMS
    void on_pushButton_25_clicked();
    void on_pushButton_26_clicked();
    void on_btnConfigSMS_clicked();
    void on_pushButton_24_clicked();

    // Configuration Gemini
    void on_ConfigurerGemini_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::MainWindow *ui;
    int idEmployeSelectionne;
    int idSponsorSelectionne;

    Chatbot *chatbot;
    SMSManager *smsManager;

    void connecterSignaux();
    void initEmployesTable();
    void initSponsorsTable();
    void afficherEmployes();
    void afficherSponsors();
    void clearEmployeFields();
    void clearSponsorFields();

    // Méthodes sponsors
    void rechercherSponsors(const QString &recherche);
    void trierSponsorsParDate();
    void exporterSponsorsPDFParContrat();

    // Méthodes de contrôle de saisie
    bool validerFormulaireSponsor();
    bool validerFormulaireModificationSponsor();

    // Méthodes pour les statistiques avec courbes
    void afficherStatistiquesCourbes();
    void creerCourbeEvolutionContrats(QVBoxLayout *layout);
    void creerCourbeBudgets(QVBoxLayout *layout);
    void creerGraphiqueCategories(QVBoxLayout *layout);
    void creerGraphiqueDureeContrats(QVBoxLayout *layout);
    void exporterStatistiquesPDF();
};

#endif // MAINWINDOW_H
