#include "predictionengine.h"
#include "connection.h"
#include <QSqlQuery>
#include <QDebug>
#include <QtMath>
#include <QDate>

PredictionEngine::PredictionEngine(QObject *parent) : QObject(parent)
{
}

QSqlDatabase PredictionEngine::getDatabase()
{
    return Connection::getInstance().getDatabase();
}

PredictionEngine::PredictionResult PredictionEngine::predictNextMonthRevenue()
{
    PredictionResult result;
    result.predictionDate = QDate::currentDate().addMonths(1);

    QVector<double> revenues = getRevenueHistory(6); // 6 derniers mois

    if(revenues.size() >= 3) {
        // Régression linéaire simple
        double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
        int n = revenues.size();

        for(int i = 0; i < n; i++) {
            sumX += i;
            sumY += revenues[i];
            sumXY += i * revenues[i];
            sumX2 += i * i;
        }

        double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
        double intercept = (sumY - slope * sumX) / n;

        result.predictedAmount = intercept + slope * n;
        result.confidence = qMax(0.0, qMin(1.0, 0.8 - qAbs(slope)/2000.0));

        // Déterminer la tendance
        if(slope > 500) {
            result.trend = "UP";
            result.recommendation = "📈 Croissance forte - Excellente performance ! Poursuivez votre stratégie actuelle.";
        } else if(slope > 100) {
            result.trend = "UP";
            result.recommendation = "📈 Croissance positive - Votre activité se développe bien.";
        } else if(slope < -500) {
            result.trend = "DOWN";
            result.recommendation = "📉 Baisse significative - Analysez les causes et ajustez votre stratégie.";
        } else if(slope < -100) {
            result.trend = "DOWN";
            result.recommendation = "📉 Légère baisse - Surveillez vos indicateurs et renforcez la relance clients.";
        } else {
            result.trend = "STABLE";
            result.recommendation = "➡️ Stabilité - Performance constante. Optimisez vos processus pour améliorer la croissance.";
        }

    } else {
        // Pas assez de données historiques
        result.predictedAmount = revenues.isEmpty() ? 0 : revenues.last();
        result.confidence = 0.3;
        result.trend = "STABLE";
        result.recommendation = "📊 Données historiques insuffisantes pour une prédiction précise. Continuez à enregistrer vos factures.";
    }

    // Ajustement saisonnier basique
    QDate currentDate = QDate::currentDate();
    int month = currentDate.month();

    // Facteurs saisonniers simplifiés
    double seasonalFactor = 1.0;
    if(month == 12) seasonalFactor = 1.2;  // Décembre - fin d'année
    else if(month == 1) seasonalFactor = 0.9; // Janvier - post-fêtes
    else if(month >= 6 && month <= 8) seasonalFactor = 1.1; // Été

    result.predictedAmount *= seasonalFactor;

    qDebug() << "🔮 Prédiction revenue:" << result.predictedAmount << "€ - Confiance:" << result.confidence << "- Tendance:" << result.trend;

    return result;
}

QVector<double> PredictionEngine::getRevenueHistory(int months)
{
    QVector<double> revenues;
    QSqlDatabase db = getDatabase();

    QSqlQuery query(db);
    query.prepare("SELECT strftime('%Y-%m', date_emission) as month, "
                  "SUM(montant) as total "
                  "FROM Facture "
                  "WHERE statut = 'Paid' "
                  "GROUP BY strftime('%Y-%m', date_emission) "
                  "ORDER BY month DESC LIMIT ?");
    query.addBindValue(months);

    if(query.exec()) {
        while(query.next()) {
            revenues.prepend(query.value(1).toDouble());
        }
    }

    return revenues;
}

