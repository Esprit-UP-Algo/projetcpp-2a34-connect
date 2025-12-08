#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool connectArduino(const QString& portName = "COM17", qint32 baudRate = QSerialPort::Baud9600);
    void disconnectArduino();
    bool isConnected() const;

    bool sendCommand(const QString& command);  // ex: "LIVE_ON:MrBeast"

signals:
    void arduinoConnected();
    void arduinoDisconnected();
    void errorOccurred(const QString& error);
    void dataReceived(const QByteArray& data);

private slots:
    void onReadyRead();

private:
    QSerialPort *m_serial;
};

#endif // ARDUINO_H
