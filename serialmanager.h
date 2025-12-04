#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDateTime>

class SerialManager : public QObject
{
    Q_OBJECT

public:
    explicit SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    bool connectToArduino();
    bool isConnected() const;
    void disconnectArduino();

    void autoriserAcces(const QString &nom);
    void refuserAcces();

signals:
    void codeReceived(const QString &code);
    void accessGranted(const QString &employeName, const QDateTime &timestamp);
    void accessDenied(const QString &code);
    void porteOuverte();
    void porteFermee();
    void connected();
    void disconnected();
    void error(const QString &message);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void tenterReconnexion();

private:
    QSerialPort *serial;
    QTimer *reconnectTimer;
    QString buffer;

    void sendCommand(const QString &command);
};

#endif // SERIALMANAGER_H
