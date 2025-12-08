#include "chatbot.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QTime>
#include <QSettings>
#include <QTimer>

Chatbot::Chatbot() : QObject()
{
    networkManager = new QNetworkAccessManager(this);
    currentReply = nullptr;
    apiConfiguree = false;
    apiKey = "AIzaSyDMCs4LXbde5YnWjTHtuWp4gKoIo87Vugc";
    model = "gemini-2.0-flash";

    connect(networkManager, &QNetworkAccessManager::finished,
            this, &Chatbot::onReponseAPIReceived);

    // Charger la configuration sauvegardée
    chargerConfiguration();

    initialiserBaseConnaissances();
    creerTableHistorique();
}

Chatbot::~Chatbot()
{
    if (currentReply) {
        currentReply->deleteLater();
    }
}

void Chatbot::chargerConfiguration()
{
    QSettings settings;
    apiKey = settings.value("gemini/apiKey", "").toString();
    model = settings.value("gemini/model", "gemini-2.0-flash").toString();

    // CONFIGURATION AUTOMATIQUE GEMINI
    if (apiKey.isEmpty()) {
        apiKey = "AIzaSyDMCs4LXbde5YnWjTHtuWp4gKoIo87Vugc";
        settings.setValue("gemini/apiKey", apiKey);
        settings.setValue("gemini/model", model);
        qDebug() << "🔑 Clé Gemini configurée automatiquement";
    }

    apiConfiguree = !apiKey.isEmpty();

    if (apiConfiguree) {
        qDebug() << "🤖 Configuration Gemini chargée - Modèle:" << model;
    }
}

bool Chatbot::testerConnexionInternet()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://www.google.com"));
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QTimer::singleShot(5000, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool connected = (reply->error() == QNetworkReply::NoError);
    reply->deleteLater();

    qDebug() << "🌐 Test connexion Internet:" << (connected ? "✅ OK" : "❌ Échec");
    return connected;
}

void Chatbot::configurerAPI(const QString &newApiKey, const QString &newModel)
{
    this->apiKey = newApiKey.trimmed();
    if (!newModel.isEmpty()) {
        this->model = newModel;
    }
    this->apiConfiguree = !apiKey.isEmpty();

    QSettings settings;
    settings.setValue("gemini/apiKey", apiKey);
    settings.setValue("gemini/model", model);

    qDebug() << "🤖 API IA configurée - Modèle:" << this->model;
}

bool Chatbot::estConfigure() const
{
    return apiConfiguree;
}

QString Chatbot::obtenirReponse(const QString &question, int idSponsor)
{
    QString questionNormalisee = normaliserTexte(question);

    qDebug() << "🤖 Chatbot reçoit:" << question;
    qDebug() << "   API configurée:" << apiConfiguree;
    qDebug() << "   Sponsor ID:" << idSponsor;

    if (question.isEmpty()) {
        return "⚠️ Veuillez entrer une question !";
    }

    if (reponsesDirectes.contains(questionNormalisee)) {
        return reponsesDirectes[questionNormalisee];
    }

    if (questionNormalisee.contains("debut") || questionNormalisee.contains("commence") ||
        questionNormalisee.contains("fin") || questionNormalisee.contains("termine") ||
        questionNormalisee.contains("budget") || questionNormalisee.contains("montant") ||
        questionNormalisee.contains("categorie") || questionNormalisee.contains("type") ||
        questionNormalisee.contains("nom") || questionNormalisee.contains("prenom") ||
        questionNormalisee.contains("email") || questionNormalisee.contains("mail") ||
        questionNormalisee.contains("tout") || questionNormalisee.contains("info") ||
        questionNormalisee.contains("conseil") || questionNormalisee.contains("conseils") ||
        questionNormalisee.contains("combien") || questionNormalisee.contains("calcul")) {

        if (idSponsor > 0) {
            QString typeInfo = "tout";
            if (questionNormalisee.contains("debut")) typeInfo = "debut";
            else if (questionNormalisee.contains("fin")) typeInfo = "fin";
            else if (questionNormalisee.contains("budget")) typeInfo = "budget";
            else if (questionNormalisee.contains("categorie")) typeInfo = "categorie";
            else if (questionNormalisee.contains("nom")) typeInfo = "nom";
            else if (questionNormalisee.contains("email")) typeInfo = "email";

            return obtenirInfoSponsor(idSponsor, typeInfo);
        }
        return "📋 Pour accéder aux informations spécifiques d'un sponsor (budget, dates, catégorie, etc.), veuillez d'abord sélectionner un sponsor dans le tableau des sponsors.";
    }

    if (apiConfiguree && !apiKey.isEmpty()) {
        qDebug() << "🔄 Utilisation de l'API IA...";
        QString reponse = appelAPI(question, idSponsor);
        if (!reponse.contains("Erreur") && !reponse.contains("indisponible") && !reponse.contains("❌")) {
            return reponse;
        }
        qDebug() << "❌ Erreur API, utilisation du mode basique";
    }

    return reponseFallback(question, idSponsor);
}

