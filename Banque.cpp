#include "Banque.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"
#include <iostream>

Banque::Banque(const std::string& nom, const std::string& lieu)
    : nom(nom), lieu(lieu) {
}

Banque::~Banque() {
    supprimerComptes();
}

// Copy constructor
Banque::Banque(const Banque& autre)
    : nom(autre.nom), lieu(autre.lieu) {
    copierComptes(autre);
}

// Assignment operator
Banque& Banque::operator=(const Banque& autre) {
    if (this != &autre) {
        supprimerComptes();
        nom = autre.nom;
        lieu = autre.lieu;
        copierComptes(autre);
    }
    return *this;
}

std::string Banque::getNom() const {
    return nom;
}

std::string Banque::getLieu() const {
    return lieu;
}

int Banque::getNombreComptes() const {
    return comptes.size();
}

void Banque::ajouterCompte(CompteBancaire* compte) {
    if (compte != nullptr) {
        comptes.push_back(compte);
        std::cout << "Compte ajoute avec succes a la banque." << std::endl;
    } else {
        std::cout << "Erreur: compte invalide." << std::endl;
    }
}

void Banque::afficherTousLesComptes() const {
    std::cout << "\n========================================" << std::endl;
    std::cout << "Banque: " << nom << " (Lieu: " << lieu << ")" << std::endl;
    std::cout << "Nombre de comptes: " << comptes.size() << std::endl;
    std::cout << "========================================" << std::endl;
    
    if (comptes.empty()) {
        std::cout << "Aucun compte enregistre." << std::endl;
    } else {
        for (size_t i = 0; i < comptes.size(); ++i) {
            std::cout << "\nCompte #" << (i + 1) << ":" << std::endl;
            comptes[i]->afficher();
        }
    }
    std::cout << "========================================\n" << std::endl;
}

CompteBancaire* Banque::rechercherCompte(const std::string& rib) const {
    for (CompteBancaire* compte : comptes) {
        if (compte->getRib() == rib) {
            return compte;
        }
    }
    return nullptr;
}

void Banque::copierComptes(const Banque& autre) {
    for (CompteBancaire* compte : autre.comptes) {
        // Deep copy: create new instances based on actual type
        CompteEpargne* epargne = dynamic_cast<CompteEpargne*>(compte);
        if (epargne != nullptr) {
            comptes.push_back(new CompteEpargne(*epargne));
        } else {
            CompteCourant* courant = dynamic_cast<CompteCourant*>(compte);
            if (courant != nullptr) {
                comptes.push_back(new CompteCourant(*courant));
            }
        }
    }
}

void Banque::supprimerComptes() {
    for (CompteBancaire* compte : comptes) {
        delete compte;
    }
    comptes.clear();
}
