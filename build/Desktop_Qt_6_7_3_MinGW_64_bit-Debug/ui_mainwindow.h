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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *sidebarLayout;
    QPushButton *financeButton;
    QPushButton *contentCreatorButton;
    QPushButton *sponsorButton;
    QPushButton *dealButton;
    QPushButton *employeeButton;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget;
    QWidget *managementPage;
    QVBoxLayout *managementLayout;
    QLabel *managementLabel;
    QWidget *financePage;
    QVBoxLayout *financeLayout;
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
    QWidget *contentCreatorPage;
    QVBoxLayout *contentCreatorLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_search;
    QLineEdit *searchEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *themeButton;
    QPushButton *exportButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_2;
    QPushButton *updateButton;
    QPushButton *deleteButton;
    QLineEdit *nameEdit;
    QLabel *label_4;
    QPushButton *addButton;
    QLabel *label_2;
    QComboBox *platformCombo;
    QLabel *label;
    QLabel *label_3;
    QLabel *titleLabel;
    QPushButton *clearButton;
    QSpinBox *subscribersSpin;
    QLineEdit *contentTypeEdit;
    QTableWidget *creatorTable;
    QWidget *sponsorPage;
    QVBoxLayout *sponsorLayout;
    QTabWidget *sponsorTabWidget;
    QWidget *tabAdd;
    QVBoxLayout *tabAddLayout;
    QGroupBox *formBoxAdd;
    QFormLayout *formLayoutAdd;
    QLabel *label_5;
    QLineEdit *lineEdit_10;
    QLabel *label1;
    QLineEdit *lineEdit;
    QLabel *label_21;
    QLineEdit *lineEdit_2;
    QLabel *label_31;
    QLineEdit *lineEdit_3;
    QLabel *label_41;
    QDateEdit *dateEdit;
    QLabel *label_9;
    QDateEdit *dateEdit_2;
    QLabel *label_10;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton;
    QWidget *tabModify;
    QVBoxLayout *tabModifyLayout;
    QGroupBox *formBoxModify;
    QFormLayout *formLayoutModify;
    QLabel *label_18;
    QLineEdit *lineEdit_11;
    QLabel *label_13;
    QLineEdit *lineEdit_5;
    QLabel *label_12;
    QLineEdit *lineEdit_9;
    QLabel *label_15;
    QLineEdit *lineEdit_8;
    QLabel *label_16;
    QDateEdit *dateEdit_4;
    QLabel *label_14;
    QDateEdit *dateEdit_3;
    QLabel *label_11;
    QLineEdit *lineEdit_7;
    QPushButton *pushButton_2;
    QTableWidget *sponsorTable;
    QWidget *dealPage;
    QVBoxLayout *dealLayout;
    QLabel *dealLabel;
    QWidget *employeePage;
    QVBoxLayout *employeeLayout;
    QLabel *employeeLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
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
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        sidebarLayout = new QVBoxLayout();
        sidebarLayout->setObjectName("sidebarLayout");
        financeButton = new QPushButton(centralwidget);
        financeButton->setObjectName("financeButton");

        sidebarLayout->addWidget(financeButton);

        contentCreatorButton = new QPushButton(centralwidget);
        contentCreatorButton->setObjectName("contentCreatorButton");

        sidebarLayout->addWidget(contentCreatorButton);

        sponsorButton = new QPushButton(centralwidget);
        sponsorButton->setObjectName("sponsorButton");

        sidebarLayout->addWidget(sponsorButton);

        dealButton = new QPushButton(centralwidget);
        dealButton->setObjectName("dealButton");

        sidebarLayout->addWidget(dealButton);

        employeeButton = new QPushButton(centralwidget);
        employeeButton->setObjectName("employeeButton");

        sidebarLayout->addWidget(employeeButton);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        sidebarLayout->addItem(verticalSpacer);


        mainLayout->addLayout(sidebarLayout);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
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
        financeLayout = new QVBoxLayout(financePage);
        financeLayout->setObjectName("financeLayout");
        topBar = new QHBoxLayout();
        topBar->setObjectName("topBar");
        searchBox = new QLineEdit(financePage);
        searchBox->setObjectName("searchBox");
        searchBox->setStyleSheet(QString::fromUtf8("border: 1px solid #ccc; border-radius: 5px; padding: 4px; background: #f9f9f9;"));

        topBar->addWidget(searchBox);

        btnSearchById = new QPushButton(financePage);
        btnSearchById->setObjectName("btnSearchById");
        btnSearchById->setStyleSheet(QString::fromUtf8("background-color: #7D4FEE; color: white; border-radius: 8px; padding: 6px 12px; font-weight: bold;"));

        topBar->addWidget(btnSearchById);

        btnDarkTheme = new QPushButton(financePage);
        btnDarkTheme->setObjectName("btnDarkTheme");
        btnDarkTheme->setStyleSheet(QString::fromUtf8("background-color: #7D4FEE; color: white; border-radius: 8px; padding: 6px 12px; font-weight: bold;"));

        topBar->addWidget(btnDarkTheme);

        btnSortByMontant = new QPushButton(financePage);
        btnSortByMontant->setObjectName("btnSortByMontant");
        btnSortByMontant->setStyleSheet(QString::fromUtf8("background-color: #7D4FEE; color: white; border-radius: 8px; padding: 6px 12px; font-weight: bold;"));

        topBar->addWidget(btnSortByMontant);

        btnExportCSV = new QPushButton(financePage);
        btnExportCSV->setObjectName("btnExportCSV");
        btnExportCSV->setStyleSheet(QString::fromUtf8("background-color: #7D4FEE; color: white; border-radius: 8px; padding: 6px 12px; font-weight: bold;"));

        topBar->addWidget(btnExportCSV);


        financeLayout->addLayout(topBar);

        formBox = new QGroupBox(financePage);
        formBox->setObjectName("formBox");
        formBox->setStyleSheet(QString::fromUtf8("font-weight: bold; border: 2px solid #7D4FEE; border-radius: 10px; margin-top: 10px;"));
        formLayout = new QFormLayout(formBox);
        formLayout->setObjectName("formLayout");
        labelId = new QLabel(formBox);
        labelId->setObjectName("labelId");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelId);

        lineId = new QLineEdit(formBox);
        lineId->setObjectName("lineId");
        lineId->setStyleSheet(QString::fromUtf8("border: 2px solid #7D4FEE; border-radius: 10px; padding: 6px; background: #FFFFFF;"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineId);

        labelMontant = new QLabel(formBox);
        labelMontant->setObjectName("labelMontant");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelMontant);

        lineMontant = new QLineEdit(formBox);
        lineMontant->setObjectName("lineMontant");
        lineMontant->setStyleSheet(QString::fromUtf8("border: 2px solid #7D4FEE; border-radius: 10px; padding: 6px; background: #FFFFFF;"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineMontant);

        labelDateEmission = new QLabel(formBox);
        labelDateEmission->setObjectName("labelDateEmission");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelDateEmission);

        dateEmission = new QDateEdit(formBox);
        dateEmission->setObjectName("dateEmission");
        dateEmission->setStyleSheet(QString::fromUtf8("border: 2px solid #7D4FEE; border-radius: 10px; padding: 6px; background: #FFFFFF;"));

        formLayout->setWidget(2, QFormLayout::FieldRole, dateEmission);

        labelDateEcheance = new QLabel(formBox);
        labelDateEcheance->setObjectName("labelDateEcheance");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelDateEcheance);

        dateEcheance = new QDateEdit(formBox);
        dateEcheance->setObjectName("dateEcheance");
        dateEcheance->setStyleSheet(QString::fromUtf8("border: 2px solid #7D4FEE; border-radius: 10px; padding: 6px; background: #FFFFFF;"));

        formLayout->setWidget(3, QFormLayout::FieldRole, dateEcheance);

        labelStatut = new QLabel(formBox);
        labelStatut->setObjectName("labelStatut");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelStatut);

        comboStatut = new QComboBox(formBox);
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->setObjectName("comboStatut");
        comboStatut->setStyleSheet(QString::fromUtf8("border: 2px solid #7D4FEE; border-radius: 10px; padding: 6px; background: #FFFFFF;"));

        formLayout->setWidget(4, QFormLayout::FieldRole, comboStatut);


        financeLayout->addWidget(formBox);

        actionLayout = new QHBoxLayout();
        actionLayout->setObjectName("actionLayout");
        btnAdd = new QPushButton(financePage);
        btnAdd->setObjectName("btnAdd");
        btnAdd->setStyleSheet(QString::fromUtf8("background-color: #7D4FEE; color: white; border-radius: 8px; padding: 6px 12px; font-weight: bold;"));

        actionLayout->addWidget(btnAdd);

        btnEdit = new QPushButton(financePage);
        btnEdit->setObjectName("btnEdit");
        btnEdit->setStyleSheet(QString::fromUtf8("background-color: #7D4FEE; color: white; border-radius: 8px; padding: 6px 12px; font-weight: bold;"));

        actionLayout->addWidget(btnEdit);

        btnDelete = new QPushButton(financePage);
        btnDelete->setObjectName("btnDelete");
        btnDelete->setStyleSheet(QString::fromUtf8("background-color: #7D4FEE; color: white; border-radius: 8px; padding: 6px 12px; font-weight: bold;"));

        actionLayout->addWidget(btnDelete);

        btnClear = new QPushButton(financePage);
        btnClear->setObjectName("btnClear");
        btnClear->setStyleSheet(QString::fromUtf8("background-color: #7D4FEE; color: white; border-radius: 8px; padding: 6px 12px; font-weight: bold;"));

        actionLayout->addWidget(btnClear);


        financeLayout->addLayout(actionLayout);

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
        tableFactures->setStyleSheet(QString::fromUtf8("border: 2px solid #7D4FEE; gridline-color: #7D4FEE; selection-background-color: #7D4FEE; selection-color: white;"));

        financeLayout->addWidget(tableFactures);

        stackedWidget->addWidget(financePage);
        contentCreatorPage = new QWidget();
        contentCreatorPage->setObjectName("contentCreatorPage");
        contentCreatorLayout = new QVBoxLayout(contentCreatorPage);
        contentCreatorLayout->setObjectName("contentCreatorLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_search = new QLabel(contentCreatorPage);
        label_search->setObjectName("label_search");

        horizontalLayout_2->addWidget(label_search);

        searchEdit = new QLineEdit(contentCreatorPage);
        searchEdit->setObjectName("searchEdit");

        horizontalLayout_2->addWidget(searchEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        themeButton = new QPushButton(contentCreatorPage);
        themeButton->setObjectName("themeButton");

        horizontalLayout_2->addWidget(themeButton);

        exportButton = new QPushButton(contentCreatorPage);
        exportButton->setObjectName("exportButton");

        horizontalLayout_2->addWidget(exportButton);


        contentCreatorLayout->addLayout(horizontalLayout_2);

        groupBox = new QGroupBox(contentCreatorPage);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        updateButton = new QPushButton(groupBox);
        updateButton->setObjectName("updateButton");

        gridLayout_2->addWidget(updateButton, 5, 1, 1, 1);

        deleteButton = new QPushButton(groupBox);
        deleteButton->setObjectName("deleteButton");

        gridLayout_2->addWidget(deleteButton, 5, 2, 1, 1);

        nameEdit = new QLineEdit(groupBox);
        nameEdit->setObjectName("nameEdit");

        gridLayout_2->addWidget(nameEdit, 1, 1, 1, 2);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");

        gridLayout_2->addWidget(label_4, 4, 0, 1, 1);

        addButton = new QPushButton(groupBox);
        addButton->setObjectName("addButton");

        gridLayout_2->addWidget(addButton, 5, 0, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        platformCombo = new QComboBox(groupBox);
        platformCombo->addItem(QString());
        platformCombo->addItem(QString());
        platformCombo->addItem(QString());
        platformCombo->addItem(QString());
        platformCombo->addItem(QString());
        platformCombo->addItem(QString());
        platformCombo->setObjectName("platformCombo");

        gridLayout_2->addWidget(platformCombo, 2, 1, 1, 2);

        label = new QLabel(groupBox);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        gridLayout_2->addWidget(label_3, 3, 0, 1, 1);

        titleLabel = new QLabel(groupBox);
        titleLabel->setObjectName("titleLabel");

        gridLayout_2->addWidget(titleLabel, 0, 0, 1, 3);

        clearButton = new QPushButton(groupBox);
        clearButton->setObjectName("clearButton");

        gridLayout_2->addWidget(clearButton, 6, 0, 1, 3);

        subscribersSpin = new QSpinBox(groupBox);
        subscribersSpin->setObjectName("subscribersSpin");
        subscribersSpin->setMinimum(100);
        subscribersSpin->setMaximum(1000000000);
        subscribersSpin->setSingleStep(100);

        gridLayout_2->addWidget(subscribersSpin, 3, 1, 1, 2);

        contentTypeEdit = new QLineEdit(groupBox);
        contentTypeEdit->setObjectName("contentTypeEdit");

        gridLayout_2->addWidget(contentTypeEdit, 4, 1, 1, 2);


        verticalLayout_2->addLayout(gridLayout_2);


        contentCreatorLayout->addWidget(groupBox);

        creatorTable = new QTableWidget(contentCreatorPage);
        if (creatorTable->columnCount() < 4)
            creatorTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        creatorTable->setObjectName("creatorTable");
        sizePolicy.setHeightForWidth(creatorTable->sizePolicy().hasHeightForWidth());
        creatorTable->setSizePolicy(sizePolicy);

        contentCreatorLayout->addWidget(creatorTable);

        stackedWidget->addWidget(contentCreatorPage);
        sponsorPage = new QWidget();
        sponsorPage->setObjectName("sponsorPage");
        sponsorLayout = new QVBoxLayout(sponsorPage);
        sponsorLayout->setObjectName("sponsorLayout");
        sponsorTabWidget = new QTabWidget(sponsorPage);
        sponsorTabWidget->setObjectName("sponsorTabWidget");
        tabAdd = new QWidget();
        tabAdd->setObjectName("tabAdd");
        tabAddLayout = new QVBoxLayout(tabAdd);
        tabAddLayout->setObjectName("tabAddLayout");
        formBoxAdd = new QGroupBox(tabAdd);
        formBoxAdd->setObjectName("formBoxAdd");
        formLayoutAdd = new QFormLayout(formBoxAdd);
        formLayoutAdd->setObjectName("formLayoutAdd");
        label_5 = new QLabel(formBoxAdd);
        label_5->setObjectName("label_5");

        formLayoutAdd->setWidget(0, QFormLayout::LabelRole, label_5);

        lineEdit_10 = new QLineEdit(formBoxAdd);
        lineEdit_10->setObjectName("lineEdit_10");

        formLayoutAdd->setWidget(0, QFormLayout::FieldRole, lineEdit_10);

        label1 = new QLabel(formBoxAdd);
        label1->setObjectName("label1");

        formLayoutAdd->setWidget(1, QFormLayout::LabelRole, label1);

        lineEdit = new QLineEdit(formBoxAdd);
        lineEdit->setObjectName("lineEdit");

        formLayoutAdd->setWidget(1, QFormLayout::FieldRole, lineEdit);

        label_21 = new QLabel(formBoxAdd);
        label_21->setObjectName("label_21");

        formLayoutAdd->setWidget(2, QFormLayout::LabelRole, label_21);

        lineEdit_2 = new QLineEdit(formBoxAdd);
        lineEdit_2->setObjectName("lineEdit_2");

        formLayoutAdd->setWidget(2, QFormLayout::FieldRole, lineEdit_2);

        label_31 = new QLabel(formBoxAdd);
        label_31->setObjectName("label_31");

        formLayoutAdd->setWidget(3, QFormLayout::LabelRole, label_31);

        lineEdit_3 = new QLineEdit(formBoxAdd);
        lineEdit_3->setObjectName("lineEdit_3");

        formLayoutAdd->setWidget(3, QFormLayout::FieldRole, lineEdit_3);

        label_41 = new QLabel(formBoxAdd);
        label_41->setObjectName("label_41");

        formLayoutAdd->setWidget(4, QFormLayout::LabelRole, label_41);

        dateEdit = new QDateEdit(formBoxAdd);
        dateEdit->setObjectName("dateEdit");

        formLayoutAdd->setWidget(4, QFormLayout::FieldRole, dateEdit);

        label_9 = new QLabel(formBoxAdd);
        label_9->setObjectName("label_9");

        formLayoutAdd->setWidget(5, QFormLayout::LabelRole, label_9);

        dateEdit_2 = new QDateEdit(formBoxAdd);
        dateEdit_2->setObjectName("dateEdit_2");

        formLayoutAdd->setWidget(5, QFormLayout::FieldRole, dateEdit_2);

        label_10 = new QLabel(formBoxAdd);
        label_10->setObjectName("label_10");

        formLayoutAdd->setWidget(6, QFormLayout::LabelRole, label_10);

        lineEdit_6 = new QLineEdit(formBoxAdd);
        lineEdit_6->setObjectName("lineEdit_6");

        formLayoutAdd->setWidget(6, QFormLayout::FieldRole, lineEdit_6);


        tabAddLayout->addWidget(formBoxAdd);

        pushButton = new QPushButton(tabAdd);
        pushButton->setObjectName("pushButton");

        tabAddLayout->addWidget(pushButton);

        sponsorTabWidget->addTab(tabAdd, QString());
        tabModify = new QWidget();
        tabModify->setObjectName("tabModify");
        tabModifyLayout = new QVBoxLayout(tabModify);
        tabModifyLayout->setObjectName("tabModifyLayout");
        formBoxModify = new QGroupBox(tabModify);
        formBoxModify->setObjectName("formBoxModify");
        formLayoutModify = new QFormLayout(formBoxModify);
        formLayoutModify->setObjectName("formLayoutModify");
        label_18 = new QLabel(formBoxModify);
        label_18->setObjectName("label_18");

        formLayoutModify->setWidget(0, QFormLayout::LabelRole, label_18);

        lineEdit_11 = new QLineEdit(formBoxModify);
        lineEdit_11->setObjectName("lineEdit_11");

        formLayoutModify->setWidget(0, QFormLayout::FieldRole, lineEdit_11);

        label_13 = new QLabel(formBoxModify);
        label_13->setObjectName("label_13");

        formLayoutModify->setWidget(1, QFormLayout::LabelRole, label_13);

        lineEdit_5 = new QLineEdit(formBoxModify);
        lineEdit_5->setObjectName("lineEdit_5");

        formLayoutModify->setWidget(1, QFormLayout::FieldRole, lineEdit_5);

        label_12 = new QLabel(formBoxModify);
        label_12->setObjectName("label_12");

        formLayoutModify->setWidget(2, QFormLayout::LabelRole, label_12);

        lineEdit_9 = new QLineEdit(formBoxModify);
        lineEdit_9->setObjectName("lineEdit_9");

        formLayoutModify->setWidget(2, QFormLayout::FieldRole, lineEdit_9);

        label_15 = new QLabel(formBoxModify);
        label_15->setObjectName("label_15");

        formLayoutModify->setWidget(3, QFormLayout::LabelRole, label_15);

        lineEdit_8 = new QLineEdit(formBoxModify);
        lineEdit_8->setObjectName("lineEdit_8");

        formLayoutModify->setWidget(3, QFormLayout::FieldRole, lineEdit_8);

        label_16 = new QLabel(formBoxModify);
        label_16->setObjectName("label_16");

        formLayoutModify->setWidget(4, QFormLayout::LabelRole, label_16);

        dateEdit_4 = new QDateEdit(formBoxModify);
        dateEdit_4->setObjectName("dateEdit_4");

        formLayoutModify->setWidget(4, QFormLayout::FieldRole, dateEdit_4);

        label_14 = new QLabel(formBoxModify);
        label_14->setObjectName("label_14");

        formLayoutModify->setWidget(5, QFormLayout::LabelRole, label_14);

        dateEdit_3 = new QDateEdit(formBoxModify);
        dateEdit_3->setObjectName("dateEdit_3");

        formLayoutModify->setWidget(5, QFormLayout::FieldRole, dateEdit_3);

        label_11 = new QLabel(formBoxModify);
        label_11->setObjectName("label_11");

        formLayoutModify->setWidget(6, QFormLayout::LabelRole, label_11);

        lineEdit_7 = new QLineEdit(formBoxModify);
        lineEdit_7->setObjectName("lineEdit_7");

        formLayoutModify->setWidget(6, QFormLayout::FieldRole, lineEdit_7);


        tabModifyLayout->addWidget(formBoxModify);

        pushButton_2 = new QPushButton(tabModify);
        pushButton_2->setObjectName("pushButton_2");

        tabModifyLayout->addWidget(pushButton_2);

        sponsorTabWidget->addTab(tabModify, QString());

        sponsorLayout->addWidget(sponsorTabWidget);

        sponsorTable = new QTableWidget(sponsorPage);
        if (sponsorTable->columnCount() < 7)
            sponsorTable->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        sponsorTable->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        sponsorTable->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        sponsorTable->setHorizontalHeaderItem(2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        sponsorTable->setHorizontalHeaderItem(3, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        sponsorTable->setHorizontalHeaderItem(4, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        sponsorTable->setHorizontalHeaderItem(5, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        sponsorTable->setHorizontalHeaderItem(6, __qtablewidgetitem15);
        sponsorTable->setObjectName("sponsorTable");
        sizePolicy.setHeightForWidth(sponsorTable->sizePolicy().hasHeightForWidth());
        sponsorTable->setSizePolicy(sizePolicy);

        sponsorLayout->addWidget(sponsorTable);

        stackedWidget->addWidget(sponsorPage);
        dealPage = new QWidget();
        dealPage->setObjectName("dealPage");
        dealLayout = new QVBoxLayout(dealPage);
        dealLayout->setObjectName("dealLayout");
        dealLabel = new QLabel(dealPage);
        dealLabel->setObjectName("dealLabel");
        dealLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        dealLayout->addWidget(dealLabel);

        stackedWidget->addWidget(dealPage);
        employeePage = new QWidget();
        employeePage->setObjectName("employeePage");
        employeeLayout = new QVBoxLayout(employeePage);
        employeeLayout->setObjectName("employeeLayout");
        employeeLabel = new QLabel(employeePage);
        employeeLabel->setObjectName("employeeLabel");
        employeeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        employeeLayout->addWidget(employeeLabel);

        stackedWidget->addWidget(employeePage);

        mainLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);
        sponsorTabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Content Creator Manager", nullptr));
        financeButton->setText(QCoreApplication::translate("MainWindow", "Finance", nullptr));
        contentCreatorButton->setText(QCoreApplication::translate("MainWindow", "Content Creator", nullptr));
        sponsorButton->setText(QCoreApplication::translate("MainWindow", "Sponsor", nullptr));
        dealButton->setText(QCoreApplication::translate("MainWindow", "Deal", nullptr));
        employeeButton->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
        managementLabel->setText(QCoreApplication::translate("MainWindow", "Management Page", nullptr));
        searchBox->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search...", nullptr));
        btnSearchById->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        btnDarkTheme->setText(QCoreApplication::translate("MainWindow", "Dark Theme", nullptr));
        btnSortByMontant->setText(QCoreApplication::translate("MainWindow", "Trier par Montant", nullptr));
        btnExportCSV->setText(QCoreApplication::translate("MainWindow", "Export to CSV", nullptr));
        formBox->setTitle(QCoreApplication::translate("MainWindow", "Manage Finance - Factures", nullptr));
        labelId->setText(QCoreApplication::translate("MainWindow", "ID Facture :", nullptr));
        labelMontant->setText(QCoreApplication::translate("MainWindow", "Montant :", nullptr));
        labelDateEmission->setText(QCoreApplication::translate("MainWindow", "Date \303\251mission :", nullptr));
        labelDateEcheance->setText(QCoreApplication::translate("MainWindow", "Date \303\251ch\303\251ance :", nullptr));
        labelStatut->setText(QCoreApplication::translate("MainWindow", "Statut :", nullptr));
        comboStatut->setItemText(0, QCoreApplication::translate("MainWindow", "Pay\303\251e", nullptr));
        comboStatut->setItemText(1, QCoreApplication::translate("MainWindow", "En attente", nullptr));
        comboStatut->setItemText(2, QCoreApplication::translate("MainWindow", "Annul\303\251e", nullptr));

        btnAdd->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        btnEdit->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        btnDelete->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Clear Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableFactures->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID Facture", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableFactures->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Montant", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableFactures->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Date \303\251mission", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableFactures->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Date \303\251ch\303\251ance", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableFactures->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        label_search->setText(QCoreApplication::translate("MainWindow", "Search:", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search by name or platform", nullptr));
        themeButton->setText(QCoreApplication::translate("MainWindow", "Dark Theme", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "Export to CSV", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Manage Content Creators", nullptr));
        updateButton->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Content Type:", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Platform:", nullptr));
        platformCombo->setItemText(0, QCoreApplication::translate("MainWindow", "YouTube", nullptr));
        platformCombo->setItemText(1, QCoreApplication::translate("MainWindow", "Twitch", nullptr));
        platformCombo->setItemText(2, QCoreApplication::translate("MainWindow", "Kick", nullptr));
        platformCombo->setItemText(3, QCoreApplication::translate("MainWindow", "Instagram", nullptr));
        platformCombo->setItemText(4, QCoreApplication::translate("MainWindow", "TikTok", nullptr));
        platformCombo->setItemText(5, QCoreApplication::translate("MainWindow", "Facebook", nullptr));

        label->setText(QCoreApplication::translate("MainWindow", "Name:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Subscribers:", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Content Creator Details", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "Clear Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = creatorTable->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = creatorTable->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Platform", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = creatorTable->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Subscribers", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = creatorTable->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Content Type", nullptr));
        formBoxAdd->setTitle(QCoreApplication::translate("MainWindow", "Add Sponsor", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "Sponsor Name", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "Category", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_41->setText(QCoreApplication::translate("MainWindow", "Start of Contract", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "End of Contract", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        sponsorTabWidget->setTabText(sponsorTabWidget->indexOf(tabAdd), QCoreApplication::translate("MainWindow", "Add", nullptr));
        formBoxModify->setTitle(QCoreApplication::translate("MainWindow", "Modify Sponsor", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Sponsor Name", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Category", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Start of Contract", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "End of Contract", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "To Modify", nullptr));
        sponsorTabWidget->setTabText(sponsorTabWidget->indexOf(tabModify), QCoreApplication::translate("MainWindow", "To Modify", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = sponsorTable->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = sponsorTable->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Sponsor Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = sponsorTable->horizontalHeaderItem(2);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Category", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = sponsorTable->horizontalHeaderItem(3);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = sponsorTable->horizontalHeaderItem(4);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Start of Contract", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = sponsorTable->horizontalHeaderItem(5);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "End of Contract", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = sponsorTable->horizontalHeaderItem(6);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        dealLabel->setText(QCoreApplication::translate("MainWindow", "Deal Page", nullptr));
        employeeLabel->setText(QCoreApplication::translate("MainWindow", "Employee Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
