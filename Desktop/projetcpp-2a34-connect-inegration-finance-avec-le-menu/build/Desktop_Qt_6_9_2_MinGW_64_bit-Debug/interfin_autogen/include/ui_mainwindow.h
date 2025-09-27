/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
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
    QFrame *menuFrame;
    QVBoxLayout *menuLayout;
    QPushButton *btnManagement;
    QPushButton *btnFinance;
    QPushButton *btnContentCreator;
    QPushButton *btnSponsor;
    QPushButton *btnDeal;
    QPushButton *btnEmployee;
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
    QLabel *labelContent;
    QWidget *sponsorPage;
    QVBoxLayout *sponsorLayout;
    QFrame *frame;
    QPushButton *updateButton_4;
    QPushButton *updateButton;
    QTableWidget *creatorTable;
    QLineEdit *searchEdit;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_6;
    QDateEdit *dateEdit;
    QDateEdit *dateEdit_2;
    QPushButton *pushButton;
    QLineEdit *lineEdit_10;
    QLabel *label_5;
    QWidget *tab_2;
    QLabel *label_11;
    QLabel *label_12;
    QDateEdit *dateEdit_3;
    QLineEdit *lineEdit_7;
    QLabel *label_13;
    QLineEdit *lineEdit_5;
    QLabel *label_14;
    QLineEdit *lineEdit_8;
    QPushButton *pushButton_2;
    QLabel *label_15;
    QLabel *label_16;
    QDateEdit *dateEdit_4;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_11;
    QLabel *label_18;
    QPushButton *updateButton_3;
    QPushButton *updateButton_2;
    QFrame *frame_2;
    QPushButton *updateButton_5;
    QPushButton *updateButton_6;
    QFrame *frame_3;
    QWidget *employeePage;
    QVBoxLayout *employeeLayout;
    QLabel *employeeLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1100, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        menuFrame = new QFrame(centralwidget);
        menuFrame->setObjectName("menuFrame");
        menuLayout = new QVBoxLayout(menuFrame);
        menuLayout->setObjectName("menuLayout");
        btnManagement = new QPushButton(menuFrame);
        btnManagement->setObjectName("btnManagement");

        menuLayout->addWidget(btnManagement);

        btnFinance = new QPushButton(menuFrame);
        btnFinance->setObjectName("btnFinance");

        menuLayout->addWidget(btnFinance);

        btnContentCreator = new QPushButton(menuFrame);
        btnContentCreator->setObjectName("btnContentCreator");

        menuLayout->addWidget(btnContentCreator);

        btnSponsor = new QPushButton(menuFrame);
        btnSponsor->setObjectName("btnSponsor");

        menuLayout->addWidget(btnSponsor);

        btnDeal = new QPushButton(menuFrame);
        btnDeal->setObjectName("btnDeal");

        menuLayout->addWidget(btnDeal);

        btnEmployee = new QPushButton(menuFrame);
        btnEmployee->setObjectName("btnEmployee");

        menuLayout->addWidget(btnEmployee);


        mainLayout->addWidget(menuFrame);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
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


        financeLayout->addLayout(topBar);

        formBox = new QGroupBox(financePage);
        formBox->setObjectName("formBox");
        formLayout = new QFormLayout(formBox);
        formLayout->setObjectName("formLayout");
        labelId = new QLabel(formBox);
        labelId->setObjectName("labelId");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelId);

        lineId = new QLineEdit(formBox);
        lineId->setObjectName("lineId");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lineId);

        labelMontant = new QLabel(formBox);
        labelMontant->setObjectName("labelMontant");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelMontant);

        lineMontant = new QLineEdit(formBox);
        lineMontant->setObjectName("lineMontant");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, lineMontant);

        labelDateEmission = new QLabel(formBox);
        labelDateEmission->setObjectName("labelDateEmission");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, labelDateEmission);

        dateEmission = new QDateEdit(formBox);
        dateEmission->setObjectName("dateEmission");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, dateEmission);

        labelDateEcheance = new QLabel(formBox);
        labelDateEcheance->setObjectName("labelDateEcheance");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, labelDateEcheance);

        dateEcheance = new QDateEdit(formBox);
        dateEcheance->setObjectName("dateEcheance");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, dateEcheance);

        labelStatut = new QLabel(formBox);
        labelStatut->setObjectName("labelStatut");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, labelStatut);

        comboStatut = new QComboBox(formBox);
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->setObjectName("comboStatut");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, comboStatut);


        financeLayout->addWidget(formBox);

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

        financeLayout->addWidget(tableFactures);

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
        sponsorLayout = new QVBoxLayout(sponsorPage);
        sponsorLayout->setObjectName("sponsorLayout");
        frame = new QFrame(sponsorPage);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        updateButton_4 = new QPushButton(frame);
        updateButton_4->setObjectName("updateButton_4");
        updateButton_4->setGeometry(QRect(710, 20, 91, 21));
        updateButton = new QPushButton(frame);
        updateButton->setObjectName("updateButton");
        updateButton->setGeometry(QRect(710, 340, 111, 21));
        creatorTable = new QTableWidget(frame);
        if (creatorTable->columnCount() < 7)
            creatorTable->setColumnCount(7);
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::BrushStyle::NoBrush);
        QFont font;
        font.setBold(true);
        font.setItalic(false);
        font.setUnderline(false);
        font.setStrikeOut(false);
        font.setKerning(false);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font);
        __qtablewidgetitem5->setBackground(QColor(255, 255, 255));
        __qtablewidgetitem5->setForeground(brush);
        creatorTable->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(5, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(6, __qtablewidgetitem11);
        creatorTable->setObjectName("creatorTable");
        creatorTable->setGeometry(QRect(440, 50, 531, 281));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(creatorTable->sizePolicy().hasHeightForWidth());
        creatorTable->setSizePolicy(sizePolicy);
        creatorTable->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 101, 255);\n"