QString Chatbot::appelAPI(const QString &question, int idSponsor)
{
    if (!testerConnexionInternet()) {
        return "❌ Aucune connexion Internet détectée. Vérifiez votre connexion.";
    }

    if (apiKey.isEmpty()) {
        return "❌ Clé API non configurée. Veuillez configurer Gemini dans les paramètres.";
    }

    if (!apiKey.startsWith("AIza")) {
        return "❌ Format de clé API invalide. La clé doit commencer par 'AIza'.";
    }

    QString contexte = "Tu es un assistant virtuel pour un système de gestion de sponsors. Réponds en français de manière concise et utile. ";

    if (idSponsor > 0) {
        QSqlQuery query;
        query.prepare("SELECT NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor WHERE IDSPONSOR = :id");
        query.bindValue(":id", idSponsor);

        if (query.exec() && query.next()) {
            QString nom = query.value(0).toString();
            QString prenom = query.value(1).toString();
            QString categorie = query.value(2).toString();
            QString email = query.value(3).toString();
            QDate debut = query.value(4).toDate();
            QDate fin = query.value(5).toDate();
            int budget = query.value(6).toInt();

            contexte += QString("Informations du sponsor consulté: %1 %2, Catégorie: %3, Email: %4, "
                                "Contrat du %5 au %6, Budget: %7 €. ")
                            .arg(nom).arg(prenom).arg(categorie).arg(email)
                            .arg(debut.toString("dd/MM/yyyy")).arg(fin.toString("dd/MM/yyyy"))
                            .arg(budget);
        }
    }

    contexte += "Utilise des émojis appropriés. Réponds en français.";

    QJsonObject textPart;
    textPart["text"] = QString("%1\n\nQuestion: %2").arg(contexte).arg(question);

    QJsonObject content;
    content["parts"] = QJsonArray{textPart};

    QJsonArray contentsArray;
    contentsArray.append(content);

    QJsonObject requestBody;
    requestBody["contents"] = contentsArray;

    QJsonDocument doc(requestBody);
    QString requestJson = doc.toJson(QJsonDocument::Compact);

    qDebug() << "🔗 Envoi requête à Gemini...";
    qDebug() << "   Modèle:" << model;

    QUrl url(QString("https://generativelanguage.googleapis.com/v1beta/models/%1:generateContent?key=%2")
                 .arg(model).arg(apiKey));

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");

    QEventLoop loop;
    QNetworkReply *reply = networkManager->post(request, doc.toJson());

    QTimer::singleShot(30000, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        QString errorDetails = reply->errorString();
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

        if (statusCode.isValid()) {
            int code = statusCode.toInt();
            errorDetails = QString("HTTP %1: %2").arg(code).arg(errorDetails);
        }

        qDebug() << "❌ Erreur réseau:" << errorDetails;
        QString erreur = QString("❌ Erreur de connexion: %1").arg(errorDetails);
        reply->deleteLater();
        return erreur;
    }

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode != 200) {
        qDebug() << "❌ Erreur HTTP:" << statusCode;

        QByteArray errorData = reply->readAll();
        QJsonDocument errorDoc = QJsonDocument::fromJson(errorData);
        if (errorDoc.isObject()) {
            QJsonObject errorObj = errorDoc.object();
            if (errorObj.contains("error")) {
                QString message = errorObj["error"].toObject()["message"].toString();
                qDebug() << "❌ Message d'erreur:" << message;
            }
        }

        QString erreur = QString("❌ Erreur serveur (HTTP %1)").arg(statusCode);
        reply->deleteLater();
        return erreur;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    qDebug() << "✅ Réponse reçue, taille:" << responseData.size();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    return parserReponseAPI(jsonResponse);
}

