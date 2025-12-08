#ifndef EMPLOYES_H
#define EMPLOYES_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QDateTime>
#include <QVector>

class Employes
{
public:
    Employes();
    Employes(int id, QString prenom, QDate date_embauche, QString email,
             QString telephone, QString sexe, QString poste, double salaire, QString id_superviseur, QString code_badge = "");

    // Getters
    int getId();
    QString getPrenom();
    QDate getDateEmbauche();
    QString getEmail();
    QString getTelephone();
    QString getSexe();
    QString getPoste();
    double getSalaire();
    QString getIdSuperviseur();
    QString getCodeBadge();

    // Setters
    void setId(int);
    void setPrenom(QString);
    void setDateEmbauche(QDate);
    void setEmail(QString);
    void setTelephone(QString);
    void setSexe(QString);
    void setPoste(QString);
    void setSalaire(double);
    void setIdSuperviseur(QString);
    void setCodeBadge(QString);

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int id, QString email, QString telephone, QString poste, double salaire, QString id_superviseur, QString code_badge = "");

    // Recherche et Tri
    static QSqlQueryModel* rechercherMultiCriteres(const QString &text);
    static QSqlQueryModel* trierParSalaire(bool croissant);
    static void getStatistiquesSalaires(QVector<int> &valeurs, QStringList &labels);

    // Validations
    static bool validerEmail(const QString& email);
    static bool validerTelephone(const QString& telephone);
    static bool validerPrenom(const QString& prenom);
    static bool validerSalaire(double salaire);
    static bool validerDateEmbauche(const QDate& date);
    static bool emailExiste(const QString& email);
    static bool superviseurExiste(const QString& id);

    // Validation supplémentaires (utilisées dans mainwindow)
    static bool validerLettres(const QString &text);
    static bool valider8Chiffres(const QString &text);
    static bool validerFormatDate(const QString &text);
    static bool validerSexe(const QString &sexe);
    static bool validerChiffres(const QString &text);

    // Gestion Badge Arduino
    static bool verifierCodeBadge(const QString &code);
    static bool codeBadgeExiste(const QString &code);
    static bool ajouterCodeBadge(int idEmploye, const QString &code);
    static bool supprimerCodeBadge(int idEmploye);
    static QSqlQueryModel* rechercherParCodeBadge(const QString &code);
    static QSqlQueryModel* getHistoriqueAcces();
    static bool enregistrerAcces(int idEmploye, const QString &nom, const QDateTime &timestamp);

private:
    int id;
    QString prenom;
    QDate date_embauche;
    QString email;
    QString telephone;
    QString sexe;
    QString poste;
    double salaire;
    QString id_superviseur;
    QString code_badge;
};

#endif // EMPLOYES_H
