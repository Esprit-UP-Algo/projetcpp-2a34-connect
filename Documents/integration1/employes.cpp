#include "employes.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QDate>
#include <QVariant>

// ==================== CONSTRUCTEURS ====================

Employes::Employes()
    : id(0), prenom(""), date_embauche(QDate::currentDate()), email(""),
    telephone(""), sexe(""), poste(""), salaire(0.0), id_superviseur("")
{
}

// Constructeur compatible avec votre base actuelle (Table 'empolye')
// Note : Le champ 'prenom' stocke ici le "Nom Complet" (Nom + Prénom)
Employes::Employes(int id, QString prenom, QDate date_embauche, QString email,
                   QString telephone, QString sexe, QString poste, double salaire, QString id_superviseur)
    : id(id), prenom(prenom), date_embauche(date_embauche), email(email),
    telephone(telephone), sexe(sexe), poste(poste), salaire(salaire), id_superviseur(id_superviseur)
{
}

// ==================== GETTERS ====================
int Employes::getId() const { return id; }
QString Employes::getPrenom() const { return prenom; }
QDate Employes::getDateEmbauche() const { return date_embauche; }
QString Employes::getEmail() const { return email; }
QString Employes::getTelephone() const { return telephone; }
QString Employes::getSexe() const { return sexe; }
QString Employes::getPoste() const { return poste; }
double Employes::getSalaire() const { return salaire; }
QString Employes::getIdSuperviseur() const { return id_superviseur; }

// ==================== SETTERS ====================
void Employes::setId(int i) { id = i; }
void Employes::setPrenom(const QString &p) { prenom = p; }
void Employes::setDateEmbauche(const QDate &d) { date_embauche = d; }
void Employes::setEmail(const QString &e) { email = e; }
void Employes::setTelephone(const QString &t) { telephone = t; }
void Employes::setSexe(const QString &s) { sexe = s; }
void Employes::setPoste(const QString &p) { poste = p; }
void Employes::setSalaire(double s) { salaire = s; }
void Employes::setIdSuperviseur(const QString &id) { id_superviseur = id; }

// ==================== CRUD ====================

bool Employes::ajouter()
{
    QSqlQuery query;
    // Table 'empolye' et colonnes conformes à main.cpp
    query.prepare("INSERT INTO empolye (PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR) "
                  "VALUES (:prenom, :date, :email, :tel, :sexe, :poste, :salaire, :sup)");

    query.bindValue(":prenom", prenom);
    query.bindValue(":date", date_embauche);
    query.bindValue(":email", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":sexe", sexe);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":sup", id_superviseur);

    if (query.exec()) {
        qDebug() << "✅ Employé ajouté avec succès.";
        return true;
    } else {
        qDebug() << "❌ Erreur ajout employé :" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Employes::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye");

    // En-têtes pour un affichage propre dans le tableau
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom (Nom)"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Emb."));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tél"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Superviseur"));

    return model;
}

bool Employes::modifier(int id_emp, QString email, QString telephone, QString poste, double salaire, QString id_superviseur)
{
    QSqlQuery query;
    query.prepare("UPDATE empolye SET EMAIL = :email, TELEPHONE = :tel, POSTE = :poste, SALAIRE = :salaire, IDSUPERVISEUR = :sup WHERE IDEMPLOYE = :id");

    query.bindValue(":id", id_emp);
    query.bindValue(":email", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":sup", id_superviseur);

    if (query.exec()) {
        qDebug() << "✅ Employé modifié avec succès.";
        return true;
    } else {
        qDebug() << "❌ Erreur modification employé :" << query.lastError().text();
        return false;
    }
}

bool Employes::supprimer(int id_emp)
{
    QSqlQuery query;
    query.prepare("DELETE FROM empolye WHERE IDEMPLOYE = :id");
    query.bindValue(":id", id_emp);

    if (query.exec()) {
        qDebug() << "✅ Employé supprimé avec succès.";
        return true;
    } else {
        qDebug() << "❌ Erreur suppression employé :" << query.lastError().text();
        return false;
    }
}

// ==================== RECHERCHE ET TRI AVANCÉS ====================

QSqlQueryModel* Employes::trierParSalaire(bool croissant)
{
    QSqlQueryModel *model = new QSqlQueryModel;

    // Choix dynamique de l'ordre (ASC ou DESC)
    QString ordre = croissant ? "ASC" : "DESC";

    model->setQuery("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR "
                    "FROM empolye ORDER BY SALAIRE " + ordre);

    // On remet les en-têtes pour garder le tableau propre
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom (Nom)"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Emb."));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tél"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Superviseur"));

    return model;
}

