#ifndef DEALWINDOW_H
#define DEALWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTimer>
#include <QSet>          // For previouslyCriticalItems

QT_BEGIN_NAMESPACE
namespace Ui { class DealWindow; }
QT_END_NAMESPACE

class DealWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DealWindow(QWidget *parent = nullptr);
    ~DealWindow();

private slots:
    void on_btnBook_deal_clicked();
    void on_btnEdit_deal_clicked();
    void on_btnDelete_deal_clicked();
    void on_btnOrder_deal_clicked();
    void on_btnSearch_deal_clicked();
    void on_btnClear_deal_clicked();
    void on_btnExportPDF_deal_clicked();

    void updateMonthlyStats();
    void checkAutoOrder();
    void deliverPendingOrders();
    void autoExportStatsWhenCritical();

private:
    Ui::DealWindow *ui;
    QMap<QString, int> stock;
    QMap<QString, int> ordered;
    QTimer *deliveryTimer = nullptr;

    // Tracks which items already triggered 100% alert (so we only export on NEW critical items)
    static QSet<QString> previouslyCriticalItems;

    // Smart conflict-free booking suggestion
    QDateTime findNextAvailableSlot(const QDateTime &requested, int requiredMask);

    int getEquipMask();
    void refreshTable(const QString &filter = "");
};

#endif // MAINWINDOW_H
