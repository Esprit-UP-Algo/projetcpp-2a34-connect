#include "CompteEpargne.h"

CompteEpargne::CompteEpargne(const std::string& rib, double solde, 
                             double tauxInteret, double soldeMinimal)
    : CompteBancaire(rib, solde), soldeMinimal(soldeMinimal), tauxInteret(tauxInteret) {
}

double CompteEpargne::getSoldeMinimal() const {
    return soldeMinimal;
}

double CompteEpargne::getTauxInteret() const {
    return tauxInteret;
}

void CompteEpargne::setTauxInteret(double taux) {
    tauxInteret = taux;
}

bool CompteEpargne::retrait(double montant) {
    if (montant <= 0) {
        std::cout << "Montant invalide pour le retrait." << std::endl;
        return false;
    }
    
    if (solde - montant < soldeMinimal) {
        std::cout << "Retrait refuse: le solde minimal de " << soldeMinimal 
                  << " DT doit etre respecte." << std::endl;
        return false;
    }
    
    solde -= montant;
    std::cout << "Retrait de " << montant << " DT effectue avec succes." << std::endl;
    return true;
}

void CompteEpargne::afficher() const {
    std::cout << "Compte Epargne - ";
    CompteBancaire::afficher();
    std::cout << "  Solde minimal: " << soldeMinimal << " DT" << std::endl;
    std::cout << "  Taux d'interet: " << (tauxInteret * 100) << "%" << std::endl;
}

double CompteEpargne::calculerInteretAnnuel() const {
    return solde * tauxInteret;
}
