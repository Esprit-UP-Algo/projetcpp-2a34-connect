#include "employes.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QMessageBox>

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
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non ouverte!";
        return false;
    }

    QSqlQuery query(db);

    // 1. D'abord essayer d'obtenir le prochain ID avec MAX+1 (plus fiable)
    query.exec("SELECT NVL(MAX(IDEMPLOYE), 0) + 1 FROM EMPOLYE");
    if (query.next()) {
        id = query.value(0).toInt();
        qDebug() << "📝 ID généré via MAX+1:" << id;
    } else {
        // En cas d'erreur, utiliser une séquence
        query.exec("SELECT EMPOLYE_SEQ.NEXTVAL FROM DUAL");
        if (query.next()) {
            id = query.value(0).toInt();
            qDebug() << "📝 ID généré via séquence:" << id;
        } else {
            qDebug() << "❌ Impossible de générer un ID!";
            return false;
        }
    }

    QString sql =
        "INSERT INTO EMPOLYE (IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, "
        "SEXE, POSTE, SALAIRE, IDSUPERVISEUR, CODE_BADGE) "
        "VALUES (:id, :prenom, TO_DATE(:date_embauche, 'DD/MM/YYYY'), :email, "
        ":telephone, :sexe, :poste, :salaire, :id_superviseur, :code_badge)";

    query.prepare(sql);
    query.bindValue(":id", id);
    query.bindValue(":prenom", prenom);
    query.bindValue(":date_embauche", date_embauche.toString("dd/MM/yyyy"));
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":sexe", sexe);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":id_superviseur", id_superviseur.isEmpty() ? QVariant() : id_superviseur);
    query.bindValue(":code_badge", code_badge.isEmpty() ? QVariant() : code_badge);

    bool result = query.exec();

    if (result) {
        qDebug() << "✅ Employé ajouté avec succès";
        qDebug() << "   ID:" << id;
        qDebug() << "   Nom:" << prenom;
        qDebug() << "   Code badge:" << (code_badge.isEmpty() ? "NULL" : code_badge);

        // Vérification immédiate
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT IDEMPLOYE, PRENOM, CODE_BADGE FROM EMPOLYE WHERE IDEMPLOYE = :id");
        checkQuery.bindValue(":id", id);
        if (checkQuery.exec() && checkQuery.next()) {
            qDebug() << "📋 Vérification BD: ID=" << checkQuery.value(0).toInt()
                << ", Nom=" << checkQuery.value(1).toString()
                << ", Badge=" << checkQuery.value(2).toString();
        }
    } else {
        qDebug() << "❌ Erreur ajout employé:" << query.lastError().text();
        qDebug() << "   Requête:" << query.lastQuery();
    }

    return result;
}
QSqlQueryModel* Employes::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;

    // Requête simplifiée et corrigée
    QString sql =
        "SELECT "
        "IDEMPLOYE AS \"ID\", "
        "PRENOM AS \"Nom Complet\", "
        "TO_CHAR(DATEDEMBAUCHE, 'DD/MM/YYYY') AS \"Date Embauche\", "
        "EMAIL AS \"Email\", "
        "TELEPHONE AS \"Téléphone\", "
        "SEXE AS \"Sexe\", "
        "POSTE AS \"Poste\", "
        "SALAIRE AS \"Salaire\", "
        "NVL(IDSUPERVISEUR, 'Aucun') AS \"Superviseur\", "
        "NVL(CODE_BADGE, 'Non assigné') AS \"Code Badge\" "
        "FROM EMPOLYE "
        "ORDER BY IDEMPLOYE";

    QSqlQuery query;
    if (query.exec(sql)) {
        model->setQuery(query);

        // Définir les en-têtes explicitement
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Complet"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Emb."));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tél"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Sexe"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Poste"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Superviseur"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Code Badge"));

        qDebug() << "📊 Affichage: " << model->rowCount() << " employés trouvés";
    } else {
        qDebug() << "❌ Erreur affichage:" << query.lastError().text();
        qDebug() << "   SQL:" << sql;
    }

    return model;
}

bool Employes::supprimer(int id_emp)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPOLYE WHERE IDEMPLOYE = :id");
    query.bindValue(":id", id_emp);

    bool result = query.exec();
    if (!result) {
        qDebug() << "❌ Erreur suppression:" << query.lastError().text();
    } else {
        qDebug() << "✅ Employé ID" << id_emp << "supprimé";
    }
    return result;
}

