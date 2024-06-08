#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "sha256.h"
#include "qnet.h"
#include <QDialog>
#include "functions.h"

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);
    ~StartScreen();
    QNet *getOnline() const;
    QString userUUID() const;
    QString userName() const;
private slots:
    void setLoginForm();
    void setRegistrationForm();
    void OnlineAuthRequest(const QString& userLogin, const QString& pwd);
    void OnlineAuthResponce(QString authPkg);
    void RegUser(const QString& userName, const QString& userLogin, const QString& pwd);
    void RegUserResponce(QString regPkg);
    void onLoggedIn(QString& pkg);
    void onRejectRequested();
signals:
    void reqAuth();
private:
    void classConnections();
    void loginChk(const QString& login, QString& err);
    void pwdChk(const QString& pwd, QString& err);
    void nameChk(const QString& name, QString& err);
private:
    Ui::StartScreen *ui;
    QString m_userUUID;
    QString m_userName;
    QNet* Online;
    SHA256 sha;
};

#endif // STARTSCREEN_H
