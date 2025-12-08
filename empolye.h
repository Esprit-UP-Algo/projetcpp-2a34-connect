#ifndef EMPLOYES_H
#define EMPLOYES_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Employes
{
public:
    Employes();
    Employes(int id, QString prenom, QDate date_embauche, QString email,
             QString telephone, QString sexe, QString poste, double salaire, QString id_superviseur);

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

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int id, QString email, QString telephone, QString poste, double salaire, QString id_superviseur);

    // Validations
    static bool validerEmail(const QString& email);
    static bool validerTelephone(const QString& telephone);
    static bool validerPrenom(const QString& prenom);
    static bool validerSalaire(double salaire);
    static bool validerDateEmbauche(const QDate& date);
    static bool emailExiste(const QString& email);
    static bool superviseurExiste(const QString& id);

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
};

#endif // EMPLOYES_H
