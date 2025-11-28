#include "smsmanager.h"
#include <QDebug>
#include <QSqlError>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include <QTextStream>

SMSManager::SMSManager()
{
    // CONFIGURATION AUTOMATIQUE TWILIO
    apiProvider = "TWILIO";
    apiKey = "AC8b98a7c0d0141c5b5e4085cf13363b0d";
    apiSecret = "f4013108664a25e13ffbef2731c3d692";
    fromNumber = "+17755224801";

    qDebug() << "✅ Configuration SMS automatique chargée";
    qDebug() << "   Fournisseur:" << apiProvider;
    qDebug() << "   Account SID:" << apiKey.left(10) + "...";
    qDebug() << "   Numéro:" << fromNumber;

    creerTableSMS();
}

void SMSManager::configurerAPI(const QString &provider, const QString &apiKey, const QString &apiSecret, const QString &fromNumber)
{
    this->apiProvider = provider.toUpper();
    this->apiKey = apiKey;
    this->apiSecret = apiSecret;
    this->fromNumber = fromNumber;

    qDebug() << "✅ Configuration SMS mise à jour:" << apiProvider << fromNumber;
}

bool SMSManager::envoyerSMS(const QString &numero, const QString &message)
{
    qDebug() << "📱 TENTATIVE D'ENVOI SMS";
    qDebug() << "   Numéro:" << numero;
    qDebug() << "   Message:" << message;

    if (!validerNumero(numero)) {
        qDebug() << "❌ Numéro invalide";
        QMessageBox::warning(nullptr, "Erreur SMS", "Le numéro de téléphone est invalide !");
        return false;
    }

    if (!validerMessage(message)) {
        qDebug() << "❌ Message invalide";
        QMessageBox::warning(nullptr, "Erreur SMS", "Le message est trop long (max 160 caractères) !");
        return false;
    }

    bool succes = false;
    QString details = "";

    if (apiProvider == "TWILIO" && !apiKey.isEmpty() && !apiSecret.isEmpty()) {
        succes = envoyerSMSTwilio(numero, message);
        details = "via Twilio API";
    } else if (apiProvider == "NEXMO" && !apiKey.isEmpty() && !apiSecret.isEmpty()) {
        succes = envoyerSMSNexmo(numero, message);
        details = "via Nexmo API";
    } else {
        qDebug() << "⚠️ Mode simulation - aucun fournisseur SMS configuré";
        succes = true;
        details = "SIMULATION";

        QMessageBox::information(nullptr, "SMS Simulation",
                                 QString("📱 SMS SIMULÉ\n\n"
                                         "À: %1\n"
                                         "Message: %2\n\n"
                                         "Pour envoyer de vrais SMS, configurez l'API Twilio ou Nexmo.")
                                     .arg(numero).arg(message));
    }

    sauvegarderSMS(-1, numero, message, succes);

    if (succes) {
        qDebug() << "✅ SMS envoyé avec succès" << details;
        QMessageBox::information(nullptr, "Succès", "✅ SMS envoyé avec succès !");
    } else {
        qDebug() << "❌ Échec envoi SMS";
        QMessageBox::critical(nullptr, "Erreur", "❌ Erreur lors de l'envoi du SMS !");
    }

    return succes;
}

bool SMSManager::envoyerSMSTwilio(const QString &numero, const QString &message)
{
    qDebug() << "🔧 Envoi via Twilio...";

    QStringList arguments;
    arguments << "/c"
              << "curl"
              << "-X" << "POST"
              << QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(apiKey)
              << "--data-urlencode" << QString("To=%1").arg(numero)
              << "--data-urlencode" << QString("From=%1").arg(fromNumber)
              << "--data-urlencode" << QString("Body=%1").arg(message)
              << "-u" << QString("%1:%2").arg(apiKey).arg(apiSecret);

    QProcess process;
    process.start("cmd", arguments);

    if (process.waitForFinished(15000)) {
        QString output = process.readAllStandardOutput();
        QString error = process.readAllStandardError();

        qDebug() << "📡 Réponse Twilio:" << output;

        if (process.exitCode() == 0) {
            if (output.contains("\"status\":\"queued\"") ||
                output.contains("\"status\":\"sent\"") ||
                output.contains("\"status\":\"accepted\"") ||
                output.contains("\"sid\"")) {
                qDebug() << "✅ SMS Twilio envoyé avec succès";
                return true;
            } else {
                qDebug() << "❌ Twilio: Réponse inattendue";
                QMessageBox::warning(nullptr, "Erreur Twilio",
                                     "Twilio a répondu mais le statut n'est pas favorable.\n\nRéponse: " + output);
                return false;
            }
        } else {
            qDebug() << "❌ Twilio erreur - Exit code:" << process.exitCode();
            qDebug() << "   Erreur:" << error;

            QString messageErreur;
            if (error.contains("Could not resolve host")) {
                messageErreur = "Erreur de connexion internet. Vérifiez votre connexion.";
            } else if (error.contains("unauthorized") || error.contains("authenticate")) {
                messageErreur = "Identifiants Twilio incorrects. Vérifiez votre Account SID et Auth Token.";
            } else if (error.contains("The 'To' number")) {
                messageErreur = "Numéro de téléphone invalide. Vérifiez le format du numéro.";
            } else {
                messageErreur = "Erreur Twilio: " + error;
            }

            QMessageBox::warning(nullptr, "Erreur Twilio", messageErreur);
            return false;
        }
    } else {
        qDebug() << "❌ Twilio: Timeout";
        QMessageBox::warning(nullptr, "Erreur Twilio",
                             "Timeout - La requête a pris trop de temps.\n"
                             "Vérifiez votre connexion internet et réessayez.");
        return false;
    }
}