QString Chatbot::parserReponseAPI(const QJsonDocument &reponse)
{
    if (!reponse.isObject()) {
        return "❌ Format de réponse invalide de l'API.";
    }

    QJsonObject root = reponse.object();

    if (root.contains("error")) {
        QJsonObject error = root["error"].toObject();
        QString message = error["message"].toString();
        int code = error["code"].toInt();

        qDebug() << "❌ Erreur API Gemini:" << code << "-" << message;

        switch (code) {
        case 401:
            return "❌ Clé API invalide. Veuillez vérifier votre clé Gemini.";
        case 403:
            return "❌ Accès refusé. Vérifiez que l'API Gemini est activée dans Google Cloud.";
        case 429:
            return "❌ Quota dépassé. Réessayez plus tard ou vérifiez votre quota Google Cloud.";
        default:
            return QString("❌ Erreur API: %1").arg(message);
        }
    }

    if (root.contains("candidates")) {
        QJsonArray candidates = root["candidates"].toArray();
        if (candidates.isEmpty()) {
            return "❌ Aucune réponse générée par l'API.";
        }

        QJsonObject candidate = candidates[0].toObject();

        if (candidate.contains("content")) {
            QJsonObject content = candidate["content"].toObject();
            if (content.contains("parts")) {
                QJsonArray parts = content["parts"].toArray();
                if (!parts.isEmpty()) {
                    QString texte = parts[0].toObject()["text"].toString();
                    if (!texte.isEmpty()) {
                        texte = texte.trimmed();
                        texte.replace("```", "");
                        return texte;
                    }
                }
            }
        }
    }

    return "🤖 Je n'ai pas pu générer une réponse appropriée. Pouvez-vous reformuler votre question ?";
}

QString Chatbot::reponseFallback(const QString &question, int idSponsor)
{
    Q_UNUSED(idSponsor)

    QString questionNormalisee = normaliserTexte(question);

    if (questionNormalisee.contains("quoi de neuf") || questionNormalisee.contains("quoi de nouveau")) {
        return "🌟 Rien de spécial ! Je suis ici pour vous aider avec la gestion des sponsors. "
               "Posez-moi des questions sur les contrats, budgets, ou autres !";
    }

    if (questionNormalisee.contains("comment ça va") || questionNormalisee.contains("ça va")) {
        return "😊 Je vais bien, merci ! Prêt à vous aider avec vos sponsors. "
               "N'hésitez pas à me poser des questions !";
    }

    if (questionNormalisee.contains("heure")) {
        return "🕐 Il est " + QTime::currentTime().toString("hh:mm") + ". Le temps passe vite !";
    }

    if (questionNormalisee.contains("statistique") || questionNormalisee.contains("stats")) {
        return "📊 Pour voir les statistiques des sponsors, cliquez sur le bouton 'Stat' dans l'onglet Sponsors !";
    }

    if (questionNormalisee.contains("pdf") || questionNormalisee.contains("exporter") || questionNormalisee.contains("export")) {
        return "📄 Pour exporter en PDF : allez dans l'onglet Sponsors et cliquez sur le bouton 'PDF' !";
    }

    if (questionNormalisee.contains("intelligent") || questionNormalisee.contains("smart")) {
        return "🤖 Je fais de mon mieux ! Configurez l'API Gemini pour des réponses plus intelligentes.";
    }

    if (questionNormalisee.contains("ia") || questionNormalisee.contains("ai") || questionNormalisee.contains("intelligence artificielle")) {
        return "🧠 L'IA est fascinante ! Pour activer l'IA avancée, configurez Gemini dans les paramètres.";
    }

    return "❓ Je n'ai pas bien compris votre question.\n\n"
           "💡 Voici ce que je peux faire :\n"
           "• Donner des infos sur les sponsors (sélectionnez-en un d'abord)\n"
           "• Expliquer le système de gestion\n"
           "• Répondre à des questions générales\n"
           "• Aider avec les statistiques et exports\n\n"
           "🔧 Pour des réponses plus avancées, configurez l'API Gemini dans les paramètres !";
}

