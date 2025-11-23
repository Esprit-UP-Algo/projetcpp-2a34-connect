#include "notificationmanager.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QTime>

NotificationManager::NotificationManager(QObject *parent) : QObject(parent)
{
    // Daily timer for complete checks
    dailyTimer = new QTimer(this);
    connect(dailyTimer, &QTimer::timeout, this, &NotificationManager::onDailyCheck);
    dailyTimer->start(24 * 60 * 60 * 1000); // 24 hours

    // Hourly timer for urgent checks
    hourlyTimer = new QTimer(this);
    connect(hourlyTimer, &QTimer::timeout, this, &NotificationManager::onHourlyCheck);
    hourlyTimer->start(60 * 60 * 1000); // 1 hour

    // First immediate check (2 second delay to let the application start)
    QTimer::singleShot(2000, this, &NotificationManager::onDailyCheck);
    QTimer::singleShot(5000, this, &NotificationManager::onHourlyCheck);

    qDebug() << "🔔 NotificationManager started with daily and hourly timers";
}

NotificationManager::~NotificationManager()
{
    if(dailyTimer && dailyTimer->isActive()) {
        dailyTimer->stop();
    }
    if(hourlyTimer && hourlyTimer->isActive()) {
        hourlyTimer->stop();
    }
    delete dailyTimer;
    delete hourlyTimer;
}

QSqlDatabase NotificationManager::getDatabase()
{
    return Connection::getInstance().getDatabase();
}

void NotificationManager::onDailyCheck()
{
    qDebug() << "🔔 Daily invoice check triggered...";
    checkOverdueInvoices();
    checkInvoiceStatusChanges();
}

void NotificationManager::onHourlyCheck()
{
    qDebug() << "🔔 Hourly due date check...";
    checkUpcomingDueDates();
}