bool SMSManager::envoyerSMSNexmo(const QString &numero, const QString &message)
{
    qDebug() << "🔧 Envoi via Nexmo...";

    QStringList arguments;
    arguments << "/c"
              << "curl"
              << "-X" << "POST"
              << "https://rest.nexmo.com/sms/json"
              << "-d" << QString("api_key=%1").arg(apiKey)
              << "-d" << QString("api_secret=%1").arg(apiSecret)
              << "-d" << QString("to=%1").arg(numero)
              << "-d" << QString("from=%1").arg(fromNumber)
              << "-d" << QString("text=%1").arg(message);

    QProcess process;
    process.start("cmd", arguments);

    if (process.waitForFinished(15000)) {
        QString output = process.readAllStandardOutput();
        QString error = process.readAllStandardError();

        qDebug() << "📡 Réponse Nexmo:" << output;

        if (process.exitCode() == 0 && output.contains("\"status\":\"0\"")) {
            qDebug() << "✅ SMS Nexmo envoyé avec succès";
            return true;
        } else {
            qDebug() << "❌ Nexmo erreur - Exit code:" << process.exitCode();
            qDebug() << "   Erreur:" << error;
            QMessageBox::warning(nullptr, "Erreur Nexmo",
                                 QString("Erreur lors de l'envoi Nexmo:\n%1").arg(error.isEmpty() ? output : error));
            return false;
        }
    } else {
        qDebug() << "❌ Nexmo: Timeout";
        QMessageBox::warning(nullptr, "Erreur Nexmo", "Timeout - Vérifiez votre connexion internet.");
        return false;
    }
}

bool SMSManager::validerNumero(const QString &numero)
{
    if (numero.isEmpty()) {
        return false;
    }

    QString numeroPropre = numero.simplified().remove(' ');

    if (numeroPropre.length() < 6) {
        return false;
    }

    bool contientChiffres = false;
    for (const QChar &c : numeroPropre) {
        if (c.isDigit()) {
            contientChiffres = true;
            break;
        }
    }

    return contientChiffres;
}

bool SMSManager::validerMessage(const QString &message)
{
    if (message.isEmpty() || message.length() > 160) {
        return false;
    }
    return true;
}

