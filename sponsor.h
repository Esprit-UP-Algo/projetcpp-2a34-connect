#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class sponsor
{
public:
    sponsor();
    sponsor(QString,QString,QString,QString,QDate,QDate,int);
    bool ajouter();
    bool modifier(int,QString,QString,QString,QString,QDate,QDate,int);
    QSqlQueryModel *afficher();
    bool supprimer(QString);
    QSqlQueryModel *trierParNom();
    QSqlQueryModel *rechercherParNom(QString nom);
    QSqlQueryModel *statistiques();

private :
    int budget;
    QString nom,prenom,categorie,email,poste;
    QDate debutcontrat,fincontrat;
};

#endif // SPONSOR_H
