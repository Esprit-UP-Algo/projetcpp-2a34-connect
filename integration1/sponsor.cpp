#include "sponsor.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

Sponsor::Sponsor() : budget(0)
{
}

Sponsor::Sponsor(QString nom, QString prenom, QString categorie, QString email,
                 QDate debutcontrat, QDate fincontrat, int budget)
    : nom(nom), prenom(prenom), categorie(categorie), email(email),
    debutcontrat(debutcontrat), fincontrat(fincontrat), budget(budget)
{
}

bool Sponsor::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO sponsor (NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET) "
                  "VALUES (:nom, :prenom, :categorie, :email, :debut, :fin, :budget)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":categorie", categorie);
    query.bindValue(":email", email);
    query.bindValue(":debut", debutcontrat);
    query.bindValue(":fin", fincontrat);
    query.bindValue(":budget", budget);

    if (query.exec()) {
        qDebug() << "✅ Sponsor ajouté avec succès";
        return true;
    } else {
        qDebug() << "❌ Erreur ajout sponsor:" << query.lastError().text();
        return false;
    }
}

bool Sponsor::modifier(int id, QString nom, QString prenom, QString categorie, QString email,
                       QDate debutcontrat, QDate fincontrat, int budget)
{
    QSqlQuery query;
    query.prepare("UPDATE sponsor SET NOM=:nom, PRENOM=:prenom, CATEGORIE=:categorie, "
                  "EMAIL=:email, DEBUTCONTRAT=:debut, FINCONTRAT=:fin, BUDGET=:budget "
                  "WHERE IDSPONSOR=:id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":categorie", categorie);
    query.bindValue(":email", email);
    query.bindValue(":debut", debutcontrat);
    query.bindValue(":fin", fincontrat);
    query.bindValue(":budget", budget);

    if (query.exec()) {
        qDebug() << "✅ Sponsor modifié avec succès ID:" << id;
        return true;
    } else {
        qDebug() << "❌ Erreur modification sponsor:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Sponsor::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor");
    return model;
}

bool Sponsor::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM sponsor WHERE IDSPONSOR=:id");
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "✅ Sponsor supprimé avec succès ID:" << id;
        return true;
    } else {
        qDebug() << "❌ Erreur suppression sponsor:" << query.lastError().text();
        return false;
    }
}

QSqlQueryModel* Sponsor::trierParDate()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET "
                    "FROM sponsor ORDER BY DEBUTCONTRAT DESC");
    return model;
}

QSqlQueryModel* Sponsor::rechercherParNom(QString nom)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET "
                  "FROM sponsor WHERE UPPER(NOM) LIKE UPPER(:nom) OR UPPER(PRENOM) LIKE UPPER(:nom)");
    query.bindValue(":nom", "%" + nom + "%");

    if (query.exec()) {
        QSqlQueryModel* resultModel = new QSqlQueryModel();
        resultModel->setQuery(std::move(query));
        return resultModel;
    } else {
        return model;
    }
}

QSqlQueryModel* Sponsor::statistiques()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT CATEGORIE, COUNT(*), SUM(BUDGET) FROM sponsor GROUP BY CATEGORIE");
    return model;
}

QSqlQueryModel* Sponsor::getEmployesSponsor(int idSponsor)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT e.IDEMPLOYE, e.PRENOM, e.POSTE FROM empolye e "
                  "INNER JOIN sponsor_employe se ON e.IDEMPLOYE = se.IDEMPLOYE "
                  "WHERE se.IDSPONSOR = :id");
    query.bindValue(":id", idSponsor);

    if (query.exec()) {
        QSqlQueryModel* resultModel = new QSqlQueryModel();
        resultModel->setQuery(std::move(query));
        return resultModel;
    } else {
        return model;
    }
}

bool Sponsor::lierEmploye(int idSponsor, int idEmploye)
{
    QSqlQuery query;
    query.prepare("INSERT INTO sponsor_employe (IDSPONSOR, IDEMPLOYE) VALUES (:idSponsor, :idEmploye)");
    query.bindValue(":idSponsor", idSponsor);
    query.bindValue(":idEmploye", idEmploye);

    return query.exec();
}

// MÉTHODES PDF
bool Sponsor::exporterPDF(const QString &nomFichier)
{
    return exporterHTML(nomFichier);
}

bool Sponsor::exporterPDF(const QString &nomFichier, QSqlQueryModel *model)
{
    QFile file(nomFichier);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "❌ Impossible d'ouvrir le fichier:" << nomFichier;
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    QString html = genererHTMLPourPDF(model);
    out << html;

    file.close();
    qDebug() << "✅ PDF exporté avec succès:" << nomFichier;
    return true;
}

bool Sponsor::exporterHTML(const QString &nomFichier)
{
    Sponsor sponsor;
    QSqlQueryModel* model = sponsor.afficher();
    return exporterPDF(nomFichier, model);
}

