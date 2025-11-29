#include "database.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

bool DatabaseManager::connectToDatabase()
{
    qDebug() << "=== CONNEXION BASE DE DONNÉES ORACLE ===";

    // Nettoyer toute connexion existante
    if (QSqlDatabase::contains("ORACLE_CONN")) {
        QSqlDatabase::removeDatabase("ORACLE_CONN");
    }

    // Créer une nouvelle connexion Oracle
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "ORACLE_CONN");

    // CONFIGURATION POUR VOTRE BASE ORACLE
    // MODIFIEZ LE MOT DE PASSE SELON VOTRE CONFIGURATION
    QString connectionString =
        "DRIVER={Oracle ODBC Driver};"
        "DBQ=localhost:1521/projet;"  // Votre service Oracle
        "UID=MOUTIE;"                 // Votre utilisateur
        "PWD=esprit2026;";    // VOTRE MOT DE PASSE

    db.setDatabaseName(connectionString);

    qDebug() << "Tentative de connexion avec:" << connectionString;

    if (db.open()) {
        qDebug() << "✅ Connexion Oracle réussie!";
        qDebug() << "Utilisateur:" << "MOUTIE";
        qDebug() << "Base: projet";
        return true;
    } else {
        qDebug() << "❌ Erreur connexion Oracle:" << db.lastError().text();
        QMessageBox::critical(nullptr, "Erreur Oracle",
                              "Impossible de se connecter à Oracle:\n" +
                                  db.lastError().text() +
                                  "\n\nVérifiez:\n"
                                  "1. Le service Oracle est démarré\n"
                                  "2. Le mot de passe est correct\n"
                                  "3. Le driver ODBC est installé");
        return false;
    }
}

QSqlDatabase DatabaseManager::getDatabase()
{
    return QSqlDatabase::database("ORACLE_CONN");
}
