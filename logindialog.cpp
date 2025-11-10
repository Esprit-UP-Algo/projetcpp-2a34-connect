#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>        // ← ADD THIS
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

// ———————————————————————— HASH FUNCTION ————————————————————————


// ———————————————————————— LOGIN BUTTON ————————————————————————
void LoginDialog::on_loginButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Enter username and password!");
        return;
    }

    qDebug() << "Trying:" << username << "/" << password;

    QSqlQuery query;
    query.prepare("SELECT ROLE FROM \"MY_USER\".\"USERS\" WHERE USERNAME = :user AND PASSWORD = :pass");
    query.bindValue(":user", username);
    query.bindValue(":pass", password);  // ← PLAIN TEXT

    if (query.exec() && query.next()) {
        m_role = query.value(0).toString();
        qDebug() << "LOGIN OK! Role:" << m_role;
        accept();
    } else {
        qDebug() << "FAILED:" << query.lastError().text();
        QMessageBox::critical(this, "Login Failed", "Invalid username or password!");
    }
}
// ———————————————————————— GET ROLE ————————————————————————
QString LoginDialog::role() const
{
    return m_role;
}
