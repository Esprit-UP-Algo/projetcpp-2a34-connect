#include "employes.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QSqlError>
#include <QRegularExpression>
#include <QSqlDatabase>

// ==================== CONSTRUCTEURS ====================

Employes::Employes()
{
    prenom = "";
    email = "";
    telephone = "";
    sexe = "";
    poste = "";
    salaire = 0;
    cin = "";
    idSuperviseur = 0;
}

Employes::Employes(QString prenom, QString email, QString telephone, QString sexe, QString poste, double salaire, QDate dateEmbauche)
{
    this->prenom = prenom;
    this->email = email;
    this->telephone = telephone;
    this->sexe = sexe;
    this->poste = poste;
    this->salaire = salaire;
    this->dateEmbauche = dateEmbauche;
    this->cin = "";
    this->idSuperviseur = 0;
}

// ==================== MÉTHODES CRUD ====================

bool Employes::creerTableSiExistePas()
{
    QSqlDatabase db = QSqlDatabase::database();
    QString driver = db.driverName();

    qDebug() << "🔧 Vérification/création table - Driver:" << driver;

    if (driver == "QSQLITE") {
        QSqlQuery query;

        // Vérifier si la table existe
        query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='empolye'");

        if (query.next()) {
            qDebug() << "✅ Table empolye existe déjà (SQLite)";
            return true;
        }

        qDebug() << "🔄 Création de la table empolye (SQLite)...";

        QString createTable =
            "CREATE TABLE empolye ("
            "IDEMPLOYE INTEGER PRIMARY KEY AUTOINCREMENT, "
            "PRENOM VARCHAR(20), "
            "DATEDEMBAUCHE DATE, "
            "EMAIL VARCHAR(20), "
            "TELEPHONE INTEGER, "
            "SEXE VARCHAR(20), "
            "POSTE VARCHAR(20), "
            "SALAIRE REAL, "
            "IDSUPERVISEUR VARCHAR(20))";

        if (!query.exec(createTable)) {
            qDebug() << "❌ Erreur création table SQLite:" << query.lastError().text();
            return false;
        }

        qDebug() << "✅ Table empolye créée avec succès (SQLite)";
        return true;

    } else if (driver == "QODBC") {
        // ORACLE / SQL SERVER
        QSqlQuery query;

        // Vérifier si la table existe (Oracle)
        if (!query.exec("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'EMPOLYE'")) {
            qDebug() << "❌ Erreur vérification table EMPOLYE:" << query.lastError().text();
            return false;
        }

        query.next();
        if (query.value(0).toInt() > 0) {
            qDebug() << "✅ Table EMPOLYE existe déjà (Oracle)";
            return true;
        }

        qDebug() << "🔄 Création de la table EMPOLYE (Oracle)...";

        // Créer la table Oracle
        QString createTable =
            "CREATE TABLE empolye ("
            "IDEMPLOYE NUMBER PRIMARY KEY, "
            "PRENOM VARCHAR2(20), "
            "DATEDEMBAUCHE DATE, "
            "EMAIL VARCHAR2(20), "
            "TELEPHONE NUMBER, "
            "SEXE VARCHAR2(20), "
            "POSTE VARCHAR2(20), "
            "SALAIRE NUMBER, "
            "IDSUPERVISEUR VARCHAR2(20)"
            ")";

        if (!query.exec(createTable)) {
            qDebug() << "❌ Erreur création table Oracle:" << query.lastError().text();
            return false;
        }

        // Créer la séquence Oracle
        if (!query.exec("CREATE SEQUENCE empolye_seq START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE")) {
            qDebug() << "❌ Erreur création séquence Oracle:" << query.lastError().text();
            return false;
        }

        // Créer le trigger Oracle
        QString createTrigger =
            "CREATE OR REPLACE TRIGGER empolye_bi "
            "BEFORE INSERT ON empolye "
            "FOR EACH ROW "
            "BEGIN "
            "    IF :NEW.IDEMPLOYE IS NULL THEN "
            "        SELECT empolye_seq.NEXTVAL INTO :NEW.IDEMPLOYE FROM DUAL; "
            "    END IF; "
            "END;";

        if (!query.exec(createTrigger)) {
            qDebug() << "❌ Erreur création trigger Oracle:" << query.lastError().text();
            return false;
        }

        qDebug() << "✅ Table EMPOLYE créée avec succès (Oracle)";
    }

    return true;
}

bool Employes::ajouter()
{
    qDebug() << "=== TENTATIVE D'AJOUT EMPLOYÉ ===";

    // Créer la table si elle n'existe pas
    if (!creerTableSiExistePas()) {
        qDebug() << "❌ Impossible de créer/accéder à la table EMPOLYE";
        return false;
    }

    QSqlQuery query;

    // REQUÊTE UNIQUE qui fonctionne pour Oracle et SQLite
    query.prepare("INSERT INTO empolye (PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR) "
                  "VALUES (:PRENOM, :DATEDEMBAUCHE, :EMAIL, :TELEPHONE, :SEXE, :POSTE, :SALAIRE, :IDSUPERVISEUR)");

    query.bindValue(":PRENOM", prenom);
    query.bindValue(":DATEDEMBAUCHE", dateEmbauche);
    query.bindValue(":EMAIL", email.isEmpty() ? QVariant() : email);
    query.bindValue(":TELEPHONE", telephone.isEmpty() ? QVariant() : telephone);
    query.bindValue(":SEXE", sexe.isEmpty() ? QVariant() : sexe);
    query.bindValue(":POSTE", poste);
    query.bindValue(":SALAIRE", salaire);

    if (idSuperviseur > 0) {
        query.bindValue(":IDSUPERVISEUR", idSuperviseur);
    } else {
        query.bindValue(":IDSUPERVISEUR", QVariant());
    }

    qDebug() << "=== DONNÉES À AJOUTER ===";
    qDebug() << "Base utilisée:" << QSqlDatabase::database().driverName();
    qDebug() << "Prénom:" << prenom;
    qDebug() << "Date:" << dateEmbauche;
    qDebug() << "Email:" << email;
    qDebug() << "Téléphone:" << telephone;
    qDebug() << "Sexe:" << sexe;
    qDebug() << "Poste:" << poste;
    qDebug() << "Salaire:" << salaire;

    bool result = query.exec();

    if (result) {
        qDebug() << "✅ EMPLOYÉ AJOUTÉ AVEC SUCCÈS!";
        qDebug() << "   ID généré:" << query.lastInsertId().toInt();
    } else {
        qDebug() << "❌ ERREUR D'AJOUT:" << query.lastError().text();
        qDebug() << "   Requête SQL:" << query.lastQuery();
    }

    return result;
}

