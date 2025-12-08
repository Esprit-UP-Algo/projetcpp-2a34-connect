#ifndef CHATBOT_H
#define CHATBOT_H

#include <QString>
#include <QMap>
#include <QDate>
#include <QSqlQuery>
#include <QList>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>
#include <QSettings>

struct Message {
    QString texte;
    QString expediteur;
    QDateTime horodatage;
};

class Chatbot : public QObject
{
    Q_OBJECT

public:
    Chatbot();
    ~Chatbot();

    QString obtenirReponse(const QString &question, int idSponsor = -1);
    void sauvegarderConversation(int idSponsor, const QString &question, const QString &reponse);
    QList<Message> chargerHistorique(int idSponsor);
    bool supprimerHistorique(int idSponsor);
    int getNombreConversations(int idSponsor);

    // Configuration API IA
    void configurerAPI(const QString &apiKey, const QString &model = "gemini-pro");
    bool estConfigure() const;
    bool testerConnexionInternet();

private slots:
    void onReponseAPIReceived();

private:
    QMap<QString, QString> reponsesDirectes;
    QNetworkAccessManager *networkManager;
    QNetworkReply *currentReply;

    // Configuration API
    QString apiKey;
    QString model;
    bool apiConfiguree;

    void initialiserBaseConnaissances();
    QString normaliserTexte(const QString &texte);
    QString obtenirInfoSponsor(int idSponsor, const QString &typeInfo);
    bool creerTableHistorique();
    void chargerConfiguration();

    // Méthodes API IA
    QString appelAPI(const QString &question, int idSponsor);
    QString parserReponseAPI(const QJsonDocument &reponse);
    QString reponseFallback(const QString &question, int idSponsor);
};

#endif // CHATBOT_H