// MÉTHODES DE VALIDATION AMÉLIORÉES
bool Sponsor::validerNom(const QString &nom)
{
    if (nom.isEmpty() || nom.length() < 2) {
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s'-]+$");
    return regex.match(nom).hasMatch();
}

bool Sponsor::validerPrenom(const QString &prenom)
{
    if (prenom.isEmpty() || prenom.length() < 2) {
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s'-]+$");
    return regex.match(prenom).hasMatch();
}

bool Sponsor::validerEmail(const QString &email)
{
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}

bool Sponsor::validerCategorie(const QString &categorie)
{
    if (categorie.isEmpty()) {
        return false;
    }

    QRegularExpression regex("^[a-zA-ZÀ-ÿ\\s'-]+$");
    return regex.match(categorie).hasMatch();
}

bool Sponsor::validerBudget(int budget)
{
    return budget >= 0;
}

bool Sponsor::validerDates(const QDate &debut, const QDate &fin)
{
    return debut.isValid() && fin.isValid() && debut <= fin;
}

bool Sponsor::validerID(int id)
{
    return id > 0;
}

bool Sponsor::validerID(const QString &idStr)
{
    bool ok;
    int id = idStr.toInt(&ok);
    return ok && id > 0;
}

bool Sponsor::validerCIN(const QString &cin)
{
    return !cin.isEmpty() && cin.length() >= 3;
}

bool Sponsor::validerTelephone(const QString &telephone)
{
    QRegularExpression regex(R"(^[0-9+\-\s\(\)]{8,15}$)");
    return regex.match(telephone).hasMatch();
}

bool Sponsor::validerSalaire(double salaire)
{
    return salaire >= 0;
}

// MÉTHODES HELPER POUR HTML
QString Sponsor::genererHTMLPourPDF()
{
    Sponsor sponsor;
    QSqlQueryModel* model = sponsor.afficher();
    return genererHTMLPourPDF(model);
}

QString Sponsor::genererHTMLPourPDF(QSqlQueryModel *model)
{
    QString html;
    html += "<!DOCTYPE html>";
    html += "<html lang='fr'>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Liste des Sponsors</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 20px; }";
    html += "h1 { color: #7D4FEE; text-align: center; }";
    html += "table { width: 100%; border-collapse: collapse; margin-top: 20px; }";
    html += "th, td { border: 1px solid #7D4FEE; padding: 8px; text-align: left; }";
    html += "th { background-color: #7D4FEE; color: white; }";
    html += "tr:nth-child(even) { background-color: #f2f2f2; }";
    html += ".footer { margin-top: 30px; text-align: center; font-size: 12px; color: #666; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<h1>Liste des Sponsors</h1>";
    html += "<p>Généré le " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";

    html += "<table>";
    html += "<tr>";
    html += "<th>ID</th><th>Nom</th><th>Prénom</th><th>Catégorie</th>";
    html += "<th>Email</th><th>Début</th><th>Fin</th><th>Budget</th>";
    html += "</tr>";

    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            html += "<td>" + model->data(model->index(row, col)).toString() + "</td>";
        }
        html += "</tr>";
    }

    html += "</table>";
    html += "<div class='footer'>";
    html += "Total: " + QString::number(model->rowCount()) + " sponsors";
    html += "</div>";
    html += "</body>";
    html += "</html>";

    return html;
}
// ==================== NOUVELLES MÉTHODES POUR EXPORT PAR CONTRAT ====================

bool Sponsor::exporterPDFParContrat(const QString &nomFichier, const QString &filtreContrat)
{
    QSqlQueryModel* model = filtrerParContrat(filtreContrat);
    if (!model) {
        qDebug() << "❌ Erreur: Impossible de filtrer les sponsors par contrat";
        return false;
    }

    QFile file(nomFichier);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "❌ Impossible d'ouvrir le fichier:" << nomFichier;
        delete model;
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    QString html = genererHTMLPourPDFParContrat(model, filtreContrat);
    out << html;

    file.close();
    delete model;

    qDebug() << "✅ PDF exporté avec succès:" << nomFichier << "Filtre:" << filtreContrat;
    return true;
}

