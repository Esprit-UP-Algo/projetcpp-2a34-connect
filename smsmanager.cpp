#include "smsmanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QEventLoop> // Pour rendre la requête synchrone
#include <QByteArray>

SMSManager::SMSManager()
{
    networkManager = new QNetworkAccessManager(this);

    // CONFIGURATION PAR DÉFAUT (TWILIO)
    // ⚠️ IMPORTANT : Remplacez ces valeurs par vos vraies clés Twilio si vous voulez tester
    // Ne jamais laisser de clés sensibles dans un code public.
    apiProvider = "TWILIO";
    apiKey = "AC8b98a7c0d0141c5b5e4085cf13363b0d";      // Account SID
    apiSecret = "f4013108664a25e13ffbef2731c3d692";   // Auth Token
    fromNumber = "+17755224801";  // Votre numéro Twilio

    creerTableSMS();
}

SMSManager::~SMSManager()
{
    // Le networkManager sera supprimé automatiquement car SMSManager est son parent
}

void SMSManager::configurerAPI(const QString &provider, const QString &key, const QString &secret, const QString &from)
{
    this->apiProvider = provider;
    this->apiKey = key;
    this->apiSecret = secret;
    this->fromNumber = from;
    qDebug() << "✅ API SMS Configurée:" << provider << from;
}

bool SMSManager::envoyerSMS(const QString &numero, const QString &message)
{
    qDebug() << "📱 Envoi SMS à" << numero << ":" << message;

    if (apiKey.isEmpty() || apiSecret.isEmpty()) {
        qDebug() << "⚠️ Mode Simulation (Clés API manquantes)";
        sauvegarderSMS(-1, numero, message, true); // On simule un succès
        return true;
    }

    QUrl url;
    QNetworkRequest request;
    QByteArray postData;

    if (apiProvider == "TWILIO") {
        // Construction de l'URL Twilio
        url.setUrl("https://api.twilio.com/2010-04-01/Accounts/" + apiKey + "/Messages.json");
        request.setUrl(url);

        // En-tête Content-Type pour formulaire
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        // Authentification Basic (AccountSID:AuthToken en Base64)
        QString concatenated = apiKey + ":" + apiSecret;
        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        // Paramètres du corps de la requête (Body)
        QUrlQuery query;
        query.addQueryItem("To", numero);
        query.addQueryItem("From", fromNumber);
        query.addQueryItem("Body", message);
        postData = query.toString(QUrl::FullyEncoded).toUtf8();
    }
    else {
        qDebug() << "❌ Fournisseur non supporté:" << apiProvider;
        return false;
    }

    // Envoi de la requête de manière SYNCHRONE (bloquante) pour simplifier l'utilisation
    // Cela évite de devoir gérer des signaux/slots complexes dans MainWindow pour un simple SMS
    QNetworkReply *reply = networkManager->post(request, postData);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec(); // Attend la réponse

    bool succes = (reply->error() == QNetworkReply::NoError);

    if (succes) {
        qDebug() << "✅ SMS envoyé avec succès via API !";
        qDebug() << "   Réponse:" << reply->readAll();
    } else {
        qDebug() << "❌ Erreur API SMS:" << reply->errorString();
        qDebug() << "   Code HTTP:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "   Détails:" << reply->readAll();
    }

    reply->deleteLater();

    // Sauvegarde dans l'historique (-1 car pas forcément lié à un sponsor spécifique ici)
    sauvegarderSMS(-1, numero, message, succes);

    return succes;
}

// ==================== MÉTHODES MÉTIER (Sponsors) ====================

bool SMSManager::envoyerSMSSponsor(int idSponsor, const QString &message)
{
    QString numero = obtenirNumeroSponsor(idSponsor);
    if (numero.isEmpty()) {
        qDebug() << "❌ Numéro introuvable pour le sponsor ID" << idSponsor;
        return false;
    }

    bool resultat = envoyerSMS(numero, message);

    // Mise à jour de l'ID sponsor dans l'historique (car envoyerSMS met -1 par défaut)
    if (resultat) {
        // Optionnel : Mettre à jour le dernier enregistrement pour lier le bon ID
        // Mais plus simple : on appelle sauvegarder ici avec le bon ID
        sauvegarderSMS(idSponsor, numero, message, true);
    }

    return resultat;
}

bool SMSManager::envoyerSMSGroupe(const QList<int> &idSponsors, const QString &message)
{
    int succesCount = 0;
    for (int id : idSponsors) {
        if (envoyerSMSSponsor(id, message)) {
            succesCount++;
        }
    }
    return succesCount > 0;
}

QString SMSManager::obtenirNumeroSponsor(int idSponsor)
{
    QSqlQuery query;
    query.prepare("SELECT EMAIL FROM sponsor WHERE IDSPONSOR = :id"); // Utilise EMAIL ou TELEPHONE selon votre table
    // Note: Dans votre fichier sponsor.cpp/h fourni, je n'ai pas vu de colonne TELEPHONE dans la table sponsor
    // Si vous avez ajouté une colonne TELEPHONE, changez la requête ci-dessous :
    // query.prepare("SELECT TELEPHONE FROM sponsor WHERE IDSPONSOR = :id");

    query.bindValue(":id", idSponsor);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}

// ==================== GESTION HISTORIQUE SQL ====================

bool SMSManager::creerTableSMS()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS sms_historique ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "IDSPONSOR INTEGER, "
                      "NUMERO VARCHAR(20), "
                      "MESSAGE TEXT, "
                      "SUCCES INTEGER, " // 1 pour vrai, 0 pour faux
                      "HORODATAGE DATETIME DEFAULT CURRENT_TIMESTAMP)");
}

bool SMSManager::sauvegarderSMS(int idSponsor, const QString &numero, const QString &message, bool succes)
{
    QSqlQuery query;
    query.prepare("INSERT INTO sms_historique (IDSPONSOR, NUMERO, MESSAGE, SUCCES) VALUES (:id, :num, :msg, :succ)");
    query.bindValue(":id", idSponsor);
    query.bindValue(":num", numero);
    query.bindValue(":msg", message);
    query.bindValue(":succ", succes ? 1 : 0);
    return query.exec();
}

QList<QString> SMSManager::chargerHistoriqueSMS(int idSponsor)
{
    QList<QString> historique;
    QSqlQuery query;
    query.prepare("SELECT MESSAGE, HORODATAGE, SUCCES FROM sms_historique WHERE IDSPONSOR = :id ORDER BY HORODATAGE DESC");
    query.bindValue(":id", idSponsor);

    if (query.exec()) {
        while (query.next()) {
            QString msg = query.value(0).toString();
            QString date = query.value(1).toDateTime().toString("dd/MM HH:mm");
            bool ok = query.value(2).toInt() == 1;
            historique.append(QString("[%1] %2 : %3").arg(date).arg(ok ? "✅" : "❌").arg(msg));
        }
    }
    return historique;
}

// ==================== VALIDATIONS ====================

bool SMSManager::validerNumero(const QString &numero)
{
    // Doit contenir au moins 8 chiffres
    int digitCount = 0;
    for (QChar c : numero) {
        if (c.isDigit()) digitCount++;
    }
    return digitCount >= 8;
}

bool SMSManager::validerMessage(const QString &message)
{
    return !message.isEmpty();
}
