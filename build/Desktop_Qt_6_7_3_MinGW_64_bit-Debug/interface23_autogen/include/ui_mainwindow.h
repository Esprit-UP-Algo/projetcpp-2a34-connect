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
    QFrame *menuFrame;
    QVBoxLayout *menuLayout;
    QPushButton *sidebarButton;
    QPushButton *sidebarButton_2;
    QPushButton *sidebarButton_3;
    QPushButton *sidebarButton_4;
    QPushButton *sidebarButton_5;
    QPushButton *sidebarButton_6;
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
    QTableWidget *sponsorTable_9;
    QLabel *label_70;
    QLineEdit *searchEdit_7;
    QPushButton *pushButton_24;
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
    QPushButton *pushButton_27;
    QPushButton *pushButton_28;
    QPushButton *pushButton_38;
    QPushButton *pushButton_39;
    QPushButton *pushButton_41;
    QTableWidget *sponsorTable_11;
    QLabel *label_78;
    QLineEdit *searchEdit_8;
    QPushButton *pushButton_42;
    QTableView *tableView;
    QPushButton *pushButton_40;
    QPushButton *refreshbtn;
    QWidget *employeePage;
    QVBoxLayout *employeeLayout;
    QLabel *employeeLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(998, 715);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        menuFrame = new QFrame(centralwidget);
        menuFrame->setObjectName("menuFrame");
        menuFrame->setGeometry(QRect(40, -40, 132, 688));
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

        sidebarButton_6 = new QPushButton(menuFrame);
        sidebarButton_6->setObjectName("sidebarButton_6");

        menuLayout->addWidget(sidebarButton_6);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(190, -30, 1152, 688));
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
        sponsorTabWidget_3->setGeometry(QRect(0, 30, 1201, 471));
        tabAdd_4 = new QWidget();
        tabAdd_4->setObjectName("tabAdd_4");
        formBoxAdd_3 = new QGroupBox(tabAdd_4);
        formBoxAdd_3->setObjectName("formBoxAdd_3");
        formBoxAdd_3->setGeometry(QRect(0, 30, 631, 261));
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
        pushButton_23->setGeometry(QRect(10, 290, 601, 31));
        pushButton_35 = new QPushButton(tabAdd_4);
        pushButton_35->setObjectName("pushButton_35");
        pushButton_35->setGeometry(QRect(10, 330, 601, 32));
        pushButton_37 = new QPushButton(tabAdd_4);
        pushButton_37->setObjectName("pushButton_37");
        pushButton_37->setGeometry(QRect(10, 370, 601, 32));
        sponsorTable_9 = new QTableWidget(tabAdd_4);
        sponsorTable_9->setObjectName("sponsorTable_9");
        sponsorTable_9->setGeometry(QRect(640, 290, 161, 151));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sponsorTable_9->sizePolicy().hasHeightForWidth());
        sponsorTable_9->setSizePolicy(sizePolicy);
        label_70 = new QLabel(tabAdd_4);
        label_70->setObjectName("label_70");
        label_70->setGeometry(QRect(650, 270, 37, 12));
        searchEdit_7 = new QLineEdit(tabAdd_4);
        searchEdit_7->setObjectName("searchEdit_7");
        searchEdit_7->setGeometry(QRect(10, 0, 241, 31));
        pushButton_24 = new QPushButton(tabAdd_4);
        pushButton_24->setObjectName("pushButton_24");
        pushButton_24->setGeometry(QRect(260, 0, 121, 31));
        sponsorTabWidget_3->addTab(tabAdd_4, QString());
        tabModify_4 = new QWidget();
        tabModify_4->setObjectName("tabModify_4");
        formBoxModify_5 = new QGroupBox(tabModify_4);
        formBoxModify_5->setObjectName("formBoxModify_5");
        formBoxModify_5->setGeometry(QRect(0, 40, 631, 261));
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

        pushButton_27 = new QPushButton(tabModify_4);
        pushButton_27->setObjectName("pushButton_27");
        pushButton_27->setGeometry(QRect(6, 565, 120, 32));
        pushButton_28 = new QPushButton(tabModify_4);
        pushButton_28->setObjectName("pushButton_28");
        pushButton_28->setGeometry(QRect(6, 601, 120, 32));
        pushButton_38 = new QPushButton(tabModify_4);
        pushButton_38->setObjectName("pushButton_38");
        pushButton_38->setGeometry(QRect(10, 300, 601, 31));
        pushButton_39 = new QPushButton(tabModify_4);
        pushButton_39->setObjectName("pushButton_39");
        pushButton_39->setGeometry(QRect(10, 340, 601, 32));
        pushButton_41 = new QPushButton(tabModify_4);
        pushButton_41->setObjectName("pushButton_41");
        pushButton_41->setGeometry(QRect(10, 380, 601, 32));
        sponsorTable_11 = new QTableWidget(tabModify_4);
        sponsorTable_11->setObjectName("sponsorTable_11");
        sponsorTable_11->setGeometry(QRect(640, 270, 161, 151));
        sizePolicy.setHeightForWidth(sponsorTable_11->sizePolicy().hasHeightForWidth());
        sponsorTable_11->setSizePolicy(sizePolicy);
        label_78 = new QLabel(tabModify_4);
        label_78->setObjectName("label_78");
        label_78->setGeometry(QRect(650, 370, 37, 12));
        searchEdit_8 = new QLineEdit(tabModify_4);
        searchEdit_8->setObjectName("searchEdit_8");
        searchEdit_8->setGeometry(QRect(10, 0, 241, 31));
        pushButton_42 = new QPushButton(tabModify_4);
        pushButton_42->setObjectName("pushButton_42");
        pushButton_42->setGeometry(QRect(260, 0, 121, 31));
        sponsorTabWidget_3->addTab(tabModify_4, QString());
        tableView = new QTableView(sponsorPage);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(40, 510, 561, 171));
        pushButton_40 = new QPushButton(sponsorPage);
        pushButton_40->setObjectName("pushButton_40");
        pushButton_40->setGeometry(QRect(640, 530, 120, 32));
        refreshbtn = new QPushButton(sponsorPage);
        refreshbtn->setObjectName("refreshbtn");
        refreshbtn->setGeometry(QRect(640, 580, 120, 32));
        stackedWidget->addWidget(sponsorPage);
        employeePage = new QWidget();
        employeePage->setObjectName("employeePage");
        employeeLayout = new QVBoxLayout(employeePage);
        employeeLayout->setObjectName("employeeLayout");
        employeeLabel = new QLabel(employeePage);
        employeeLabel->setObjectName("employeeLabel");
        employeeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        employeeLayout->addWidget(employeeLabel);

        stackedWidget->addWidget(employeePage);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 998, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(3);
        sponsorTabWidget_3->setCurrentIndex(1);


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
        sidebarButton_6->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
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
        label_64->setText(QCoreApplication::translate("MainWindow", "Sponsor prenom", nullptr));
        label_65->setText(QCoreApplication::translate("MainWindow", "Category", nullptr));
        label_66->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_67->setText(QCoreApplication::translate("MainWindow", "Start of Contract", nullptr));
        label_68->setText(QCoreApplication::translate("MainWindow", "End of Contract", nullptr));
        label_69->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        label_63->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        pushButton_23->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        pushButton_35->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        pushButton_37->setText(QCoreApplication::translate("MainWindow", "Sort by name", nullptr));
        label_70->setText(QCoreApplication::translate("MainWindow", "stat", nullptr));
        searchEdit_7->setText(QCoreApplication::translate("MainWindow", "search by name .....", nullptr));
        searchEdit_7->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search by name or platform", nullptr));
        pushButton_24->setText(QCoreApplication::translate("MainWindow", "search", nullptr));
        sponsorTabWidget_3->setTabText(sponsorTabWidget_3->indexOf(tabAdd_4), QCoreApplication::translate("MainWindow", "Add", nullptr));
        formBoxModify_5->setTitle(QCoreApplication::translate("MainWindow", "Modify Sponsor", nullptr));
        label_71->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_73->setText(QCoreApplication::translate("MainWindow", "Category", nullptr));
        label_74->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_75->setText(QCoreApplication::translate("MainWindow", "Start of Contract", nullptr));
        label_76->setText(QCoreApplication::translate("MainWindow", "End of Contract", nullptr));
        label_77->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        label_72->setText(QCoreApplication::translate("MainWindow", "Sponsor prenom", nullptr));
        label_79->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        pushButton_27->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        pushButton_28->setText(QCoreApplication::translate("MainWindow", "Sort by name", nullptr));
        pushButton_38->setText(QCoreApplication::translate("MainWindow", "to modify", nullptr));
        pushButton_39->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        pushButton_41->setText(QCoreApplication::translate("MainWindow", "Sort by name", nullptr));
        label_78->setText(QCoreApplication::translate("MainWindow", "stat", nullptr));
        searchEdit_8->setText(QCoreApplication::translate("MainWindow", "search by name .....", nullptr));
        searchEdit_8->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search by name or platform", nullptr));
        pushButton_42->setText(QCoreApplication::translate("MainWindow", "search", nullptr));
        sponsorTabWidget_3->setTabText(sponsorTabWidget_3->indexOf(tabModify_4), QCoreApplication::translate("MainWindow", "To Modify", nullptr));
        pushButton_40->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        refreshbtn->setText(QCoreApplication::translate("MainWindow", "refresh", nullptr));
        employeeLabel->setText(QCoreApplication::translate("MainWindow", "Employee Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
