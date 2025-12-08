#ifndef CURVEWIDGET_H
#define CURVEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QString>

class CurveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurveWidget(QWidget *parent = nullptr);
    void setData(const QVector<QPointF> &data, const QString &title, const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPointF> m_data;
    QString m_title;
    QColor m_color;
};

#endif // CURVEWIDGET_H
