#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H

#include <QDialog>
#include <QMap>
#include <QDate>

namespace Ui {
class CalendarDialog;
}

class CalendarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalendarDialog(QWidget *parent = nullptr);
    ~CalendarDialog();

private:
    Ui::CalendarDialog *ui;

    // Stores how many bookings exist for each date
    QMap<QDate, int> bookingCount;

    void loadData();
    void applyColors();
};

#endif // CALENDARDIALOG_H