bool Employes::modifier(int id, QString email, QString telephone, QString poste, double salaire, QString cin)
{
    QSqlQuery query;
    query.prepare("UPDATE empolye SET EMAIL=:EMAIL, TELEPHONE=:TELEPHONE, POSTE=:POSTE, SALAIRE=:SALAIRE WHERE IDEMPLOYE=:ID");

    query.bindValue(":ID", id);
    query.bindValue(":EMAIL", email);
    query.bindValue(":TELEPHONE", telephone);
    query.bindValue(":POSTE", poste);
    query.bindValue(":SALAIRE", salaire);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Erreur modification employé:" << query.lastError().text();
    }
    return result;
}

bool Employes::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM empolye WHERE IDEMPLOYE=:ID");
    query.bindValue(":ID", id);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Erreur suppression employé:" << query.lastError().text();
    }
    return result;
}

QSqlQueryModel *Employes::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Embauche"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Superviseur"));
    return model;
}

QSqlQueryModel *Employes::trierParSalaire()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye ORDER BY SALAIRE DESC");
    return model;
}

QSqlQueryModel *Employes::rechercherParPrenom(QString prenom)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    query.prepare("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye WHERE UPPER(PRENOM) LIKE UPPER(:PRENOM)");
    query.bindValue(":PRENOM", "%" + prenom + "%");

    if (query.exec()) {
        model->setQuery(std::move(query));
    }
    return model;
}

// ==================== MÉTHODES DE VALIDATION ====================

bool Employes::validerPrenom(const QString &prenom)
{
    if (prenom.isEmpty() || prenom.length() > 20) {
        qDebug() << "Prénom invalide: vide ou trop long (max 20 caractères)";
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s\\-']+$");
    if (!regex.match(prenom).hasMatch()) {
        qDebug() << "Prénom contient des caractères invalides";
        return false;
    }

    return true;
}

bool Employes::validerEmail(const QString &email)
{
    if (email.isEmpty()) {
        return true; // Email optionnel
    }

    if (email.length() > 20) {
        qDebug() << "Email trop long (max 20 caractères)";
        return false;
    }

    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.(com|net|org|edu|gov|fr|tn|ma|dz)$");
    if (!regex.match(email).hasMatch()) {
        qDebug() << "Format d'email invalide";
        return false;
    }

    return true;
}

bool Employes::validerTelephone(const QString &telephone)
{
    if (telephone.isEmpty()) {
        return true; // Téléphone optionnel
    }

    bool ok;
    telephone.toLongLong(&ok);
    if (!ok) {
        qDebug() << "Téléphone doit être un nombre";
        return false;
    }

    if (telephone.length() < 8 || telephone.length() > 15) {
        qDebug() << "Téléphone doit avoir entre 8 et 15 chiffres";
        return false;
    }

    return true;
}

bool Employes::validerSexe(const QString &sexe)
{
    if (sexe.isEmpty()) {
        return true; // Sexe optionnel
    }

    if (sexe.length() > 20) {
        qDebug() << "Sexe trop long (max 20 caractères)";
        return false;
    }

    QString sexeLower = sexe.toLower();
    if (sexeLower != "homme" && sexeLower != "femme" && sexeLower != "male" && sexeLower != "female" &&
        sexeLower != "h" && sexeLower != "f" && sexeLower != "m") {
        qDebug() << "Sexe invalide. Doit être: Homme, Femme, Male, Female, H, F ou M";
        return false;
    }

    return true;
}

bool Employes::validerPoste(const QString &poste)
{
    if (poste.isEmpty() || poste.length() > 20) {
        qDebug() << "Poste invalide: vide ou trop long (max 20 caractères)";
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ0-9\\s\\-&]+$");
    if (!regex.match(poste).hasMatch()) {
        qDebug() << "Poste contient des caractères invalides";
        return false;
    }

    return true;
}

bool Employes::validerSalaire(double salaire)
{
    if (salaire < 0 || salaire > 100000) {
        qDebug() << "Salaire invalide:" << salaire;
        return false;
    }

    return true;
}

bool Employes::validerCIN(const QString &cin)
{
    if (cin.isEmpty()) {
        return true; // CIN optionnel
    }

    bool ok;
    cin.toLongLong(&ok);
    if (!ok) {
        qDebug() << "CIN doit être un nombre";
        return false;
    }

    if (cin.length() != 8) {
        qDebug() << "CIN doit avoir 8 chiffres";
        return false;
    }

    return true;
}

bool Employes::validerID(int id)
{
    if (id <= 0) {
        qDebug() << "ID invalide: doit être positif";
        return false;
    }

    if (id > 999999) {
        qDebug() << "ID trop grand";
        return false;
    }

    return true;
}
