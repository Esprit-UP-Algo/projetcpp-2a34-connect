#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // Change le label dans le .ui ou ici (optionnel)
    ui->usernameEdit->setPlaceholderText("12345678");
    ui->passwordEdit->setPlaceholderText("Mot de passe");
    setWindowTitle("Connexion Employé - Smart Media Agency");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginButton_clicked()
{
    QString cin = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    // TEMPORAIRE : Autoriser la connexion avec admin/admin
    if (cin == "admin" && password == "admin") {
        m_cin = cin;
        m_nomComplet = "Administrateur";
        m_role = "admin";
        accept();
        return;
    }

    if (cin.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer CIN et mot de passe !");
        return;
    }

    // ... reste du code existant ...
}

// ✅ ADDED: Missing getter implementations
QString LoginDialog::role() const
{
    return m_role;
}

QString LoginDialog::cin() const
{
    return m_cin;
}

QString LoginDialog::nomComplet() const
{
    return m_nomComplet;
}
