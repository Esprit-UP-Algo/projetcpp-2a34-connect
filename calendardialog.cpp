#include "calendardialog.h"
#include "ui_calendardialog.h"
#include <QSqlQuery>
#include <QTextCharFormat>

#include <QSqlError>
#include <QDebug>

CalendarDialog::CalendarDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalendarDialog)
{
    ui->setupUi(this);

    loadData();
    applyColors();
}

CalendarDialog::~CalendarDialog()
{
    delete ui;
}

void CalendarDialog::loadData()
{
    bookingCount.clear();

    QSqlQuery query;
    QString sql = "SELECT date_time FROM booking ORDER BY date_time ASC";

    if (!query.exec(sql)) {
        qDebug() << "SQL ERROR loading calendar dates:" << query.lastError();
        return;
    }

    while (query.next()) {
        QString dateString = query.value(0).toString();

        // Convert to QDate
        QDate date = QDate::fromString(dateString.left(10), "yyyy-MM-dd");

        if (!date.isValid()) {
            qDebug() << "Invalid date in DB:" << dateString;
            continue;
        }

        bookingCount[date] += 1;
    }

    applyColors();
}


void CalendarDialog::applyColors()
{
    for (auto it = bookingCount.begin(); it != bookingCount.end(); ++it) {

        QDate date = it.key();
        int count = it.value();

        QTextCharFormat fmt;

        if (count == 1) {
            fmt.setBackground(QColor("#4CAF50")); // green
            fmt.setForeground(Qt::white);
        }
        else if (count >= 2) {
            fmt.setBackground(QColor("#F44336")); // red
            fmt.setForeground(Qt::white);
        }

        ui->calendarWidget->setDateTextFormat(date, fmt);
    }
}


