#include "Banque.h"
#include "CompteEpargne.h"
#include "CompteCourant.h"
#include <iostream>
#include <iomanip>

using namespace std;

void afficherMenu() {
    cout << "\n====== SYSTEME DE GESTION BANCAIRE ======" << endl;
    cout << "1. Ajouter un compte epargne" << endl;
    cout << "2. Ajouter un compte courant" << endl;
    cout << "3. Afficher tous les comptes" << endl;
    cout << "4. Rechercher un compte par RIB" << endl;
    cout << "5. Effectuer un depot" << endl;
    cout << "6. Effectuer un retrait" << endl;
    cout << "7. Calculer l'interet annuel (compte epargne)" << endl;
    cout << "8. Quitter" << endl;
    cout << "==========================================" << endl;
    cout << "Votre choix: ";
}

int main() {
    // Create a bank
    Banque banque("Banque Nationale", "Tunis");
    
    cout << fixed << setprecision(2);
    cout << "\nBienvenue dans le systeme de gestion de la " << banque.getNom() << endl;
    cout << "Lieu: " << banque.getLieu() << "\n" << endl;
    
    // Demonstration with some initial accounts
    cout << "=== Demonstration initiale ===" << endl;
    
    // Create savings accounts
    CompteEpargne* epargne1 = new CompteEpargne("RIB001", 1000.0, 0.05, 5.0);
    CompteEpargne* epargne2 = new CompteEpargne("RIB002", 5000.0, 0.04, 10.0);
    
    // Create current accounts
    CompteCourant* courant1 = new CompteCourant("RIB003", 2000.0, -500.0);
    CompteCourant* courant2 = new CompteCourant("RIB004", 500.0, -1000.0);
    
    // Add accounts to bank
    banque.ajouterCompte(epargne1);
    banque.ajouterCompte(epargne2);
    banque.ajouterCompte(courant1);
    banque.ajouterCompte(courant2);
    
    // Display all accounts
    banque.afficherTousLesComptes();
    
    // Test deposit
    cout << "\n=== Test de depot ===" << endl;
    epargne1->depot(500.0);
    epargne1->afficher();
    
    // Test withdrawal
    cout << "\n=== Test de retrait ===" << endl;
    courant1->retrait(300.0);
    courant1->afficher();
    
    // Test withdrawal with minimum balance constraint
    cout << "\n=== Test de retrait avec contrainte de solde minimal ===" << endl;
    epargne1->retrait(1600.0); // Should fail (would go below 5 DT)
    epargne1->afficher();
    
    // Test calculation of annual interest
    cout << "\n=== Calcul d'interet annuel ===" << endl;
    double interet1 = epargne1->calculerInteretAnnuel();
    cout << "Interet annuel pour le compte " << epargne1->getRib() 
         << ": " << interet1 << " DT" << endl;
    
    double interet2 = epargne2->calculerInteretAnnuel();
    cout << "Interet annuel pour le compte " << epargne2->getRib() 
         << ": " << interet2 << " DT" << endl;
    
    // Test search by RIB
    cout << "\n=== Recherche de compte par RIB ===" << endl;
    string ribRecherche = "RIB003";
    CompteBancaire* compteRecherche = banque.rechercherCompte(ribRecherche);
    if (compteRecherche != nullptr) {
        cout << "Compte trouve:" << endl;
        compteRecherche->afficher();
    } else {
        cout << "Compte non trouve." << endl;
    }
    
    // Test with non-existent RIB
    ribRecherche = "RIB999";
    compteRecherche = banque.rechercherCompte(ribRecherche);
    if (compteRecherche != nullptr) {
        cout << "Compte trouve:" << endl;
        compteRecherche->afficher();
    } else {
        cout << "Compte " << ribRecherche << " non trouve." << endl;
    }
    
    // Test overdraft on current account
    cout << "\n=== Test de decouvert autorise ===" << endl;
    courant1->retrait(2200.0); // Should allow overdraft
    courant1->afficher();
    
    // Test assignment operator (deep copy)
    cout << "\n=== Test de l'operateur d'affectation ===" << endl;
    Banque banque2("Banque Copiee", "Sfax");
    banque2 = banque;
    cout << "\nBanque originale:" << endl;
    banque.afficherTousLesComptes();
    cout << "\nBanque copiee:" << endl;
    banque2.afficherTousLesComptes();
    
    // Interactive menu
    int choix;
    bool continuer = true;
    
    while (continuer) {
        afficherMenu();
        cin >> choix;
        
        switch (choix) {
            case 1: {
                string rib;
                double solde, taux;
                cout << "RIB: ";
                cin >> rib;
                cout << "Solde initial: ";
                cin >> solde;
                cout << "Taux d'interet (ex: 0.05 pour 5%): ";
                cin >> taux;
                banque.ajouterCompte(new CompteEpargne(rib, solde, taux));
                break;
            }
            case 2: {
                string rib;
                double solde;
                cout << "RIB: ";
                cin >> rib;
                cout << "Solde initial: ";
                cin >> solde;
                banque.ajouterCompte(new CompteCourant(rib, solde));
                break;
            }
            case 3:
                banque.afficherTousLesComptes();
                break;
            case 4: {
                string rib;
                cout << "RIB a rechercher: ";
                cin >> rib;
                CompteBancaire* compte = banque.rechercherCompte(rib);
                if (compte != nullptr) {
                    cout << "\nCompte trouve:" << endl;
                    compte->afficher();
                } else {
                    cout << "Compte non trouve." << endl;
                }
                break;
            }
            case 5: {
                string rib;
                double montant;
                cout << "RIB du compte: ";
                cin >> rib;
                CompteBancaire* compte = banque.rechercherCompte(rib);
                if (compte != nullptr) {
                    cout << "Montant a deposer: ";
                    cin >> montant;
                    compte->depot(montant);
                } else {
                    cout << "Compte non trouve." << endl;
                }
                break;
            }
            case 6: {
                string rib;
                double montant;
                cout << "RIB du compte: ";
                cin >> rib;
                CompteBancaire* compte = banque.rechercherCompte(rib);
                if (compte != nullptr) {
                    cout << "Montant a retirer: ";
                    cin >> montant;
                    compte->retrait(montant);
                } else {
                    cout << "Compte non trouve." << endl;
                }
                break;
            }
            case 7: {
                string rib;
                cout << "RIB du compte epargne: ";
                cin >> rib;
                CompteBancaire* compte = banque.rechercherCompte(rib);
                if (compte != nullptr) {
                    CompteEpargne* epargne = dynamic_cast<CompteEpargne*>(compte);
                    if (epargne != nullptr) {
                        double interet = epargne->calculerInteretAnnuel();
                        cout << "Interet annuel: " << interet << " DT" << endl;
                    } else {
                        cout << "Ce n'est pas un compte epargne." << endl;
                    }
                } else {
                    cout << "Compte non trouve." << endl;
                }
                break;
            }
            case 8:
                continuer = false;
                cout << "Merci d'avoir utilise le systeme bancaire." << endl;
                break;
            default:
                cout << "Choix invalide." << endl;
        }
    }
    
    return 0;
}