void NotificationManager::checkOverdueInvoices()
{
    QSqlDatabase db = getDatabase();
    if(!db.isOpen()) {
        qDebug() << "❌ Database not open for overdue check";
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id_facture, montant, date_echeance, statut FROM Facture WHERE statut = 'Pending'");

    if (!query.exec()) {
        qDebug() << "❌ Error checking overdue invoices:" << query.lastError().text();
        return;
    }

    QDate today = QDate::currentDate();
    int notificationsCreated = 0;

    while (query.next()) {
        QString invoiceId = query.value(0).toString();
        double amount = query.value(1).toDouble();
        QDate dueDate = query.value(2).toDate();
        int daysOverdue = dueDate.daysTo(today);

        if (daysOverdue > 0) {
            createOverdueNotification(invoiceId, amount, daysOverdue);
            notificationsCreated++;
        }
    }

    if(notificationsCreated > 0) {
        qDebug() << "🔔 Created" << notificationsCreated << "overdue notifications";
        emit notificationsUpdated();
    }
}

void NotificationManager::checkUpcomingDueDates()
{
    QSqlDatabase db = getDatabase();
    if(!db.isOpen()) return;

    QSqlQuery query(db);
    query.prepare("SELECT id_facture, montant, date_echeance FROM Facture "
                  "WHERE statut = 'Pending' AND date_echeance BETWEEN date('now') AND date('now', '+7 days')");

    if (!query.exec()) {
        qDebug() << "❌ Error checking upcoming due dates:" << query.lastError().text();
        return;
    }

    QDate today = QDate::currentDate();
    int notificationsCreated = 0;

    while (query.next()) {
        QString invoiceId = query.value(0).toString();
        double amount = query.value(1).toDouble();
        QDate dueDate = query.value(2).toDate();
        int daysUntilDue = today.daysTo(dueDate);

        if (daysUntilDue >= 0 && daysUntilDue <= 7) {
            createDueSoonNotification(invoiceId, amount, dueDate);
            notificationsCreated++;
        }
    }

    if(notificationsCreated > 0) {
        qDebug() << "🔔 Created" << notificationsCreated << "upcoming due date notifications";
        emit notificationsUpdated();
    }
}

void NotificationManager::createOverdueNotification(const QString &invoiceId, double amount, int daysOverdue)
{
    QString message;
    QString alertType;

    if (daysOverdue == 1) {
        alertType = "Overdue";
        message = QString("Invoice #%1 was due yesterday - Amount: %2 €")
                      .arg(invoiceId)
                      .arg(amount, 0, 'f', 2);
    } else if (daysOverdue <= 3) {
        alertType = "Slight delay";
        message = QString("Invoice #%1 overdue by %2 days - Amount: %3 €")
                      .arg(invoiceId)
                      .arg(daysOverdue)
                      .arg(amount, 0, 'f', 2);
    } else if (daysOverdue <= 7) {
        alertType = "Moderate delay";
        message = QString("ALERT - Invoice #%1 unpaid for %2 days - Amount: %3 €")
                      .arg(invoiceId)
                      .arg(daysOverdue)
                      .arg(amount, 0, 'f', 2);
    } else if (daysOverdue <= 15) {
        alertType = "Significant delay";
        message = QString("URGENT - Invoice #%1 unpaid for %2 days - Contact the client")
                      .arg(invoiceId)
                      .arg(daysOverdue);
    } else {
        alertType = "CRITICAL";
        message = QString("🚨 CRITICAL - Invoice #%1 - %2 days overdue - Legal risk")
                      .arg(invoiceId)
                      .arg(daysOverdue);
    }

    createNotification(invoiceId, alertType, message);
}

void NotificationManager::createDueSoonNotification(const QString &invoiceId, double amount, const QDate &dueDate)
{
    QDate today = QDate::currentDate();
    int daysUntilDue = today.daysTo(dueDate);

    QString alertType;
    QString message;

    if (daysUntilDue == 0) {
        alertType = "Due today";
        message = QString("⚠️ Invoice #%1 is due today - Amount: %2 €")
                      .arg(invoiceId)
                      .arg(amount, 0, 'f', 2);
    } else if (daysUntilDue == 1) {
        alertType = "Due tomorrow";
        message = QString("📅 Invoice #%1 is due tomorrow - Amount: %2 €")
                      .arg(invoiceId)
                      .arg(amount, 0, 'f', 2);
    } else {
        alertType = "Due soon";
        message = QString("📋 Invoice #%1 due in %2 days - Amount: %3 €")
                      .arg(invoiceId)
                      .arg(daysUntilDue)
                      .arg(amount, 0, 'f', 2);
    }

    createNotification(invoiceId, alertType, message);
}

void NotificationManager::checkInvoiceStatusChanges()
{
    // This method can be extended to detect status changes
    // and create appropriate notifications
    qDebug() << "🔔 Checking status changes...";
}

void NotificationManager::createNotification(const QString &invoiceId, const QString &type, const QString &message)
{
    QSqlDatabase db = getDatabase();
    if(!db.isOpen()) {
        qDebug() << "❌ Database not open for notification creation";
        return;
    }

    // Check if a similar notification already exists today
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM Notifications WHERE id_facture = ? AND type_alerte = ? AND DATE(date_creation) = DATE('now')");
    checkQuery.addBindValue(invoiceId);
    checkQuery.addBindValue(type);

    if (checkQuery.exec() && checkQuery.next()) {
        if (checkQuery.value(0).toInt() > 0) {
            qDebug() << "🔔 Notification already created today for" << invoiceId << "-" << type;
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO Notifications (id_facture, type_alerte, message, date_creation, statut) "
                  "VALUES (?, ?, ?, datetime('now'), 'Unread')");
    query.addBindValue(invoiceId);
    query.addBindValue(type);
    query.addBindValue(message);

    if (query.exec()) {
        qDebug() << "✅ Notification created:" << type << "-" << message;
        emit notificationsUpdated();
        emit newNotificationCreated(message);
    } else {
        qDebug() << "❌ Error creating notification:" << query.lastError().text();
    }
}

void NotificationManager::markAsRead(int notificationId)
{
    QSqlDatabase db = getDatabase();
    if(!db.isOpen()) return;

    QSqlQuery query(db);
    query.prepare("UPDATE Notifications SET statut = 'Read' WHERE id_notification = ?");
    query.addBindValue(notificationId);

    if(query.exec()) {
        qDebug() << "✅ Notification" << notificationId << "marked as read";
        emit notificationsUpdated();
    } else {
        qDebug() << "❌ Error marking notification as read:" << query.lastError().text();
    }
}

void NotificationManager::markAllAsRead()
{
    QSqlDatabase db = getDatabase();
    if(!db.isOpen()) return;

    QSqlQuery query(db);
    query.prepare("UPDATE Notifications SET statut = 'Read' WHERE statut = 'Unread'");

    if(query.exec()) {
        int affected = query.numRowsAffected();
        qDebug() << "✅" << affected << "notifications marked as read";
        emit notificationsUpdated();
    } else {
        qDebug() << "❌ Error marking all notifications as read:" << query.lastError().text();
    }
}

QVector<QString> NotificationManager::getUnreadNotifications()
{
    QVector<QString> notifications;
    QSqlDatabase db = getDatabase();
    if(!db.isOpen()) return notifications;

    QSqlQuery query(db);
    query.prepare("SELECT message FROM Notifications WHERE statut = 'Unread' ORDER BY date_creation DESC");

    if(query.exec()) {
        while (query.next()) {
            notifications.append(query.value(0).toString());
        }
    }
    return notifications;
}

int NotificationManager::getUnreadCount()
{
    QSqlDatabase db = getDatabase();
    if(!db.isOpen()) return 0;

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM Notifications WHERE statut = 'Unread'");

    if(query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
