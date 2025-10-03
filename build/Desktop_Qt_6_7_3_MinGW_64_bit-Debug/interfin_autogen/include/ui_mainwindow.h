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
#include <QtWidgets/QPushButton>
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
    QVBoxLayout *contentLayout;
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
        btnManagement->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        menuLayout->addWidget(btnManagement);

        btnFinance = new QPushButton(menuFrame);
        btnFinance->setObjectName("btnFinance");
        btnFinance->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        menuLayout->addWidget(btnFinance);

        btnContentCreator = new QPushButton(menuFrame);
        btnContentCreator->setObjectName("btnContentCreator");
        btnContentCreator->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        menuLayout->addWidget(btnContentCreator);

        btnSponsor = new QPushButton(menuFrame);
        btnSponsor->setObjectName("btnSponsor");
        btnSponsor->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        menuLayout->addWidget(btnSponsor);

        btnDeal = new QPushButton(menuFrame);
        btnDeal->setObjectName("btnDeal");
        btnDeal->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        menuLayout->addWidget(btnDeal);

        btnEmployee = new QPushButton(menuFrame);
        btnEmployee->setObjectName("btnEmployee");
        btnEmployee->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        menuLayout->addWidget(btnEmployee);


        mainLayout->addWidget(menuFrame);

        contentLayout = new QVBoxLayout();
        contentLayout->setObjectName("contentLayout");
        topBar = new QHBoxLayout();
        topBar->setObjectName("topBar");
        searchBox = new QLineEdit(centralwidget);
        searchBox->setObjectName("searchBox");
        searchBox->setStyleSheet(QString::fromUtf8("  border: 1px solid #ccc;\n"
"        border-radius: 5px;\n"
"        padding: 4px;\n"
"        background: #f9f9f9;"));

        topBar->addWidget(searchBox);

        btnSearchById = new QPushButton(centralwidget);
        btnSearchById->setObjectName("btnSearchById");
        btnSearchById->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        topBar->addWidget(btnSearchById);

        btnDarkTheme = new QPushButton(centralwidget);
        btnDarkTheme->setObjectName("btnDarkTheme");
        btnDarkTheme->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        topBar->addWidget(btnDarkTheme);

        btnSortByMontant = new QPushButton(centralwidget);
        btnSortByMontant->setObjectName("btnSortByMontant");
        btnSortByMontant->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        topBar->addWidget(btnSortByMontant);

        btnExportCSV = new QPushButton(centralwidget);
        btnExportCSV->setObjectName("btnExportCSV");
        btnExportCSV->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        topBar->addWidget(btnExportCSV);


        contentLayout->addLayout(topBar);

        formBox = new QGroupBox(centralwidget);
        formBox->setObjectName("formBox");
        formBox->setStyleSheet(QString::fromUtf8("font-weight: bold;\n"
"        border: 1px solid #aaa;\n"
"        border-radius: 6px;\n"
"        margin-top: 10px;"));
        formLayout = new QFormLayout(formBox);
        formLayout->setObjectName("formLayout");
        labelId = new QLabel(formBox);
        labelId->setObjectName("labelId");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelId);

        lineId = new QLineEdit(formBox);
        lineId->setObjectName("lineId");
        lineId->setStyleSheet(QString::fromUtf8("  border: 1px solid #ccc;\n"
"        border-radius: 5px;\n"
"        padding: 4px;\n"
"        background: #f9f9f9;"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineId);

        labelMontant = new QLabel(formBox);
        labelMontant->setObjectName("labelMontant");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelMontant);

        lineMontant = new QLineEdit(formBox);
        lineMontant->setObjectName("lineMontant");
        lineMontant->setStyleSheet(QString::fromUtf8("  border: 1px solid #ccc;\n"
"        border-radius: 5px;\n"
"        padding: 4px;\n"
"        background: #f9f9f9;"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineMontant);

        labelDateEmission = new QLabel(formBox);
        labelDateEmission->setObjectName("labelDateEmission");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelDateEmission);

        dateEmission = new QDateEdit(formBox);
        dateEmission->setObjectName("dateEmission");
        dateEmission->setStyleSheet(QString::fromUtf8("  border: 1px solid #ccc;\n"
"        border-radius: 5px;\n"
"        padding: 4px;\n"
"        background: #f9f9f9;"));

        formLayout->setWidget(2, QFormLayout::FieldRole, dateEmission);

        labelDateEcheance = new QLabel(formBox);
        labelDateEcheance->setObjectName("labelDateEcheance");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelDateEcheance);

        dateEcheance = new QDateEdit(formBox);
        dateEcheance->setObjectName("dateEcheance");
        dateEcheance->setStyleSheet(QString::fromUtf8("  border: 1px solid #ccc;\n"
"        border-radius: 5px;\n"
"        padding: 4px;\n"
"        background: #f9f9f9;"));

        formLayout->setWidget(3, QFormLayout::FieldRole, dateEcheance);

        labelStatut = new QLabel(formBox);
        labelStatut->setObjectName("labelStatut");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelStatut);

        comboStatut = new QComboBox(formBox);
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->setObjectName("comboStatut");
        comboStatut->setStyleSheet(QString::fromUtf8("  border: 1px solid #ccc;\n"
"        border-radius: 5px;\n"
"        padding: 4px;\n"
"        background: #f9f9f9;"));

        formLayout->setWidget(4, QFormLayout::FieldRole, comboStatut);


        contentLayout->addWidget(formBox);

        actionLayout = new QHBoxLayout();
        actionLayout->setObjectName("actionLayout");
        btnAdd = new QPushButton(centralwidget);
        btnAdd->setObjectName("btnAdd");
        btnAdd->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        actionLayout->addWidget(btnAdd);

        btnEdit = new QPushButton(centralwidget);
        btnEdit->setObjectName("btnEdit");
        btnEdit->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        actionLayout->addWidget(btnEdit);

        btnDelete = new QPushButton(centralwidget);
        btnDelete->setObjectName("btnDelete");
        btnDelete->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        actionLayout->addWidget(btnDelete);

        btnClear = new QPushButton(centralwidget);
        btnClear->setObjectName("btnClear");
        btnClear->setStyleSheet(QString::fromUtf8("background-color: #6c63ff;\n"
"        color: white;\n"
"        border-radius: 8px;\n"
"        padding: 6px 12px;\n"
"        font-weight: bold;"));

        actionLayout->addWidget(btnClear);


        contentLayout->addLayout(actionLayout);

        tableFactures = new QTableWidget(centralwidget);
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
        tableFactures->setStyleSheet(QString::fromUtf8(" border: 1px solid #ccc;\n"
"        gridline-color: #ddd;\n"
"        selection-background-color: #6c63ff;\n"
"        selection-color: white;"));

        contentLayout->addWidget(tableFactures);


        mainLayout->addLayout(contentLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Finance Management", nullptr));
        btnManagement->setText(QCoreApplication::translate("MainWindow", "Management", nullptr));
        btnFinance->setText(QCoreApplication::translate("MainWindow", "Finance", nullptr));
        btnContentCreator->setText(QCoreApplication::translate("MainWindow", "Content Creator", nullptr));
        btnSponsor->setText(QCoreApplication::translate("MainWindow", "Sponsor", nullptr));
        btnDeal->setText(QCoreApplication::translate("MainWindow", "Deal", nullptr));
        btnEmployee->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
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
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
