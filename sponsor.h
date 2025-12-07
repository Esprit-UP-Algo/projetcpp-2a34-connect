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
#include <QVector>   // <--- Nouveau : Nécessaire pour les graphiques
#include <QPointF>   // <--- Nouveau : Pour les points des courbes
#include <QStringList> // <--- Nouveau : Pour les labels des catégories

class Sponsor
{
public:
    // Constructeurs
    Sponsor();
    Sponsor(QString nom, QString prenom, QString categorie, QString email, QDate debutcontrat, QDate fincontrat, int budget);

    // CRUD
    bool ajouter();
    bool modifier(int id, QString nom, QString prenom, QString categorie, QString email, QDate debutcontrat, QDate fincontrat, int budget);
    bool supprimer(int id);
    QSqlQueryModel *afficher();

    // Recherche et Tri
    QSqlQueryModel *trierParDate();
    QSqlQueryModel *rechercherParNom(QString nom);

    // Relations
    QSqlQueryModel *getEmployesSponsor(int idSponsor);
    bool lierEmploye(int idSponsor, int idEmploye);

    // Statistiques SQL de base (Ancienne méthode, gardée pour compatibilité)
    QSqlQueryModel *statistiques();

    // --- NOUVEAU : MÉTHODES POUR LES STATISTIQUES GRAPHIQUES ---
    // Ces méthodes préparent les données pour CurveWidget et nettoient MainWindow
    static QVector<QPointF> getDonneesEvolutionContrats();
    static void getDonneesBudgetParCategorie(QVector<QPointF> &data, QStringList &labels);
    static void getDonneesRepartitionCategories(QVector<QPointF> &data, QStringList &labels);
    static void getDonneesDureeContrats(QVector<QPointF> &data);
    static void getResumeStatistiques(int &totalSponsors, double &totalBudget, double &avgBudget);
    // -----------------------------------------------------------

    // MÉTHODES EXPORT PDF
    static bool exporterPDF(const QString &nomFichier);
    static bool exporterPDF(const QString &nomFichier, QSqlQueryModel *model);
    static bool exporterHTML(const QString &nomFichier);

    // MÉTHODES EXPORT PAR TYPE DE CONTRAT
    static bool exporterPDFParContrat(const QString &nomFichier, const QString &filtreContrat = "tous");
    static QSqlQueryModel* filtrerParContrat(const QString &filtreContrat);
    static QStringList obtenirTypesContrat();

    // MÉTHODES DE VALIDATION
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

    // Méthodes internes pour générer le HTML
    static QString genererHTMLPourPDF();
    static QString genererHTMLPourPDF(QSqlQueryModel *model);
    static QString genererHTMLPourPDFParContrat(QSqlQueryModel *model, const QString &filtreContrat);
};

#endif // SPONSOR_H
