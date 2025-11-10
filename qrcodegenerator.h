#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <QPixmap>
#include <QString>

// === Creator DEFINITION HERE (BEFORE QRCodeGenerator) ===
struct Creator {
    int id;
    QString name;
    QString platform;
    QString type;
    int subscribers;
    QPixmap photo;
};

class QRCodeGenerator
{
public:
    static QPixmap generate(const Creator &c, const QPixmap &logo = QPixmap());
    static QString getJsonData(const Creator &c);
};

#endif // QRCODEGENERATOR_H
