#include "employes.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QSqlDatabase>

// ==================== CONSTRUCTEURS ====================

Employes::Employes()
    : id(-1), prenom(""), date_embauche(QDate::currentDate()), email(""),
    telephone(""), sexe("M"), poste(""), salaire(0.0), id_superviseur(""), code_badge("")
{
}

Employes::Employes(int id, QString prenom, QDate date_embauche, QString email,
                   QString telephone, QString sexe, QString poste, double salaire,
                   QString id_superviseur, QString code_badge)
    : id(id), prenom(prenom), date_embauche(date_embauche), email(email),
    telephone(telephone), sexe(sexe), poste(poste), salaire(salaire),
    id_superviseur(id_superviseur), code_badge(code_badge)
{
}

// ==================== GETTERS ====================

int Employes::getId() { return id; }
QString Employes::getPrenom() { return prenom; }
QDate Employes::getDateEmbauche() { return date_embauche; }
QString Employes::getEmail() { return email; }
QString Employes::getTelephone() { return telephone; }
QString Employes::getSexe() { return sexe; }
QString Employes::getPoste() { return poste; }
double Employes::getSalaire() { return salaire; }
QString Employes::getIdSuperviseur() { return id_superviseur; }
QString Employes::getCodeBadge() { return code_badge; }

// ==================== SETTERS ====================

void Employes::setId(int i) { id = i; }
void Employes::setPrenom(QString p) { prenom = p; }
void Employes::setDateEmbauche(QDate d) { date_embauche = d; }
void Employes::setEmail(QString e) { email = e; }
void Employes::setTelephone(QString t) { telephone = t; }
void Employes::setSexe(QString s) { sexe = s; }
void Employes::setPoste(QString p) { poste = p; }
void Employes::setSalaire(double s) { salaire = s; }
void Employes::setIdSuperviseur(QString id) { id_superviseur = id; }
void Employes::setCodeBadge(QString code) { code_badge = code; }

// ==================== CRUD ====================

bool Employes::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    // Détecter le nom exact de la table
    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) {
        qDebug() << "❌ Table EMPOLYE/EMPLOYE non trouvée";
        return false;
    }

    QSqlQuery query;
    QString sql = QString("INSERT INTO %1 (PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR) "
                          "VALUES (:prenom, :date_embauche, :email, :telephone, :sexe, :poste, :salaire, :id_superviseur)")
                      .arg(tableName);

    query.prepare(sql);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_embauche", date_embauche.toString("yyyy-MM-dd"));
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":sexe", sexe);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":id_superviseur", id_superviseur.isEmpty() ? QVariant() : QVariant(id_superviseur));

    bool result = query.exec();
    if (!result) {
        qDebug() << "❌ Erreur ajouter:" << query.lastError().text();
        qDebug() << "   SQL:" << query.lastQuery();
    } else {
        qDebug() << "✅ Employé ajouté avec succès";
    }
    return result;
}

QSqlQueryModel* Employes::afficher()
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    if (!tableName.isEmpty()) {
        QString sql = QString("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR, "
                              "COALESCE(CODE_BADGE, 'Non assigné') FROM %1 ORDER BY IDEMPLOYE").arg(tableName);
        model->setQuery(sql);
    }
    return model;
}

bool Employes::supprimer(int id_emp)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) return false;

    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE IDEMPLOYE = :id").arg(tableName));
    query.bindValue(":id", id_emp);
    return query.exec();
}

bool Employes::modifier(int id_emp, QString email, QString telephone, QString poste,
                        double salaire, QString id_superviseur, QString code_badge)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) return false;

    QSqlQuery query;
    QString sql = QString("UPDATE %1 SET EMAIL = :email, TELEPHONE = :telephone, POSTE = :poste, "
                          "SALAIRE = :salaire, IDSUPERVISEUR = :id_superviseur, CODE_BADGE = :code_badge "
                          "WHERE IDEMPLOYE = :id").arg(tableName);

    query.prepare(sql);
    query.bindValue(":id", id_emp);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":id_superviseur", id_superviseur.isEmpty() ? QVariant() : QVariant(id_superviseur));
    query.bindValue(":code_badge", code_badge.isEmpty() ? QVariant() : QVariant(code_badge));

    bool result = query.exec();
    if (!result) {
        qDebug() << "❌ Erreur modifier:" << query.lastError().text();
    }
    return result;
}

