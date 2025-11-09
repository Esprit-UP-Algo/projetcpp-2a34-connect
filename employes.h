#ifndef EMPLOYES_H
#define EMPLOYES_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Employes
{
public:
    Employes();
    Employes(QString prenom, QString email, QString telephone, QString sexe, QString poste, double salaire, QDate dateEmbauche);

    bool ajouter();
    bool modifier(int id, QString email, QString telephone, QString poste, double salaire, QString cin);
    bool supprimer(int id);
    QSqlQueryModel *afficher();
    QSqlQueryModel *trierParSalaire();
    QSqlQueryModel *rechercherParPrenom(QString prenom);

    // Getters et Setters
    QString getPrenom() const { return prenom; }
    QString getEmail() const { return email; }
    QString getTelephone() const { return telephone; }
    QString getSexe() const { return sexe; }
    QString getPoste() const { return poste; }
    double getSalaire() const { return salaire; }
    QDate getDateEmbauche() const { return dateEmbauche; }
    QString getCin() const { return cin; }
    int getIdSuperviseur() const { return idSuperviseur; }

    void setPrenom(const QString &p) { prenom = p; }
    void setEmail(const QString &e) { email = e; }
    void setTelephone(const QString &t) { telephone = t; }
    void setSexe(const QString &s) { sexe = s; }
    void setPoste(const QString &p) { poste = p; }
    void setSalaire(double s) { salaire = s; }
    void setDateEmbauche(const QDate &d) { dateEmbauche = d; }
    void setCin(const QString &c) { cin = c; }
    void setIdSuperviseur(int id) { idSuperviseur = id; }

    // Méthodes de validation
    static bool validerPrenom(const QString &prenom);
    static bool validerEmail(const QString &email);
    static bool validerTelephone(const QString &telephone);
    static bool validerSexe(const QString &sexe);
    static bool validerPoste(const QString &poste);
    static bool validerSalaire(double salaire);
    static bool validerCIN(const QString &cin);
    static bool validerID(int id);

private:
    QString prenom;
    QString email;
    QString telephone;
    QString sexe;
    QString poste;
    double salaire;
    QDate dateEmbauche;
    QString cin;
    int idSuperviseur;

    bool creerTableSiExistePas();
};

#endif // EMPLOYES_H