QVector<QPair<QString, double>> PredictionEngine::getPaymentProbability()
{
    QVector<QPair<QString, double>> probabilities;
    QSqlDatabase db = getDatabase();

    QSqlQuery query(db);
    query.prepare("SELECT id_facture, montant, date_echeance, statut, "
                  "julianday('now') - julianday(date_echeance) as days_overdue "
                  "FROM Facture WHERE statut = 'Pending'");

    if(query.exec()) {
        while(query.next()) {
            QString invoiceId = query.value(0).toString();
            double amount = query.value(1).toDouble();
            QDate dueDate = query.value(2).toDate();

            double probability = calculatePaymentProbability(invoiceId, amount, dueDate);
            probabilities.append(qMakePair(
                QString("%1 - %2 € - Échéance: %3")
                    .arg(invoiceId)
                    .arg(amount, 0, 'f', 2)
                    .arg(dueDate.toString("dd/MM/yyyy")),
                probability * 100
                ));
        }
    }

    // Trier par probabilité (décroissant)
    std::sort(probabilities.begin(), probabilities.end(),
              [](const QPair<QString, double> &a, const QPair<QString, double> &b) {
                  return a.second > b.second;
              });

    return probabilities;
}

double PredictionEngine::calculatePaymentProbability(const QString &invoiceId, double amount, const QDate &dueDate)
{
    QDate today = QDate::currentDate();
    int daysOverdue = dueDate.daysTo(today);

    // Probabilité de base
    double probability = 0.85;

    // Facteur montant (factures élevées = risque plus élevé)
    if(amount > 10000) probability -= 0.2;
    else if(amount > 5000) probability -= 0.1;
    else if(amount > 1000) probability -= 0.05;

    // Facteur retard
    if(daysOverdue > 0) {
        probability -= (daysOverdue * 0.08); // -8% par jour de retard
    } else {
        // Facteur anticipation (paiement avant échéance)
        int daysUntilDue = today.daysTo(dueDate);
        if(daysUntilDue > 30) probability += 0.05;
        else if(daysUntilDue > 15) probability += 0.02;
    }

    // Facteur historique (simplifié)
    QSqlDatabase db = getDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM Facture WHERE id_facture LIKE ? AND statut = 'Paid'");
    query.addBindValue(invoiceId.left(7) + "%"); // Préfixe commun pour le même client

    if(query.exec() && query.next()) {
        int paidHistory = query.value(0).toInt();
        if(paidHistory > 5) probability += 0.1;
        else if(paidHistory > 2) probability += 0.05;
    }

    return qMax(0.05, qMin(0.95, probability));
}