void Chatbot::onReponseAPIReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    reply->deleteLater();
}

void Chatbot::initialiserBaseConnaissances()
{
    reponsesDirectes["bonjour"] = "👋 Bonjour ! Je suis votre assistant virtuel pour la gestion des sponsors. Comment puis-je vous aider ?";
    reponsesDirectes["salut"] = "👋 Salut ! Que puis-je faire pour vous ?";
    reponsesDirectes["hello"] = "👋 Hello ! How can I help you?";
    reponsesDirectes["hi"] = "👋 Hi ! What can I do for you?";
    reponsesDirectes["merci"] = "😊 De rien ! N'hésitez pas si vous avez d'autres questions.";
    reponsesDirectes["au revoir"] = "👋 Au revoir ! À bientôt !";
    reponsesDirectes["bye"] = "👋 Bye ! See you soon!";
    reponsesDirectes["contrat"] = "📄 Votre contrat contient : la date de début, la date de fin, le budget alloué et votre catégorie.\n\nPour plus de détails, demandez-moi : 'Quand finit mon contrat ?' ou 'Quel est mon budget ?'";
    reponsesDirectes["date"] = "📅 Pour connaître vos dates de contrat, dites-moi : 'Date de début' ou 'Date de fin'";
    reponsesDirectes["budget"] = "💰 Demandez-moi : 'Quel est mon budget ?' pour connaître le montant alloué";
    reponsesDirectes["categorie"] = "🏷️ Votre catégorie définit le type de sponsoring (Sport, Technologie, Culture, etc.).\n\nDemandez : 'Quelle est ma catégorie ?'";
    reponsesDirectes["contact"] = "📞 Pour nous contacter :\n📧 Email : support@sponsors.com\n📱 Tél : +216 12 345 678\n🕐 Horaires : Lun-Ven 9h-18h";
    reponsesDirectes["email"] = "📧 Notre email : support@sponsors.com";
    reponsesDirectes["telephone"] = "📱 Notre téléphone : +216 12 345 678";
    reponsesDirectes["aide"] = "❓ Je peux vous aider avec :\n• Informations sur votre contrat\n• Dates de début/fin\n• Budget alloué\n• Votre catégorie\n• Contact support\n\nPosez-moi une question !";
    reponsesDirectes["help"] = "❓ I can help you with:\n• Contract information\n• Start/end dates\n• Budget\n• Category\n• Support contact";
    reponsesDirectes["exporter"] = "📊 Pour exporter les sponsors :\n1. Allez dans l'onglet Sponsors\n2. Cliquez sur le bouton 'PDF'\n3. Choisissez l'emplacement de sauvegarde";
    reponsesDirectes["ajouter"] = "➕ Pour ajouter un sponsor :\n1. Allez dans l'onglet 'Add'\n2. Remplissez le formulaire\n3. Cliquez sur 'Add'";
    reponsesDirectes["rechercher"] = "🔍 Pour rechercher un sponsor :\n• Utilisez la barre de recherche en haut\n• Tapez le nom ou la catégorie\n• La recherche se fait en temps réel";
    reponsesDirectes["systeme"] = "🤖 Ce système permet de gérer :\n• Les sponsors et leurs contrats\n• Les employés et leurs informations\n• L'envoi de messages SMS\n• Les conversations avec le chatbot";
    reponsesDirectes["fonctionnalites"] = "🚀 Fonctionnalités disponibles :\n✅ Gestion CRUD des sponsors\n✅ Gestion des employés\n✅ Chatbot intelligent\n✅ Envoi de SMS\n✅ Export PDF\n✅ Recherche et tri\n✅ Statistiques";
    reponsesDirectes["conseils budget"] = "💰 Conseils pour votre budget :\n• Planifiez vos dépenses mensuellement\n• Gardez 20% pour les imprévus\n• Suivez vos dépenses régulièrement\n• Renégociez si nécessaire";
    reponsesDirectes["exporter donnees"] = reponsesDirectes["exporter"];
    reponsesDirectes["ajouter sponsor"] = reponsesDirectes["ajouter"];
    reponsesDirectes["recherche"] = reponsesDirectes["rechercher"];
    reponsesDirectes["fonctionnalite"] = reponsesDirectes["fonctionnalites"];

    qDebug() << "✅ Base de connaissances initialisée avec" << reponsesDirectes.size() << "réponses";
}

