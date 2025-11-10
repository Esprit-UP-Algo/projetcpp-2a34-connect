#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString role() const;

private slots:
    void on_loginButton_clicked();

private:
    Ui::LoginDialog *ui;
    QString m_role;

    // HASH FUNCTION
    QString hashPassword(const QString &username, const QString &password);
};

#endif // LOGINDIALOG_H