// ==================== RECHERCHE ET TRI ====================

QSqlQueryModel* Employes::rechercherMultiCriteres(const QString &text)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    if (!tableName.isEmpty()) {
        QString sql = QString("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR, "
                              "COALESCE(CODE_BADGE, 'Non assigné') FROM %1 "
                              "WHERE PRENOM LIKE :text OR EMAIL LIKE :text OR TELEPHONE LIKE :text "
                              "ORDER BY IDEMPLOYE").arg(tableName);

        QSqlQuery query;
        query.prepare(sql);
        query.bindValue(":text", "%" + text + "%");
        query.exec();
        model->setQuery(query);
    }
    return model;
}

QSqlQueryModel* Employes::trierParSalaire(bool croissant)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    if (!tableName.isEmpty()) {
        QString ordre = croissant ? "ASC" : "DESC";
        QString sql = QString("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR, "
                              "COALESCE(CODE_BADGE, 'Non assigné') FROM %1 ORDER BY SALAIRE %2").arg(tableName).arg(ordre);
        model->setQuery(sql);
    }
    return model;
}

void Employes::getStatistiquesSalaires(QVector<int> &valeurs, QStringList &labels)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) return;

    QSqlQuery query(QString("SELECT PRENOM, SALAIRE FROM %1 ORDER BY SALAIRE DESC LIMIT 10").arg(tableName));

    valeurs.clear();
    labels.clear();

    while (query.next()) {
        labels << query.value(0).toString();
        valeurs << query.value(1).toInt();
    }
}

// ==================== VALIDATIONS ====================

bool Employes::validerEmail(const QString& email)
{
    if (email.isEmpty()) return false;
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return regex.match(email).hasMatch();
}

bool Employes::validerTelephone(const QString& telephone)
{
    if (telephone.isEmpty()) return false;
    QRegularExpression regex("^\\d{8}$");
    return regex.match(telephone).hasMatch();
}

bool Employes::validerPrenom(const QString& prenom)
{
    return !prenom.isEmpty();
}

bool Employes::validerSalaire(double salaire)
{
    return salaire >= 0.0;
}

bool Employes::validerDateEmbauche(const QDate& date)
{
    return date.isValid() && date <= QDate::currentDate();
}

bool Employes::emailExiste(const QString& email)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) return false;

    QSqlQuery query;
    query.prepare(QString("SELECT COUNT(*) FROM %1 WHERE EMAIL = :email").arg(tableName));
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool Employes::superviseurExiste(const QString& id)
{
    if (id.isEmpty()) return true;

    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) return false;

    QSqlQuery query;
    query.prepare(QString("SELECT COUNT(*) FROM %1 WHERE IDEMPLOYE = :id").arg(tableName));
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool Employes::validerLettres(const QString &text)
{
    if (text.isEmpty()) return false;
    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s'-]+$");
    return regex.match(text).hasMatch();
}

bool Employes::valider8Chiffres(const QString &text)
{
    QRegularExpression regex("^\\d{8}$");
    return regex.match(text).hasMatch();
}

bool Employes::validerFormatDate(const QString &text)
{
    QRegularExpression regex("^(0[1-9]|1[0-2])/(0[1-9]|[12][0-9]|3[01])/\\d{4}$");
    return regex.match(text).hasMatch();
}

bool Employes::validerSexe(const QString &sexe)
{
    return sexe.toUpper() == "M" || sexe.toUpper() == "F";
}

bool Employes::validerChiffres(const QString &text)
{
    QRegularExpression regex("^\\d+$");
    return regex.match(text).hasMatch();
}

