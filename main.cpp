#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

bool initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("gestion_sponsors.db");

    if (!db.open()) {
        qDebug() << "Erreur d'ouverture de la base de données:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ Base de données ouverte: gestion_sponsors.db";

    // Création de la table sponsor
    QSqlQuery query;
    QString createSponsorTable =
        "CREATE TABLE IF NOT EXISTS sponsor ("
        "IDSPONSOR INTEGER PRIMARY KEY AUTOINCREMENT, "
        "NOM TEXT NOT NULL, "
        "PRENOM TEXT NOT NULL, "
        "CATEGORIE TEXT, "
        "EMAIL TEXT, "
        "DEBUTCONTRAT DATE, "
        "FINCONTRAT DATE, "
        "BUDGET REAL)";

    if (!query.exec(createSponsorTable)) {
        qDebug() << "Erreur création table sponsor:" << query.lastError().text();
        return false;
    }

    // ✅ CORRECTION: Table employé avec CODE_BADGE
    QString createEmployeTable =
        "CREATE TABLE IF NOT EXISTS empolye ("
        "IDEMPLOYE INTEGER PRIMARY KEY AUTOINCREMENT, "
        "PRENOM TEXT NOT NULL, "
        "EMAIL TEXT, "
        "TELEPHONE TEXT, "
        "SEXE TEXT, "
        "POSTE TEXT, "
        "SALAIRE REAL, "
        "DATEDEMBAUCHE DATE, "
        "IDSUPERVISEUR INTEGER, "
        "CODE_BADGE TEXT)";  // ✅ AJOUT DE LA COLONNE

    if (!query.exec(createEmployeTable)) {
        qDebug() << "Erreur création table empolye:" << query.lastError().text();
        return false;
    }

    // ✅ AJOUTER la colonne CODE_BADGE si elle n'existe pas déjà
    QSqlQuery checkColumn("PRAGMA table_info(empolye)");
    bool hasCodeBadge = false;

    while (checkColumn.next()) {
        QString columnName = checkColumn.value(1).toString();
        if (columnName.toUpper() == "CODE_BADGE") {
            hasCodeBadge = true;
            break;
        }
    }

    if (!hasCodeBadge) {
        qDebug() << "⚠️ Colonne CODE_BADGE manquante - Ajout...";
        if (query.exec("ALTER TABLE empolye ADD COLUMN CODE_BADGE TEXT")) {
            qDebug() << "✅ Colonne CODE_BADGE ajoutée avec succès";
        } else {
            qDebug() << "❌ Erreur ajout CODE_BADGE:" << query.lastError().text();
        }
    } else {
        qDebug() << "✅ Colonne CODE_BADGE existe déjà";
    }

    // ✅ CRÉATION TABLE HISTORIQUE_ACCES
    QString createHistoriqueAccesTable =
        "CREATE TABLE IF NOT EXISTS HISTORIQUE_ACCES ("
        "ID_ACCES INTEGER PRIMARY KEY AUTOINCREMENT, "
        "ID_EMPLOYE INTEGER, "
        "NOM_EMPLOYE TEXT, "
        "HEURE_ACCES DATETIME, "
        "TYPE_ACCES TEXT)";

    if (!query.exec(createHistoriqueAccesTable)) {
        qDebug() << "Erreur création table HISTORIQUE_ACCES:" << query.lastError().text();
    } else {
        qDebug() << "✅ Table HISTORIQUE_ACCES créée";
    }

    // Création de la table historique chatbot
    QString createHistoriqueTable =
        "CREATE TABLE IF NOT EXISTS chat_historique ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "IDSPONSOR INTEGER, "
        "QUESTION TEXT, "
        "REPONSE TEXT, "
        "HORODATAGE DATETIME DEFAULT CURRENT_TIMESTAMP)";

    if (!query.exec(createHistoriqueTable)) {
        qDebug() << "Erreur création table historique:" << query.lastError().text();
    }

    // Création de la table SMS
    QString createSMSTable =
        "CREATE TABLE IF NOT EXISTS sms_historique ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "IDSPONSOR INTEGER, "
        "NUMERO TEXT, "
        "MESSAGE TEXT, "
        "SUCCES BOOLEAN, "
        "HORODATAGE DATETIME DEFAULT CURRENT_TIMESTAMP)";

    if (!query.exec(createSMSTable)) {
        qDebug() << "Erreur création table SMS:" << query.lastError().text();
    }

    // ✅ VÉRIFICATION: Afficher le nombre d'employés
    QSqlQuery countQuery("SELECT COUNT(*) FROM empolye");
    if (countQuery.next()) {
        int count = countQuery.value(0).toInt();
        qDebug() << "📊 Nombre d'employés dans la BD:" << count;

        if (count > 0) {
            // Afficher les 3 premiers employés avec leur code badge
            QSqlQuery showQuery("SELECT IDEMPLOYE, PRENOM, EMAIL, CODE_BADGE FROM empolye LIMIT 3");
            qDebug() << "👥 Premiers employés:";
            while (showQuery.next()) {
                qDebug() << "   ID:" << showQuery.value(0).toInt()
                << "| Nom:" << showQuery.value(1).toString()
                << "| Email:" << showQuery.value(2).toString()
                << "| Badge:" << (showQuery.value(3).isNull() ? "Non assigné" : showQuery.value(3).toString());
            }
        }
    }

    qDebug() << "✅ Base de données initialisée avec succès\n";
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Activer les messages de debug détaillés
    qSetMessagePattern("[%{type}] %{function}: %{message}");

    // Initialiser la base de données
    if (!initializeDatabase()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'initialiser la base de données !");
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}


/*
 * ==================== INSTRUCTIONS APRÈS COMPILATION ====================
 *
 * 1. Recompile ton projet (Build → Rebuild All)
 *
 * 2. Si tu as DÉJÀ des employés mais sans CODE_BADGE, exécute ce SQL dans DB Browser:
 *
 *    UPDATE empolye SET CODE_BADGE = '9999' WHERE IDEMPLOYE = 1;
 *    UPDATE empolye SET CODE_BADGE = '1234' WHERE IDEMPLOYE = 2;
 *    UPDATE empolye SET CODE_BADGE = '5678' WHERE IDEMPLOYE = 3;
 *
 * 3. Relance l'application
 *
 * 4. Scanne ton badge Arduino avec le code 9999
 *
 * 5. Tu devrais voir dans la console:
 *    Code badge reçu: "9999"
 *    ✅ Code valide!
 *    ✅ Accès autorisé pour: [nom de l'employé]
 *
 */
