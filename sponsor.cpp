#include "sponsor.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextStream>
#include <QFile>
#include <QDesktopServices>  // Ajoutez cette ligne
#include <QUrl>              // Ajoutez cette ligne

// Inclure QPrinter si vous avez ajouté printsupport
#ifdef QT_PRINTSUPPORT_LIB
#include <QPrinter>
#include <QPainter>
#endif

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
    query.exec("SELECT strftime('%Y-%m', DEBUTCONTRAT) as mois, COUNT(*) as nb FROM sponsor WHERE DEBUTCONTRAT IS NOT NULL GROUP BY mois ORDER BY mois");
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
    } else {
        totalSponsors = 0;
        totalBudget = 0.0;
        avgBudget = 0.0;
    }
}

// VALIDATIONS
bool Sponsor::validerNom(const QString &nom) { return !nom.isEmpty() && nom.length() >= 2; }
bool Sponsor::validerBudget(int budget) { return budget >= 0; }

// EXPORTS PDF
bool Sponsor::exporterPDF(const QString &nomFichier) {
    Sponsor sponsorObj;
    QSqlQueryModel *model = sponsorObj.afficher();
    return exporterPDF(nomFichier, model);
}

bool Sponsor::exporterPDFParContrat(const QString &nomFichier, const QString &filtre) {
    // 1. Récupérer les données selon le filtre
    QSqlQuery query;
    if (filtre.toLower() == "tous" || filtre.isEmpty()) {
        query.exec("SELECT * FROM sponsor");
    } else {
        query.prepare("SELECT * FROM sponsor WHERE CATEGORIE = :categorie");
        query.bindValue(":categorie", filtre);
        query.exec();
    }

    // 2. Créer le contenu HTML
    QString html;
    html += "<!DOCTYPE html>";
    html += "<html lang='fr'>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Liste des Sponsors - ConnectPlus</title>";
    html += "<style>";
    html += "body { font-family: 'Segoe UI', Arial, sans-serif; margin: 0; padding: 20px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; }";
    html += ".container { background: white; padding: 30px; border-radius: 10px; box-shadow: 0 10px 30px rgba(0,0,0,0.2); max-width: 1200px; margin: 20px auto; }";
    html += "h1 { color: #7D4FEE; text-align: center; border-bottom: 3px solid #7D4FEE; padding-bottom: 15px; margin-bottom: 30px; }";
    html += ".header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 30px; flex-wrap: wrap; }";
    html += ".logo { display: flex; align-items: center; gap: 15px; }";
    html += ".logo img { width: 50px; height: 50px; }";
    html += ".info-box { background: linear-gradient(to right, #f8f9fa, #e9ecef); padding: 20px; border-radius: 8px; margin: 20px 0; border-left: 4px solid #7D4FEE; }";
    html += "table { width: 100%; border-collapse: collapse; margin: 20px 0; box-shadow: 0 5px 15px rgba(0,0,0,0.05); }";
    html += "th { background: linear-gradient(to right, #7D4FEE, #5a3fd8); color: white; padding: 15px; text-align: left; font-weight: bold; position: sticky; top: 0; }";
    html += "td { border: 1px solid #e0e0e0; padding: 12px; }";
    html += "tr:nth-child(even) { background-color: #f9f9f9; }";
    html += "tr:hover { background-color: #f0f0ff; transform: scale(1.01); transition: all 0.2s; }";
    html += ".budget { text-align: right; font-weight: bold; color: #2E7D32; }";
    html += ".footer { text-align: center; margin-top: 40px; color: #666; font-size: 0.9em; padding-top: 20px; border-top: 1px solid #eee; }";
    html += ".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; margin-top: 30px; }";
    html += ".stat-card { background: white; padding: 20px; border-radius: 8px; box-shadow: 0 3px 10px rgba(0,0,0,0.08); text-align: center; border-top: 4px solid #7D4FEE; }";
    html += ".stat-value { font-size: 2em; font-weight: bold; color: #7D4FEE; margin: 10px 0; }";
    html += ".stat-label { color: #666; font-size: 0.9em; }";
    html += ".print-section { margin: 30px 0; padding: 20px; background: #fff3e0; border-radius: 8px; border: 2px dashed #ff9800; }";
    html += ".print-btn { background: linear-gradient(to right, #7D4FEE, #5a3fd8); color: white; border: none; padding: 12px 24px; border-radius: 5px; cursor: pointer; font-size: 16px; display: flex; align-items: center; gap: 10px; margin: 0 auto; }";
    html += ".print-btn:hover { background: linear-gradient(to right, #6a42d4, #4a34b5); transform: translateY(-2px); box-shadow: 0 5px 15px rgba(125, 79, 238, 0.4); }";
    html += ".contract-active { color: #2E7D32; font-weight: bold; }";
    html += ".contract-expired { color: #d32f2f; font-weight: bold; }";
    html += "@media print {";
    html += "    body { background: white; margin: 0; padding: 0; }";
    html += "    .print-section, .print-btn { display: none; }";
    html += "    .container { box-shadow: none; margin: 0; padding: 10px; }";
    html += "    .info-box { border: 1px solid #ccc; }";
    html += "}";
    html += "</style>";
    html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css'>";
    html += "</head>";
    html += "<body>";

    html += "<div class='container'>";

    // En-tête avec logo
    html += "<div class='header'>";
    html += "<div class='logo'>";
    html += "<div style='background: #7D4FEE; color: white; width: 50px; height: 50px; border-radius: 50%; display: flex; align-items: center; justify-content: center; font-size: 24px;'>";
    html += "<i class='fas fa-handshake'></i>";
    html += "</div>";
    html += "<div>";
    html += "<h1 style='margin: 0;'>CONNECTPLUS SPONSORS</h1>";
    html += "<p style='margin: 0; color: #666;'>Gestion des partenaires et sponsors</p>";
    html += "</div>";
    html += "</div>";
    html += "<div style='text-align: right;'>";
    html += "<p><strong>Date :</strong> " + QDate::currentDate().toString("dddd d MMMM yyyy") + "</p>";
    html += "<p><strong>Heure :</strong> " + QTime::currentTime().toString("HH:mm") + "</p>";
    html += "</div>";
    html += "</div>";

    // Information sur le filtre
    html += "<div class='info-box'>";
    html += "<h3 style='margin-top: 0;'><i class='fas fa-filter'></i> Filtre appliqué</h3>";
    html += "<p><strong>Catégorie :</strong> " + (filtre.isEmpty() ? "Toutes les catégories" : filtre) + "</p>";
    html += "<p><strong>Généré par :</strong> Système ConnectPlus v2.0</p>";
    html += "</div>";

    // Statistiques en cartes
    int totalBudget = 0;
    int rowCount = 0;
    int activeContracts = 0;
    int expiredContracts = 0;
    QDate today = QDate::currentDate();

    QVector<QPair<QString, int>> categoryBudgets;
    QHash<QString, int> categoryMap;

    while (query.next()) {
        rowCount++;
        QDate finContrat = query.value("FINCONTRAT").toDate();
        bool isActive = (finContrat >= today);

        if (isActive) {
            activeContracts++;
        } else {
            expiredContracts++;
        }

        int budget = query.value("BUDGET").toInt();
        totalBudget += budget;

        QString categorie = query.value("CATEGORIE").toString();
        categoryMap[categorie] += budget;
    }

    // Ré-exécuter la requête pour le tableau
    query.exec(filtre.isEmpty() ? "SELECT * FROM sponsor" :
                   "SELECT * FROM sponsor WHERE CATEGORIE = '" + filtre + "'");

    html += "<div class='stats-grid'>";
    html += "<div class='stat-card'>";
    html += "<div class='stat-label'>Total Sponsors</div>";
    html += "<div class='stat-value'>" + QString::number(rowCount) + "</div>";
    html += "<i class='fas fa-users' style='color: #7D4FEE; font-size: 24px;'></i>";
    html += "</div>";

    html += "<div class='stat-card'>";
    html += "<div class='stat-label'>Budget Total</div>";
    html += "<div class='stat-value'>" + QString::number(totalBudget) + "€</div>";
    html += "<i class='fas fa-euro-sign' style='color: #2E7D32; font-size: 24px;'></i>";
    html += "</div>";

    html += "<div class='stat-card'>";
    html += "<div class='stat-label'>Contrats Actifs</div>";
    html += "<div class='stat-value'>" + QString::number(activeContracts) + "</div>";
    html += "<i class='fas fa-check-circle' style='color: #4CAF50; font-size: 24px;'></i>";
    html += "</div>";

    html += "<div class='stat-card'>";
    html += "<div class='stat-label'>Contrats Expirés</div>";
    html += "<div class='stat-value'>" + QString::number(expiredContracts) + "</div>";
    html += "<i class='fas fa-times-circle' style='color: #f44336; font-size: 24px;'></i>";
    html += "</div>";
    html += "</div>";

    // Instructions pour impression
    html += "<div class='print-section'>";
    html += "<h3><i class='fas fa-print'></i> Instructions pour l'impression PDF</h3>";
    html += "<p>Pour générer un PDF de haute qualité :</p>";
    html += "<ol>";
    html += "<li>Cliquez sur le bouton <strong>'Imprimer'</strong> ci-dessous</li>";
    html += "<li>Dans la fenêtre d'impression, choisissez <strong>'Enregistrer au format PDF'</strong></li>";
    html += "<li>Sélectionnez <strong>Orientation Paysage</strong> pour une meilleure lisibilité</li>";
    html += "<li>Cliquez sur <strong>Enregistrer</strong> et choisissez un emplacement</li>";
    html += "</ol>";
    html += "<button class='print-btn' onclick='window.print()'>";
    html += "<i class='fas fa-print'></i> Imprimer / Générer PDF";
    html += "</button>";
    html += "</div>";

    // Tableau des sponsors
    html += "<h2><i class='fas fa-table'></i> Détail des Sponsors</h2>";
    html += "<div style='overflow-x: auto;'>";
    html += "<table>";
    html += "<thead>";
    html += "<tr>";
    html += "<th>ID</th>";
    html += "<th>Nom</th>";
    html += "<th>Prénom</th>";
    html += "<th>Catégorie</th>";
    html += "<th>Email</th>";
    html += "<th>Début Contrat</th>";
    html += "<th>Fin Contrat</th>";
    html += "<th>Statut</th>";
    html += "<th>Budget (€)</th>";
    html += "</tr>";
    html += "</thead>";
    html += "<tbody>";

    while (query.next()) {
        QString id = query.value("IDSPONSOR").toString();
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();
        QString categorie = query.value("CATEGORIE").toString();
        QString email = query.value("EMAIL").toString();
        QDate debut = query.value("DEBUTCONTRAT").toDate();
        QDate fin = query.value("FINCONTRAT").toDate();
        int budget = query.value("BUDGET").toInt();

        bool isActive = (fin >= today);
        int joursRestants = debut.daysTo(fin);

        html += "<tr>";
        html += "<td><strong>" + id + "</strong></td>";
        html += "<td>" + nom + "</td>";
        html += "<td>" + prenom + "</td>";
        html += "<td><span style='background: #e0e7ff; color: #7D4FEE; padding: 4px 8px; border-radius: 4px;'>" + categorie + "</span></td>";
        html += "<td><a href='mailto:" + email + "' style='color: #2196F3;'>" + email + "</a></td>";
        html += "<td>" + debut.toString("dd/MM/yyyy") + "</td>";
        html += "<td>" + fin.toString("dd/MM/yyyy") + "</td>";

        if (isActive) {
            html += "<td class='contract-active'><i class='fas fa-check-circle'></i> Actif (" + QString::number(joursRestants) + " jours)</td>";
        } else {
            html += "<td class='contract-expired'><i class='fas fa-times-circle'></i> Expiré</td>";
        }

        html += "<td class='budget'>" + QString::number(budget) + " €</td>";
        html += "</tr>";
    }

    html += "</tbody>";
    html += "</table>";
    html += "</div>";

    // Résumé détaillé
    html += "<div class='info-box' style='margin-top: 30px;'>";
    html += "<h3><i class='fas fa-chart-bar'></i> Analyse par Catégorie</h3>";

    // Calculer les budgets par catégorie
    QSqlQuery catQuery("SELECT CATEGORIE, SUM(BUDGET), COUNT(*) FROM sponsor GROUP BY CATEGORIE");
    html += "<div style='display: grid; grid-template-columns: repeat(auto-fill, minmax(250px, 1fr)); gap: 15px; margin-top: 15px;'>";

    while (catQuery.next()) {
        QString cat = catQuery.value(0).toString();
        int budgetCat = catQuery.value(1).toInt();
        int countCat = catQuery.value(2).toInt();
        double pourcentage = rowCount > 0 ? (countCat * 100.0 / rowCount) : 0;

        html += "<div style='padding: 15px; background: white; border-radius: 8px; box-shadow: 0 2px 5px rgba(0,0,0,0.1);'>";
        html += "<h4 style='margin-top: 0;'>" + cat + "</h4>";
        html += "<p><strong>Nombre :</strong> " + QString::number(countCat) + " (" + QString::number(pourcentage, 'f', 1) + "%)</p>";
        html += "<p><strong>Budget :</strong> " + QString::number(budgetCat) + " €</p>";
        html += "<div style='height: 8px; background: #e0e0e0; border-radius: 4px; margin-top: 10px;'>";
        html += "<div style='height: 100%; width: " + QString::number(pourcentage > 100 ? 100 : pourcentage) + "%; background: #7D4FEE; border-radius: 4px;'></div>";
        html += "</div>";
        html += "</div>";
    }

    html += "</div>";
    html += "</div>";

    // Pied de page
    html += "<div class='footer'>";
    html += "<p><i class='fas fa-info-circle'></i> Document généré automatiquement par ConnectPlus Management System</p>";
    html += "<p><i class='fas fa-copyright'></i> " + QString::number(QDate::currentDate().year()) + " ConnectPlus. Tous droits réservés.</p>";
    html += "<p><small>Dernière mise à jour : " + QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm:ss") + "</small></p>";
    html += "</div>";

    html += "</div>"; // Fermeture du container

    // Script JavaScript
    html += "<script>";
    html += "function exportToPDF() {";
    html += "    window.print();";
    html += "}";
    html += "function sortTable(columnIndex) {";
    html += "    const table = document.querySelector('table');";
    html += "    const rows = Array.from(table.querySelectorAll('tbody tr'));";
    html += "    const isNumeric = columnIndex === 0 || columnIndex === 8;";
    html += "    rows.sort((a, b) => {";
    html += "        const aVal = a.cells[columnIndex].textContent.trim();";
    html += "        const bVal = b.cells[columnIndex].textContent.trim();";
    html += "        if (isNumeric) return parseInt(aVal) - parseInt(bVal);";
    html += "        return aVal.localeCompare(bVal);";
    html += "    });";
    html += "    const tbody = table.querySelector('tbody');";
    html += "    tbody.innerHTML = '';";
    html += "    rows.forEach(row => tbody.appendChild(row));";
    html += "}";
    html += "</script>";

    html += "</body>";
    html += "</html>";

    // 3. Écrire le contenu dans un fichier
    QFile file(nomFichier);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        stream << html;
        file.close();

        // Ouvrir le fichier dans le navigateur par défaut
        QDesktopServices::openUrl(QUrl::fromLocalFile(nomFichier));

        return true;
    } else {
        qDebug() << "Erreur : Impossible d'écrire dans le fichier" << nomFichier;
        return false;
    }
}
bool Sponsor::exporterPDF(const QString &nomFichier, QSqlQueryModel *model) {
    if (!model || model->rowCount() == 0) {
        qDebug() << "Erreur : Modèle vide ou invalide";
        return false;
    }

    // Générer le contenu HTML
    QString html = genererHTMLPourPDF(model);

    // Écrire dans un fichier
    QFile file(nomFichier);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << html;
        file.close();

        // Ouvrir dans le navigateur
        QDesktopServices::openUrl(QUrl::fromLocalFile(nomFichier));
        return true;
    } else {
        qDebug() << "Erreur : Impossible d'écrire dans" << nomFichier;
        return false;
    }
}

