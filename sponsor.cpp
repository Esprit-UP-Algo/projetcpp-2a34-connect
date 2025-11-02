#include "sponsor.h"
#include <QSqlQuery>
#include<QtDebug>
#include<QObject>

sponsor::sponsor()
{
    //initialisation des variables //
    nom="";
    prenom="";
    categorie="";
    email="";
    budget=0;
}
sponsor::sponsor(QString nom,QString prenom,QString categorie ,QString email ,QDate debutcontrat,QDate fincontrat, int budget )
{
    this->nom=nom ;
    this->prenom=prenom;
    this->categorie=categorie;
    this ->email=email;
    this ->debutcontrat=debutcontrat ;
    this->fincontrat=fincontrat;
    this->budget=budget;

}
bool sponsor::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO sponsor (NOM,PRENOM,CATEGORIE,EMAIL,DEBUTCONTRAT,FINCONTRAT,BUDGET) "
                  "VALUES (:NOM, :PRENOM, :CATEGORIE, :EMAIL, :DEBUTCONTRAT, :FINCONTRAT, :BUDGET)");

    query.bindValue(":NOM", nom);             // QString
    query.bindValue(":PRENOM", prenom);
    query.bindValue(":CATEGORIE", categorie);       // QString    // QString
    query.bindValue(":EMAIL", email);         // QString
    query.bindValue(":DEBUTCONTRAT", debutcontrat); // QString
    query.bindValue(":FINCONTRAT", fincontrat); // int
    query.bindValue(":BUDGET", budget);         // QString

    return query.exec();
}

bool sponsor::modifier(int idsponsor,QString nom,QString prenom ,QString categorie ,QString EMAIL,QDate debutcontrat,QDate fincontrat,int budget)
{
    QSqlQuery query;
    query.prepare("Update sponsor set nom = :nom , prenom = :prenom , categorie=:categorie , email=:email , debutcontrat=:debutcontrat , fincontrat=:fincontrat , budget=:budget    where IDSPONSOR = :idsponsor");
    query.bindValue(":idsponsor", idsponsor);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":categorie", categorie);
    query.bindValue(":email",email);
    query.bindValue(":debutcontrat", debutcontrat);
    query.bindValue(":fincontrat", fincontrat);
    query.bindValue(":budget", budget);

    return    query.exec();
}
QSqlQueryModel * sponsor::afficher()
{

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT* FROM sponsor");
    model->setHeaderData(0, Qt::Horizontal, QObject:: tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("categorie"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("debutcontrat"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("fincontrat"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("budget"));


    return model;
}
bool sponsor::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("delete from SPONSOR where idsponsor=:id");
    query.bindValue(0, id);
    return query.exec();
}