"background-color: rgb(255, 255, 255);\n"
"QHeaderView::section {\n"
"    background-color: white;\n"
"    color: black;   /* texte en noir pour qu\342\200\231il reste lisible */\n"
"    font-weight: bold;\n"
"    border: 1px solid #dcdcdc; /* optionnel, pour s\303\251parer les colonnes */\n"
"}\n"
""));
        searchEdit = new QLineEdit(frame);
        searchEdit->setObjectName("searchEdit");
        searchEdit->setGeometry(QRect(450, 20, 251, 21));
        tabWidget = new QTabWidget(frame);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(10, 30, 421, 301));
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(125, 79, 254);\n"
"QTabWidget::pane {\n"
"    border: 2px solid white;    /* Bordure blanche autour de la zone de contenu */\n"
"    background: #f0f0f0;        /* Fond clair (\303\240 adapter) */\n"
"}\n"
"\n"
""));
        tab = new QWidget();
        tab->setObjectName("tab");
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 40, 141, 16));
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 70, 61, 16));
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 100, 37, 12));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_4 = new QLabel(tab);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 130, 201, 20));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_9 = new QLabel(tab);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(10, 160, 121, 20));
        label_9->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_10 = new QLabel(tab);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(10, 190, 121, 20));
        label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(120, 40, 191, 20));
        lineEdit->setStyleSheet(QString::fromUtf8("alternate-background-color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(120, 70, 191, 20));
        lineEdit_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_3 = new QLineEdit(tab);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(120, 100, 191, 20));
        lineEdit_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_6 = new QLineEdit(tab);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(120, 190, 191, 20));
        lineEdit_6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        dateEdit = new QDateEdit(tab);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(120, 130, 191, 22));
        dateEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        dateEdit_2 = new QDateEdit(tab);
        dateEdit_2->setObjectName("dateEdit_2");
        dateEdit_2->setGeometry(QRect(120, 160, 191, 22));
        dateEdit_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(160, 220, 101, 41));
        lineEdit_10 = new QLineEdit(tab);
        lineEdit_10->setObjectName("lineEdit_10");
        lineEdit_10->setGeometry(QRect(120, 10, 191, 20));
        lineEdit_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(255, 255, 255);"));
        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 10, 16, 16));
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_11 = new QLabel(tab_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(10, 193, 81, 16));
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(10, 73, 71, 16));
        label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        dateEdit_3 = new QDateEdit(tab_2);
        dateEdit_3->setObjectName("dateEdit_3");
        dateEdit_3->setGeometry(QRect(120, 163, 191, 22));
        dateEdit_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_7 = new QLineEdit(tab_2);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(120, 193, 191, 20));
        lineEdit_7->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_13 = new QLabel(tab_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(10, 43, 101, 16));
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_5 = new QLineEdit(tab_2);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(120, 43, 191, 20));
        lineEdit_5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(10, 163, 101, 16));
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_8 = new QLineEdit(tab_2);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(120, 103, 191, 20));
        lineEdit_8->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(140, 220, 131, 41));
        pushButton_2->setStyleSheet(QString::fromUtf8("button {\n"
"	background-color: rgb(255, 255, 255);\n"
"  background-color: white;\n"
"  color: black;\n"
"  border: 1px solid #ccc;\n"
"}\n"
""));
        label_15 = new QLabel(tab_2);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 103, 37, 12));
        label_15->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_16 = new QLabel(tab_2);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(10, 133, 101, 16));
        label_16->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"QPushButton* button = new QPushButton(\"Mon Bouton\");\n"
