#ifndef SMSMANAGER_H
#define SMSMANAGER_H

#include <QString>
#include <QList>
#include <QDate>
#include <QDateTime>
#include <QObject>

class SMSManager : public QObject
{
    Q_OBJECT

public:
    SMSManager();
    bool envoyerSMS(const QString &numero, const QString &message);
    static bool validerNumero(const QString &numero);
    static bool validerMessage(const QString &message);

    // Méthodes avancées pour les sponsors
    bool envoyerSMSSponsor(int idSponsor, const QString &message);
    bool envoyerSMSGroupe(const QList<int> &idSponsors, const QString &message);
    bool envoyerConfirmationAjout(int idSponsor);
    bool envoyerRappelContrat(int idSponsor, int joursAvant);
    bool envoyerNotificationExpiration(int idSponsor);

    // Gestion historique
    bool sauvegarderSMS(int idSponsor, const QString &numero, const QString &message, bool succes);
    QList<QString> chargerHistoriqueSMS(int idSponsor);

    // Configuration API
    void configurerAPI(const QString &provider, const QString &apiKey, const QString &apiSecret, const QString &fromNumber);

private:
    QString obtenirNumeroSponsor(int idSponsor);
    bool creerTableSMS();
    bool envoyerSMSTwilio(const QString &numero, const QString &message);
    bool envoyerSMSNexmo(const QString &numero, const QString &message);
     QString formaterNumeroInternational(const QString &numero);

    QString apiProvider;
    QString apiKey;
    QString apiSecret;
    QString fromNumber;
};

#endif // SMSMANAGER_H
