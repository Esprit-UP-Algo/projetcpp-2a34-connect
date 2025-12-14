#ifndef COMPTEBANCAIRE_H
#define COMPTEBANCAIRE_H

#include <string>
#include <iostream>

class CompteBancaire {
protected:
    std::string rib;
    double solde;

public:
    CompteBancaire(const std::string& rib = "", double solde = 0.0);
    virtual ~CompteBancaire();

    // Getters
    std::string getRib() const;
    double getSolde() const;

    // Operations
    virtual void depot(double montant);
    virtual bool retrait(double montant) = 0; // Pure virtual - abstract class
    virtual void afficher() const;

    // Setter for solde
    void setSolde(double nouveauSolde);
};

#endif // COMPTEBANCAIRE_H
