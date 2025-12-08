#include "barchartwidget.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QFontMetrics>

BarChartWidget::BarChartWidget(QWidget *parent)
    : QWidget(parent), m_barColor(QColor(125, 79, 238)) // Violet #7D4FEE
{
    setMinimumSize(500, 300);
}

void BarChartWidget::setData(const QVector<int> &values, const QStringList &labels)
{
    m_values = values;
    m_labels = labels;
    update(); // Redessine le widget
}

void BarChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fond blanc
    painter.fillRect(rect(), Qt::white);

    if (m_values.isEmpty()) return;

    // Marges
    int margin = 50;
    int graphWidth = width() - 2 * margin;
    int graphHeight = height() - 2 * margin;

    // Trouver la valeur max pour l'échelle
    int maxValue = 0;
    for (int v : m_values) if (v > maxValue) maxValue = v;
    if (maxValue == 0) maxValue = 1; // Éviter division par zéro

    // Dessiner les axes
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(margin, height() - margin, width() - margin, height() - margin); // Axe X
    painter.drawLine(margin, margin, margin, height() - margin);                      // Axe Y

    // Dessiner les barres
    int barWidth = graphWidth / m_values.size();
    int spacing = barWidth * 0.2; // Espace entre barres
    int actualBarWidth = barWidth - spacing;

    for (int i = 0; i < m_values.size(); ++i) {
        int barHeight = (m_values[i] * graphHeight) / maxValue;
        int x = margin + i * barWidth + spacing / 2;
        int y = height() - margin - barHeight;

        // Barre
        painter.setBrush(m_barColor);
        painter.setPen(Qt::NoPen);
        painter.drawRect(x, y, actualBarWidth, barHeight);

        // Valeur au-dessus de la barre
        painter.setPen(Qt::black);
        painter.drawText(QRect(x, y - 20, actualBarWidth, 20), Qt::AlignCenter, QString::number(m_values[i]));

        // Étiquette en dessous (ex: "0-1000")
        painter.drawText(QRect(x, height() - margin + 5, actualBarWidth, 20), Qt::AlignCenter, m_labels[i]);
    }

    // Titre Axe Y
    painter.save();
    painter.translate(15, height() / 2);
    painter.rotate(-90);
    painter.drawText(QRect(-100, -10, 200, 20), Qt::AlignCenter, "Nombre d'Employés");
    painter.restore();
}
