#include "smtp.h"

Smtp::Smtp(const QString &user, const QString &pass, const QString &host, int port)
{
    this->socket = new QSslSocket(this);
    this->user = user;
    this->pass = pass;
    this->host = host;
    this->port = port;

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

Smtp::~Smtp()
{
    delete t;
    delete socket;
}

void Smtp::sendMail(const QString &from, const QString &to, const QString &subject, const QString &body)
{
    this->from = from;
    this->rcpt = to;

    // Construction de l'en-tête du mail
    message = "To: " + to + "\n";
    message.append("From: " + from + "\n");
    message.append("Subject: " + subject + "\n");
    message.append("MIME-Version: 1.0\n");
    message.append("Content-Type: text/plain; charset=UTF-8\n\n"); // Important pour les accents !

    message.append(body);
    message.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    message.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));

    this->state = Init;
    socket->connectToHostEncrypted(host, port); // Connexion SSL (Port 465)

    if (!socket->waitForConnected(30000)) {
        qDebug() << "Erreur de connexion SMTP";
    }
}

void Smtp::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "SMTP State:" << socketState;
}

void Smtp::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << "SMTP Error:" << socketError;
}

void Smtp::disconnected()
{
    qDebug() << "SMTP Disconnected";
}

void Smtp::connected()
{
    qDebug() << "SMTP Connected";
}

void Smtp::readyRead()
{
    // Machine à états pour dialoguer avec le serveur Gmail
    QTextStream t(socket);
    QString response = socket->readAll();
    qDebug() << "Server Response:" << response;

    if (state == Init && response.startsWith("220")) {
        t << "EHLO localhost" << "\r\n";
        t.flush();
        state = HandShake;
    }
    else if (state == HandShake && response.startsWith("250")) {
        t << "AUTH LOGIN" << "\r\n";
        t.flush();
        state = Auth;
    }
    else if (state == Auth && response.startsWith("334")) {
        t << QByteArray().append(user.toUtf8()).toBase64() << "\r\n";
        t.flush();
        state = User;
    }
    else if (state == User && response.startsWith("334")) {
        t << QByteArray().append(pass.toUtf8()).toBase64() << "\r\n";
        t.flush();
        state = Pass;
    }
    else if (state == Pass && response.startsWith("235")) {
        // Authentification réussie
        t << "MAIL FROM:<" << from << ">\r\n";
        t.flush();
        state = Mail;
    }
    else if (state == Mail && response.startsWith("250")) {
        t << "RCPT TO:<" << rcpt << ">\r\n";
        t.flush();
        state = Rcpt;
    }
    else if (state == Rcpt && response.startsWith("250")) {
        t << "DATA\r\n";
        t.flush();
        state = Data;
    }
    else if (state == Data && response.startsWith("354")) {
        t << message << "\r\n.\r\n"; // Envoi du message + point final
        t.flush();
        state = Body;
    }
    else if (state == Body && response.startsWith("250")) {
        t << "QUIT\r\n";
        t.flush();
        state = Quit;
    }
    else if (state == Quit) {
        socket->close();
    }
}
