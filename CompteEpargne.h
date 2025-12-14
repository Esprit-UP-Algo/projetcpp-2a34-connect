#ifndef COMPTEEPARGNE_H
#define COMPTEEPARGNE_H

#include "CompteBancaire.h"

class CompteEpargne : public CompteBancaire {
private:
    double soldeMinimal;
    double tauxInteret;

public:
    CompteEpargne(const std::string& rib = "", double solde = 0.0, 
                  double tauxInteret = 0.0, double soldeMinimal = 5.0);
    
    // Getters
    double getSoldeMinimal() const;
    double getTauxInteret() const;

    // Setters
    void setTauxInteret(double taux);

    // Override virtual methods
    bool retrait(double montant) override;
    void afficher() const override;

    // Specific method
    double calculerInteretAnnuel() const;
};

#endif // COMPTEEPARGNE_H
