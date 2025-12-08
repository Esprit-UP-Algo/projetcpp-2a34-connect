#ifndef CREATEUR_H
#define CREATEUR_H

#include <QString>
#include <QPixmap>
#include <QJsonObject>

struct Createur
{
    int id = 0;
    QString name;
    QString platform;
    QString type;
    int subscribers = 0;
    QPixmap photo;

    // Constructeur par défaut
    Createur() = default;

    // Constructeur complet
    Createur(int id, const QString& name, const QString& platform,
             const QString& type, int subscribers, const QPixmap& photo = QPixmap())
        : id(id), name(name), platform(platform), type(type), subscribers(subscribers), photo(photo) {}

    // Conversion depuis/vers JSON (super utile pour l'intégration API ou sauvegarde)
    QJsonObject toJson() const;
    static Createur fromJson(const QJsonObject& obj);

    // Pour débogage
    QString toString() const;
};

#endif // CREATEUR_H
