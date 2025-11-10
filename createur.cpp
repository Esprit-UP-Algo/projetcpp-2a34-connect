#include "createur.h"

Createur::Createur() : idCreateur(0), abonne(0), idEmployee(0) {}

Createur::Createur(int idCreateur, const QString& nom, const QString& plateforme,
                   const QString& genre, int abonne, const QString& typeDeContenu, int idEmployee)
    : idCreateur(idCreateur), nom(nom), plateforme(plateforme), genre(genre),
    abonne(abonne), typeDeContenu(typeDeContenu), idEmployee(idEmployee) {}