PredictionEngine::FinancialHealth PredictionEngine::analyzeFinancialHealth()
{
    FinancialHealth health;
    QSqlDatabase db = getDatabase();

    // Revenus des 30 derniers jours
    QSqlQuery query(db);
    query.prepare("SELECT SUM(montant) FROM Facture WHERE statut = 'Paid' "
                  "AND date_emission >= date('now', '-30 days')");
    double monthlyRevenue = 0;
    if(query.exec() && query.next()) {
        monthlyRevenue = query.value(0).toDouble();
    }

    // Factures en attente
    query.prepare("SELECT SUM(montant) FROM Facture WHERE statut = 'Pending'");
    double pendingAmount = 0;
    if(query.exec() && query.next()) {
        pendingAmount = query.value(0).toDouble();
    }

    // Taux de conversion
    query.prepare("SELECT COUNT(*) FROM Facture WHERE statut = 'Paid'");
    int paidCount = 0;
    if(query.exec() && query.next()) {
        paidCount = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM Facture");
    int totalCount = 0;
    if(query.exec() && query.next()) {
        totalCount = query.value(0).toInt();
    }

    double conversionRate = totalCount > 0 ? (paidCount * 100.0 / totalCount) : 0;

    // Factures en retard
    query.prepare("SELECT COUNT(*) FROM Facture WHERE statut = 'Pending' AND date_echeance < date('now')");
    int overdueCount = 0;
    if(query.exec() && query.next()) {
        overdueCount = query.value(0).toInt();
    }

    // Calcul du score de santé (0-100)
    int score = 0;

    // Composante revenus (40 points)
    if(monthlyRevenue > 50000) score += 40;
    else if(monthlyRevenue > 20000) score += 30;
    else if(monthlyRevenue > 10000) score += 25;
    else if(monthlyRevenue > 5000) score += 20;
    else if(monthlyRevenue > 2000) score += 15;
    else if(monthlyRevenue > 1000) score += 10;
    else score += 5;

    // Composante conversion (30 points)
    if(conversionRate > 90) score += 30;
    else if(conversionRate > 80) score += 25;
    else if(conversionRate > 70) score += 20;
    else if(conversionRate > 60) score += 15;
    else if(conversionRate > 50) score += 10;
    else score += 5;

    // Composante retard (30 points)
    double overdueRatio = totalCount > 0 ? (overdueCount * 100.0 / totalCount) : 0;
    if(overdueRatio < 5) score += 30;
    else if(overdueRatio < 10) score += 25;
    else if(overdueRatio < 15) score += 20;
    else if(overdueRatio < 20) score += 15;
    else if(overdueRatio < 30) score += 10;
    else score += 5;

    health.score = score;

    // Détermination du statut
    if(score >= 85) {
        health.status = "EXCELLENT";
        health.analysis = "💪 Excellente santé financière - Votre entreprise performe remarquablement bien !";
    } else if(score >= 70) {
        health.status = "GOOD";
        health.analysis = "✅ Bonne santé financière - Performance solide avec quelques opportunités d'amélioration.";
    } else if(score >= 50) {
        health.status = "WARNING";
        health.analysis = "⚠️ Santé financière moyenne - Attention requise sur certains aspects.";
    } else {
        health.status = "CRITICAL";
        health.analysis = "🔴 Santé financière critique - Actions correctives nécessaires rapidement.";
    }

    // Recommandations
    if(conversionRate < 60) {
        health.recommendations.append("📞 Améliorez votre processus de relance client");
    }
    if(overdueRatio > 15) {
        health.recommendations.append("⏰ Renforcez le suivi des factures en retard");
    }
    if(monthlyRevenue < 5000) {
        health.recommendations.append("💼 Développez votre portefeuille clients");
    }
    if(health.recommendations.isEmpty()) {
        health.recommendations.append("🎯 Continuez votre excellente gestion actuelle");
    }

    qDebug() << "🏥 Analyse santé financière - Score:" << score << "- Statut:" << health.status;

    return health;
}

QVector<QPair<QString, double>> PredictionEngine::getMonthlyTrends(int months)
{
    QVector<QPair<QString, double>> trends;
    QSqlDatabase db = getDatabase();

    QSqlQuery query(db);
    query.prepare("SELECT strftime('%Y-%m', date_emission) as month, "
                  "SUM(montant) as total "
                  "FROM Facture "
                  "WHERE statut = 'Paid' "
                  "GROUP BY strftime('%Y-%m', date_emission) "
                  "ORDER BY month DESC LIMIT ?");
    query.addBindValue(months);

    if(query.exec()) {
        while(query.next()) {
            QString month = query.value(0).toString();
            double total = query.value(1).toDouble();
            trends.prepend(qMakePair(month, total));
        }
    }

    return trends;
}

double PredictionEngine::calculateCashFlowForecast()
{
    QSqlDatabase db = getDatabase();

    // Revenus attendus (factures en attente)
    QSqlQuery query(db);
    query.prepare("SELECT SUM(montant) FROM Facture WHERE statut = 'Pending'");
    double expectedRevenue = 0;
    if(query.exec() && query.next()) {
        expectedRevenue = query.value(0).toDouble();
    }

    // Historique des revenus pour estimation
    double historicalMonthlyAverage = calculateMovingAverage(6);

    // Prévision = revenus attendus + moyenne historique * facteur de confiance
    double forecast = expectedRevenue + (historicalMonthlyAverage * 0.7);

    return forecast;
}

double PredictionEngine::calculateMovingAverage(int months)
{
    QVector<double> revenues = getRevenueHistory(months);
    if(revenues.isEmpty()) return 0;

    double sum = 0;
    for(double revenue : revenues) {
        sum += revenue;
    }

    return sum / revenues.size();
}

double PredictionEngine::calculateGrowthRate()
{
    QVector<double> revenues = getRevenueHistory(3); // 3 derniers mois
    if(revenues.size() < 2) return 0;

    double latest = revenues.last();
    double previous = revenues.first();

    if(previous == 0) return 0;

    return ((latest - previous) / previous) * 100;
}
