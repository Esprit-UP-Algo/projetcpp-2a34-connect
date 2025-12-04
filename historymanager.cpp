#include "historymanager.h"

HistoryManager::HistoryManager() {}

void HistoryManager::ajouterLog(const QString &type, const QString &details)
{
    QFile file("historique_employes.txt");
    // Mode "Append" pour ajouter à la fin sans effacer le reste
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);

        // Format: [Date Heure] [TYPE] Détails
        QString date = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss");
        out << "[" << date << "] [" << type << "] " << details << "\n";

        file.close();
    } else {
        qDebug() << "❌ Erreur : Impossible d'écrire dans l'historique.";
    }
}

QString HistoryManager::lireHistorique()
{
    QFile file("historique_employes.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "Aucun historique disponible pour le moment.";
    }

    QTextStream in(&file);
    QString contenu = in.readAll();
    file.close();
    return contenu;
}