QSqlQueryModel* Sponsor::filtrerParContrat(const QString &filtreContrat)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString sql = "SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor WHERE 1=1";

    QDate aujourdhui = QDate::currentDate();

    if (filtreContrat == "actifs") {
        sql += " AND DEBUTCONTRAT <= :aujourdhui AND FINCONTRAT >= :aujourdhui";
    } else if (filtreContrat == "expires") {
        sql += " AND FINCONTRAT < :aujourdhui";
    } else if (filtreContrat == "bientot_expires") {
        sql += " AND FINCONTRAT BETWEEN :aujourdhui AND :dans30jours";
    } else if (filtreContrat == "futurs") {
        sql += " AND DEBUTCONTRAT > :aujourdhui";
    }
    // "tous" ne nécessite pas de filtre supplémentaire

    sql += " ORDER BY NOM, PRENOM";

    QSqlQuery query;
    query.prepare(sql);

    query.bindValue(":aujourdhui", aujourdhui);

    if (filtreContrat == "bientot_expires") {
        query.bindValue(":dans30jours", aujourdhui.addDays(30));
    }

    if (query.exec()) {
        model->setQuery(std::move(query));
        return model;
    } else {
        qDebug() << "❌ Erreur lors du filtrage:" << query.lastError().text();
        delete model;
        return nullptr;
    }
}

QStringList Sponsor::obtenirTypesContrat()
{
    return QStringList() << "tous" << "actifs" << "expires" << "bientot_expires" << "futurs";
}

QString Sponsor::genererHTMLPourPDFParContrat(QSqlQueryModel *model, const QString &filtreContrat)
{
    QString nomFiltre;
    if (filtreContrat == "actifs") nomFiltre = "Contrats Actifs";
    else if (filtreContrat == "expires") nomFiltre = "Contrats Expirés";
    else if (filtreContrat == "bientot_expires") nomFiltre = "Contrats Bientôt Expirés (30 jours)";
    else if (filtreContrat == "futurs") nomFiltre = "Contrats Futurs";
    else nomFiltre = "Tous les Sponsors";

    QString html;
    html += "<!DOCTYPE html>";
    html += "<html lang='fr'>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Liste des Sponsors - " + nomFiltre + "</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; margin: 20px; }";
    html += "h1 { color: #7D4FEE; text-align: center; }";
    html += "h2 { color: #333; text-align: center; }";
    html += "table { width: 100%; border-collapse: collapse; margin-top: 20px; }";
    html += "th, td { border: 1px solid #7D4FEE; padding: 8px; text-align: left; }";
    html += "th { background-color: #7D4FEE; color: white; }";
    html += "tr:nth-child(even) { background-color: #f2f2f2; }";
    html += ".footer { margin-top: 30px; text-align: center; font-size: 12px; color: #666; }";
    html += ".info-box { background-color: #f0f8ff; border: 1px solid #7D4FEE; padding: 10px; margin: 10px 0; border-radius: 5px; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<h1>Liste des Sponsors</h1>";
    html += "<h2>" + nomFiltre + "</h2>";

    html += "<div class='info-box'>";
    html += "<strong>Date de génération:</strong> " + QDate::currentDate().toString("dd/MM/yyyy") + "<br>";
    html += "<strong>Heure de génération:</strong> " + QTime::currentTime().toString("hh:mm:ss") + "<br>";
    html += "<strong>Total d'enregistrements:</strong> " + QString::number(model->rowCount());
    html += "</div>";

    html += "<table>";
    html += "<tr>";
    html += "<th>ID</th><th>Nom</th><th>Prénom</th><th>Catégorie</th>";
    html += "<th>Email</th><th>Début Contrat</th><th>Fin Contrat</th><th>Budget (€)</th>";
    html += "</tr>";

    if (model->rowCount() == 0) {
        html += "<tr><td colspan='8' style='text-align: center;'>Aucun sponsor trouvé pour ce filtre</td></tr>";
    } else {
        for (int row = 0; row < model->rowCount(); ++row) {
            html += "<tr>";
            for (int col = 0; col < model->columnCount(); ++col) {
                html += "<td>" + model->data(model->index(row, col)).toString() + "</td>";
            }
            html += "</tr>";
        }
    }

    html += "</table>";

    // Statistiques
    if (model->rowCount() > 0) {
        double totalBudget = 0;
        QDate aujourdhui = QDate::currentDate();
        int contratsActifs = 0;
        int contratsExpires = 0;

        for (int row = 0; row < model->rowCount(); ++row) {
            totalBudget += model->data(model->index(row, 7)).toDouble(); // Budget à l'index 7

            QDate finContrat = QDate::fromString(model->data(model->index(row, 6)).toString(), "yyyy-MM-dd");
            if (finContrat >= aujourdhui) {
                contratsActifs++;
            } else {
                contratsExpires++;
            }
        }

        html += "<div class='footer'>";
        html += "<strong>Statistiques:</strong><br>";
        html += "• Budget total: " + QString::number(totalBudget, 'f', 0) + " €<br>";
        html += "• Budget moyen: " + QString::number(totalBudget / model->rowCount(), 'f', 0) + " €<br>";
        html += "• Contrats actifs: " + QString::number(contratsActifs) + "<br>";
        html += "• Contrats expirés: " + QString::number(contratsExpires);
        html += "</div>";
    }

    html += "</body>";
    html += "</html>";

    return html;
}
