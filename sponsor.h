#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Sponsor
{
public:
    Sponsor();
    Sponsor(QString nom, QString prenom, QString categorie, QString email, QDate debutcontrat, QDate fincontrat, int budget);

    bool ajouter();
    bool modifier(int id, QString nom, QString prenom, QString categorie, QString email, QDate debutcontrat, QDate fincontrat, int budget);
    QSqlQueryModel *afficher();
    bool supprimer(int id);
    QSqlQueryModel *trierParNom();
    QSqlQueryModel *rechercherParNom(QString nom);
    QSqlQueryModel *statistiques();
    QSqlQueryModel *getEmployesSponsor(int idSponsor);
    bool lierEmploye(int idSponsor, int idEmploye);

    // Méthodes de validation
    static bool validerNom(const QString &nom);
    static bool validerPrenom(const QString &prenom);
    static bool validerEmail(const QString &email);
    static bool validerCategorie(const QString &categorie);
    static bool validerBudget(int budget);
    static bool validerDates(const QDate &debut, const QDate &fin);
    static bool validerID(int id);
    static bool validerID(const QString &idStr);
    static bool validerCIN(const QString &cin);
    static bool validerTelephone(const QString &telephone);
    static bool validerSalaire(double salaire);

private:
    int budget;
    QString nom, prenom, categorie, email;
    QDate debutcontrat, fincontrat;
};

#endif // SPONSOR_H
