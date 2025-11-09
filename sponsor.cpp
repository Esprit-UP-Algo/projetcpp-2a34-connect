#include "sponsor.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QSqlError>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

// ==================== MÉTHODES DE VALIDATION ====================

bool Sponsor::validerNom(const QString &nom)
{
    if (nom.isEmpty() || nom.length() > 50) {
        qDebug() << "Nom invalide: vide ou trop long";
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s\\-']+$");
    if (!regex.match(nom).hasMatch()) {
        qDebug() << "Nom contient des caractères invalides";
        return false;
    }

    return true;
}

bool Sponsor::validerPrenom(const QString &prenom)
{
    if (prenom.isEmpty() || prenom.length() > 50) {
        qDebug() << "Prénom invalide: vide ou trop long";
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s\\-']+$");
    if (!regex.match(prenom).hasMatch()) {
        qDebug() << "Prénom contient des caractères invalides";
        return false;
    }

    return true;
}

bool Sponsor::validerEmail(const QString &email)
{
    if (email.isEmpty()) {
        return true; // Email optionnel
    }

    if (email.length() > 100) {
        qDebug() << "Email trop long";
        return false;
    }

    // Validation STRICTE d'email avec domaine
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.(com|net|org|edu|gov|fr|tn|ma|dz)$");
    if (!regex.match(email).hasMatch()) {
        qDebug() << "Format d'email invalide - doit contenir @ et se terminer par .com, .fr, etc.";
        return false;
    }

    return true;
}

bool Sponsor::validerCategorie(const QString &categorie)
{
    if (categorie.isEmpty() || categorie.length() > 50) {
        qDebug() << "Catégorie invalide: vide ou trop longue";
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ0-9\\s\\-&]+$");
    if (!regex.match(categorie).hasMatch()) {
        qDebug() << "Catégorie contient des caractères invalides";
        return false;
    }

    return true;
}

bool Sponsor::validerBudget(int budget)
{
    if (budget < 0 || budget > 10000000) {
        qDebug() << "Budget invalide:" << budget;
        return false;
    }

    return true;
}

bool Sponsor::validerDates(const QDate &debut, const QDate &fin)
{
    QDate today = QDate::currentDate();

    // Vérifier que la date de début n'est pas dans le passé
    if (debut < today) {
        qDebug() << "Date de début dans le passé";
        return false;
    }

    // CORRECTION: Vérifier que la date de début est AVANT la date de fin
    if (debut >= fin) {
        qDebug() << "Date de début doit être STRICTEMENT AVANT date de fin";
        return false;
    }

    // Vérifier que le contrat ne dépasse pas 5 ans
    if (debut.daysTo(fin) > 365 * 5) {
        qDebug() << "Durée du contrat trop longue (max 5 ans)";
        return false;
    }

    return true;
}

bool Sponsor::validerID(int id)
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

bool Sponsor::validerID(const QString &idStr)
{
    if (idStr.isEmpty()) {
        qDebug() << "ID vide";
        return false;
    }

    bool ok;
    int id = idStr.toInt(&ok);
    if (!ok) {
        qDebug() << "ID doit être un nombre";
        return false;
    }

    return validerID(id);
}

bool Sponsor::validerCIN(const QString &cin)
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

bool Sponsor::validerTelephone(const QString &telephone)
{
    if (telephone.isEmpty()) {
        return true; // Téléphone optionnel
    }

    // Vérifier que c'est un nombre
    bool ok;
    telephone.toLongLong(&ok);
    if (!ok) {
        qDebug() << "Téléphone doit être un nombre";
        return false;
    }

    // Vérifier la longueur (8 à 15 chiffres)
    if (telephone.length() < 8 || telephone.length() > 15) {
        qDebug() << "Téléphone doit avoir entre 8 et 15 chiffres";
        return false;
    }

    return true;
}

bool Sponsor::validerSalaire(double salaire)
{
    if (salaire < 0 || salaire > 100000) {
        qDebug() << "Salaire invalide:" << salaire;
        return false;
    }

    return true;
}

// ==================== CONSTRUCTEURS ====================

Sponsor::Sponsor()
{
    nom = "";
    prenom = "";
    categorie = "";
    email = "";
    budget = 0;
}

Sponsor::Sponsor(QString nom, QString prenom, QString categorie, QString email, QDate debutcontrat, QDate fincontrat, int budget)
{
    this->nom = nom;
    this->prenom = prenom;
    this->categorie = categorie;
    this->email = email;
    this->debutcontrat = debutcontrat;
    this->fincontrat = fincontrat;
    this->budget = budget;
}

// ==================== MÉTHODES CRUD ====================

bool Sponsor::ajouter()
{
    // Contrôle de saisie avant l'ajout
    if (!validerNom(nom)) {
        qDebug() << "Erreur: Nom invalide";
        return false;
    }

    if (!validerPrenom(prenom)) {
        qDebug() << "Erreur: Prénom invalide";
        return false;
    }

    if (!validerEmail(email)) {
        qDebug() << "Erreur: Email invalide";
        return false;
    }

    if (!validerCategorie(categorie)) {
        qDebug() << "Erreur: Catégorie invalide";
        return false;
    }

    if (!validerBudget(budget)) {
        qDebug() << "Erreur: Budget invalide";
        return false;
    }

    if (!validerDates(debutcontrat, fincontrat)) {
        qDebug() << "Erreur: Dates invalides";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO sponsor (NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET) "
                  "VALUES (:NOM, :PRENOM, :CATEGORIE, :EMAIL, :DEBUTCONTRAT, :FINCONTRAT, :BUDGET)");

    query.bindValue(":NOM", nom);
    query.bindValue(":PRENOM", prenom);
    query.bindValue(":CATEGORIE", categorie);
    query.bindValue(":EMAIL", email);
    query.bindValue(":DEBUTCONTRAT", debutcontrat);
    query.bindValue(":FINCONTRAT", fincontrat);
    query.bindValue(":BUDGET", budget);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Erreur ajout sponsor:" << query.lastError().text();
    }
    return result;
}

bool Sponsor::modifier(int idsponsor, QString nom, QString prenom, QString categorie, QString email, QDate debutcontrat, QDate fincontrat, int budget)
{
    // Contrôle de saisie avant la modification
    if (!validerID(idsponsor)) {
        qDebug() << "Erreur: ID sponsor invalide";
        return false;
    }

    if (!validerNom(nom)) {
        qDebug() << "Erreur: Nom invalide";
        return false;
    }

    if (!validerPrenom(prenom)) {
        qDebug() << "Erreur: Prénom invalide";
        return false;
    }

    if (!validerEmail(email)) {
        qDebug() << "Erreur: Email invalide";
        return false;
    }

    if (!validerCategorie(categorie)) {
        qDebug() << "Erreur: Catégorie invalide";
        return false;
    }

    if (!validerBudget(budget)) {
        qDebug() << "Erreur: Budget invalide";
        return false;
    }

    if (!validerDates(debutcontrat, fincontrat)) {
        qDebug() << "Erreur: Dates invalides";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE sponsor SET nom=:nom, prenom=:prenom, categorie=:categorie, email=:email, debutcontrat=:debutcontrat, fincontrat=:fincontrat, budget=:budget WHERE IDSPONSOR=:idsponsor");

    query.bindValue(":idsponsor", idsponsor);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":categorie", categorie);
    query.bindValue(":email", email);
    query.bindValue(":debutcontrat", debutcontrat);
    query.bindValue(":fincontrat", fincontrat);
    query.bindValue(":budget", budget);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Erreur modification sponsor:" << query.lastError().text();
    }
    return result;
}

