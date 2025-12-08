#include "qrcodegenerator.h"
#include "qrcodegen.hpp"
#include <QPainter>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

QPixmap QRCodeGenerator::generate(const Createur &c, const QPixmap &logo)
{
    QString json = getJsonData(c);
    std::string utf8 = json.toStdString();

    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(utf8.c_str(), qrcodegen::QrCode::Ecc::MEDIUM);
    int s = qr.getSize(), scale = 10;
    QPixmap pix(s * scale, s * scale);
    pix.fill(Qt::white);
    QPainter p(&pix);
    for (int y = 0; y < s; ++y)
        for (int x = 0; x < s; ++x)
            if (qr.getModule(x, y))
                p.fillRect(x * scale, y * scale, scale, scale, Qt::black);

    if (!logo.isNull()) {
        QPixmap l = logo.scaled(pix.size() * 0.2, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        p.drawPixmap((pix.width() - l.width()) / 2, (pix.height() - l.height()) / 2, l);
    }

    QPixmap final(pix.width(), pix.height() + 100);
    final.fill(Qt::white);
    QPainter fp(&final);
    fp.drawPixmap(0, 0, pix);
    fp.drawPixmap(10, pix.height() + 10, c.photo.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    fp.setFont(QFont("Arial", 12, QFont::Bold));
    fp.drawText(100, pix.height() + 40, c.name + " • " + c.platform);
    fp.drawText(100, pix.height() + 65, QString::number(c.subscribers) + " subs");

    return final;
}

QString QRCodeGenerator::getJsonData(const Createur &c)
{
    QJsonObject o;
    o["id"] = c.id;
    o["name"] = c.name;
    o["platform"] = c.platform;
    o["subscribers"] = c.subscribers;
    o["type"] = c.type;
    o["generated_at"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    return QJsonDocument(o).toJson(QJsonDocument::Compact);
}
