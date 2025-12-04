/********************************************************************************
** Form generated from reading UI file 'calendardialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALENDARDIALOG_H
#define UI_CALENDARDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CalendarDialog
{
public:
    QCalendarWidget *calendarWidget;
    QLabel *calendertitle;
    QPushButton *btnClose;

    void setupUi(QDialog *CalendarDialog)
    {
        if (CalendarDialog->objectName().isEmpty())
            CalendarDialog->setObjectName("CalendarDialog");
        CalendarDialog->resize(546, 406);
        calendarWidget = new QCalendarWidget(CalendarDialog);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(0, 60, 541, 291));
        calendarWidget->setStyleSheet(QString::fromUtf8("QCalendarWidget QToolButton {\n"
"    color: white;\n"
"    background: #4a148c;\n"
"    border-radius: 8px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QCalendarWidget QWidget#qt_calendar_navigationbar {\n"
"    background: #4a148c;\n"
"}\n"
"\n"
"QCalendarWidget QSpinBox {\n"
"    color: white;\n"
"    background: #4a148c;\n"
"    border: none;\n"
"}\n"
"\n"
"QCalendarWidget QAbstractItemView {\n"
"    selection-background-color: transparent;\n"
"}\n"
""));
        calendertitle = new QLabel(CalendarDialog);
        calendertitle->setObjectName("calendertitle");
        calendertitle->setGeometry(QRect(140, 10, 241, 41));
        calendertitle->setStyleSheet(QString::fromUtf8("  font-size: 16px;\n"
"    font-weight: bold;\n"
"    color: #2c3e50;          /* dark gray-blue */\n"
"        background-color: rgb(255, 255, 255);\n"
"    padding: 6px 12px;\n"
"        qproperty-alignment: 'AlignCenter';"));
        btnClose = new QPushButton(CalendarDialog);
        btnClose->setObjectName("btnClose");
        btnClose->setGeometry(QRect(210, 360, 90, 29));
        btnClose->setStyleSheet(QString::fromUtf8(" background-color: #7D4FEE;   /* light violet */\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 12px;         /* rounded edges */\n"
"    padding:  5px 8px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;"));

        retranslateUi(CalendarDialog);

        QMetaObject::connectSlotsByName(CalendarDialog);
    } // setupUi

    void retranslateUi(QDialog *CalendarDialog)
    {
        CalendarDialog->setWindowTitle(QCoreApplication::translate("CalendarDialog", "Dialog", nullptr));
        calendertitle->setText(QCoreApplication::translate("CalendarDialog", "Booking Calendar Overview", nullptr));
        btnClose->setText(QCoreApplication::translate("CalendarDialog", "close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalendarDialog: public Ui_CalendarDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALENDARDIALOG_H