// ==================== GESTION BADGE ARDUINO ====================

bool Employes::verifierCodeBadge(const QString &code)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) return false;

    QSqlQuery query;
    query.prepare(QString("SELECT COUNT(*) FROM %1 WHERE CODE_BADGE = :code").arg(tableName));
    query.bindValue(":code", code.trimmed());

    if (query.exec() && query.next()) {
        bool exists = query.value(0).toInt() > 0;
        qDebug() << "Vérification code" << code << ":" << (exists ? "TROUVÉ" : "NON TROUVÉ");
        return exists;
    }

    qDebug() << "Erreur requête vérification:" << query.lastError().text();
    return false;
}

bool Employes::codeBadgeExiste(const QString &code)
{
    return verifierCodeBadge(code);
}

bool Employes::ajouterCodeBadge(int idEmploye, const QString &code)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) return false;

    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET CODE_BADGE = :code WHERE IDEMPLOYE = :id").arg(tableName));
    query.bindValue(":code", code);
    query.bindValue(":id", idEmploye);

    bool result = query.exec();
    if (!result) {
        qDebug() << "❌ Erreur ajout code badge:" << query.lastError().text();
    }
    return result;
}

bool Employes::supprimerCodeBadge(int idEmploye)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    if (tableName.isEmpty()) return false;

    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET CODE_BADGE = NULL WHERE IDEMPLOYE = :id").arg(tableName));
    query.bindValue(":id", idEmploye);

    return query.exec();
}

QSqlQueryModel* Employes::rechercherParCodeBadge(const QString &code)
{
    QSqlDatabase db = QSqlDatabase::database();
    QString tableName = "";

    if (db.driverName().contains("ORACLE", Qt::CaseInsensitive)) {
        QSqlQuery checkQuery("SELECT TABLE_NAME FROM USER_TABLES WHERE TABLE_NAME IN ('EMPOLYE', 'EMPLOYE')");
        if (checkQuery.next()) {
            tableName = checkQuery.value(0).toString();
        }
    } else {
        QSqlQuery listQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (listQuery.next()) {
            QString name = listQuery.value(0).toString();
            if (name.toUpper() == "EMPOLYE" || name.toUpper() == "EMPLOYE") {
                tableName = name;
                break;
            }
        }
    }

    QSqlQueryModel *model = new QSqlQueryModel;
    if (!tableName.isEmpty()) {
        QString sql = QString("SELECT IDEMPLOYE, PRENOM, EMAIL, CODE_BADGE FROM %1 WHERE CODE_BADGE = :code").arg(tableName);
        QSqlQuery query;
        query.prepare(sql);
        query.bindValue(":code", code.trimmed());
        query.exec();
        model->setQuery(query);

        qDebug() << "Recherche employé pour code" << code << "- Résultats:" << model->rowCount();
    }
    return model;
}

QSqlQueryModel* Employes::getHistoriqueAcces()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString sql = "SELECT ID_EMPLOYE, NOM_EMPLOYE, "
                  "strftime('%d/%m/%Y %H:%M:%S', HEURE_ACCES), TYPE_ACCES "
                  "FROM HISTORIQUE_ACCES ORDER BY HEURE_ACCES DESC";
    model->setQuery(sql);
    return model;
}

bool Employes::enregistrerAcces(int idEmploye, const QString &nom, const QDateTime &timestamp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO HISTORIQUE_ACCES (ID_EMPLOYE, NOM_EMPLOYE, HEURE_ACCES, TYPE_ACCES) "
                  "VALUES (:id, :nom, :heure, :type)");
    query.bindValue(":id", idEmploye);
    query.bindValue(":nom", nom);
    query.bindValue(":heure", timestamp.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":type", "ENTREE");

    bool result = query.exec();
    if (!result) {
        qDebug() << "❌ Erreur enregistrement accès:" << query.lastError().text();
    } else {
        qDebug() << "✅ Accès enregistré pour" << nom;
    }
    return result;
}
