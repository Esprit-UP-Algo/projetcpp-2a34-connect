#ifndef EMPLOYES_H
#define EMPLOYES_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVector>     // Indispensable pour les stats
#include <QStringList> // Indispensable pour les stats

class Employes
{
public:
    // --- CONSTRUCTEURS ---
    Employes();
    // Constructeur compatible avec votre base actuelle (table 'empolye')
    // Le champ 'prenom' ici stockera "Nom Prénom" combinés
    Employes(int id, QString prenom, QDate date_embauche, QString email,
             QString telephone, QString sexe, QString poste, double salaire, QString id_superviseur);

    // --- GETTERS ---
    int getId() const;
    QString getPrenom() const;
    QDate getDateEmbauche() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getSexe() const;
    QString getPoste() const;
    double getSalaire() const;
    QString getIdSuperviseur() const;

    // --- SETTERS ---
    void setId(int id);
    void setPrenom(const QString &prenom);
    void setDateEmbauche(const QDate &date);
    void setEmail(const QString &email);
    void setTelephone(const QString &telephone);
    void setSexe(const QString &sexe);
    void setPoste(const QString &poste);
    void setSalaire(double salaire);
    void setIdSuperviseur(const QString &id);

    // --- MÉTHODES CRUD ---
    bool ajouter();
    bool modifier(int id, QString email, QString telephone, QString poste, double salaire, QString id_superviseur);
    bool supprimer(int id);
    QSqlQueryModel *afficher();

    // --- MÉTHODES AVANCÉES ---

    // Tri dynamique (true = Croissant / false = Décroissant)
    QSqlQueryModel *trierParSalaire(bool croissant);

    // Recherche intelligente (Nom OU Prénom OU Cin/Tél OU ID)
    QSqlQueryModel *rechercherMultiCriteres(QString recherche);

    // Compatibilité (appelle rechercherMultiCriteres)
    QSqlQueryModel *rechercherParPrenom(QString prenom);

    // ✅ NOUVELLE MÉTHODE : Statistiques pour le Bar Chart
    static void getStatistiquesSalaires(QVector<int> &valeurs, QStringList &etiquettes);

    // --- VALIDATIONS STRICTES (Statiques) ---
    // Utilisées dans MainWindow pour vérifier les saisies avant envoi
    static bool validerLettres(const QString &texte);      // Pour Nom et Prénom
    static bool valider8Chiffres(const QString &texte);    // Pour Téléphone et CIN
    static bool validerChiffres(const QString &texte);     // Pour ID et Superviseur
    static bool validerEmail(const QString &email);        // Doit contenir '@'
    static bool validerSexe(const QString &sexe);          // "M" ou "F"
    static bool validerSalaire(double salaire);            // Positif
    static bool validerFormatDate(const QString &dateTexte); // Format MM/JJ/AAAA

private:
    int id;
    QString prenom; // Correspond à la colonne PRENOM de la BDD (stocke Nom + Prénom)
    QDate date_embauche;
    QString email;
    QString telephone;
    QString sexe;
    QString poste;
    double salaire;
    QString id_superviseur;
};

#endif // EMPLOYES_H