QString Chatbot::normaliserTexte(const QString &texte)
{
    QString normalise = texte.toLower().trimmed();
    normalise.replace("?", "").replace("!", "").replace(".", "").replace(",", "");
    normalise.replace("é", "e").replace("è", "e").replace("ê", "e");
    normalise.replace("à", "a").replace("â", "a");
    normalise.replace("ù", "u").replace("û", "u");
    normalise.replace("ç", "c");
    normalise = normalise.trimmed();
    return normalise;
}

QString Chatbot::obtenirInfoSponsor(int idSponsor, const QString &typeInfo)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor WHERE IDSPONSOR = :id");
    query.bindValue(":id", idSponsor);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL chatbot:" << query.lastError().text();
        return "❌ Erreur lors de la récupération des informations.";
    }

    if (!query.next()) {
        return "❌ Sponsor non trouvé.";
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    QString categorie = query.value(2).toString();
    QString email = query.value(3).toString();
    QDate debut = query.value(4).toDate();
    QDate fin = query.value(5).toDate();
    int budget = query.value(6).toInt();

    int dureeJours = debut.daysTo(fin);
    int joursRestants = QDate::currentDate().daysTo(fin);
    QString etatContrat;

    if (joursRestants < 0) {
        etatContrat = QString("⚠️ Contrat expiré depuis %1 jours").arg(-joursRestants);
    } else if (joursRestants == 0) {
        etatContrat = "⏰ Contrat expire aujourd'hui !";
    } else if (joursRestants <= 30) {
        etatContrat = QString("🔔 Attention : expire dans %1 jours").arg(joursRestants);
    } else {
        etatContrat = QString("✅ Contrat actif (%1 jours restants)").arg(joursRestants);
    }

    double budgetQuotidien = (dureeJours > 0) ? static_cast<double>(budget) / dureeJours : 0;

    if (typeInfo == "debut") {
        int joursDepuisDebut = debut.daysTo(QDate::currentDate());
        return QString("📅 Date de début : %1\n🕐 Il y a %2 jours")
            .arg(debut.toString("dd/MM/yyyy"))
            .arg(joursDepuisDebut > 0 ? joursDepuisDebut : -joursDepuisDebut);
    }

    if (typeInfo == "fin") {
        return QString("📅 Date de fin : %1\n%2")
            .arg(fin.toString("dd/MM/yyyy"))
            .arg(etatContrat);
    }

    if (typeInfo == "budget") {
        return QString("💰 Budget alloué : %1 €\n"
                       "📊 Pour une durée de %2 jours\n"
                       "💵 Budget quotidien : %3 €/jour\n"
                       "📈 %4")
            .arg(budget)
            .arg(dureeJours)
            .arg(QString::number(budgetQuotidien, 'f', 2))
            .arg(etatContrat);
    }

    if (typeInfo == "categorie") {
        return QString("🏷️ Catégorie : %1\n📋 Type de sponsoring : %2")
            .arg(categorie)
            .arg(categorie);
    }

    if (typeInfo == "nom") {
        return QString("👤 Nom complet : %1 %2\n🏷️ Catégorie : %3")
            .arg(nom)
            .arg(prenom)
            .arg(categorie);
    }

    if (typeInfo == "email") {
        return QString("📧 Email : %1\n👤 %2 %3")
            .arg(email)
            .arg(nom)
            .arg(prenom);
    }

    if (typeInfo == "tout") {
        return QString("📋 INFORMATIONS COMPLÈTES\n\n"
                       "👤 Nom : %1 %2\n"
                       "🏷️ Catégorie : %3\n"
                       "📧 Email : %4\n"
                       "📅 Début : %5\n"
                       "📅 Fin : %6\n"
                       "💰 Budget : %7 €\n"
                       "💵 Quotidien : %8 €/jour\n"
                       "⏱️ Durée : %9 jours\n"
                       "📊 %10")
            .arg(nom)
            .arg(prenom)
            .arg(categorie)
            .arg(email)
            .arg(debut.toString("dd/MM/yyyy"))
            .arg(fin.toString("dd/MM/yyyy"))
            .arg(budget)
            .arg(QString::number(budgetQuotidien, 'f', 2))
            .arg(dureeJours)
            .arg(etatContrat);
    }

    return "❓ Type d'information non reconnu.";
}

