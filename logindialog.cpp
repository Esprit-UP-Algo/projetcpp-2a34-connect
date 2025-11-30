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

    if (cin.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer CIN et mot de passe !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, ROLE FROM MY_USER.EMPLOYES "
                  "WHERE CIN = :cin AND PASSWORD = :pass");

    query.bindValue(":cin", cin);
    query.bindValue(":pass", password);  // mot de passe en clair (OK pour PFE)

    if (query.exec()) {
        if (query.next()) {
            m_cin = query.value("CIN").toString();
            m_nomComplet = query.value("NOM").toString() + " " + query.value("PRENOM").toString();
            m_role = query.value("ROLE").toString();

            qDebug() << "LOGIN RÉUSSI →" << m_nomComplet << "| Rôle:" << m_role;

            accept(); // ferme la fenêtre avec succès
            return;
        }
    }

    qDebug() << "ÉCHEC LOGIN:" << query.lastError().text();
    QMessageBox::critical(this, "Échec de connexion", "CIN ou mot de passe incorrect !");
}

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
