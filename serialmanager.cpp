#include "serialmanager.h"
#include <QCoreApplication>
#include <QRegularExpression>  // Inclusion nécessaire pour QRegularExpression

SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
    , serial(nullptr)
    , reconnectTimer(nullptr)
{
    serial = new QSerialPort(this);

    // Configuration du port série
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Timer pour reconnexion automatique
    reconnectTimer = new QTimer(this);
    reconnectTimer->setInterval(5000); // 5 secondes
    connect(reconnectTimer, &QTimer::timeout, this, [this]() {
        if (!serial->isOpen()) {
            qDebug() << "Tentative de reconnexion Arduino...";
            connectToArduino();
        }
    });

    connect(serial, &QSerialPort::readyRead, this, &SerialManager::onReadyRead);
    connect(serial, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        if (error != QSerialPort::NoError && error != QSerialPort::ResourceError) {
            qDebug() << "Erreur port série:" << serial->errorString();
            emit this->error(serial->errorString());
        }
    });
}

SerialManager::~SerialManager()
{
    disconnectArduino();
}

bool SerialManager::connectToArduino(const QString &portName)
{
    disconnectArduino();

    QString portToUse = portName;

    // Si aucun port spécifié, chercher automatiquement
    if (portToUse.isEmpty()) {
        QStringList ports = getAvailablePorts();
        if (ports.isEmpty()) {
            qDebug() << "Aucun port série disponible";
            emit error("Aucun port série disponible");
            return false;
        }

        // Essayer les ports COM habituels (Arduino)
        QStringList preferredPorts = {"COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10", "COM17"};
        for (const QString &preferred : preferredPorts) {
            if (ports.contains(preferred)) {
                portToUse = preferred;
                break;
            }
        }

        // Si aucun port préféré trouvé, prendre le premier
        if (portToUse.isEmpty()) {
            portToUse = ports.first();
        }
    }

    serial->setPortName(portToUse);

    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Impossible d'ouvrir le port" << portToUse << ":" << serial->errorString();
        emit error("Impossible d'ouvrir le port " + portToUse + ": " + serial->errorString());
        return false;
    }

    // Test de la connexion
    if (testArduinoConnection()) {
        m_currentPort = portToUse;
        qDebug() << "✅ Arduino connecté sur" << portToUse;
        emit connected(portToUse);

        // Démarrer le timer de reconnexion
        reconnectTimer->start();

        return true;
    } else {
        serial->close();
        qDebug() << "❌ Arduino non détecté sur" << portToUse;
        emit error("Arduino non détecté sur " + portToUse);
        return false;
    }
}

bool SerialManager::isConnected() const
{
    return serial && serial->isOpen();
}

void SerialManager::disconnectArduino()
{
    if (serial && serial->isOpen()) {
        serial->close();
        emit disconnected();
    }
    reconnectTimer->stop();
}

void SerialManager::sendCommand(const QString &command)
{
    if (!isConnected()) {
        qDebug() << "⚠️ Impossible d'envoyer la commande: Arduino non connecté";
        emit error("Arduino non connecté");
        return;
    }

    QString cmd = command;
    if (!cmd.endsWith('\n')) {
        cmd += '\n';
    }

    qint64 bytesWritten = serial->write(cmd.toUtf8());
    if (bytesWritten == -1) {
        qDebug() << "❌ Erreur d'écriture sur le port série:" << serial->errorString();
        emit error("Erreur d'écriture: " + serial->errorString());
    } else {
        qDebug() << "📤 Commande envoyée à Arduino:" << command.trimmed();
        serial->flush();
    }
}

QStringList SerialManager::getAvailablePorts() const
{
    QStringList ports;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        // Filtrer les ports USB (typiques pour Arduino)
        if (!info.portName().isEmpty()) {
            ports << info.portName();
            qDebug() << "Port disponible:" << info.portName()
                     << "Description:" << info.description()
                     << "Manufacturer:" << info.manufacturer();
        }
    }
    return ports;
}

QString SerialManager::currentPort() const
{
    return m_currentPort;
}

void SerialManager::onReadyRead()
{
    if (!serial || !serial->isOpen()) {
        return;
    }

    m_buffer.append(serial->readAll());

    // Traiter les lignes complètes
    int pos;
    while ((pos = m_buffer.indexOf('\n')) != -1) {
        QByteArray line = m_buffer.left(pos).trimmed();
        m_buffer = m_buffer.mid(pos + 1);

        if (!line.isEmpty()) {
            processReceivedData(line);
        }
    }
}

void SerialManager::processReceivedData(const QByteArray &data)
{
    QString received = QString::fromUtf8(data).trimmed();
    qDebug() << "📥 Ligne reçue d'Arduino:" << received;

    // Traitement des codes badges (format: "CODE:XXXX")
    if (received.startsWith("CODE:")) {
        QString code = received.mid(5).trimmed(); // Extraire les 4 chiffres après "CODE:"
        qDebug() << "🔑 Code badge détecté:" << code;
        emit codeReceived(code);
    }
    // Traitement des réponses d'autorisation
    else if (received.contains("=== ACCES AUTORISE ===")) {
        // On ne fait rien ici, car le nom viendra sur la ligne suivante
        qDebug() << "✅ Accès autorisé par Arduino";
    }
    else if (received.startsWith("Bienvenue ")) {
        // Extraire le nom de la ligne "Bienvenue NOM"
        QString nom = received.mid(10).trimmed();
        qDebug() << "👤 Nom employé:" << nom;
        emit accessGranted(nom);
    }
    else if (received.contains("=== ACCES REFUSE ===")) {
        qDebug() << "❌ Accès refusé par Arduino";
        emit accessDenied(""); // Pas de code spécifique
    }
    // Traitement des états de la porte
    else if (received.contains("PORTE: Ouverte")) {
        emit porteOuverte();
    }
    else if (received.contains("PORTE: Fermee")) {
        emit porteFermee();
    }
    else if (received.contains("ERREUR:") || received.contains("Code efface")) {
        // Messages d'erreur ou d'information
        qDebug() << "📝 Message Arduino:" << received;
    }
    else {
        // Autres données
        qDebug() << "📝 Message Arduino non traité:" << received;
    }
}

bool SerialManager::testArduinoConnection()
{
    if (!serial || !serial->isOpen()) {
        return false;
    }

    // Envoyer une commande de test
    sendCommand("TEST");

    // Attendre une réponse (simplifié - dans une vraie app, utiliserait un timeout)
    QCoreApplication::processEvents();

    // Vérifier si le port répond
    return serial->isOpen() && serial->isReadable() && serial->isWritable();
}
