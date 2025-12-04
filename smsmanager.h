#ifndef SMSMANAGER_H
#define SMSMANAGER_H

#include <QString>
#include <QList>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSqlDatabase>

class SMSManager : public QObject
{
    Q_OBJECT

public:
    SMSManager();
    ~SMSManager();

    // Méthode principale d'envoi
    bool envoyerSMS(const QString &numero, const QString &message);

    // Configuration
    void configurerAPI(const QString &provider, const QString &apiKey, const QString &apiSecret, const QString &fromNumber);

    // Validations statiques
    static bool validerNumero(const QString &numero);
    static bool validerMessage(const QString &message);

    // Méthodes métier (Sponsors)
    bool envoyerSMSSponsor(int idSponsor, const QString &message);
    bool envoyerSMSGroupe(const QList<int> &idSponsors, const QString &message);

    // Gestion de l'historique
    bool sauvegarderSMS(int idSponsor, const QString &numero, const QString &message, bool succes);
    QList<QString> chargerHistoriqueSMS(int idSponsor);

private:
    QNetworkAccessManager *networkManager;

    // Configuration API
    QString apiProvider; // "TWILIO" par défaut
    QString apiKey;      // Account SID
    QString apiSecret;   // Auth Token
    QString fromNumber;

    // Méthodes internes
    bool creerTableSMS();
    QString obtenirNumeroSponsor(int idSponsor);
};

#endif // SMSMANAGER_H
