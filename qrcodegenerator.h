#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <QPixmap>
#include <QString>
#include "createur.h"  // Use the Createur struct you provided

class QRCodeGenerator
{
public:
    static QPixmap generate(const Createur &c, const QPixmap &logo = QPixmap());
    static QString getJsonData(const Createur &c);
};

#endif // QRCODEGENERATOR_H