QSqlQueryModel *Sponsor::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM sponsor");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Début Contrat"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Fin Contrat"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Budget"));
    return model;
}

bool Sponsor::supprimer(int id)
{
    if (!validerID(id)) {
        qDebug() << "Erreur: ID invalide pour suppression";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM sponsor WHERE idsponsor=:id");
    query.bindValue(":id", id);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Erreur suppression sponsor:" << query.lastError().text();
    }
    return result;
}

QSqlQueryModel *Sponsor::trierParNom()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM sponsor ORDER BY NOM ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Début Contrat"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Fin Contrat"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Budget"));
    return model;
}

QSqlQueryModel *Sponsor::rechercherParNom(QString nom)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    query.prepare("SELECT * FROM sponsor WHERE UPPER(NOM) LIKE UPPER(:nom)");
    query.bindValue(":nom", "%" + nom + "%");

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    }
    return model;
}

QSqlQueryModel *Sponsor::statistiques()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(
        "SELECT "
        "CATEGORIE, "
        "COUNT(*) as Nombre, "
        "SUM(BUDGET) as Budget_Total, "
        "AVG(BUDGET) as Budget_Moyen "
        "FROM sponsor "
        "GROUP BY CATEGORIE "
        "ORDER BY CATEGORIE"
        );

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Catégorie"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Budget Total"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Budget Moyen"));
    return model;
}