QString Sponsor::genererHTMLPourPDF(QSqlQueryModel *model) {
    QString html;
    html += "<!DOCTYPE html>";
    html += "<html><head><meta charset='UTF-8'>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; }";
    html += "h1 { color: #7D4FEE; text-align: center; }";
    html += "table { width: 100%; border-collapse: collapse; margin: 20px 0; }";
    html += "th { background-color: #7D4FEE; color: white; padding: 10px; text-align: left; }";
    html += "td { border: 1px solid #ddd; padding: 8px; }";
    html += "tr:nth-child(even) { background-color: #f2f2f2; }";
    html += ".footer { text-align: right; font-style: italic; margin-top: 30px; }";
    html += "</style>";
    html += "<title>Liste des Sponsors</title>";
    html += "</head><body>";
    html += "<h1>Liste des Sponsors</h1>";
    html += "<table>";
    html += "<tr><th>ID</th><th>Nom</th><th>Prénom</th><th>Catégorie</th><th>Email</th><th>Début</th><th>Fin</th><th>Budget</th></tr>";

    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            html += "<td>" + model->data(model->index(row, col)).toString() + "</td>";
        }
        html += "</tr>";
    }

    html += "</table>";
    html += "<div class='footer'>";
    html += "<p>Généré le " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";
    html += "</div>";
    html += "</body></html>";
    return html;
}