bool Chatbot::creerTableHistorique()
{
    QSqlQuery query;
    QString sql = "CREATE TABLE IF NOT EXISTS chat_historique ("
                  "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "IDSPONSOR INTEGER, "
                  "QUESTION TEXT, "
                  "REPONSE TEXT, "
                  "HORODATAGE DATETIME DEFAULT CURRENT_TIMESTAMP)";

    if (!query.exec(sql)) {
        qDebug() << "❌ Erreur création table historique:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Table chat_historique prête";
    return true;
}

void Chatbot::sauvegarderConversation(int idSponsor, const QString &question, const QString &reponse)
{
    QSqlQuery query;
    query.prepare("INSERT INTO chat_historique (IDSPONSOR, QUESTION, REPONSE) VALUES (:id, :q, :r)");
    query.bindValue(":id", idSponsor);
    query.bindValue(":q", question);
    query.bindValue(":r", reponse);

    if (!query.exec()) {
        qDebug() << "❌ Erreur sauvegarde conversation:" << query.lastError().text();
    } else {
        qDebug() << "✅ Conversation sauvegardée pour sponsor" << idSponsor;
    }
}

QList<Message> Chatbot::chargerHistorique(int idSponsor)
{
    QList<Message> historique;
    QSqlQuery query;
    query.prepare("SELECT QUESTION, REPONSE, HORODATAGE FROM chat_historique WHERE IDSPONSOR = :id ORDER BY HORODATAGE ASC");
    query.bindValue(":id", idSponsor);

    if (query.exec()) {
        while (query.next()) {
            Message msgUser;
            msgUser.texte = query.value(0).toString();
            msgUser.expediteur = "user";
            msgUser.horodatage = query.value(2).toDateTime();
            historique.append(msgUser);

            Message msgBot;
            msgBot.texte = query.value(1).toString();
            msgBot.expediteur = "bot";
            msgBot.horodatage = query.value(2).toDateTime();
            historique.append(msgBot);
        }
    }

    return historique;
}

bool Chatbot::supprimerHistorique(int idSponsor)
{
    QSqlQuery query;
    query.prepare("DELETE FROM chat_historique WHERE IDSPONSOR = :id");
    query.bindValue(":id", idSponsor);
    return query.exec();
}

int Chatbot::getNombreConversations(int idSponsor)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM chat_historique WHERE IDSPONSOR = :id");
    query.bindValue(":id", idSponsor);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
