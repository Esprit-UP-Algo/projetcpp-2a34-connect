#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

class HistoryManager
{
public:
    HistoryManager();

    // Enregistre une action dans le fichier historique.txt
    static void ajouterLog(const QString &type, const QString &details);

    // Lit tout le contenu du fichier pour l'affichage
    static QString lireHistorique();
};

#endif // HISTORYMANAGER_H
