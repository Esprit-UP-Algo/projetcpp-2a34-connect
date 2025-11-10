#ifndef CREATEUR_H
#define CREATEUR_H

#include <QString>

class Createur {
private:
    int idCreateur;
    QString nom;
    QString plateforme;
    QString genre;
    int abonne;
    QString typeDeContenu;
    int idEmployee;

public:
    Createur();
    Createur(int idCreateur, const QString& nom, const QString& plateforme,
             const QString& genre, int abonne, const QString& typeDeContenu, int idEmployee);

    // Getters
    int getIdCreateur() const { return idCreateur; }
    QString getNom() const { return nom; }
    QString getPlateforme() const { return plateforme; }
    QString getGenre() const { return genre; }
    int getAbonne() const { return abonne; }
    QString getTypeDeContenu() const { return typeDeContenu; }
    int getIdEmployee() const { return idEmployee; }

    // Setters
    void setIdCreateur(int id) { idCreateur = id; }
    void setNom(const QString& n) { nom = n; }
    void setPlateforme(const QString& p) { plateforme = p; }
    void setGenre(const QString& g) { genre = g; }
    void setAbonne(int a) { abonne = a; }
    void setTypeDeContenu(const QString& t) { typeDeContenu = t; }
    void setIdEmployee(int id) { idEmployee = id; }
};

#endif // CREATEUR_H
