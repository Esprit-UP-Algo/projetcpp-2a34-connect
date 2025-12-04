#include "serialmanager.h"
#include <QDebug>
#include <QThread>

SerialManager::SerialManager(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);

    connect(serial, &QSerialPort::readyRead, this, &SerialManager::readData);
    connect(serial, &QSerialPort::errorOccurred, this, &SerialManager::handleError);

    reconnectTimer = new QTimer(this);
    reconnectTimer->setInterval(5000);
    connect(reconnectTimer, &QTimer::timeout, this, &SerialManager::tenterReconnexion);
}

SerialManager::~SerialManager()
{
    disconnectArduino();
}

bool SerialManager::connectToArduino()
{
    if (serial->isOpen()) {
        serial->close();
    }

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Port:" << info.portName()
        << "Description:" << info.description()
        << "Manufacturer:" << info.manufacturer()
        << "Serial Number:" << info.serialNumber();

        // Détection plus flexible pour Arduino
        bool isArduino = info.description().contains("Arduino", Qt::CaseInsensitive) ||
                         info.manufacturer().contains("Arduino", Qt::CaseInsensitive) ||
                         info.serialNumber().contains("Arduino", Qt::CaseInsensitive) ||
                         info.portName().contains("COM", Qt::CaseInsensitive) || // Windows
                         info.portName().contains("ttyUSB", Qt::CaseInsensitive) || // Linux
                         info.portName().contains("ttyACM", Qt::CaseInsensitive); // Linux

        if (isArduino) {
            serial->setPort(info);
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);

            if (serial->open(QIODevice::ReadWrite)) {
                qDebug() << "✅ Connecté à Arduino sur" << info.portName();
                reconnectTimer->stop();
                emit connected();

                // Envoyer un message de test
                sendCommand("TEST_CONNECTION");

                return true;
            } else {
                qDebug() << "❌ Échec connexion sur" << info.portName() << ":" << serial->errorString();
            }
        }
    }

    qDebug() << "❌ Arduino non détecté sur les ports disponibles";
    emit error("Arduino non détecté. Vérifiez la connexion USB.");
    reconnectTimer->start();
    return false;
}
bool SerialManager::isConnected() const
{
    return serial->isOpen();
}

void SerialManager::disconnectArduino()
{
    if (serial->isOpen()) {
        serial->close();
    }
    reconnectTimer->stop();
    emit disconnected();
}

void SerialManager::tenterReconnexion()
{
    if (!isConnected()) {
        qDebug() << "Tentative de reconnexion...";
        connectToArduino();
    }
}

void SerialManager::readData()
{
    QByteArray data = serial->readAll();
    buffer.append(data);

    int newlineIndex;
    while ((newlineIndex = buffer.indexOf('\n')) != -1) {
        QString line = buffer.left(newlineIndex).trimmed();
        buffer.remove(0, newlineIndex + 1);

        if (!line.isEmpty()) {
            qDebug() << "Arduino:" << line;

            if (line.startsWith("CODE:")) {
                QString code = line.mid(5);
                emit codeReceived(code);
            }
            else if (line.startsWith("PORTE: Ouverte")) {
                emit porteOuverte();
            }
            else if (line.startsWith("PORTE: Fermee")) {
                emit porteFermee();
            }
            else if (line.startsWith("Bienvenue ")) {
                QString nom = line.mid(10);
                emit accessGranted(nom, QDateTime::currentDateTime());
            }
        }
    }
}

void SerialManager::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError) {
        qDebug() << "Erreur série:" << serial->errorString();

        if (error == QSerialPort::ResourceError) {
            emit disconnected();
            reconnectTimer->start();
        }
    }
}

void SerialManager::sendCommand(const QString &command)
{
    if (serial->isOpen() && serial->isWritable()) {
        serial->write((command + "\n").toUtf8());
        serial->flush();
        qDebug() << "→ Arduino:" << command;
    }
}

void SerialManager::autoriserAcces(const QString &nom)
{
    sendCommand("AUTORISE:" + nom);
}

void SerialManager::refuserAcces()
{
    sendCommand("REFUSE");
}
