#include "employes.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>

Employes::Employes()
    : id(-1), prenom(""), date_embauche(QDate::currentDate()), email(""),
    telephone(""), sexe("M"), poste(""), salaire(0.0), id_superviseur("")
{
}

Employes::Employes(int id, QString prenom, QDate date_embauche, QString email,
                   QString telephone, QString sexe, QString poste, double salaire, QString id_superviseur)
    : id(id), prenom(prenom), date_embauche(date_embauche), email(email),
    telephone(telephone), sexe(sexe), poste(poste), salaire(salaire), id_superviseur(id_superviseur)
{
}

// Getters
int Employes::getId() { return id; }
QString Employes::getPrenom() { return prenom; }
QDate Employes::getDateEmbauche() { return date_embauche; }
QString Employes::getEmail() { return email; }
QString Employes::getTelephone() { return telephone; }
QString Employes::getSexe() { return sexe; }
QString Employes::getPoste() { return poste; }
double Employes::getSalaire() { return salaire; }
QString Employes::getIdSuperviseur() { return id_superviseur; }

// Setters
void Employes::setId(int i) { id = i; }
void Employes::setPrenom(QString p) { prenom = p; }
void Employes::setDateEmbauche(QDate d) { date_embauche = d; }
void Employes::setEmail(QString e) { email = e; }
void Employes::setTelephone(QString t) { telephone = t; }
void Employes::setSexe(QString s) { sexe = s; }
void Employes::setPoste(QString p) { poste = p; }
void Employes::setSalaire(double s) { salaire = s; }
void Employes::setIdSuperviseur(QString id) { id_superviseur = id; }

// CRUD
bool Employes::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYES (PRENOM, DATEDENBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR) "
                  "VALUES (:prenom, :date_embauche, :email, :telephone, :sexe, :poste, :salaire, :id_superviseur)");

    query.bindValue(":prenom", prenom);
    query.bindValue(":date_embauche", date_embauche);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":sexe", sexe);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":id_superviseur", id_superviseur.isEmpty() ? QVariant() : QVariant(id_superviseur));

    bool result = query.exec();
    if (!result) {
        qDebug() << "✗ Erreur ajouter:" << query.lastError().text();
    }
    return result;
}

QSqlQueryModel* Employes::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query("SELECT IDEMPLOTE, PRENOM, DATEDENBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM EMPLOYES");
    model->setQuery(query);
    return model;
}

bool Employes::supprimer(int id_emp)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYES WHERE IDEMPLOTE = :id");
    query.bindValue(":id", id_emp);
    return query.exec();
}

bool Employes::modifier(int id_emp, QString email, QString telephone, QString poste, double salaire, QString id_superviseur)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYES SET EMAIL = :email, TELEPHONE = :telephone, POSTE = :poste, SALAIRE = :salaire, IDSUPERVISEUR = :id_superviseur WHERE IDEMPLOTE = :id");

    query.bindValue(":id", id_emp);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":id_superviseur", id_superviseur.isEmpty() ? QVariant() : QVariant(id_superviseur));

    bool result = query.exec();
    if (!result) {
        qDebug() << "✗ Erreur modifier:" << query.lastError().text();
    }
    return result;
}

// Validations
bool Employes::validerEmail(const QString& email)
{
    if (email.isEmpty()) return true;
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return regex.match(email).hasMatch();
}

bool Employes::validerTelephone(const QString& telephone)
{
    if (telephone.isEmpty()) return true;
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
    return date.isValid();
}

bool Employes::emailExiste(const QString& email)
{
    if (email.isEmpty()) return false;
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPLOYES WHERE EMAIL = :email");
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
    query.prepare("SELECT COUNT(*) FROM EMPLOYES WHERE IDEMPLOTE = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}
