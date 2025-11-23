#ifndef PREDICTIONENGINE_H
#define PREDICTIONENGINE_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include <QPair>
#include <QDate>

class PredictionEngine : public QObject
{
    Q_OBJECT

public:
    explicit PredictionEngine(QObject *parent = nullptr);

    struct PredictionResult {
        double predictedAmount;
        double confidence;
        QString trend; // "UP", "DOWN", "STABLE"
        QString recommendation;
        QDate predictionDate;
    };

    struct FinancialHealth {
        QString status; // "EXCELLENT", "GOOD", "WARNING", "CRITICAL"
        int score; // 0-100
        QString analysis;
        QVector<QString> recommendations;
    };

    PredictionResult predictNextMonthRevenue();
    QVector<QPair<QString, double>> getPaymentProbability();
    FinancialHealth analyzeFinancialHealth();
    QVector<QPair<QString, double>> getMonthlyTrends(int months = 6);
    double calculateCashFlowForecast();

private:
    QSqlDatabase getDatabase();
    double calculateMovingAverage(int months = 6);
    double calculateGrowthRate();
    double calculatePaymentProbability(const QString &invoiceId, double amount, const QDate &dueDate);
    QVector<double> getRevenueHistory(int months);
};

#endif // PREDICTIONENGINE_H