bool SMSManager::creerTableSMS()
{
    QSqlQuery query;
    QString sql = "CREATE TABLE IF NOT EXISTS sms_historique ("
                  "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "IDSPONSOR INTEGER, "
                  "NUMERO VARCHAR(15), "
                  "MESSAGE TEXT, "
                  "SUCCES BOOLEAN, "
                  "HORODATAGE DATETIME DEFAULT CURRENT_TIMESTAMP)";

    if (!query.exec(sql)) {
        qDebug() << "❌ Erreur création table SMS:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Table sms_historique prête";
    return true;
}

bool SMSManager::envoyerSMSSponsor(int idSponsor, const QString &message)
{
    QString numero = obtenirNumeroSponsor(idSponsor);

    if (numero.isEmpty()) {
        qDebug() << "❌ Numéro non trouvé pour sponsor" << idSponsor;
        return false;
    }

    bool succes = envoyerSMS(numero, message);
    return succes;
}

bool SMSManager::envoyerSMSGroupe(const QList<int> &idSponsors, const QString &message)
{
    int succes = 0;
    int echecs = 0;

    for (int id : idSponsors) {
        if (envoyerSMSSponsor(id, message)) {
            succes++;
        } else {
            echecs++;
        }
    }

    qDebug() << "📊 Envoi groupé: " << succes << " réussis," << echecs << " échoués";
    return echecs == 0;
}

bool SMSManager::envoyerConfirmationAjout(int idSponsor)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM sponsor WHERE IDSPONSOR = :id");
    query.bindValue(":id", idSponsor);

    if (!query.exec() || !query.next()) {
        return false;
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();

    QString message = "Bonjour " + nom + " " + prenom + ",\n\n"
                                                        "Votre inscription en tant que sponsor a été confirmée avec succès!\n\n"
                                                        "Bienvenue dans notre programme.";

    return envoyerSMSSponsor(idSponsor, message);
}

bool SMSManager::envoyerRappelContrat(int idSponsor, int joursAvant)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, FINCONTRAT FROM sponsor WHERE IDSPONSOR = :id");
    query.bindValue(":id", idSponsor);

    if (!query.exec() || !query.next()) {
        return false;
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    QDate dateFin = query.value(2).toDate();

    int joursRestants = QDate::currentDate().daysTo(dateFin);

    if (joursRestants != joursAvant) {
        return false;
    }

    QString message = "Rappel " + nom + " " + prenom + ":\n\n"
                                                       "Votre contrat expire dans " + QString::number(joursRestants) + " jours " +
                      "(le " + dateFin.toString("dd/MM/yyyy") + ").\n\n"
                                                                "Contactez-nous pour renouveler.";

    return envoyerSMSSponsor(idSponsor, message);
}

bool SMSManager::envoyerNotificationExpiration(int idSponsor)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, FINCONTRAT FROM sponsor WHERE IDSPONSOR = :id");
    query.bindValue(":id", idSponsor);

    if (!query.exec() || !query.next()) {
        return false;
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    QDate dateFin = query.value(2).toDate();

    QString message = "Attention " + nom + " " + prenom + "!\n\n"
                                                          "Votre contrat a expiré le " + dateFin.toString("dd/MM/yyyy") + ".\n\n"
                                                         "Merci de nous contacter rapidement.";

    return envoyerSMSSponsor(idSponsor, message);
}

bool SMSManager::sauvegarderSMS(int idSponsor, const QString &numero, const QString &message, bool succes)
{
    QSqlQuery query;
    query.prepare("INSERT INTO sms_historique (IDSPONSOR, NUMERO, MESSAGE, SUCCES) VALUES (:id, :num, :msg, :succ)");
    query.bindValue(":id", idSponsor);
    query.bindValue(":num", numero);
    query.bindValue(":msg", message);
    query.bindValue(":succ", succes);

    if (!query.exec()) {
        qDebug() << "❌ Erreur sauvegarde SMS:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ SMS sauvegardé dans l'historique";
    return true;
}

QList<QString> SMSManager::chargerHistoriqueSMS(int idSponsor)
{
    QList<QString> historique;
    QSqlQuery query;
    query.prepare("SELECT MESSAGE, HORODATAGE, SUCCES FROM sms_historique WHERE IDSPONSOR = :id ORDER BY HORODATAGE DESC");
    query.bindValue(":id", idSponsor);

    if (query.exec()) {
        while (query.next()) {
            QString message = query.value(0).toString();
            QDateTime date = query.value(1).toDateTime();
            bool succes = query.value(2).toBool();

            QString ligne = "[" + date.toString("dd/MM/yyyy hh:mm") + "] " +
                            (succes ? "✅" : "❌") + " " + message;
            historique.append(ligne);
        }
    }

    return historique;
}

QString SMSManager::obtenirNumeroSponsor(int idSponsor)
{
    QSqlQuery query;
    query.prepare("SELECT TELEPHONE FROM sponsor WHERE IDSPONSOR = :id");
    query.bindValue(":id", idSponsor);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}

QString SMSManager::formaterNumeroInternational(const QString &numero)
{
    QString numeroPropre = numero;
    numeroPropre = numeroPropre.remove(' ')
                       .remove('-')
                       .remove('(')
                       .remove(')')
                       .remove('.');

    if (numeroPropre.startsWith('+')) {
        return numeroPropre;
    }

    if (numeroPropre.startsWith("00")) {
        return "+" + numeroPropre.mid(2);
    }

    if (numeroPropre.length() == 8 && numeroPropre.startsWith("2") ||
        numeroPropre.length() == 8 && numeroPropre.startsWith("5") ||
        numeroPropre.length() == 8 && numeroPropre.startsWith("9")) {
        return "+216" + numeroPropre;
    }

    if (numeroPropre.length() == 9 && (numeroPropre.startsWith("2") ||
                                       numeroPropre.startsWith("5") ||
                                       numeroPropre.startsWith("9"))) {
        return "+216" + numeroPropre.mid(1);
    }

    if (numeroPropre.length() == 11 && numeroPropre.startsWith("216")) {
        return "+" + numeroPropre;
    }

    return numeroPropre;
}
