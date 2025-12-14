#ifndef COMPTECOURANT_H
#define COMPTECOURANT_H

#include "CompteBancaire.h"

class CompteCourant : public CompteBancaire {
private:
    double soldeMinimal;

public:
    CompteCourant(const std::string& rib = "", double solde = 0.0, 
                  double soldeMinimal = -500.0);
    
    // Getter
    double getSoldeMinimal() const;

    // Override virtual methods
    bool retrait(double montant) override;
    void afficher() const override;
};

#endif // COMPTECOURANT_H
