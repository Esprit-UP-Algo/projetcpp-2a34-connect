#ifndef FICHEDEPAIE_H
#define FICHEDEPAIE_H

#include <QString>
#include <QDate>

class FichePaie
{
public:
    // Constructeur par défaut
    FichePaie();

    // La méthode principale qui fait tout le travail
    // Elle renvoie true si le PDF a été créé, false sinon
    bool genererPDF(int idEmploye, const QString &cheminFichier);

private:
    // Méthodes internes pour aider au dessin (rend le code plus propre)
    double calculerCNSS(double brut);
    double calculerIRPP(double imposable);
};

#endif // FICHEPAIE_H
