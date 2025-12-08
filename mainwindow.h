
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

#include <QSerialPort>

#include <QDebug>

#include <QSqlTableModel>

#include <QMainWindow>

#include <QString>

#include <QPixmap>

#include "arduino.h"



// Includes des modules principaux

#include "chatbot.h"

#include "smsmanager.h"

#include "employes.h"

#include "sponsor.h"

#include "historymanager.h"

#include "calendardialog.h"

#include "serialmanager.h"



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

    QString equipmentMaskToText(int mask);

    void setUserRole(const QString &role);

    void showWelcomeMessage(const QString &name);



private slots:

    // --- Navigation (Sidebar) ---

    void on_sidebarButton_clicked();

    void on_sidebarButton_2_clicked();

    void on_sidebarButton_3_clicked();

    void on_sidebarButton_4_clicked();

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

    void on_MAILING_clicked();       // Mailing



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



    void on_btnBook_deal_clicked();

    void on_btnEdit_deal_clicked();

    void on_btnDelete_deal_clicked();

    void on_btnOrder_deal_clicked();

    void on_btnExportPDF_deal_clicked();

    void on_btnSearch_deal_clicked();

    void on_btnClear_deal_clicked();



    // internal timers

    void deliverPendingOrders();

    void checkAutoOrder();

    void on_btnCalendar_deal_clicked();

    void on_themeButton_7_clicked();



    // === CONTENT CREATOR CRUD ===

    void on_addButton_7_clicked();

    void on_updateButton_7_clicked();

    void on_deleteButton_7_clicked();

    void on_clearButton_7_clicked();

    void on_exportButton_7_clicked();

    void on_searchEdit_9_textChanged(const QString &text);

    void fillFormFromTable();  // Ajouté

    void updateSimpleStats();  // Ajouté



    // === QR CODE ===

    void generateQR();

    void saveQR();

    void copyQR();



    void on_pushButton_stop_live_clicked();

    void on_pushButton_live_clicked();

    void on_sidebarButton_5_clicked();



    // === FINANCE ===

    void addInvoice();

    void editInvoice();

    void deleteInvoice();

    void clearForm();

    void sortByAmount();

    void searchById();

    void exportToCSV();

    void toggleDarkTheme();

    void onInvoiceSelected();



    // === ARDUINO/BADGE ===

    void onCodeReceived(const QString &code);

    void onAccessGranted(const QString &nomEmploye);

    void onAccessDenied(const QString &code);

    void onPorteOuverte();

    void onPorteFermee();

    void onArduinoConnected();

    void onArduinoDisconnected();

    void onArduinoError(const QString &error);

    void on_pushButton_assignBadge_clicked();

    void on_pushButton_removeBadge_clicked();

    void on_btnShowAccessHistory_clicked();

    void onArduinoDataReceived(const QString &data);

    void processBadgeCode(const QString &code);

    void reconnectArduino();







private:

    Ui::MainWindow *ui;

    bool darkTheme = false;

    int editedRow = -1;



    // stock management

    QMap<QString, int> stock;

    QMap<QString, int> ordered;

    QTimer *deliveryTimer = nullptr;

    static QSet<QString> previouslyCriticalItems;

    bool autoExportDone = false;



    // Core functions

    int getEquipMask();

    QDateTime findNextAvailableSlot(const QDateTime &requested, int requiredMask);

    void refreshTable(const QString &filter = "");



    // Variables de sélection

    int idEmployeSelectionne;

    int idSponsorSelectionne;



    // Managers

    Chatbot *chatbot;

    SMSManager *smsManager;

    SerialManager *serialManager;

    Arduino *arduino;



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



    void updateMonthlyStats();

    QString downloadsPathFileName(const QString &prefix);



    // PDF export

    void exportTableToPDF(QTableWidget *tableWidget, const QString &filePath);

    void exportTableToPDF_HTML(QTableWidget *tableWidget, const QString &filePath);



    // auto-export functions

    void autoExportStatsPDF();

    void autoExportStatsWhenCritical();



    // Database initialization

    void initDatabase();

    void assignDefaultBadgesAutomatically();



    // login and qr

    QString m_userRole;

    QPixmap m_currentQR;



    // === CONTENT CREATOR ===

    void loadCreateurs();

    void clearCreateurFields();

    bool isValidName(const QString &name) const;



    // === ROLE SYSTEM ===

    void disableAllInputs();

    void applyRoleRestrictions();



    void mettreEnLiveStudio(const QString &nomCreateur);

    void arreterLiveStudio();



    // === THEME ===

    void applyLightTheme();

    void applyDarkTheme();





    // === FINANCE ===

    void insertInvoiceInTable(QString id, double amount, QDate issueDate, QDate dueDate, QString status);

    void loadInvoicesFromDatabase();

    void initializeVisualStatistics();

    void updateVisualStatistics();



    // === DEAL PAGE ===

    void updateStatsTable();



    // === SPONSOR VALIDATION ===

    void setupSponsorValidators();

    bool validerChampsSponsor();

    void clearSponsorValidationErrors();

    void checkAndCreateFinanceTables();  // <-- AJOUTEZ CETTE LIGNE

    void debugTableStructure();          // Optionnelle

    void resetFinanceTables();           // Optionnelle



};



#endif // MAINWINDOW_H



