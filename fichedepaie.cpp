#include "fichedepaie.h"
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QPageSize>

FichePaie::FichePaie() {}

double FichePaie::calculerCNSS(double brut) {
    return brut * 0.0918; // 9.18%
}

double FichePaie::calculerIRPP(double imposable) {
    return 0.0; // Simplifié pour l'exemple
}

bool FichePaie::genererPDF(int idEmploye, const QString &cheminFichier)
{
    QSqlQuery query;

    // CORRECTION : On ne sélectionne QUE les colonnes qui existent vraiment dans votre table 'empolye'
    // Pas de NOM, pas de CIN. On utilise PRENOM pour le nom complet.
    query.prepare("SELECT PRENOM, TELEPHONE, DATEDEMBAUCHE, SALAIRE, POSTE FROM empolye WHERE IDEMPLOYE = :id");
    query.bindValue(":id", idEmploye);

    // Si la requête échoue, on affiche l'erreur dans la console pour comprendre
    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL FichePaie :" << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qDebug() << "❌ Employé introuvable ID :" << idEmploye;
        return false;
    }

    // Récupération des données (Index selon l'ordre du SELECT ci-dessus)
    QString nomComplet = query.value(0).toString(); // PRENOM
    QString telephone = query.value(1).toString();  // TELEPHONE
    QString cin = telephone;                        // On utilise le Tél comme CIN par défaut car pas de colonne CIN
    QDate dateEmb = query.value(2).toDate();        // DATEDEMBAUCHE
    double salaireBrut = query.value(3).toDouble(); // SALAIRE
    QString poste = query.value(4).toString();      // POSTE

    // Calculs
    double montantCNSS = calculerCNSS(salaireBrut);
    double salaireNet = salaireBrut - montantCNSS;

    // Création du PDF
    QPdfWriter pdfWriter(cheminFichier);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(30, 30, 30, 30));

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    int y = 100;
    int w = pdfWriter.width();

    // --- DESSIN ---

    // En-tête
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(0, y, "CONNECT PLUS+"); y += 300;

    painter.setFont(QFont("Arial", 10));
    painter.drawText(0, y, "Esprit Ariana"); y += 150;
    painter.drawText(0, y, "Tel: +216 71 123 456"); y += 400;

    // Titre (Couleur Violette #7D4FEE)
    // Note : L'avertissement jaune de votre image parlait de cette ligne, c'est sans danger.
    painter.setPen(QColor(125, 79, 238));
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(0, y, w, 500), Qt::AlignCenter, "BULLETIN DE PAIE");
    y += 600;

    // Cadre Employé
    painter.setPen(Qt::black);
    painter.drawRect(0, y, w - 60, 1800);

    int cx = 200; // Marge interne X
    int cy = y + 300; // Marge interne Y

    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.drawText(cx, cy, "INFORMATIONS EMPLOYÉ"); cy += 400;

    painter.setFont(QFont("Arial", 10));
    painter.drawText(cx, cy, "Nom Complet : " + nomComplet.toUpper()); cy += 250;
    painter.drawText(cx, cy, "ID / Tél : " + cin); cy += 250;
    painter.drawText(cx, cy, "Poste : " + poste); cy += 250;
    painter.drawText(cx, cy, "Date d'embauche : " + dateEmb.toString("dd/MM/yyyy"));

    y += 2000; // Saut après le cadre

    // Tableau Rémunération
    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.drawText(0, y, "DÉTAILS DE LA RÉMUNÉRATION"); y += 300;

    // Fonction locale pour dessiner une ligne
    auto drawLine = [&](QString label, QString value, bool isBold = false) {
        if(isBold) painter.setFont(QFont("Arial", 10, QFont::Bold));
        else painter.setFont(QFont("Arial", 10));

        painter.drawText(100, y, label);
        // Alignement à droite pour le montant
        painter.drawText(QRect(w/2, y, w/2 - 100, 300), Qt::AlignRight, value);
        y += 300;

        // Ligne de séparation fine
        painter.setPen(QColor(200, 200, 200));
        painter.drawLine(50, y-50, w-50, y-50);
        painter.setPen(Qt::black);
    };

    drawLine("Salaire Brut", QString::number(salaireBrut, 'f', 3) + " TND");

    y += 100;
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(0, y, "RETENUES"); y += 300;

    drawLine("CNSS (9.18%)", QString::number(montantCNSS, 'f', 3));
    drawLine("IRPP (Impot)", "0.000"); // Fixe pour l'instant

    // Total Net (Fond Violet)
    y += 200;
    painter.setBrush(QColor(125, 79, 238));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, y, w, 600);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(QRect(100, y, w/2, 600), Qt::AlignVCenter|Qt::AlignLeft, "NET À PAYER");
    painter.drawText(QRect(w/2, y, w/2 - 100, 600), Qt::AlignVCenter|Qt::AlignRight, QString::number(salaireNet, 'f', 3) + " TND");

    painter.end();
    return true;
}
