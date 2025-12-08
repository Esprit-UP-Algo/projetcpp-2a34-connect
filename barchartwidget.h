#ifndef BARCHARTWIDGET_H
#define BARCHARTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QStringList>
#include <QColor>

class BarChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BarChartWidget(QWidget *parent = nullptr);
    void setData(const QVector<int> &values, const QStringList &labels);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int> m_values;
    QStringList m_labels;
    QColor m_barColor;
};

#endif // BARCHARTWIDGET_H
