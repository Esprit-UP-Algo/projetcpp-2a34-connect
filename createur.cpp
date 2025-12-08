#include "createur.h"
#include <QJsonDocument>

QJsonObject Createur::toJson() const
{
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = name;
    obj["platform"] = platform;
    obj["type"] = type;
    obj["subscribers"] = subscribers;
    // Note : on ne sérialise pas la photo ici (trop lourde), on la gère à part
    return obj;
}

Createur Createur::fromJson(const QJsonObject& obj)
{
    Createur c;
    c.id = obj["id"].toInt();
    c.name = obj["name"].toString();
    c.platform = obj["platform"].toString();
    c.type = obj["type"].toString();
    c.subscribers = obj["subscribers"].toInt();
    return c;
}

QString Createur::toString() const
{
    return QString("Createur{id=%1, name=%2, platform=%3, subs=%4}")
    .arg(id).arg(name).arg(platform).arg(subscribers);
}
