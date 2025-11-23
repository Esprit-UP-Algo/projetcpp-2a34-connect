#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QTextDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>
#include <QTime>

class Sponsor
{
public:
    Sponsor();
    Sponsor(QString nom, QString prenom, QString categorie, QString email, QDate debutcontrat, QDate fincontrat, int budget);

    bool ajouter();
    bool modifier(int id, QString nom, QString prenom, QString categorie, QString email, QDate debutcontrat, QDate fincontrat, int budget);
    QSqlQueryModel *afficher();
    bool supprimer(int id);
    QSqlQueryModel *trierParDate();
    QSqlQueryModel *rechercherParNom(QString nom);
    QSqlQueryModel *statistiques();
    QSqlQueryModel *getEmployesSponsor(int idSponsor);
    bool lierEmploye(int idSponsor, int idEmploye);

    // MÉTHODES PDF
    static bool exporterPDF(const QString &nomFichier);
    static bool exporterPDF(const QString &nomFichier, QSqlQueryModel *model);
    static bool exporterHTML(const QString &nomFichier);

    // NOUVELLES MÉTHODES POUR EXPORT PAR CONTRAT
    static bool exporterPDFParContrat(const QString &nomFichier, const QString &filtreContrat = "tous");
    static QSqlQueryModel* filtrerParContrat(const QString &filtreContrat);
    static QStringList obtenirTypesContrat();

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

    // Méthodes helper pour générer le HTML
    static QString genererHTMLPourPDF();
    static QString genererHTMLPourPDF(QSqlQueryModel *model);
    static QString genererHTMLPourPDFParContrat(QSqlQueryModel *model, const QString &filtreContrat);
};

#endif // SPONSOR_H