QSqlQueryModel *Sponsor::getEmployesSponsor(int idSponsor)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;

    query.exec("SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'SPONSOR_EMPLOYE'");
    query.next();
    if (query.value(0).toInt() == 0) {
        qDebug() << "Table SPONSOR_EMPLOYE n'existe pas";
        return model;
    }

    query.prepare("SELECT e.IDEMPLOTE, e.PRENOM, e.POSTE, e.EMAIL "
                  "FROM EMPLOYES e "
                  "INNER JOIN SPONSOR_EMPLOYE se ON e.IDEMPLOTE = se.ID_EMPLOYE "
                  "WHERE se.ID_SPONSOR = :id_sponsor");
    query.bindValue(":id_sponsor", idSponsor);

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Employé"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Poste"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    } else {
        qDebug() << "Erreur getEmployesSponsor:" << query.lastError().text();
    }

    return model;
}

bool Sponsor::lierEmploye(int idSponsor, int idEmploye)
{
    if (!validerID(idSponsor)) {
        qDebug() << "Erreur: ID sponsor invalide pour liaison";
        return false;
    }

    QSqlQuery checkEmploye;
    checkEmploye.prepare("SELECT COUNT(*) FROM EMPLOYES WHERE IDEMPLOTE = :id");
    checkEmploye.bindValue(":id", idEmploye);

    if (!checkEmploye.exec() || !checkEmploye.next() || checkEmploye.value(0).toInt() == 0) {
        qDebug() << "Erreur: ID employé invalide pour liaison";
        return false;
    }

    QSqlQuery query;

    query.exec(
        "IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'SPONSOR_EMPLOYE') "
        "CREATE TABLE SPONSOR_EMPLOYE ("
        "ID_LIAISON INT PRIMARY KEY IDENTITY(1,1), "
        "ID_SPONSOR INT, "
        "ID_EMPLOYE INT, "
        "DATE_LIAISON DATETIME DEFAULT GETDATE(), "
        "FOREIGN KEY (ID_SPONSOR) REFERENCES sponsor(IDSPONSOR) ON DELETE CASCADE, "
        "FOREIGN KEY (ID_EMPLOYE) REFERENCES EMPLOYES(IDEMPLOTE) ON DELETE CASCADE)"
        );

    query.prepare("INSERT INTO SPONSOR_EMPLOYE (ID_SPONSOR, ID_EMPLOYE) VALUES (:id_sponsor, :id_employe)");
    query.bindValue(":id_sponsor", idSponsor);
    query.bindValue(":id_employe", idEmploye);

    bool result = query.exec();
    if (!result) {
        qDebug() << "Erreur lierEmploye:" << query.lastError().text();
    }
    return result;
}