bool Employes::modifier(int id_emp, QString email, QString telephone, QString poste,
                        double salaire, QString id_superviseur, QString code_badge)
{
    QSqlQuery query;
    QString sql =
        "UPDATE EMPOLYE SET "
        "EMAIL = :email, "
        "TELEPHONE = :telephone, "
        "POSTE = :poste, "
        "SALAIRE = :salaire, "
        "IDSUPERVISEUR = :id_superviseur, "
        "CODE_BADGE = :code_badge "
        "WHERE IDEMPLOYE = :id";

    query.prepare(sql);
    query.bindValue(":id", id_emp);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":id_superviseur", id_superviseur.isEmpty() ? QVariant(QVariant::String) : id_superviseur);
    query.bindValue(":code_badge", code_badge.isEmpty() ? QVariant(QVariant::String) : code_badge);

    bool result = query.exec();
    if (!result) {
        qDebug() << "❌ Erreur modification:" << query.lastError().text();
        qDebug() << "   SQL:" << query.lastQuery();
    } else {
        qDebug() << "✅ Employé ID" << id_emp << "modifié";
        qDebug() << "   Nouveau code badge:" << (code_badge.isEmpty() ? "NULL" : code_badge);
    }
    return result;
}

// ==================== RECHERCHE ET TRI ====================

QSqlQueryModel* Employes::rechercherMultiCriteres(const QString &text)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    QString sql =
        "SELECT IDEMPLOYE, PRENOM, "
        "TO_CHAR(DATEDEMBAUCHE, 'DD/MM/YYYY'), "
        "EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, "
        "IDSUPERVISEUR, "
        "NVL(CODE_BADGE, 'Non assigné') "
        "FROM EMPOLYE "
        "WHERE PRENOM LIKE :text OR EMAIL LIKE :text OR TELEPHONE LIKE :text "
        "ORDER BY IDEMPLOYE";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":text", "%" + text + "%");
    query.exec();
    model->setQuery(query);

    return model;
}

QSqlQueryModel* Employes::trierParSalaire(bool croissant)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    QString ordre = croissant ? "ASC" : "DESC";
    QString sql =
        "SELECT IDEMPLOYE, PRENOM, "
        "TO_CHAR(DATEDEMBAUCHE, 'DD/MM/YYYY'), "
        "EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, "
        "IDSUPERVISEUR, "
        "NVL(CODE_BADGE, 'Non assigné') "
        "FROM EMPOLYE "
        "ORDER BY SALAIRE " + ordre;

    model->setQuery(sql);
    return model;
}

void Employes::getStatistiquesSalaires(QVector<int> &valeurs, QStringList &labels)
{
    QSqlQuery query(
        "SELECT PRENOM, SALAIRE FROM EMPOLYE "
        "WHERE ROWNUM <= 10 ORDER BY SALAIRE DESC");

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
    if (telephone.isEmpty()) return true; // Optionnel
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
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPOLYE WHERE EMAIL = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool Employes::superviseurExiste(const QString& id)
{
    if (id.isEmpty()) return true;

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPOLYE WHERE IDEMPLOYE = :id");
    query.bindValue(":id", id.toInt());

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

// ==================== GESTION BADGE ARDUINO ====================

bool Employes::verifierCodeBadge(const QString &code)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPOLYE WHERE CODE_BADGE = :code");
    query.bindValue(":code", code.trimmed());

    if (query.exec() && query.next()) {
        bool exists = query.value(0).toInt() > 0;
        qDebug() << "Vérification code" << code << ":" << (exists ? "TROUVÉ" : "NON TROUVÉ");
        return exists;
    }

    qDebug() << "Erreur vérification code badge:" << query.lastError().text();
    return false;
}

bool Employes::codeBadgeExiste(const QString &code)
{
    return verifierCodeBadge(code);
}

bool Employes::ajouterCodeBadge(int idEmploye, const QString &code)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPOLYE SET CODE_BADGE = :code WHERE IDEMPLOYE = :id");
    query.bindValue(":code", code);
    query.bindValue(":id", idEmploye);

    bool result = query.exec();
    if (result) {
        qDebug() << "✅ Code badge" << code << "assigné à l'employé ID" << idEmploye;

        // Vérification
        QSqlQuery check("SELECT PRENOM, CODE_BADGE FROM EMPOLYE WHERE IDEMPLOYE = :id");
        check.bindValue(":id", idEmploye);
        check.exec();
        if (check.next()) {
            qDebug() << "   Vérification - Nom:" << check.value(0).toString()
            << "| Badge:" << check.value(1).toString();
        }
    } else {
        qDebug() << "❌ Erreur ajout code badge:" << query.lastError().text();
    }
    return result;
}

