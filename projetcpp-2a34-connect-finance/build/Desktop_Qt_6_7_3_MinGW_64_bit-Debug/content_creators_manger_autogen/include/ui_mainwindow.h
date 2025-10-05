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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QPushButton *sidebarButton;
    QPushButton *sidebarButton1;
    QPushButton *sidebarButton2;
    QPushButton *sidebarButton3;
    QPushButton *sidebarButton4;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget;
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
    QWidget *managementPage;
    QVBoxLayout *managementLayout;
    QLabel *managementLabel;
    QWidget *financePage;
    QVBoxLayout *financeLayout;
    QLabel *financeLabel;
    QWidget *sponsorPage;
    QVBoxLayout *sponsorLayout;
    QLabel *sponsorLabel;
    QWidget *dealPage;
    QVBoxLayout *dealLayout;
    QLabel *dealLabel;
    QWidget *employeePage;
    QVBoxLayout *employeeLayout;
    QLabel *employeeLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(788, 457);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"QMainWindow {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"}\n"
"QMainWindow[darkTheme=\"true\"] {\n"
"    background-color: #000000;\n"
"}\n"
"QGroupBox {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    font: bold 14pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"    margin: 10px;\n"
"}\n"
"QGroupBox[darkTheme=\"true\"] {\n"
"    background-color: #000000;\n"
"    border: 2px solid #7D4FEE;\n"
"    color: #7D4FEE;\n"
"}\n"
"QGroupBox::title {\n"
"    color: #7D4FEE;\n"
"    padding: 0 5px;\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"}\n"
"QPushButton {\n"
"    background-color: #7D4FEE;\n"
"    color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"    min-width: 100px;\n"
"    font: 10pt \"Arial\";\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #FFFFFF;\n"
"    color: #7D4FEE;\n"
"    border: 2px solid #7D4FEE;\n"
"}\n"
"QLineEdit, QCom"
                        "boBox, QSpinBox {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"QLineEdit[darkTheme=\"true\"], QComboBox[darkTheme=\"true\"], QSpinBox[darkTheme=\"true\"] {\n"
"    background-color: #000000;\n"
"    border: 2px solid #7D4FEE;\n"
"    color: #7D4FEE;\n"
"}\n"
"QTableWidget {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #7D4FEE;\n"
"    border-radius: 10px;\n"
"    gridline-color: #7D4FEE;\n"
"    font: 10pt \"Arial\";\n"
"    color: #7D4FEE;\n"
"}\n"
"QTableWidget[darkTheme=\"true\"] {\n"
"    background-color: #1A1A1A;\n"
"    border: 2px solid #7D4FEE;\n"
"    color: #7D4FEE;\n"
"}\n"
"QTableWidget::item:selected {\n"
"    background-color: #7D4FEE;\n"
"    color: #FFFFFF;\n"
"}\n"
"QLabel {\n"
"    color: #7D4FEE;\n"
"    font: 10pt \"Arial\";\n"
"}\n"
"QLabel[darkTheme=\"true\"] {\n"
"    color: #7D4FEE;\n"
"}\n"
"QWidget#sidebar {\n"
"    background-col"
                        "or: #C49FFF;\n"
"    border-radius: 10px 10px 10px 10px;\n"
"    \n"
"}\n"
"QPushButton#sidebarButton {\n"
"    background-color: #00000;\n"
"    color: #FFFFFF;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    font: 10pt \"Arial\";\n"
"    width: 100%;\n"
"    text-align: left;\n"
"}\n"
"QPushButton#sidebarButton:hover {\n"
"    background-color: #ffffffff;\n"
"    color: #7D4FEE;\n"
"    border: 2px solid #7D4FEE;}\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        sidebar = new QWidget(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setStyleSheet(QString::fromUtf8("background-color: #C49FFF;\n"
"border radius:10px;"));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarButton = new QPushButton(sidebar);
        sidebarButton->setObjectName("sidebarButton");
        sidebarButton->setStyleSheet(QString::fromUtf8(""));

        sidebarLayout->addWidget(sidebarButton);

        sidebarButton1 = new QPushButton(sidebar);
        sidebarButton1->setObjectName("sidebarButton1");
        sidebarButton1->setStyleSheet(QString::fromUtf8(""));

        sidebarLayout->addWidget(sidebarButton1);

        sidebarButton2 = new QPushButton(sidebar);
        sidebarButton2->setObjectName("sidebarButton2");
        sidebarButton2->setStyleSheet(QString::fromUtf8(""));

        sidebarLayout->addWidget(sidebarButton2);

        sidebarButton3 = new QPushButton(sidebar);
        sidebarButton3->setObjectName("sidebarButton3");
        sidebarButton3->setStyleSheet(QString::fromUtf8(""));

        sidebarLayout->addWidget(sidebarButton3);

        sidebarButton4 = new QPushButton(sidebar);
        sidebarButton4->setObjectName("sidebarButton4");
        sidebarButton4->setStyleSheet(QString::fromUtf8(""));

        sidebarLayout->addWidget(sidebarButton4);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        sidebarLayout->addItem(verticalSpacer);

        sidebarLayout->setStretch(0, 1);
        sidebarLayout->setStretch(1, 1);
        sidebarLayout->setStretch(2, 1);
        sidebarLayout->setStretch(3, 1);
        sidebarLayout->setStretch(4, 1);
        sidebarLayout->setStretch(5, 1);

        mainLayout->addWidget(sidebar);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy);
        stackedWidget->setStyleSheet(QString::fromUtf8(""));
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
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        creatorTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        creatorTable->setObjectName("creatorTable");
        sizePolicy.setHeightForWidth(creatorTable->sizePolicy().hasHeightForWidth());
        creatorTable->setSizePolicy(sizePolicy);

        contentCreatorLayout->addWidget(creatorTable);

        stackedWidget->addWidget(contentCreatorPage);
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
        financeLabel = new QLabel(financePage);
        financeLabel->setObjectName("financeLabel");
        financeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        financeLayout->addWidget(financeLabel);

        stackedWidget->addWidget(financePage);
        sponsorPage = new QWidget();
        sponsorPage->setObjectName("sponsorPage");
        sponsorLayout = new QVBoxLayout(sponsorPage);
        sponsorLayout->setObjectName("sponsorLayout");
        sponsorLabel = new QLabel(sponsorPage);
        sponsorLabel->setObjectName("sponsorLabel");
        sponsorLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sponsorLayout->addWidget(sponsorLabel);

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

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Content Creator Manager", nullptr));
        sidebarButton->setText(QCoreApplication::translate("MainWindow", "Finance", nullptr));
        sidebarButton1->setText(QCoreApplication::translate("MainWindow", "Content Creator", nullptr));
        sidebarButton2->setText(QCoreApplication::translate("MainWindow", "Sponsor", nullptr));
        sidebarButton3->setText(QCoreApplication::translate("MainWindow", "Deal", nullptr));
        sidebarButton4->setText(QCoreApplication::translate("MainWindow", "Employee", nullptr));
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
        QTableWidgetItem *___qtablewidgetitem = creatorTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = creatorTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Platform", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = creatorTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Subscribers", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = creatorTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Content Type", nullptr));
        managementLabel->setText(QCoreApplication::translate("MainWindow", "Management Page", nullptr));
        financeLabel->setText(QCoreApplication::translate("MainWindow", "Finance Page", nullptr));
        sponsorLabel->setText(QCoreApplication::translate("MainWindow", "Sponsor Page", nullptr));
        dealLabel->setText(QCoreApplication::translate("MainWindow", "Deal Page", nullptr));
        employeeLabel->setText(QCoreApplication::translate("MainWindow", "Employee Page", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
