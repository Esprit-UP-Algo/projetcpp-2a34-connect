#include "curvewidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QPaintEvent>
#include <QtMath> // Pour qMin/qMax

CurveWidget::CurveWidget(QWidget *parent)
    : QWidget(parent), m_color(Qt::blue)
{
    setMinimumSize(400, 300);
}

void CurveWidget::setData(const QVector<QPointF> &data, const QString &title, const QColor &color)
{
    m_data = data;
    m_title = title;
    m_color = color;
    update(); // Redessine le widget
}

void CurveWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Fond avec dégradé
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(255, 255, 255, 200));
    gradient.setColorAt(1, QColor(240, 240, 255, 200));
    painter.fillRect(rect(), gradient);

    // Bordure
    painter.setPen(QPen(QColor(125, 79, 238), 2));
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);

    if (m_data.isEmpty()) {
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, "Aucune donnée disponible");
        return;
    }

    // Calcul des limites (Min/Max)
    double minX = m_data[0].x(), maxX = m_data[0].x();
    double minY = m_data[0].y(), maxY = m_data[0].y();

    for (const QPointF &point : m_data) {
        if (point.x() < minX) minX = point.x();
        if (point.x() > maxX) maxX = point.x();
        if (point.y() < minY) minY = point.y();
        if (point.y() > maxY) maxY = point.y();
    }

    // Ajouter une marge
    double xRange = maxX - minX;
    double yRange = maxY - minY;
    if (xRange == 0) xRange = 1;
    if (yRange == 0) yRange = 1;

    minY = qMax(0.0, minY - yRange * 0.1);
    maxY = maxY + yRange * 0.1;

    // Zone de dessin avec marges
    QRectF plotRect(50, 40, width() - 80, height() - 80);

    // Dessiner la grille
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    for (int i = 0; i <= 5; ++i) {
        double y = plotRect.bottom() - (plotRect.height() * i / 5);
        painter.drawLine(plotRect.left(), y, plotRect.right(), y);
    }

    // Dessiner les axes
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(plotRect.left(), plotRect.bottom(), plotRect.right(), plotRect.bottom()); // Axe X
    painter.drawLine(plotRect.left(), plotRect.top(), plotRect.left(), plotRect.bottom());     // Axe Y

    // Dessiner la courbe
    QPainterPath path;
    bool firstPoint = true;

    for (const QPointF &point : m_data) {
        double x = plotRect.left() + ((point.x() - minX) / xRange) * plotRect.width();
        double y = plotRect.bottom() - ((point.y() - minY) / (maxY - minY)) * plotRect.height();

        if (firstPoint) {
            path.moveTo(x, y);
            firstPoint = false;
        } else {
            path.lineTo(x, y);
        }
    }

    // Dessiner la ligne de la courbe
    QPen curvePen(m_color, 3);
    curvePen.setCapStyle(Qt::RoundCap);
    painter.setPen(curvePen);
    painter.drawPath(path);

    // Dessiner les points
    painter.setBrush(m_color);
    painter.setPen(QPen(Qt::white, 2));

    for (const QPointF &point : m_data) {
        double x = plotRect.left() + ((point.x() - minX) / xRange) * plotRect.width();
        double y = plotRect.bottom() - ((point.y() - minY) / (maxY - minY)) * plotRect.height();

        painter.drawEllipse(QPointF(x, y), 6, 6);

        // Afficher les valeurs sur les points
        painter.setPen(Qt::black);
        painter.drawText(QRectF(x - 20, y - 25, 40, 20),
                         Qt::AlignCenter,
                         QString::number(point.y(), 'f', 0));
        painter.setPen(QPen(Qt::white, 2));
    }

    // Titre
    painter.setPen(QColor(125, 79, 238));
    QFont titleFont = painter.font();
    titleFont.setBold(true);
    titleFont.setPointSize(10);
    painter.setFont(titleFont);
    painter.drawText(QRect(0, 10, width(), 20), Qt::AlignCenter, m_title);

    // Labels des axes
    painter.setPen(Qt::black);
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);

    painter.drawText(QRectF(0, plotRect.bottom() + 5, width(), 20),
                     Qt::AlignCenter, "Période");
    painter.save();
    painter.translate(20, height() / 2);
    painter.rotate(-90);
    painter.drawText(QRectF(-100, 0, 200, 20), Qt::AlignCenter, "Valeurs");
    painter.restore();
}
