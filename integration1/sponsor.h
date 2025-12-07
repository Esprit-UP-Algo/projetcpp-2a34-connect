#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QVector>
#include <QPointF>
#include <QStringList>

class Sponsor
{
public:
    // Constructeurs
    Sponsor();
    Sponsor(QString nom, QString prenom, QString categorie, QString email,
            QDate debutcontrat, QDate fincontrat, int budget);

    // CRUD
    bool ajouter();
    bool modifier(int id, QString nom, QString prenom, QString categorie,
                  QString email, QDate debutcontrat, QDate fincontrat, int budget);
    bool supprimer(int id);
    QSqlQueryModel *afficher();

    // Recherche et Tri
    QSqlQueryModel *trierParDate();
    QSqlQueryModel *rechercherParNom(QString nom);

    // --- MÉTHODES POUR LES STATISTIQUES GRAPHIQUES ---
    static QVector<QPointF> getDonneesEvolutionContrats();
    static void getDonneesBudgetParCategorie(QVector<QPointF> &data, QStringList &labels);
    static void getDonneesRepartitionCategories(QVector<QPointF> &data, QStringList &labels);
    static void getDonneesDureeContrats(QVector<QPointF> &data);
    static void getResumeStatistiques(int &totalSponsors, double &totalBudget, double &avgBudget);

    // MÉTHODES EXPORT PDF
    static bool exporterPDF(const QString &nomFichier);
    static bool exporterPDF(const QString &nomFichier, QSqlQueryModel *model);
    static bool exporterPDFParContrat(const QString &nomFichier, const QString &filtre = "tous");
    static bool exporterHTMLParContrat(const QString &nomFichier, const QString &filtre = "tous");

    // MÉTHODES DE VALIDATION
    static bool validerNom(const QString &nom);
    static bool validerPrenom(const QString &prenom);
    static bool validerEmail(const QString &email);
    static bool validerCategorie(const QString &categorie);
    static bool validerBudget(int budget);
    static bool validerDates(const QDate &debut, const QDate &fin);
    static bool validerCIN(const QString &cin);
    static bool validerTelephone(const QString &telephone);
    static bool validerSalaire(double salaire);

private:
    int budget;
    QString nom, prenom, categorie, email;
    QDate debutcontrat, fincontrat;

    // Méthode interne pour générer le HTML (utilisé pour le PDF)
    static QString genererHTMLPourPDF(QSqlQueryModel *model);
};

#endif // SPONSOR_H
