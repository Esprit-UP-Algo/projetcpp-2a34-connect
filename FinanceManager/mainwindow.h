#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include "notificationmanager.h"
#include "predictionengine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showManagementPage();
    void showFinancePage();
    void showContentCreatorPage();
    void showSponsorPage();
    void showEmployeePage();
    void showNotificationsPage();

    void addInvoice();
    void editInvoice();
    void deleteInvoice();
    void clearForm();
    void sortByAmount();
    void searchById();
    void exportToCSV();
    void toggleDarkTheme();
    void onInvoiceSelected();

    void updateNotificationBadge();
    void markAllAsRead();
    void onNotificationClicked();
    void updateCharts();
    void showAnalyticsPage();

private:
    Ui::MainWindow *ui;
    bool darkTheme = false;
    NotificationManager *notificationManager;
    PredictionEngine *predictionEngine;

    QChart *statusChart;
    QChart *trendChart;
    QChartView *statusChartView;
    QChartView *trendChartView;

    void insertInvoiceInTable(QString id, double amount, QDate issueDate, QDate dueDate, QString status);
    void loadInvoicesFromDatabase();
    void initializeVisualStatistics();
    void updateVisualStatistics();
    void applyLightTheme();
    void applyDarkTheme();
    void loadNotifications();
    void createPaymentNotification(const QString &invoiceId, double amount, const QDate &paymentDate, const QDate &nextDue);
    void createCancellationNotification(const QString &invoiceId, double amount, const QString &sponsor, const QString &reason);

    void initializeCharts();
    void createStatusPieChart();
    void createTrendLineChart();
    void updateAnalytics();
};

#endif // MAINWINDOW_H