bool Employes::supprimerCodeBadge(int idEmploye)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPOLYE SET CODE_BADGE = NULL WHERE IDEMPLOYE = :id");
    query.bindValue(":id", idEmploye);

    return query.exec();
}

QSqlQueryModel* Employes::rechercherParCodeBadge(const QString &code)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    QString sql =
        "SELECT IDEMPLOYE, PRENOM, EMAIL, CODE_BADGE "
        "FROM EMPOLYE WHERE CODE_BADGE = :code";

    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":code", code.trimmed());
    query.exec();
    model->setQuery(query);

    qDebug() << "Recherche employé pour code" << code << "- Résultats:" << model->rowCount();
    return model;
}

QSqlQueryModel* Employes::getHistoriqueAcces()
{
    QSqlQueryModel *model = new QSqlQueryModel;

    // Vérifier si la table existe
    QSqlQuery checkQuery("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'HISTORIQUE_ACCES'");
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QString sql =
            "SELECT ID_EMPLOYE, NOM_EMPLOYE, "
            "TO_CHAR(HEURE_ACCES, 'DD/MM/YYYY HH24:MI:SS'), TYPE_ACCES "
            "FROM HISTORIQUE_ACCES ORDER BY HEURE_ACCES DESC";
        model->setQuery(sql);
    }

    return model;
}

bool Employes::enregistrerAcces(int idEmploye, const QString &nom, const QDateTime &timestamp)
{
    // Vérifier/Créer la table si nécessaire
    QSqlQuery checkQuery("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'HISTORIQUE_ACCES'");
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        QSqlQuery createQuery(
            "CREATE TABLE HISTORIQUE_ACCES ("
            "ID_ACCES NUMBER PRIMARY KEY, "
            "ID_EMPLOYE NUMBER, "
            "NOM_EMPLOYE VARCHAR2(50), "
            "HEURE_ACCES DATE, "
            "TYPE_ACCES VARCHAR2(20))");
        qDebug() << "Table HISTORIQUE_ACCES créée";
    }

    QSqlQuery query;
    query.prepare(
        "INSERT INTO HISTORIQUE_ACCES (ID_ACCES, ID_EMPLOYE, NOM_EMPLOYE, HEURE_ACCES, TYPE_ACCES) "
        "VALUES (HISTORIQUE_SEQ.NEXTVAL, :id, :nom, :heure, :type)");
    query.bindValue(":id", idEmploye);
    query.bindValue(":nom", nom);
    query.bindValue(":heure", timestamp);
    query.bindValue(":type", "ENTREE");

    bool result = query.exec();
    if (!result) {
        // Essayer sans séquence si elle n'existe pas
        QSqlQuery maxQuery("SELECT NVL(MAX(ID_ACCES), 0) + 1 FROM HISTORIQUE_ACCES");
        if (maxQuery.exec() && maxQuery.next()) {
            int nextId = maxQuery.value(0).toInt();

            QSqlQuery query2;
            query2.prepare(
                "INSERT INTO HISTORIQUE_ACCES (ID_ACCES, ID_EMPLOYE, NOM_EMPLOYE, HEURE_ACCES, TYPE_ACCES) "
                "VALUES (:id_acces, :id, :nom, :heure, :type)");
            query2.bindValue(":id_acces", nextId);
            query2.bindValue(":id", idEmploye);
            query2.bindValue(":nom", nom);
            query2.bindValue(":heure", timestamp);
            query2.bindValue(":type", "ENTREE");

            result = query2.exec();
        }
    }

    if (result) {
        qDebug() << "✅ Accès enregistré pour" << nom;
    } else {
        qDebug() << "❌ Erreur enregistrement accès:" << query.lastError().text();
    }
    return result;
}
// ==================== VALIDATIONS SUPPLEMENTAIRES ====================

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
