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
#include <QTextEdit>

// Includes des modules principaux
#include "chatbot.h"
#include "smsmanager.h"
#include "employes.h"
#include "sponsor.h"
#include "historymanager.h" // Historique

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
    // --- Navigation (Sidebar) ---
    void on_sidebarButton_clicked();
    void on_sidebarButton_2_clicked();
    void on_sidebarButton_3_clicked();
    void on_sidebarButton_4_clicked();
    void on_sidebarButton_5_clicked();
    void on_sidebarButton12_clicked();

    // --- Module Employés ---
    void on_pushButton_11_clicked(); // Ajouter
    void on_pushButton_8_clicked();  // Modifier
    void on_pushButton_10_clicked(); // Supprimer
    void on_pushButton_12_clicked(); // Trier
    void on_lineEdit_6_textChanged(const QString &arg1); // Recherche
    void on_tableWidget_clicked(const QModelIndex &index); // Sélection

    // Fonctionnalités Avancées Employés
    void on_pushButton_13_clicked(); // Fiche de Paie (PDF)
    void on_STATS_clicked();         // Statistiques Salaires (Bar Chart)
    void on_HISTORY_clicked();       // Historique des actions

    // ✅ CORRECTION ICI : Ajout du slot manquant pour le Mailing
    void on_MAILING_clicked();

    // --- Module Sponsors ---
    void on_pushButton_23_clicked(); // Ajouter
    void on_pushButton_38_clicked(); // Modifier
    void on_pushButton_40_clicked(); // Supprimer
    void on_sponsorTable_9_clicked(const QModelIndex &index); // Sélection
    void on_refreshbtn_clicked();    // Rafraîchir
    void on_pushButton_stats_clicked(); // Statistiques Sponsors

    // --- Outils Sponsors ---
    void on_pushButton_35_clicked(); // PDF
    void on_pushButton_37_clicked(); // Tri
    void on_searchEdit_7_textChanged(const QString &arg1); // Recherche
    void on_pushButton_39_clicked(); // PDF
    void on_pushButton_41_clicked(); // Tri
    void on_searchEdit_8_textChanged(const QString &arg1); // Recherche
    void on_pushButton_24_clicked(); // Bouton Recherche

    // --- Chatbot & SMS ---
    void on_pushButton_25_clicked(); // Envoyer Chat
    void on_pushButton_26_clicked(); // Envoyer SMS
    void on_btnConfigSMS_clicked();  // Config SMS
    void on_ConfigurerGemini_clicked(); // Config Gemini

    // --- Slots techniques ---
    void on_lineEdit_textEdited(const QString &arg1);
    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::MainWindow *ui;

    // Variables de sélection
    int idEmployeSelectionne;
    int idSponsorSelectionne;

    // Managers
    Chatbot *chatbot;
    SMSManager *smsManager;

    // --- Méthodes Internes (Helpers) ---
    void initEmployesTable();
    void initSponsorsTable();
    void afficherEmployes();
    void afficherSponsors();
    void clearEmployeFields();
    void clearSponsorFields();

    // Logique Métier Sponsors
    void rechercherSponsors(const QString &recherche);
    void trierSponsorsParDate();
    void exporterSponsorsPDFParContrat();
    bool validerFormulaireSponsor();

    // Logique Statistiques Sponsors (Courbes)
    void afficherStatistiquesCourbes();
    void creerCourbeEvolutionContrats(QVBoxLayout *layout);
    void creerCourbeBudgets(QVBoxLayout *layout);
    void creerGraphiqueCategories(QVBoxLayout *layout);
    void creerGraphiqueDureeContrats(QVBoxLayout *layout);
    void exporterStatistiquesPDF();
};

#endif // MAINWINDOW_H