// Méthodes de validation supplémentaires
bool Sponsor::validerEmail(const QString &email) {
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return regex.match(email).hasMatch();
}

bool Sponsor::validerDates(const QDate &debut, const QDate &fin) {
    return debut.isValid() && fin.isValid() && debut <= fin;
}

bool Sponsor::validerCategorie(const QString &categorie) {
    return !categorie.isEmpty();
}

bool Sponsor::validerPrenom(const QString &prenom) {
    return !prenom.isEmpty() && prenom.length() >= 2;
}

bool Sponsor::validerCIN(const QString &cin) {
    QRegularExpression regex("^[0-9]{8}$");
    return regex.match(cin).hasMatch();
}

bool Sponsor::validerTelephone(const QString &telephone) {
    QRegularExpression regex("^[0-9]{8}$");
    return regex.match(telephone).hasMatch();
}

bool Sponsor::validerSalaire(double salaire) {
    return salaire >= 0;
}
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>

// Ajoutez cette fonction à la fin du fichier :
bool Sponsor::exporterHTMLParContrat(const QString &nomFichier, const QString &filtre) {
    // 1. Récupérer les données selon le filtre
    QSqlQuery query;
    if (filtre.toLower() == "tous" || filtre.isEmpty()) {
        query.exec("SELECT * FROM sponsor");
    } else {
        query.prepare("SELECT * FROM sponsor WHERE CATEGORIE = :categorie");
        query.bindValue(":categorie", filtre);
        query.exec();
    }

    // 2. Créer le contenu HTML
    QString html;
    html += "<!DOCTYPE html>";
    html += "<html lang='fr'>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<title>Liste des Sponsors - ConnectPlus</title>";
    html += "<style>";
    html += "body { font-family: 'Segoe UI', Arial, sans-serif; margin: 40px; background-color: #f5f5f5; }";
    html += ".container { background: white; padding: 30px; border-radius: 10px; box-shadow: 0 0 20px rgba(0,0,0,0.1); }";
    html += "h1 { color: #7D4FEE; text-align: center; border-bottom: 3px solid #7D4FEE; padding-bottom: 15px; }";
    html += ".header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 30px; }";
    html += ".info-box { background: #f0f0f0; padding: 15px; border-radius: 5px; margin: 20px 0; }";
    html += "table { width: 100%; border-collapse: collapse; margin: 20px 0; }";
    html += "th { background-color: #7D4FEE; color: white; padding: 15px; text-align: left; font-weight: bold; }";
    html += "td { border: 1px solid #ddd; padding: 12px; }";
    html += "tr:nth-child(even) { background-color: #f9f9f9; }";
    html += "tr:hover { background-color: #f0f0ff; }";
    html += ".budget { text-align: right; font-weight: bold; color: #2E7D32; }";
    html += ".footer { text-align: center; margin-top: 40px; color: #666; font-size: 0.9em; }";
    html += ".stats { background: #e8f5e9; padding: 15px; border-radius: 5px; margin-top: 30px; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";

    html += "<div class='container'>";

    // En-tête avec titre et date
    html += "<div class='header'>";
    html += "<div>";
    html += "<h1>📋 LISTE DES SPONSORS</h1>";
    html += "<p><strong>Système :</strong> ConnectPlus Management</p>";
    html += "</div>";
    html += "<div style='text-align: right;'>";
    html += "<p><strong>Date :</strong> " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";
    html += "<p><strong>Heure :</strong> " + QTime::currentTime().toString("hh:mm") + "</p>";
    html += "</div>";
    html += "</div>";

    // Information sur le filtre
    html += "<div class='info-box'>";
    html += "<strong>Filtre appliqué :</strong> " + (filtre.isEmpty() ? "Tous les sponsors" : "Catégorie : " + filtre);
    html += "</div>";

    // Tableau des sponsors
    html += "<table>";
    html += "<thead>";
    html += "<tr>";
    html += "<th>ID</th>";
    html += "<th>Nom</th>";
    html += "<th>Prénom</th>";
    html += "<th>Catégorie</th>";
    html += "<th>Email</th>";
    html += "<th>Début Contrat</th>";
    html += "<th>Fin Contrat</th>";
    html += "<th>Budget (€)</th>";
    html += "</tr>";
    html += "</thead>";
    html += "<tbody>";

    int totalBudget = 0;
    int rowCount = 0;
    int activeContracts = 0;
    QDate today = QDate::currentDate();

    while (query.next()) {
        rowCount++;
        QDate finContrat = query.value("FINCONTRAT").toDate();
        bool isActive = (finContrat >= today);

        html += "<tr>";
        html += "<td>" + query.value("IDSPONSOR").toString() + "</td>";
        html += "<td>" + query.value("NOM").toString() + "</td>";
        html += "<td>" + query.value("PRENOM").toString() + "</td>";
        html += "<td>" + query.value("CATEGORIE").toString() + "</td>";
        html += "<td>" + query.value("EMAIL").toString() + "</td>";
        html += "<td>" + query.value("DEBUTCONTRAT").toDate().toString("dd/MM/yyyy") + "</td>";

        // Colorer la date de fin si le contrat est expiré
        if (!isActive) {
            html += "<td style='color: #d32f2f; font-weight: bold;'>" + finContrat.toString("dd/MM/yyyy") + " (EXPIRÉ)</td>";
        } else {
            html += "<td>" + finContrat.toString("dd/MM/yyyy") + "</td>";
            activeContracts++;
        }

        int budget = query.value("BUDGET").toInt();
        totalBudget += budget;
        html += "<td class='budget'>" + QString::number(budget) + " €</td>";
        html += "</tr>";
    }

    html += "</tbody>";
    html += "</table>";

    // Résumé statistique
    html += "<div class='stats'>";
    html += "<h3>📊 RÉSUMÉ STATISTIQUE</h3>";
    html += "<table style='width: 100%;'>";
    html += "<tr>";
    html += "<td><strong>Nombre total de sponsors :</strong></td>";
    html += "<td>" + QString::number(rowCount) + "</td>";
    html += "<td><strong>Contrats actifs :</strong></td>";
    html += "<td>" + QString::number(activeContracts) + "</td>";
    html += "</tr>";
    html += "<tr>";
    html += "<td><strong>Budget total :</strong></td>";
    html += "<td style='color: #2E7D32; font-weight: bold;'>" + QString::number(totalBudget) + " €</td>";
    html += "<td><strong>Budget moyen :</strong></td>";
    html += "<td>" + (rowCount > 0 ? QString::number(totalBudget / rowCount) : "0") + " €</td>";
    html += "</tr>";
    html += "</table>";
    html += "</div>";

    // Instructions pour l'utilisateur
    html += "<div style='margin: 30px 0; padding: 15px; background: #fff3e0; border-radius: 5px;'>";
    html += "<p><strong>💡 Comment imprimer en PDF :</strong></p>";
    html += "<ol>";
    html += "<li>Appuyez sur <strong>Ctrl+P</strong> (Windows/Linux) ou <strong>Cmd+P</strong> (Mac)</li>";
    html += "<li>Choisissez l'imprimante <strong>'Microsoft Print to PDF'</strong> ou <strong>'Save as PDF'</strong></li>";
    html += "<li>Sélectionnez l'orientation <strong>Paysage</strong> pour une meilleure lisibilité</li>";
    html += "<li>Cliquez sur <strong>Enregistrer</strong> pour créer le fichier PDF</li>";
    html += "</ol>";
    html += "</div>";

    // Pied de page
    html += "<div class='footer'>";
    html += "<p>Document généré automatiquement par ConnectPlus Management System</p>";
    html += "<p>© " + QString::number(QDate::currentDate().year()) + " ConnectPlus - Tous droits réservés</p>";
    html += "</div>";

    html += "</div>"; // Fermeture du container
    html += "</body>";
    html += "</html>";

    // 3. Écrire le contenu dans un fichier
    QFile file(nomFichier);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << html;
        file.close();

        // Ouvrir le fichier dans le navigateur par défaut
        QDesktopServices::openUrl(QUrl::fromLocalFile(nomFichier));

        return true;
    } else {
        qDebug() << "Erreur : Impossible d'écrire dans le fichier" << nomFichier;
        return false;
    }
}