QSqlQueryModel* Employes::rechercherMultiCriteres(QString recherche)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;

    // Si la recherche est vide, on retourne nullptr pour signaler d'afficher tout
    if (recherche.trimmed().isEmpty()) {
        delete model;
        return nullptr;
    }

    // Requête intelligente : Cherche dans PRENOM (Nom+Prénom) OU TELEPHONE (CIN) OU ID
    query.prepare("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR "
                  "FROM empolye "
                  "WHERE UPPER(PRENOM) LIKE UPPER(:txt) "   // Cherche Nom/Prénom (insensible à la casse)
                  "OR TELEPHONE LIKE :txt "                 // Cherche dans Téléphone/CIN
                  "OR CAST(IDEMPLOYE AS CHAR) LIKE :txt");  // Cherche dans l'ID

    query.bindValue(":txt", "%" + recherche + "%");

    if (query.exec()) {
        model->setQuery(std::move(query));

        // Réappliquer les en-têtes
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom (Nom)"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date Emb."));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Tél"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Sexe"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Poste"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Superviseur"));
    }
    return model;
}

// Pour compatibilité si l'ancienne méthode est appelée quelque part
QSqlQueryModel* Employes::rechercherParPrenom(QString prenom)
{
    return rechercherMultiCriteres(prenom);
}

// ==================== STATISTIQUES (Bar Chart) ====================

void Employes::getStatistiquesSalaires(QVector<int> &valeurs, QStringList &etiquettes)
{
    valeurs.clear();
    etiquettes.clear();

    // Initialisation : 5 tranches de salaires
    // 0: <1000, 1: 1000-2000, 2: 2000-3000, 3: 3000-4000, 4: >4000
    QVector<int> compteurs(5, 0);

    QSqlQuery query("SELECT SALAIRE FROM empolye");
    while (query.next()) {
        double s = query.value(0).toDouble();

        if (s < 1000) compteurs[0]++;
        else if (s < 2000) compteurs[1]++;
        else if (s < 3000) compteurs[2]++;
        else if (s < 4000) compteurs[3]++;
        else compteurs[4]++;
    }

    valeurs = compteurs;
    etiquettes << "< 1000" << "1000-2000" << "2000-3000" << "3000-4000" << "> 4000";
}

// ==================== VALIDATIONS STRICTES (STATIQUES) ====================

bool Employes::validerLettres(const QString &texte)
{
    // Vérifie la longueur et les caractères (Lettres Unicode, espaces, tirets, apostrophes)
    if (texte.trimmed().length() < 2) return false;
    QRegularExpression regex("^[\\p{L}\\s'-]+$");
    return regex.match(texte).hasMatch();
}

bool Employes::valider8Chiffres(const QString &texte)
{
    // Exactement 8 chiffres
    QRegularExpression regex("^\\d{8}$");
    return regex.match(texte).hasMatch();
}

bool Employes::validerChiffres(const QString &texte)
{
    // Chiffres uniquement (ou vide si autorisé)
    if (texte.isEmpty()) return true;
    QRegularExpression regex("^\\d+$");
    return regex.match(texte).hasMatch();
}

bool Employes::validerEmail(const QString &email)
{
    // Vérification basique de format
    return email.contains("@") && email.contains(".");
}

bool Employes::validerSexe(const QString &sexe)
{
    QString s = sexe.toUpper().trimmed();
    return (s == "M" || s == "F");
}

bool Employes::validerSalaire(double salaire)
{
    return salaire >= 0.0;
}

bool Employes::validerFormatDate(const QString &dateTexte)
{
    // Format MM/JJ/AAAA (Ex: 11/28/2025)
    // Regex explicite : (Mois 01-12) / (Jour 01-31) / (Année 4 chiffres)
    QRegularExpression regex("^(0[1-9]|1[0-2])/(0[1-9]|[12][0-9]|3[01])/\\d{4}$");

    if (!regex.match(dateTexte).hasMatch()) {
        return false; // Format incorrect
    }

    // Vérifie si c'est une date réelle (ex: pas le 30 Février)
    QDate date = QDate::fromString(dateTexte, "MM/dd/yyyy");
    return date.isValid();
}
