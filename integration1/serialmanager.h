#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>

class SerialManager : public QObject
{
    Q_OBJECT

public:
    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    bool connectToArduino(const QString &portName = "");
    bool isConnected() const;
    void disconnectArduino();
    void sendCommand(const QString &command);

    QStringList getAvailablePorts() const;
    QString currentPort() const;

signals:
    void codeReceived(const QString &code);
    void accessGranted(const QString &nomEmploye);
    void accessDenied(const QString &code);
    void porteOuverte();
    void porteFermee();
    void connected(const QString &portName);
    void disconnected();
    void error(const QString &message);
    void dataReceived(const QString &data);

public slots:
    void onReadyRead();

private:
    QSerialPort *serial;
    QString m_currentPort;
    QTimer *reconnectTimer;
    QByteArray m_buffer;

    void processReceivedData(const QByteArray &data);
    bool testArduinoConnection();
};

#endif // SERIALMANAGER_H