"button->setStyleSheet(\"background-color: white; color: black; border: 1px solid gray;\");\n"
""));
        dateEdit_4 = new QDateEdit(tab_2);
        dateEdit_4->setObjectName("dateEdit_4");
        dateEdit_4->setGeometry(QRect(120, 133, 191, 22));
        dateEdit_4->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_9 = new QLineEdit(tab_2);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(120, 73, 191, 20));
        lineEdit_9->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_11 = new QLineEdit(tab_2);
        lineEdit_11->setObjectName("lineEdit_11");
        lineEdit_11->setGeometry(QRect(120, 10, 191, 20));
        lineEdit_11->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_18 = new QLabel(tab_2);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(10, 10, 101, 16));
        label_18->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        tabWidget->addTab(tab_2, QString());
        updateButton_3 = new QPushButton(frame);
        updateButton_3->setObjectName("updateButton_3");
        updateButton_3->setGeometry(QRect(450, 340, 111, 21));
        updateButton_2 = new QPushButton(frame);
        updateButton_2->setObjectName("updateButton_2");
        updateButton_2->setGeometry(QRect(580, 340, 111, 21));
        frame_2 = new QFrame(frame);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(759, 430, 181, 131));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        updateButton_5 = new QPushButton(frame);
        updateButton_5->setObjectName("updateButton_5");
        updateButton_5->setGeometry(QRect(810, 400, 51, 21));
        updateButton_6 = new QPushButton(frame);
        updateButton_6->setObjectName("updateButton_6");
        updateButton_6->setGeometry(QRect(491, 400, 51, 21));
        frame_3 = new QFrame(frame);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(440, 430, 181, 131));
        frame_3->setFrameShape(QFrame::Shape::StyledPanel);
        frame_3->setFrameShadow(QFrame::Shadow::Raised);

        sponsorLayout->addWidget(frame);

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

        mainLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(3);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Project Manager", nullptr));
        btnManagement->setText(QCoreApplication::translate("MainWindow", "Management", nullptr));
        btnFinance->setText(QCoreApplication::translate("MainWindow", "Finance", nullptr));
        btnContentCreator->setText(QCoreApplication::translate("MainWindow", "Content Creator", nullptr));
        btnSponsor->setText(QCoreApplication::translate("MainWindow", "Sponsor", nullptr));
        btnDeal->setText(QCoreApplication::translate("MainWindow", "Deal", nullptr));
        btnEmployee->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
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
        labelContent->setText(QCoreApplication::translate("MainWindow", "Content Creator Page", nullptr));
        updateButton_4->setText(QCoreApplication::translate("MainWindow", "serch", nullptr));
        updateButton->setText(QCoreApplication::translate("MainWindow", "delete", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = creatorTable->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "id", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = creatorTable->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", " sponsor name", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = creatorTable->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "category", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = creatorTable->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "email", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = creatorTable->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "start of contract", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = creatorTable->horizontalHeaderItem(5);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "end of contract", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = creatorTable->horizontalHeaderItem(6);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "budget", nullptr));
        searchEdit->setText(QCoreApplication::translate("MainWindow", "serch...", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search by name or platform", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "sponsor name", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "category", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "start of contract", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "end of contract", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "add", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\342\200\215ID", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "add", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Budget", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "category", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "sponsor name", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "end of contract", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "to modify", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "start of contract", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "\342\200\215ID", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "to modify", nullptr));
        updateButton_3->setText(QCoreApplication::translate("MainWindow", "sort by name", nullptr));
        updateButton_2->setText(QCoreApplication::translate("MainWindow", "pdf", nullptr));
        updateButton_5->setText(QCoreApplication::translate("MainWindow", "Stat", nullptr));
        updateButton_6->setText(QCoreApplication::translate("MainWindow", "QR code", nullptr));
        employeeLabel->setText(QCoreApplication::translate("MainWindow", "Employee Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
