/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *menuFrame;
    QVBoxLayout *menuLayout;
    QPushButton *sidebarButton;
    QPushButton *sidebarButton_2;
    QPushButton *sidebarButton_3;
    QPushButton *sidebarButton_4;
    QPushButton *sidebarButton_5;
    QPushButton *sidebarButton12;
    QStackedWidget *stackedWidget;
    QWidget *managementPage;
    QVBoxLayout *managementLayout;
    QLabel *managementLabel;
    QWidget *financePage;
    QHBoxLayout *financeLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topBar;
    QLineEdit *searchBox;
    QPushButton *btnSearchById;
    QPushButton *btnDarkTheme;
    QPushButton *btnSortByMontant;
    QPushButton *btnExportCSV;
    QGroupBox *formBox;
    QFormLayout *formLayout;
    QLabel *labelId;
    QLineEdit *lineId;
    QLabel *labelMontant;
    QLineEdit *lineMontant;
    QLabel *labelDateEmission;
    QDateEdit *dateEmission;
    QLabel *labelDateEcheance;
    QDateEdit *dateEcheance;
    QLabel *labelStatut;
    QComboBox *comboStatut;
    QHBoxLayout *actionLayout;
    QPushButton *btnAdd;
    QPushButton *btnEdit;
    QPushButton *btnDelete;
    QPushButton *btnClear;
    QTableWidget *tableFactures;
    QGroupBox *statsBox;
    QVBoxLayout *statsLayout;
    QTableWidget *tableStats;
    QWidget *contentCreatorPage;
    QVBoxLayout *contentCreatorLayout;
    QLabel *labelContent;
    QWidget *sponsorPage;
    QTabWidget *sponsorTabWidget_3;
    QWidget *tabAdd_4;
    QGroupBox *formBoxAdd_3;
    QFormLayout *formLayoutAdd_3;
    QLabel *label_64;
    QLineEdit *prenom;
    QLabel *label_65;
    QLineEdit *categorie;
    QLabel *label_66;
    QLineEdit *email;
    QLabel *label_67;
    QDateEdit *debut;
    QLabel *label_68;
    QDateEdit *fin;
    QLabel *label_69;
    QLineEdit *budget;
    QLineEdit *nom;
    QLabel *label_63;
    QPushButton *pushButton_23;
    QPushButton *pushButton_35;
    QPushButton *pushButton_37;
    QLineEdit *searchEdit_7;
    QPushButton *pushButton_24;
    QTableWidget *tableWidget_2;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_25;
    QLabel *label_70;
    QTableWidget *tableWidget_3;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLabel *label_80;
    QLabel *label_81;
    QPushButton *pushButton_26;
    QPushButton *btnConfigSMS;
    QPushButton *ConfigurerGemini;
    QWidget *tabModify_4;
    QGroupBox *formBoxModify_5;
    QFormLayout *formLayoutModify_4;
    QLabel *label_71;
    QLineEdit *ids;
    QLineEdit *nom2;
    QLabel *label_73;
    QLineEdit *categorie2;
    QLabel *label_74;
    QLineEdit *email2;
    QLabel *label_75;
    QDateEdit *debut2;
    QLabel *label_76;
    QDateEdit *fin2;
    QLabel *label_77;
    QLineEdit *budget2;
    QLineEdit *prenom2;
    QLabel *label_72;
    QLabel *label_79;
    QPushButton *pushButton_38;
    QPushButton *pushButton_39;
    QPushButton *pushButton_41;
    QLabel *label_78;
    QLineEdit *searchEdit_8;
    QPushButton *pushButton_42;
    QTableView *tableView;
    QPushButton *pushButton_40;
    QPushButton *refreshbtn;
    QTableWidget *sponsorTable_9;
    QPushButton *pushButton_stats;
    QWidget *employeePage;
    QLabel *employeeLabel;
    QLabel *label_6;
    QLineEdit *lineEdit_6;
    QWidget *widget;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_12;
    QLabel *label_16;
    QLineEdit *lineEdit_12;
    QLineEdit *lineEdit_13;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QLineEdit *lineEdit_16;
    QPushButton *pushButton_8;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *lineEdit_17;
    QLineEdit *lineEdit_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLineEdit *lineEdit_19;
    QLineEdit *lineEdit_20;
    QLineEdit *lineEdit_21;
    QPushButton *pushButton_12;
    QTableWidget *tableWidget;
    QPushButton *pushButton_13;
    QPushButton *STATS;
    QPushButton *HISTORY;
    QPushButton *MAILING;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(2159, 1510);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        menuFrame = new QFrame(centralwidget);
        menuFrame->setObjectName("menuFrame");
        menuFrame->setStyleSheet(QString::fromUtf8("\n"
"     QMainWindow {\n"
"         background-color: #FFFFFF;\n"
"         border-radius: 10px;\n"
"     }\n"
"     QMainWindow[darkTheme=\"true\"] {\n"
"         background-color: #000000;\n"
"     }\n"
"     QGroupBox {\n"
"         background-color: #FFFFFF;\n"
"         border: 2px solid #7D4FEE;\n"
"         border-radius: 10px;\n"
"         font: bold 14pt \"Arial\";\n"
"         color: #7D4FEE;\n"
"         margin: 10px;\n"
"     }\n"
"     QGroupBox[darkTheme=\"true\"] {\n"
"         background-color: #000000;\n"
"         border: 2px solid #7D4FEE;\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QGroupBox::title {\n"
"         color: #7D4FEE;\n"
"         padding: 0 5px;\n"
"         subcontrol-origin: margin;\n"
"         subcontrol-position: top left;\n"
"     }\n"
"     QPushButton {\n"
"         background-color: #7D4FEE;\n"
"         color: #FFFFFF;\n"
"         border-radius: 10px;\n"
"         padding: 8px;\n"
"         min-width: 100px;\n"
"         font: 10pt \"Arial\";\n"
"         border"
                        ": 2px solid #7D4FEE;\n"
"     }\n"
"     QPushButton:hover {\n"
"         background-color: #FFFFFF;\n"
"         color: #7D4FEE;\n"
"         border: 2px solid #7D4FEE;\n"
"     }\n"
"     QLineEdit, QComboBox, QSpinBox, QDateEdit {\n"
"         background-color: #FFFFFF;\n"
"         border: 2px solid #7D4FEE;\n"
"         border-radius: 10px;\n"
"         padding: 6px;\n"
"         font: 10pt \"Arial\";\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QLineEdit[darkTheme=\"true\"], QComboBox[darkTheme=\"true\"], QSpinBox[darkTheme=\"true\"], QDateEdit[darkTheme=\"true\"] {\n"
"         background-color: #000000;\n"
"         border: 2px solid #7D4FEE;\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QTableWidget {\n"
"         background-color: #FFFFFF;\n"
"         border: 2px solid #7D4FEE;\n"
"         border-radius: 10px;\n"
"         gridline-color: #7D4FEE;\n"
"         font: 10pt \"Arial\";\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QTableWidget[darkTheme=\"true\"] {\n"
"         background-col"
                        "or: #1A1A1A;\n"
"         border: 2px solid #7D4FEE;\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QTableWidget::item:selected {\n"
"         background-color: #7D4FEE;\n"
"         color: #FFFFFF;\n"
"     }\n"
"     QLabel {\n"
"         color: #7D4FEE;\n"
"         font: 10pt \"Arial\";\n"
"     }\n"
"     QLabel[darkTheme=\"true\"] {\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QWidget#sidebar {\n"
"         background-color: #7D4FEE;\n"
"         border-radius: 10px 0 0 10px;\n"
"     }\n"
"     QPushButton#sidebarButton {\n"
"         background-color: #7D4FEE;\n"
"         color: #FFFFFF;\n"
"         border: none;\n"
"         border-radius: 10px;\n"
"         padding: 10px;\n"
"         font: 10pt \"Arial\";\n"
"         width: 100%;\n"
"         text-align: left;\n"
"     }\n"
"     QPushButton#sidebarButton:hover {\n"
"         background-color: #FFFFFF;\n"
"         color: #7D4FEE;\n"
"         border: 2px solid #7D4FEE;\n"
"     }\n"
"QFrame#menuFrame {\n"
"    background-color: white;\n"
""
                        "}\n"
"\n"
"   "));
        menuLayout = new QVBoxLayout(menuFrame);
        menuLayout->setObjectName("menuLayout");
        sidebarButton = new QPushButton(menuFrame);
        sidebarButton->setObjectName("sidebarButton");

        menuLayout->addWidget(sidebarButton);

        sidebarButton_2 = new QPushButton(menuFrame);
        sidebarButton_2->setObjectName("sidebarButton_2");

        menuLayout->addWidget(sidebarButton_2);

        sidebarButton_3 = new QPushButton(menuFrame);
        sidebarButton_3->setObjectName("sidebarButton_3");

        menuLayout->addWidget(sidebarButton_3);

        sidebarButton_4 = new QPushButton(menuFrame);
        sidebarButton_4->setObjectName("sidebarButton_4");

        menuLayout->addWidget(sidebarButton_4);

        sidebarButton_5 = new QPushButton(menuFrame);
        sidebarButton_5->setObjectName("sidebarButton_5");

        menuLayout->addWidget(sidebarButton_5);

        sidebarButton12 = new QPushButton(menuFrame);
        sidebarButton12->setObjectName("sidebarButton12");

        menuLayout->addWidget(sidebarButton12);


        gridLayout->addWidget(menuFrame, 0, 0, 1, 1);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setStyleSheet(QString::fromUtf8("\n"
"     QMainWindow {\n"
"         background-color: #FFFFFF;\n"
"         border-radius: 10px;\n"
"     }\n"
"     QMainWindow[darkTheme=\"true\"] {\n"
"         background-color: #000000;\n"
"     }\n"
"     QGroupBox {\n"
"         background-color: #FFFFFF;\n"
"         border: 2px solid #7D4FEE;\n"
"         border-radius: 10px;\n"
"         font: bold 14pt \"Arial\";\n"
"         color: #7D4FEE;\n"
"         margin: 10px;\n"
"     }\n"
"     QGroupBox[darkTheme=\"true\"] {\n"
"         background-color: #000000;\n"
"         border: 2px solid #7D4FEE;\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QGroupBox::title {\n"
"         color: #7D4FEE;\n"
"         padding: 0 5px;\n"
"         subcontrol-origin: margin;\n"
"         subcontrol-position: top left;\n"
"     }\n"
"     QPushButton {\n"
"         background-color: #7D4FEE;\n"
"         color: #FFFFFF;\n"
"         border-radius: 10px;\n"
"         padding: 8px;\n"
"         min-width: 100px;\n"
"         font: 10pt \"Arial\";\n"
"         border"
                        ": 2px solid #7D4FEE;\n"
"     }\n"
"     QPushButton:hover {\n"
"         background-color: #FFFFFF;\n"
"         color: #7D4FEE;\n"
"         border: 2px solid #7D4FEE;\n"
"     }\n"
"     QLineEdit, QComboBox, QSpinBox, QDateEdit {\n"
"         background-color: #FFFFFF;\n"
"         border: 2px solid #7D4FEE;\n"
"         border-radius: 10px;\n"
"         padding: 6px;\n"
"         font: 10pt \"Arial\";\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QLineEdit[darkTheme=\"true\"], QComboBox[darkTheme=\"true\"], QSpinBox[darkTheme=\"true\"], QDateEdit[darkTheme=\"true\"] {\n"
"         background-color: #000000;\n"
"         border: 2px solid #7D4FEE;\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QTableWidget {\n"
"         background-color: #FFFFFF;\n"
"         border: 2px solid #7D4FEE;\n"
"         border-radius: 10px;\n"
"         gridline-color: #7D4FEE;\n"
"         font: 10pt \"Arial\";\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QTableWidget[darkTheme=\"true\"] {\n"
"         background-col"
                        "or: #1A1A1A;\n"
"         border: 2px solid #7D4FEE;\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QTableWidget::item:selected {\n"
"         background-color: #7D4FEE;\n"
"         color: #FFFFFF;\n"
"     }\n"
"     QLabel {\n"
"         color: #7D4FEE;\n"
"         font: 10pt \"Arial\";\n"
"     }\n"
"     QLabel[darkTheme=\"true\"] {\n"
"         color: #7D4FEE;\n"
"     }\n"
"     QWidget#sidebar {\n"
"         background-color: #7D4FEE;\n"
"         border-radius: 10px 0 0 10px;\n"
"     }\n"
"     QPushButton#sidebarButton {\n"
"         background-color: #7D4FEE;\n"
"         color: #FFFFFF;\n"
"         border: none;\n"
"         border-radius: 10px;\n"
"         padding: 10px;\n"
"         font: 10pt \"Arial\";\n"
"         width: 100%;\n"
"         text-align: left;\n"
"     }\n"
"     QPushButton#sidebarButton:hover {\n"
"         background-color: #FFFFFF;\n"
"         color: #7D4FEE;\n"
"         border: 2px solid #7D4FEE;\n"
"     }\n"
"QStackedWidget {\n"
"    background-color: white;\n"
"}\n"
""
                        "\n"
"   "));
        managementPage = new QWidget();
        managementPage->setObjectName("managementPage");
        managementLayout = new QVBoxLayout(managementPage);
        managementLayout->setObjectName("managementLayout");
        managementLabel = new QLabel(managementPage);
        managementLabel->setObjectName("managementLabel");
        managementLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        managementLayout->addWidget(managementLabel);

        stackedWidget->addWidget(managementPage);
        financePage = new QWidget();
        financePage->setObjectName("financePage");
        financeLayout = new QHBoxLayout(financePage);
        financeLayout->setObjectName("financeLayout");
        leftLayout = new QVBoxLayout();
        leftLayout->setObjectName("leftLayout");
        topBar = new QHBoxLayout();
        topBar->setObjectName("topBar");
        searchBox = new QLineEdit(financePage);
        searchBox->setObjectName("searchBox");

        topBar->addWidget(searchBox);

        btnSearchById = new QPushButton(financePage);
        btnSearchById->setObjectName("btnSearchById");

        topBar->addWidget(btnSearchById);

        btnDarkTheme = new QPushButton(financePage);
        btnDarkTheme->setObjectName("btnDarkTheme");

        topBar->addWidget(btnDarkTheme);

        btnSortByMontant = new QPushButton(financePage);
        btnSortByMontant->setObjectName("btnSortByMontant");

        topBar->addWidget(btnSortByMontant);

        btnExportCSV = new QPushButton(financePage);
        btnExportCSV->setObjectName("btnExportCSV");

        topBar->addWidget(btnExportCSV);


        leftLayout->addLayout(topBar);

        formBox = new QGroupBox(financePage);
        formBox->setObjectName("formBox");
        formLayout = new QFormLayout(formBox);
        formLayout->setObjectName("formLayout");
        labelId = new QLabel(formBox);
        labelId->setObjectName("labelId");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelId);

        lineId = new QLineEdit(formBox);
        lineId->setObjectName("lineId");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineId);

        labelMontant = new QLabel(formBox);
        labelMontant->setObjectName("labelMontant");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelMontant);

        lineMontant = new QLineEdit(formBox);
        lineMontant->setObjectName("lineMontant");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineMontant);

        labelDateEmission = new QLabel(formBox);
        labelDateEmission->setObjectName("labelDateEmission");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelDateEmission);

        dateEmission = new QDateEdit(formBox);
        dateEmission->setObjectName("dateEmission");

        formLayout->setWidget(2, QFormLayout::FieldRole, dateEmission);

        labelDateEcheance = new QLabel(formBox);
        labelDateEcheance->setObjectName("labelDateEcheance");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelDateEcheance);

        dateEcheance = new QDateEdit(formBox);
        dateEcheance->setObjectName("dateEcheance");

        formLayout->setWidget(3, QFormLayout::FieldRole, dateEcheance);

        labelStatut = new QLabel(formBox);
        labelStatut->setObjectName("labelStatut");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelStatut);

        comboStatut = new QComboBox(formBox);
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->setObjectName("comboStatut");

        formLayout->setWidget(4, QFormLayout::FieldRole, comboStatut);


        leftLayout->addWidget(formBox);

        actionLayout = new QHBoxLayout();
        actionLayout->setObjectName("actionLayout");
        btnAdd = new QPushButton(financePage);
        btnAdd->setObjectName("btnAdd");

        actionLayout->addWidget(btnAdd);

        btnEdit = new QPushButton(financePage);
        btnEdit->setObjectName("btnEdit");

        actionLayout->addWidget(btnEdit);

        btnDelete = new QPushButton(financePage);
        btnDelete->setObjectName("btnDelete");

        actionLayout->addWidget(btnDelete);

        btnClear = new QPushButton(financePage);
        btnClear->setObjectName("btnClear");

        actionLayout->addWidget(btnClear);


        leftLayout->addLayout(actionLayout);

        tableFactures = new QTableWidget(financePage);
        if (tableFactures->columnCount() < 5)
            tableFactures->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableFactures->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableFactures->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableFactures->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableFactures->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableFactures->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableFactures->setObjectName("tableFactures");

        leftLayout->addWidget(tableFactures);


        financeLayout->addLayout(leftLayout);

        statsBox = new QGroupBox(financePage);
        statsBox->setObjectName("statsBox");
        statsLayout = new QVBoxLayout(statsBox);
        statsLayout->setObjectName("statsLayout");
        tableStats = new QTableWidget(statsBox);
        if (tableStats->columnCount() < 2)
            tableStats->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableStats->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableStats->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        tableStats->setObjectName("tableStats");

        statsLayout->addWidget(tableStats);


        financeLayout->addWidget(statsBox);

        stackedWidget->addWidget(financePage);
        contentCreatorPage = new QWidget();
        contentCreatorPage->setObjectName("contentCreatorPage");
        contentCreatorLayout = new QVBoxLayout(contentCreatorPage);
        contentCreatorLayout->setObjectName("contentCreatorLayout");
        labelContent = new QLabel(contentCreatorPage);
        labelContent->setObjectName("labelContent");
        labelContent->setAlignment(Qt::AlignmentFlag::AlignCenter);

        contentCreatorLayout->addWidget(labelContent);

        stackedWidget->addWidget(contentCreatorPage);
        sponsorPage = new QWidget();
        sponsorPage->setObjectName("sponsorPage");
        sponsorTabWidget_3 = new QTabWidget(sponsorPage);
        sponsorTabWidget_3->setObjectName("sponsorTabWidget_3");
        sponsorTabWidget_3->setGeometry(QRect(-10, 30, 2051, 651));
        sponsorTabWidget_3->setStyleSheet(QString::fromUtf8("QTabWidget::pane {\n"
"    background: white;\n"
"}\n"
"/* Fond global du QTabWidget */\n"
"QTabWidget {\n"
"    background: white;\n"
"}\n"
"\n"
"/* Zone principale (pages internes) */\n"
"QTabWidget::pane {\n"
"    background: white;\n"
"    border: 1px solid #ccc;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"/* Onglets */\n"
"QTabBar::tab {\n"
"    background: white;\n"
"    color: black;\n"
"    padding: 6px 12px;\n"
"    border: 1px solid #ccc;\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 6px;\n"
"    border-top-right-radius: 6px;\n"
"}\n"
"\n"
"/* Onglet s\303\251lectionn\303\251 */\n"
"QTabBar::tab:selected {\n"
"    background: #f2f2f2;\n"
"    color: black;\n"
"}\n"
"\n"
"/* Onglet survol\303\251 */\n"
"QTabBar::tab:hover {\n"
"    background: #e6e6e6;\n"
"}\n"
"\n"
""));
        tabAdd_4 = new QWidget();
        tabAdd_4->setObjectName("tabAdd_4");
        formBoxAdd_3 = new QGroupBox(tabAdd_4);
        formBoxAdd_3->setObjectName("formBoxAdd_3");
        formBoxAdd_3->setGeometry(QRect(0, 30, 881, 291));
        formLayoutAdd_3 = new QFormLayout(formBoxAdd_3);
        formLayoutAdd_3->setObjectName("formLayoutAdd_3");
        label_64 = new QLabel(formBoxAdd_3);
        label_64->setObjectName("label_64");

        formLayoutAdd_3->setWidget(1, QFormLayout::LabelRole, label_64);

        prenom = new QLineEdit(formBoxAdd_3);
        prenom->setObjectName("prenom");

        formLayoutAdd_3->setWidget(1, QFormLayout::FieldRole, prenom);

        label_65 = new QLabel(formBoxAdd_3);
        label_65->setObjectName("label_65");

        formLayoutAdd_3->setWidget(2, QFormLayout::LabelRole, label_65);

        categorie = new QLineEdit(formBoxAdd_3);
        categorie->setObjectName("categorie");

        formLayoutAdd_3->setWidget(2, QFormLayout::FieldRole, categorie);

        label_66 = new QLabel(formBoxAdd_3);
        label_66->setObjectName("label_66");

        formLayoutAdd_3->setWidget(3, QFormLayout::LabelRole, label_66);

        email = new QLineEdit(formBoxAdd_3);
        email->setObjectName("email");

        formLayoutAdd_3->setWidget(3, QFormLayout::FieldRole, email);

        label_67 = new QLabel(formBoxAdd_3);
        label_67->setObjectName("label_67");

        formLayoutAdd_3->setWidget(4, QFormLayout::LabelRole, label_67);

        debut = new QDateEdit(formBoxAdd_3);
        debut->setObjectName("debut");

        formLayoutAdd_3->setWidget(4, QFormLayout::FieldRole, debut);

        label_68 = new QLabel(formBoxAdd_3);
        label_68->setObjectName("label_68");

        formLayoutAdd_3->setWidget(5, QFormLayout::LabelRole, label_68);

        fin = new QDateEdit(formBoxAdd_3);
        fin->setObjectName("fin");

        formLayoutAdd_3->setWidget(5, QFormLayout::FieldRole, fin);

        label_69 = new QLabel(formBoxAdd_3);
        label_69->setObjectName("label_69");

        formLayoutAdd_3->setWidget(6, QFormLayout::LabelRole, label_69);

        budget = new QLineEdit(formBoxAdd_3);
        budget->setObjectName("budget");

        formLayoutAdd_3->setWidget(6, QFormLayout::FieldRole, budget);

        nom = new QLineEdit(formBoxAdd_3);
        nom->setObjectName("nom");

        formLayoutAdd_3->setWidget(0, QFormLayout::FieldRole, nom);

        label_63 = new QLabel(formBoxAdd_3);
        label_63->setObjectName("label_63");

        formLayoutAdd_3->setWidget(0, QFormLayout::LabelRole, label_63);

        pushButton_23 = new QPushButton(tabAdd_4);
        pushButton_23->setObjectName("pushButton_23");
        pushButton_23->setGeometry(QRect(10, 320, 861, 31));
        pushButton_35 = new QPushButton(tabAdd_4);
        pushButton_35->setObjectName("pushButton_35");
        pushButton_35->setGeometry(QRect(10, 360, 861, 32));
        pushButton_37 = new QPushButton(tabAdd_4);
        pushButton_37->setObjectName("pushButton_37");
        pushButton_37->setGeometry(QRect(10, 400, 861, 32));
        searchEdit_7 = new QLineEdit(tabAdd_4);
        searchEdit_7->setObjectName("searchEdit_7");
        searchEdit_7->setGeometry(QRect(10, 0, 351, 31));
        pushButton_24 = new QPushButton(tabAdd_4);
        pushButton_24->setObjectName("pushButton_24");
        pushButton_24->setGeometry(QRect(370, 0, 121, 31));
        tableWidget_2 = new QTableWidget(tabAdd_4);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(880, 0, 461, 401));
        lineEdit = new QLineEdit(tabAdd_4);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(880, 0, 461, 291));
        lineEdit->setMinimumSize(QSize(461, 291));
        lineEdit->setMaximumSize(QSize(461, 291));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial")});
        font.setPointSize(10);
        font.setBold(false);
        font.setItalic(false);
        lineEdit->setFont(font);
        lineEdit->setStyleSheet(QString::fromUtf8("# D\303\251finir une classe CSS r\303\251utilisable\n"
"STYLE_SHEET = \"\"\"\n"
"    .large-text {\n"
"        font-size: 16px;\n"
"        font-family: Segoe UI, Arial;\n"
"    }\n"
"    \n"
"    .response-area {\n"
"        font-size: 15px;\n"
"        background-color: #e8f4f8;\n"
"        padding: 8px;\n"
"        border: 1px solid #ccc;\n"
"        border-radius: 4px;\n"
"    }\n"
"\"\"\"\n"
"# Au lieu de \303\247a :\n"
"self.setStyleSheet(\"font-size: 16px;\")\n"
"\n"
"# Essayez \303\247a :\n"
"self.setStyleSheet(\"\"\"\n"
"    QWidget {\n"
"        font-size: 16px;\n"
"        font-family: Arial;\n"
"    }\n"
"    QLabel, QLineEdit, QTextEdit, QPushButton {\n"
"        font-size: 16px;\n"
"    }\n"
"\"\"\")\n"
"\n"
"self.setStyleSheet(STYLE_SHEET)\n"
"\n"
"# Appliquer aux widgets\n"
"self.line_edit.setProperty(\"class\", \"large-text\")\n"
"self.response_label.setProperty(\"class\", \"response-area\")"));
        lineEdit->setCursorPosition(28);
        lineEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
        lineEdit_2 = new QLineEdit(tabAdd_4);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(890, 300, 451, 31));
        pushButton_25 = new QPushButton(tabAdd_4);
        pushButton_25->setObjectName("pushButton_25");
        pushButton_25->setGeometry(QRect(910, 350, 121, 31));
        label_70 = new QLabel(tabAdd_4);
        label_70->setObjectName("label_70");
        label_70->setGeometry(QRect(900, 0, 81, 21));
        tableWidget_3 = new QTableWidget(tabAdd_4);
        tableWidget_3->setObjectName("tableWidget_3");
        tableWidget_3->setGeometry(QRect(890, 420, 641, 181));
        lineEdit_3 = new QLineEdit(tabAdd_4);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(960, 450, 211, 31));
        lineEdit_4 = new QLineEdit(tabAdd_4);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(960, 520, 211, 31));
        label_80 = new QLabel(tabAdd_4);
        label_80->setObjectName("label_80");
        label_80->setGeometry(QRect(960, 430, 81, 21));
        label_81 = new QLabel(tabAdd_4);
        label_81->setObjectName("label_81");
        label_81->setGeometry(QRect(960, 490, 81, 21));
        pushButton_26 = new QPushButton(tabAdd_4);
        pushButton_26->setObjectName("pushButton_26");
        pushButton_26->setGeometry(QRect(920, 560, 120, 31));
        btnConfigSMS = new QPushButton(tabAdd_4);
        btnConfigSMS->setObjectName("btnConfigSMS");
        btnConfigSMS->setGeometry(QRect(1110, 560, 120, 31));
        ConfigurerGemini = new QPushButton(tabAdd_4);
        ConfigurerGemini->setObjectName("ConfigurerGemini");
        ConfigurerGemini->setGeometry(QRect(1050, 350, 121, 31));
        sponsorTabWidget_3->addTab(tabAdd_4, QString());
        tabModify_4 = new QWidget();
        tabModify_4->setObjectName("tabModify_4");
        formBoxModify_5 = new QGroupBox(tabModify_4);
        formBoxModify_5->setObjectName("formBoxModify_5");
        formBoxModify_5->setGeometry(QRect(0, 30, 871, 301));
        formLayoutModify_4 = new QFormLayout(formBoxModify_5);
        formLayoutModify_4->setObjectName("formLayoutModify_4");
        label_71 = new QLabel(formBoxModify_5);
        label_71->setObjectName("label_71");

        formLayoutModify_4->setWidget(0, QFormLayout::LabelRole, label_71);

        ids = new QLineEdit(formBoxModify_5);
        ids->setObjectName("ids");

        formLayoutModify_4->setWidget(0, QFormLayout::FieldRole, ids);

        nom2 = new QLineEdit(formBoxModify_5);
        nom2->setObjectName("nom2");

        formLayoutModify_4->setWidget(1, QFormLayout::FieldRole, nom2);

        label_73 = new QLabel(formBoxModify_5);
        label_73->setObjectName("label_73");

        formLayoutModify_4->setWidget(3, QFormLayout::LabelRole, label_73);

        categorie2 = new QLineEdit(formBoxModify_5);
        categorie2->setObjectName("categorie2");

        formLayoutModify_4->setWidget(3, QFormLayout::FieldRole, categorie2);

        label_74 = new QLabel(formBoxModify_5);
        label_74->setObjectName("label_74");

        formLayoutModify_4->setWidget(4, QFormLayout::LabelRole, label_74);

        email2 = new QLineEdit(formBoxModify_5);
        email2->setObjectName("email2");

        formLayoutModify_4->setWidget(4, QFormLayout::FieldRole, email2);

        label_75 = new QLabel(formBoxModify_5);
        label_75->setObjectName("label_75");

        formLayoutModify_4->setWidget(5, QFormLayout::LabelRole, label_75);

        debut2 = new QDateEdit(formBoxModify_5);
        debut2->setObjectName("debut2");

        formLayoutModify_4->setWidget(5, QFormLayout::FieldRole, debut2);

        label_76 = new QLabel(formBoxModify_5);
        label_76->setObjectName("label_76");

        formLayoutModify_4->setWidget(6, QFormLayout::LabelRole, label_76);

        fin2 = new QDateEdit(formBoxModify_5);
        fin2->setObjectName("fin2");

        formLayoutModify_4->setWidget(6, QFormLayout::FieldRole, fin2);

        label_77 = new QLabel(formBoxModify_5);
        label_77->setObjectName("label_77");

        formLayoutModify_4->setWidget(7, QFormLayout::LabelRole, label_77);

        budget2 = new QLineEdit(formBoxModify_5);
        budget2->setObjectName("budget2");

        formLayoutModify_4->setWidget(7, QFormLayout::FieldRole, budget2);

        prenom2 = new QLineEdit(formBoxModify_5);
        prenom2->setObjectName("prenom2");

        formLayoutModify_4->setWidget(2, QFormLayout::FieldRole, prenom2);

        label_72 = new QLabel(formBoxModify_5);
        label_72->setObjectName("label_72");

        formLayoutModify_4->setWidget(2, QFormLayout::LabelRole, label_72);

        label_79 = new QLabel(formBoxModify_5);
        label_79->setObjectName("label_79");

        formLayoutModify_4->setWidget(1, QFormLayout::LabelRole, label_79);

        pushButton_38 = new QPushButton(tabModify_4);
        pushButton_38->setObjectName("pushButton_38");
        pushButton_38->setGeometry(QRect(10, 330, 841, 31));
        pushButton_39 = new QPushButton(tabModify_4);
        pushButton_39->setObjectName("pushButton_39");
        pushButton_39->setGeometry(QRect(10, 370, 841, 32));
        pushButton_41 = new QPushButton(tabModify_4);
        pushButton_41->setObjectName("pushButton_41");
        pushButton_41->setGeometry(QRect(10, 410, 841, 32));
        label_78 = new QLabel(tabModify_4);
        label_78->setObjectName("label_78");
        label_78->setGeometry(QRect(650, 370, 37, 12));
        searchEdit_8 = new QLineEdit(tabModify_4);
        searchEdit_8->setObjectName("searchEdit_8");
        searchEdit_8->setGeometry(QRect(10, 0, 351, 31));
        pushButton_42 = new QPushButton(tabModify_4);
        pushButton_42->setObjectName("pushButton_42");
        pushButton_42->setGeometry(QRect(370, 0, 121, 31));
        sponsorTabWidget_3->addTab(tabModify_4, QString());
        tableView = new QTableView(sponsorPage);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(910, 690, 411, 151));
        pushButton_40 = new QPushButton(sponsorPage);
        pushButton_40->setObjectName("pushButton_40");
        pushButton_40->setGeometry(QRect(770, 790, 120, 32));
        refreshbtn = new QPushButton(sponsorPage);
        refreshbtn->setObjectName("refreshbtn");
        refreshbtn->setGeometry(QRect(770, 740, 120, 32));
        sponsorTable_9 = new QTableWidget(sponsorPage);
        sponsorTable_9->setObjectName("sponsorTable_9");
        sponsorTable_9->setGeometry(QRect(10, 690, 751, 191));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sponsorTable_9->sizePolicy().hasHeightForWidth());
        sponsorTable_9->setSizePolicy(sizePolicy);
        pushButton_stats = new QPushButton(sponsorPage);
        pushButton_stats->setObjectName("pushButton_stats");
        pushButton_stats->setGeometry(QRect(770, 700, 120, 32));
        stackedWidget->addWidget(sponsorPage);
        employeePage = new QWidget();
        employeePage->setObjectName("employeePage");
        employeeLabel = new QLabel(employeePage);
        employeeLabel->setObjectName("employeeLabel");
        employeeLabel->setGeometry(QRect(6, 6, 71, 16));
        employeeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_6 = new QLabel(employeePage);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(50, 50, 81, 31));
        label_6->setStyleSheet(QString::fromUtf8("QLabel#label_6 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}"));
        lineEdit_6 = new QLineEdit(employeePage);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(160, 50, 341, 31));
        lineEdit_6->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_6 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_6:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_6:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        widget = new QWidget(employeePage);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 80, 861, 501));
        widget->setStyleSheet(QString::fromUtf8("QWidget#widget {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    font: bold 14pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"    margin: 10px;\n"
"}\n"
"\n"
""));
        label_7 = new QLabel(widget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(50, 20, 91, 31));
        label_7->setStyleSheet(QString::fromUtf8("QLabel#label {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
""));
        label_8 = new QLabel(widget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(50, 60, 91, 31));
        label_8->setStyleSheet(QString::fromUtf8("QLabel#label_2 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"\n"
""));
        label_9 = new QLabel(widget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(50, 100, 91, 31));
        label_9->setStyleSheet(QString::fromUtf8("QLabel#label_3 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_3:hover {\n"
"    background-color: #F5F5FF;\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_3:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        label_12 = new QLabel(widget);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(50, 140, 91, 31));
        label_12->setStyleSheet(QString::fromUtf8("QLabel#label_4 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_4:hover {\n"
"    background-color: #F5F5FF;\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_4:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        label_16 = new QLabel(widget);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(50, 180, 91, 31));
        label_16->setStyleSheet(QString::fromUtf8("QLabel#label_5 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_5:hover {\n"
"    background-color: #F5F5FF;\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_5:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_12 = new QLineEdit(widget);
        lineEdit_12->setObjectName("lineEdit_12");
        lineEdit_12->setGeometry(QRect(240, 20, 401, 31));
        lineEdit_12->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_13 = new QLineEdit(widget);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(240, 60, 401, 31));
        lineEdit_13->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_2 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_2:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_2:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_14 = new QLineEdit(widget);
        lineEdit_14->setObjectName("lineEdit_14");
        lineEdit_14->setGeometry(QRect(240, 100, 401, 31));
        lineEdit_14->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_3 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_3:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_3:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_15 = new QLineEdit(widget);
        lineEdit_15->setObjectName("lineEdit_15");
        lineEdit_15->setGeometry(QRect(230, 140, 401, 31));
        lineEdit_15->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_4 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_4:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_4:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_16 = new QLineEdit(widget);
        lineEdit_16->setObjectName("lineEdit_16");
        lineEdit_16->setGeometry(QRect(240, 180, 401, 31));
        lineEdit_16->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_5 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_5:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_5:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        pushButton_8 = new QPushButton(widget);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(660, 430, 151, 41));
        pushButton_8->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_6 {\n"
"    background-color: #7D4FEE;\n"
"    color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"    min-width: 100px;\n"
"    font: 10pt \"Arial\";\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
"\n"
"QPushButton#pushButton_6:hover {\n"
"    background-color: #FFFFFF;\n"
"    color: #7D4FEE;\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
""));
        pushButton_10 = new QPushButton(widget);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(500, 440, 151, 31));
        pushButton_10->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_7 {\n"
"    background-color: #7D4FEE;\n"
"    color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"    min-width: 100px;\n"
"    font: 10pt \"Arial\";\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
"\n"
"QPushButton#pushButton_7:hover {\n"
"    background-color: #FFFFFF;\n"
"    color: #7D4FEE;\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
""));
        pushButton_11 = new QPushButton(widget);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(10, 440, 471, 31));
        pushButton_11->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_9 {\n"
"    background-color: #7D4FEE;\n"
"    color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"    min-width: 100px;\n"
"    font: 10pt \"Arial\";\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
"\n"
"QPushButton#pushButton_9:hover {\n"
"    background-color: #FFFFFF;\n"
"    color: #7D4FEE;\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
""));
        label_17 = new QLabel(widget);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(60, 270, 101, 31));
        label_17->setStyleSheet(QString::fromUtf8("QLabel#label_10 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_10:hover {\n"
"    background-color: #F5F5FF;\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_10:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        label_18 = new QLabel(widget);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(50, 220, 91, 31));
        label_18->setStyleSheet(QString::fromUtf8("QLabel#label_11 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_11:hover {\n"
"    background-color: #F5F5FF;\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_11:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_17 = new QLineEdit(widget);
        lineEdit_17->setObjectName("lineEdit_17");
        lineEdit_17->setGeometry(QRect(240, 220, 401, 31));
        lineEdit_17->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_7 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_7:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_7:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_18 = new QLineEdit(widget);
        lineEdit_18->setObjectName("lineEdit_18");
        lineEdit_18->setGeometry(QRect(240, 260, 401, 31));
        lineEdit_18->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_8 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_8:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_8:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        label_19 = new QLabel(widget);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(50, 310, 131, 31));
        label_19->setStyleSheet(QString::fromUtf8("QLabel#label_13 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_10:hover {\n"
"    background-color: #F5F5FF;\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_10:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        label_20 = new QLabel(widget);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(40, 350, 141, 31));
        label_20->setStyleSheet(QString::fromUtf8("QLabel#label_14 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_10:hover {\n"
"    background-color: #F5F5FF;\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_10:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        label_21 = new QLabel(widget);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(20, 400, 171, 31));
        label_21->setStyleSheet(QString::fromUtf8("QLabel#label_15 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_10:hover {\n"
"    background-color: #F5F5FF;\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLabel#label_10:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_19 = new QLineEdit(widget);
        lineEdit_19->setObjectName("lineEdit_19");
        lineEdit_19->setGeometry(QRect(240, 300, 401, 31));
        lineEdit_19->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_9 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_8:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_8:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_20 = new QLineEdit(widget);
        lineEdit_20->setObjectName("lineEdit_20");
        lineEdit_20->setGeometry(QRect(240, 340, 401, 31));
        lineEdit_20->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_10 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_8:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_8:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        lineEdit_21 = new QLineEdit(widget);
        lineEdit_21->setObjectName("lineEdit_21");
        lineEdit_21->setGeometry(QRect(240, 390, 401, 31));
        lineEdit_21->setStyleSheet(QString::fromUtf8("QLineEdit#lineEdit_11 {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"\n"
"QLineEdit#lineEdit_8:focus {\n"
"    border: 2px solid #5A2BE2; /* darker purple when focused */\n"
"}\n"
"\n"
"QLineEdit#lineEdit_8:disabled {\n"
"    background-color: #F0F0F0;\n"
"    color: #AAAAAA;\n"
"    border: 2px solid #CCCCCC;\n"
"}\n"
""));
        pushButton_12 = new QPushButton(employeePage);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(520, 40, 120, 41));
        pushButton_12->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_10 {\n"
"    background-color: #7D4FEE;\n"
"    color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"    min-width: 100px;\n"
"    font: 10pt \"Arial\";\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
"\n"
"QPushButton#pushButton_10:hover {\n"
"    background-color: #FFFFFF;\n"
"    color: #7D4FEE;\n"
"    border: 2px solid #7D4FEE;\n"
"}"));
        tableWidget = new QTableWidget(employeePage);
        if (tableWidget->columnCount() < 10)
            tableWidget->setColumnCount(10);
        QFont font1;
        font1.setPointSize(7);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setFont(font1);
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        __qtablewidgetitem8->setFont(font1);
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        __qtablewidgetitem9->setFont(font1);
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        __qtablewidgetitem10->setFont(font1);
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        __qtablewidgetitem11->setFont(font1);
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        __qtablewidgetitem12->setFont(font1);
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        __qtablewidgetitem13->setFont(font1);
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        __qtablewidgetitem14->setFont(font1);
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        __qtablewidgetitem15->setFont(font1);
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        __qtablewidgetitem16->setFont(font1);
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem16);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(30, 610, 861, 191));
        tableWidget->setStyleSheet(QString::fromUtf8("QTableWidget#tableWidget {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    gridline-color: #7D4FEE;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
""));
        pushButton_13 = new QPushButton(employeePage);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(660, 40, 120, 41));
        pushButton_13->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_11 {\n"
"    background-color: #7D4FEE;\n"
"    color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"    min-width: 100px;\n"
"    font: 10pt \"Arial\";\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
"\n"
"QPushButton#pushButton_11:hover {\n"
"    background-color: #FFFFFF;\n"
"    color: #7D4FEE;\n"
"    border: 2px solid #7D4FEE;\n"
"}"));
        STATS = new QPushButton(employeePage);
        STATS->setObjectName("STATS");
        STATS->setGeometry(QRect(800, 40, 120, 41));
        HISTORY = new QPushButton(employeePage);
        HISTORY->setObjectName("HISTORY");
        HISTORY->setGeometry(QRect(950, 40, 120, 41));
        MAILING = new QPushButton(employeePage);
        MAILING->setObjectName("MAILING");
        MAILING->setGeometry(QRect(920, 100, 120, 41));
        stackedWidget->addWidget(employeePage);

        gridLayout->addWidget(stackedWidget, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 2159, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(4);
        sponsorTabWidget_3->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        sidebarButton->setText(QCoreApplication::translate("MainWindow", "Management", nullptr));
        sidebarButton_2->setText(QCoreApplication::translate("MainWindow", "Finance", nullptr));
        sidebarButton_3->setText(QCoreApplication::translate("MainWindow", "Content Creator", nullptr));
        sidebarButton_4->setText(QCoreApplication::translate("MainWindow", "Sponsor", nullptr));
        sidebarButton_5->setText(QCoreApplication::translate("MainWindow", "Deal", nullptr));
        sidebarButton12->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
        managementLabel->setText(QCoreApplication::translate("MainWindow", "Management Page", nullptr));
        searchBox->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search...", nullptr));
        btnSearchById->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        btnDarkTheme->setText(QCoreApplication::translate("MainWindow", "Dark Theme", nullptr));
        btnSortByMontant->setText(QCoreApplication::translate("MainWindow", "Sort by Amount", nullptr));
        btnExportCSV->setText(QCoreApplication::translate("MainWindow", "Export to CSV", nullptr));
        formBox->setTitle(QCoreApplication::translate("MainWindow", "Manage Invoices", nullptr));
        labelId->setText(QCoreApplication::translate("MainWindow", "Invoice ID:", nullptr));
        labelMontant->setText(QCoreApplication::translate("MainWindow", "Amount:", nullptr));
        labelDateEmission->setText(QCoreApplication::translate("MainWindow", "Issue Date:", nullptr));
        labelDateEcheance->setText(QCoreApplication::translate("MainWindow", "Due Date:", nullptr));
        labelStatut->setText(QCoreApplication::translate("MainWindow", "Status:", nullptr));
        comboStatut->setItemText(0, QCoreApplication::translate("MainWindow", "Paid", nullptr));
        comboStatut->setItemText(1, QCoreApplication::translate("MainWindow", "Pending", nullptr));
        comboStatut->setItemText(2, QCoreApplication::translate("MainWindow", "Cancelled", nullptr));

        btnAdd->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        btnEdit->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        btnDelete->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Clear Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableFactures->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Invoice ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableFactures->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Amount", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableFactures->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Issue Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableFactures->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Due Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableFactures->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        statsBox->setTitle(QCoreApplication::translate("MainWindow", "Financial Statistics", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableStats->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Metric", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableStats->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Value", nullptr));
        labelContent->setText(QCoreApplication::translate("MainWindow", "Content Creator Page", nullptr));
        formBoxAdd_3->setTitle(QCoreApplication::translate("MainWindow", "Add Sponsor", nullptr));
        label_64->setText(QCoreApplication::translate("MainWindow", "last name", nullptr));
        label_65->setText(QCoreApplication::translate("MainWindow", "Category", nullptr));
        label_66->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_67->setText(QCoreApplication::translate("MainWindow", "Start of Contract", nullptr));
        label_68->setText(QCoreApplication::translate("MainWindow", "End of Contract", nullptr));
        label_69->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        label_63->setText(QCoreApplication::translate("MainWindow", "name", nullptr));
        pushButton_23->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        pushButton_35->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        pushButton_37->setText(QCoreApplication::translate("MainWindow", "Sort by date", nullptr));
        searchEdit_7->setText(QCoreApplication::translate("MainWindow", "search by name .....", nullptr));
        searchEdit_7->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search by name or platform", nullptr));
        pushButton_24->setText(QCoreApplication::translate("MainWindow", "search", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"justify\"><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit->setText(QCoreApplication::translate("MainWindow", "hello , how can i help you ?", nullptr));
        pushButton_25->setText(QCoreApplication::translate("MainWindow", "send", nullptr));
        label_70->setText(QCoreApplication::translate("MainWindow", "chat boot", nullptr));
        label_80->setText(QCoreApplication::translate("MainWindow", "number", nullptr));
        label_81->setText(QCoreApplication::translate("MainWindow", "msg", nullptr));
        pushButton_26->setText(QCoreApplication::translate("MainWindow", "send sms", nullptr));
        btnConfigSMS->setText(QCoreApplication::translate("MainWindow", "configurer sms", nullptr));
        ConfigurerGemini->setText(QCoreApplication::translate("MainWindow", "Configurer Gemini", nullptr));
        sponsorTabWidget_3->setTabText(sponsorTabWidget_3->indexOf(tabAdd_4), QCoreApplication::translate("MainWindow", "Add", nullptr));
        formBoxModify_5->setTitle(QCoreApplication::translate("MainWindow", "Modify Sponsor", nullptr));
        label_71->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_73->setText(QCoreApplication::translate("MainWindow", "Category", nullptr));
        label_74->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_75->setText(QCoreApplication::translate("MainWindow", "Start of Contract", nullptr));
        label_76->setText(QCoreApplication::translate("MainWindow", "End of Contract", nullptr));
        label_77->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        label_72->setText(QCoreApplication::translate("MainWindow", "last name", nullptr));
        label_79->setText(QCoreApplication::translate("MainWindow", "name", nullptr));
        pushButton_38->setText(QCoreApplication::translate("MainWindow", "to modify", nullptr));
        pushButton_39->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        pushButton_41->setText(QCoreApplication::translate("MainWindow", "Sort by name", nullptr));
        label_78->setText(QString());
        searchEdit_8->setText(QCoreApplication::translate("MainWindow", "search by name .....", nullptr));
        searchEdit_8->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search by name or platform", nullptr));
        pushButton_42->setText(QCoreApplication::translate("MainWindow", "search", nullptr));
        sponsorTabWidget_3->setTabText(sponsorTabWidget_3->indexOf(tabModify_4), QCoreApplication::translate("MainWindow", "To Modify", nullptr));
        pushButton_40->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        refreshbtn->setText(QCoreApplication::translate("MainWindow", "refresh", nullptr));
        pushButton_stats->setText(QCoreApplication::translate("MainWindow", "stat", nullptr));
        employeeLabel->setText(QCoreApplication::translate("MainWindow", "Employee Page", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">search</p></body></html>", nullptr));
        lineEdit_6->setText(QCoreApplication::translate("MainWindow", "by name , last name ,position", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">ID</p></body></html>", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">NOM</p></body></html>", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">PRENOM</p></body></html>", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">CIN</p></body></html>", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">DATE_EMBAUCHE</p></body></html>", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">TELEPHONE</p></body></html>", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">EMAIL</p></body></html>", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">SEXE</p></body></html>", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">SALAIRE</p></body></html>", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">ID_SUPERVISEUR</p></body></html>", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "Sort By Salary", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "NOM", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "PRENOM", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "EMAIL", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "TELEPHONE", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "SEXE", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "SALAIRE", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "DATE_EMBAUCHE", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "ID_SUPERVISEUR", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "PAYSLIP", nullptr));
        STATS->setText(QCoreApplication::translate("MainWindow", "STATS", nullptr));
        HISTORY->setText(QCoreApplication::translate("MainWindow", "HISTORY DB", nullptr));
        MAILING->setText(QCoreApplication::translate("MainWindow", "MAILING", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
