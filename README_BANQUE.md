# Système de Gestion Bancaire - TD 6 : Polymorphisme

## Description

Cette application C++ implémente un système complet de gestion de comptes bancaires démontrant les concepts avancés de programmation orientée objet, notamment :

- **Polymorphisme** : Utilisation de méthodes virtuelles et de classes abstraites
- **Héritage** : Hiérarchie de classes (CompteBancaire → CompteEpargne/CompteCourant)
- **Collections polymorphiques** : Gestion d'une collection d'objets hétérogènes
- **Copie profonde** : Redéfinition de l'opérateur d'affectation

## Structure du Projet

### Classes Implémentées

#### 1. CompteBancaire (Classe Abstraite)
**Fichiers** : `CompteBancaire.h`, `CompteBancaire.cpp`

Classe de base abstraite représentant un compte bancaire générique.

**Attributs** :
- `rib` (string) : Numéro de compte (RIB)
- `solde` (double) : Solde du compte en DT

**Méthodes** :
- `depot(double montant)` : Effectue un dépôt
- `retrait(double montant)` : Méthode virtuelle pure pour effectuer un retrait
- `afficher()` : Affiche les informations du compte
- Getters et setters pour les attributs

#### 2. CompteEpargne (Compte Épargne)
**Fichiers** : `CompteEpargne.h`, `CompteEpargne.cpp`

Hérite de CompteBancaire. Représente un compte d'épargne avec intérêts.

**Attributs supplémentaires** :
- `soldeMinimal` (double) : Solde minimal autorisé (par défaut : 5 DT)
- `tauxInteret` (double) : Taux d'intérêt annuel (ex: 0.05 pour 5%)

**Méthodes spécifiques** :
- `calculerInteretAnnuel()` : Calcule l'intérêt annuel (solde × taux)
- `retrait()` : Redéfinie pour respecter le solde minimal

#### 3. CompteCourant (Compte Courant)
**Fichiers** : `CompteCourant.h`, `CompteCourant.cpp`

Hérite de CompteBancaire. Représente un compte courant avec découvert autorisé.

**Attributs supplémentaires** :
- `soldeMinimal` (double) : Solde minimal autorisé avec découvert (par défaut : -500 DT)

**Méthodes** :
- `retrait()` : Redéfinie pour autoriser le découvert jusqu'au solde minimal

#### 4. Banque
**Fichiers** : `Banque.h`, `Banque.cpp`

Classe de gestion d'une banque contenant plusieurs comptes.

**Attributs** :
- `nom` (string) : Nom de la banque
- `lieu` (string) : Localisation de la banque
- `comptes` (vector<CompteBancaire*>) : Collection polymorphique de comptes

**Méthodes** :
- `ajouterCompte(CompteBancaire*)` : Ajoute un compte à la banque
- `afficherTousLesComptes()` : Affiche tous les comptes
- `rechercherCompte(string rib)` : Recherche un compte par son RIB
- `operator=(const Banque&)` : Opérateur d'affectation avec copie profonde
- Constructeur de copie pour la copie profonde
- Destructeur pour libérer la mémoire

## Compilation et Exécution

### Prérequis
- Compilateur C++ supportant C++17 (g++ recommandé)
- Make (optionnel mais recommandé)

### Compilation avec Make
```bash
make clean      # Nettoyer les fichiers objets
make           # Compiler l'application
make run       # Compiler et exécuter
```

### Compilation manuelle
```bash
g++ -std=c++17 -Wall -Wextra -o banque_app main_banque.cpp CompteBancaire.cpp CompteEpargne.cpp CompteCourant.cpp Banque.cpp
./banque_app
```

## Fonctionnalités

### Démonstration Automatique
Au lancement, l'application exécute une démonstration complète :
1. Création de comptes épargne et courants
2. Ajout des comptes à la banque
3. Affichage de tous les comptes
4. Tests de dépôt et retrait
5. Vérification des contraintes de solde minimal
6. Calcul d'intérêts annuels
7. Recherche de comptes par RIB
8. Test du découvert autorisé
9. Test de la copie profonde (opérateur d'affectation)

### Menu Interactif
Après la démonstration, un menu interactif permet de :
1. Ajouter un compte épargne
2. Ajouter un compte courant
3. Afficher tous les comptes
4. Rechercher un compte par RIB
5. Effectuer un dépôt
6. Effectuer un retrait
7. Calculer l'intérêt annuel (compte épargne)
8. Quitter

## Concepts de POO Démontrés

### 1. Polymorphisme
- Méthode virtuelle pure `retrait()` dans CompteBancaire
- Différentes implémentations dans CompteEpargne et CompteCourant
- Collection polymorphique dans la classe Banque

### 2. Classe Abstraite
- CompteBancaire ne peut pas être instanciée directement
- Doit être héritée par des classes concrètes

### 3. Héritage
- CompteEpargne et CompteCourant héritent de CompteBancaire
- Réutilisation du code et spécialisation

### 4. Redéfinition de Méthodes
- `retrait()` et `afficher()` redéfinies dans les classes dérivées
- Utilisation de `override` pour la sécurité

### 5. Opérateur d'Affectation
- Implémentation de `operator=` avec copie profonde
- Évite les problèmes de mémoire avec les pointeurs

### 6. Gestion de Mémoire
- Allocation dynamique dans Banque
- Libération correcte dans le destructeur
- Copie profonde dans le constructeur de copie

## Exemples d'Utilisation

### Créer un compte épargne
```cpp
CompteEpargne* epargne = new CompteEpargne("RIB001", 1000.0, 0.05, 5.0);
// RIB: RIB001, Solde: 1000 DT, Taux: 5%, Solde minimal: 5 DT
```

### Créer un compte courant
```cpp
CompteCourant* courant = new CompteCourant("RIB002", 2000.0, -500.0);
// RIB: RIB002, Solde: 2000 DT, Découvert: -500 DT
```

### Ajouter à une banque
```cpp
Banque banque("Banque Nationale", "Tunis");
banque.ajouterCompte(epargne);
banque.ajouterCompte(courant);
```

### Effectuer des opérations
```cpp
epargne->depot(500.0);           // Dépôt
epargne->retrait(300.0);         // Retrait
double interet = epargne->calculerInteretAnnuel();  // Calcul d'intérêt
```

### Rechercher un compte
```cpp
CompteBancaire* compte = banque.rechercherCompte("RIB001");
if (compte != nullptr) {
    compte->afficher();
}
```

## Contraintes Implémentées

### Compte Épargne
- ✅ Solde minimal de 5 DT (configurable)
- ✅ Taux d'intérêt annuel
- ✅ Calcul automatique des intérêts
- ✅ Rejet des retraits violant le solde minimal

### Compte Courant
- ✅ Découvert autorisé jusqu'à -500 DT (configurable)
- ✅ Rejet des retraits dépassant le découvert autorisé

### Tous les Comptes
- ✅ Validation des montants (> 0)
- ✅ Affichage formaté avec 2 décimales
- ✅ Messages clairs pour les opérations

## Auteurs
Implémentation pour le TD 6 : Polymorphisme
Programmation Orientée Objet C++
A.U. 2018/2019
