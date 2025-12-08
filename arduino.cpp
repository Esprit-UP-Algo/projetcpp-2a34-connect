#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent)
    : QObject(parent), m_serial(new QSerialPort(this))
{
    connect(m_serial, &QSerialPort::readyRead, this, &Arduino::onReadyRead);
}

Arduino::~Arduino()
{
    disconnectArduino();
}

bool Arduino::connectArduino(const QString& portName, qint32 baudRate)
{
    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);

    if (m_serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Arduino connecté sur" << portName;
        emit arduinoConnected();
        return true;
    } else {
        emit errorOccurred(m_serial->errorString());
        return false;
    }
}

void Arduino::disconnectArduino()
{
    if (m_serial->isOpen()) {
        m_serial->close();
        emit arduinoDisconnected();
    }
}

bool Arduino::isConnected() const
{
    return m_serial->isOpen();
}

bool Arduino::sendCommand(const QString& command)
{
    if (!m_serial->isOpen()) return false;

    QByteArray data = (command + "\n").toUtf8();
    qint64 written = m_serial->write(data);
    m_serial->flush();
    return written == data.size();
}

void Arduino::onReadyRead()
{
    QByteArray data = m_serial->readAll();
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}
