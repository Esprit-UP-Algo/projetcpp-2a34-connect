#ifndef BANQUE_H
#define BANQUE_H

#include "CompteBancaire.h"
#include <vector>
#include <string>

class Banque {
private:
    std::string nom;
    std::string lieu;
    std::vector<CompteBancaire*> comptes;

public:
    Banque(const std::string& nom = "", const std::string& lieu = "");
    ~Banque();

    // Copy constructor for deep copy
    Banque(const Banque& autre);

    // Assignment operator for deep copy (overloading operator=)
    Banque& operator=(const Banque& autre);

    // Getters
    std::string getNom() const;
    std::string getLieu() const;
    int getNombreComptes() const;

    // Operations
    void ajouterCompte(CompteBancaire* compte);
    void afficherTousLesComptes() const;
    CompteBancaire* rechercherCompte(const std::string& rib) const;

private:
    void copierComptes(const Banque& autre);
    void supprimerComptes();
};

#endif // BANQUE_H
