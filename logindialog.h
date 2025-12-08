// logindialog.h
#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    // ←←← CES 3 LIGNES MANQUAIENT ←←←
    QString role() const;        // getter pour le rôle
    QString cin() const;         // getter pour le CIN
    QString nomComplet() const; // getter pour le nom complet

private slots:
    void on_loginButton_clicked();

private:
    Ui::LoginDialog *ui;
    QString m_role;
    QString m_cin;
    QString m_nomComplet;
};

#endif // LOGINDIALOG_H
