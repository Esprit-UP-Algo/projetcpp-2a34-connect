#include "CompteBancaire.h"

CompteBancaire::CompteBancaire(const std::string& rib, double solde)
    : rib(rib), solde(solde) {
}

CompteBancaire::~CompteBancaire() {
}

std::string CompteBancaire::getRib() const {
    return rib;
}

double CompteBancaire::getSolde() const {
    return solde;
}

void CompteBancaire::depot(double montant) {
    if (montant > 0) {
        solde += montant;
        std::cout << "Depot de " << montant << " DT effectue avec succes." << std::endl;
    } else {
        std::cout << "Montant invalide pour le depot." << std::endl;
    }
}

void CompteBancaire::afficher() const {
    std::cout << "RIB: " << rib << ", Solde: " << solde << " DT" << std::endl;
}

void CompteBancaire::setSolde(double nouveauSolde) {
    solde = nouveauSolde;
}
