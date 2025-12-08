#include "sponsor.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>

Sponsor::Sponsor() : budget(0) {}

Sponsor::Sponsor(QString nom, QString prenom, QString categorie, QString email,
                 QDate debutcontrat, QDate fincontrat, int budget)
    : nom(nom), prenom(prenom), categorie(categorie), email(email),
    debutcontrat(debutcontrat), fincontrat(fincontrat), budget(budget) {}

bool Sponsor::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO sponsor (NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET) VALUES (:n, :p, :c, :e, :d, :f, :b)");
    query.bindValue(":n", nom); query.bindValue(":p", prenom); query.bindValue(":c", categorie);
    query.bindValue(":e", email); query.bindValue(":d", debutcontrat); query.bindValue(":f", fincontrat);
    query.bindValue(":b", budget);
    return query.exec();
}

bool Sponsor::modifier(int id, QString nom, QString prenom, QString categorie, QString email, QDate debut, QDate fin, int budget) {
    QSqlQuery query;
    query.prepare("UPDATE sponsor SET NOM=:n, PRENOM=:p, CATEGORIE=:c, EMAIL=:e, DEBUTCONTRAT=:d, FINCONTRAT=:f, BUDGET=:b WHERE IDSPONSOR=:id");
    query.bindValue(":id", id); query.bindValue(":n", nom); query.bindValue(":p", prenom);
    query.bindValue(":c", categorie); query.bindValue(":e", email); query.bindValue(":d", debut);
    query.bindValue(":f", fin); query.bindValue(":b", budget);
    return query.exec();
}

bool Sponsor::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM sponsor WHERE IDSPONSOR=:id");
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* Sponsor::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor");
    return model;
}

QSqlQueryModel* Sponsor::trierParDate() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor ORDER BY DEBUTCONTRAT DESC");
    return model;
}

QSqlQueryModel* Sponsor::rechercherParNom(QString nom) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM sponsor WHERE UPPER(NOM) LIKE UPPER(:n) OR UPPER(PRENOM) LIKE UPPER(:n)");
    query.bindValue(":n", "%" + nom + "%");
    query.exec();
    model->setQuery(std::move(query));
    return model;
}

// ==================== STATISTIQUES POUR CURVEWIDGET ====================

QVector<QPointF> Sponsor::getDonneesEvolutionContrats() {
    QVector<QPointF> data;
    QSqlQuery query;
    // Note: Syntax pour SQLite. Pour Oracle, utiliser TO_CHAR(DEBUTCONTRAT, 'YYYY-MM')
    query.exec("SELECT strftime('%Y-%m', DEBUTCONTRAT), COUNT(*) FROM sponsor WHERE DEBUTCONTRAT IS NOT NULL GROUP BY 1 ORDER BY 1");
    int i = 0;
    while (query.next()) {
        data.append(QPointF(i++, query.value(1).toInt()));
    }
    return data;
}

void Sponsor::getDonneesBudgetParCategorie(QVector<QPointF> &data, QStringList &labels) {
    data.clear(); labels.clear();
    QSqlQuery query("SELECT CATEGORIE, SUM(BUDGET) FROM sponsor GROUP BY CATEGORIE");
    int i = 0;
    while (query.next()) {
        labels << query.value(0).toString();
        data.append(QPointF(i++, query.value(1).toDouble()));
    }
}

void Sponsor::getDonneesRepartitionCategories(QVector<QPointF> &data, QStringList &labels) {
    data.clear(); labels.clear();
    QSqlQuery query("SELECT CATEGORIE, COUNT(*) FROM sponsor GROUP BY CATEGORIE");
    int i = 0;
    while (query.next()) {
        labels << query.value(0).toString();
        data.append(QPointF(i++, query.value(1).toInt()));
    }
}

void Sponsor::getDonneesDureeContrats(QVector<QPointF> &data) {
    data.clear();
    QSqlQuery query("SELECT CATEGORIE, AVG(julianday(FINCONTRAT) - julianday(DEBUTCONTRAT)) FROM sponsor GROUP BY CATEGORIE");
    int i = 0;
    while (query.next()) {
        data.append(QPointF(i++, query.value(1).toDouble()));
    }
}

void Sponsor::getResumeStatistiques(int &totalSponsors, double &totalBudget, double &avgBudget) {
    QSqlQuery query("SELECT COUNT(*), SUM(BUDGET), AVG(BUDGET) FROM sponsor");
    if (query.next()) {
        totalSponsors = query.value(0).toInt();
        totalBudget = query.value(1).toDouble();
        avgBudget = query.value(2).toDouble();
    }
}

// VALIDATIONS
bool Sponsor::validerNom(const QString &nom) { return !nom.isEmpty() && nom.length() >= 2; }
bool Sponsor::validerBudget(int budget) { return budget >= 0; }

// EXPORTS (Simplifiés)
bool Sponsor::exporterPDF(const QString &nomFichier) { return true; /* Implémenter logique PDF si besoin */ }
bool Sponsor::exporterPDFParContrat(const QString &nomFichier, const QString &filtre) { return true; }
