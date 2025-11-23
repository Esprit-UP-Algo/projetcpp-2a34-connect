#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QTimer>
#include <QVector>
#include <QDateTime>

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    explicit NotificationManager(QObject *parent = nullptr);
    ~NotificationManager();

    void checkOverdueInvoices();
    void createNotification(const QString &invoiceId, const QString &type, const QString &message);
    void markAsRead(int notificationId);
    void markAllAsRead();
    QVector<QString> getUnreadNotifications();
    int getUnreadCount();
    void checkUpcomingDueDates();

signals:
    void notificationsUpdated();
    void newNotificationCreated(const QString &message);

private slots:
    void onDailyCheck();
    void onHourlyCheck();

private:
    QSqlDatabase getDatabase();
    QTimer *dailyTimer;
    QTimer *hourlyTimer;

    void checkInvoiceStatusChanges();
    void createOverdueNotification(const QString &invoiceId, double amount, int daysOverdue);
    void createDueSoonNotification(const QString &invoiceId, double amount, const QDate &dueDate);
};

#endif // NOTIFICATIONMANAGER_H
