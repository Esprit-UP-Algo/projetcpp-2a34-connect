#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "connection.h"

bool initializeDatabase() {
    // Utiliser la connexion Oracle
    Connection c;
    if (!c.createconnect()) {
        qDebug() << "❌ Erreur: Impossible de se connecter à Oracle";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    qDebug() << "✅ Connexion Oracle établie";

    // Vérifier/Créer les tables nécessaires
    QSqlQuery query(db);

    // 1. Vérifier si la table EMPOLYE existe
    QString checkEmpolye =
        "SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'EMPOLYE'";

    if (!query.exec(checkEmpolye)) {
        qDebug() << "❌ Erreur vérification table EMPOLYE:" << query.lastError().text();
        return false;
    }

    if (query.next() && query.value(0).toInt() == 0) {
        // Créer la table EMPOLYE
        QString createEmpolye =
            "CREATE TABLE EMPOLYE ("
            "IDEMPLOYE NUMBER PRIMARY KEY, "
            "PRENOM VARCHAR2(20) NOT NULL, "
            "EMAIL VARCHAR2(20), "
            "TELEPHONE NUMBER, "
            "SEXE VARCHAR2(20), "
            "POSTE VARCHAR2(20), "
            "SALAIRE NUMBER, "
            "DATEDEMBAUCHE DATE, "
            "IDSUPERVISEUR VARCHAR2(20), "
            "CODE_BADGE VARCHAR2(20))";

        if (!query.exec(createEmpolye)) {
            qDebug() << "❌ Erreur création table EMPOLYE:" << query.lastError().text();
            return false;
        }
        qDebug() << "✅ Table EMPOLYE créée";
    }

    // 2. Vérifier si la colonne CODE_BADGE existe
    QString checkCodeBadge =
        "SELECT COUNT(*) FROM USER_TAB_COLUMNS "
        "WHERE TABLE_NAME = 'EMPOLYE' AND COLUMN_NAME = 'CODE_BADGE'";

    if (query.exec(checkCodeBadge) && query.next()) {
        if (query.value(0).toInt() == 0) {
            // Ajouter la colonne CODE_BADGE
            QString addCodeBadge = "ALTER TABLE EMPOLYE ADD (CODE_BADGE VARCHAR2(20))";
            if (query.exec(addCodeBadge)) {
                qDebug() << "✅ Colonne CODE_BADGE ajoutée";
            } else {
                qDebug() << "⚠️ Erreur ajout CODE_BADGE:" << query.lastError().text();
            }
        } else {
            qDebug() << "✅ Colonne CODE_BADGE existe déjà";
        }
    }

    // 3. Vérifier/Créer la table HISTORIQUE_ACCES
    QString checkHistorique =
        "SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'HISTORIQUE_ACCES'";

    if (query.exec(checkHistorique) && query.next()) {
        if (query.value(0).toInt() == 0) {
            QString createHistorique =
                "CREATE TABLE HISTORIQUE_ACCES ("
                "ID_ACCES NUMBER PRIMARY KEY, "
                "ID_EMPLOYE NUMBER, "
                "NOM_EMPLOYE VARCHAR2(50), "
                "HEURE_ACCES DATE, "
                "TYPE_ACCES VARCHAR2(20))";

            if (query.exec(createHistorique)) {
                qDebug() << "✅ Table HISTORIQUE_ACCES créée";
            }
        }
    }

    // 4. Vérifier/Créer la table SPONSOR
    QString checkSponsor =
        "SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'SPONSOR'";

    if (query.exec(checkSponsor) && query.next()) {
        if (query.value(0).toInt() == 0) {
            QString createSponsor =
                "CREATE TABLE SPONSOR ("
                "IDSPONSOR NUMBER PRIMARY KEY, "
                "NOM VARCHAR2(20), "
                "PRENOM VARCHAR2(20), "
                "CATEGORIE VARCHAR2(20), "
                "EMAIL VARCHAR2(20), "
                "DEBUTCONTRAT DATE, "
                "FINCONTRAT DATE, "
                "BUDGET NUMBER)";

            query.exec(createSponsor);
        }
    }

    // 5. Compter le nombre d'employés
    if (query.exec("SELECT COUNT(*) FROM EMPOLYE")) {
        if (query.next()) {
            int count = query.value(0).toInt();
            qDebug() << "📊 Nombre d'employés dans Oracle:" << count;

            if (count > 0) {
                // Afficher les 3 premiers employés
                QSqlQuery showQuery("SELECT IDEMPLOYE, PRENOM, EMAIL, CODE_BADGE FROM EMPOLYE WHERE ROWNUM <= 3");
                qDebug() << "👥 Premiers employés:";
                while (showQuery.next()) {
                    qDebug() << "   ID:" << showQuery.value(0).toInt()
                    << "| Nom:" << showQuery.value(1).toString()
                    << "| Email:" << showQuery.value(2).toString()
                    << "| Badge:" << (showQuery.value(3).isNull() ? "Non assigné" : showQuery.value(3).toString());
                }
            }
        }
    }

    qDebug() << "✅ Base de données Oracle initialisée avec succès";
    return true;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Activer les messages de debug détaillés
    qSetMessagePattern("[%{type}] %{function}: %{message}");

    // Initialiser la base de données Oracle
    if (!initializeDatabase()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'initialiser la base de données Oracle !");
        return -1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
