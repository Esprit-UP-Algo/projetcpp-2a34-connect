#include "CompteCourant.h"

CompteCourant::CompteCourant(const std::string& rib, double solde, double soldeMinimal)
    : CompteBancaire(rib, solde), soldeMinimal(soldeMinimal) {
}

double CompteCourant::getSoldeMinimal() const {
    return soldeMinimal;
}

bool CompteCourant::retrait(double montant) {
    if (montant <= 0) {
        std::cout << "Montant invalide pour le retrait." << std::endl;
        return false;
    }
    
    if (solde - montant < soldeMinimal) {
        std::cout << "Retrait refuse: le solde minimal de " << soldeMinimal 
                  << " DT doit etre respecte (decouvert autorise)." << std::endl;
        return false;
    }
    
    solde -= montant;
    std::cout << "Retrait de " << montant << " DT effectue avec succes." << std::endl;
    return true;
}

void CompteCourant::afficher() const {
    std::cout << "Compte Courant - ";
    CompteBancaire::afficher();
    std::cout << "  Solde minimal (decouvert): " << soldeMinimal << " DT" << std::endl;
}
