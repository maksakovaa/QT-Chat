#include "startscreen.h"
#include "ui_startscreen.h"

StartScreen::StartScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    Online = new QNet;
    classConnections();
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartScreen::setRegistrationForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StartScreen::onLoggedIn(QString& pkg)
{
    m_userName = pkg.first(pkg.indexOf("<|>"));
    pkg.remove(0, pkg.indexOf("<|>") + QString("<|>").length());
    m_userUUID = pkg;
    accept();
}

void StartScreen::onRejectRequested()
{
    reject();
}

void StartScreen::OnlineAuthRequest(const QString &userLogin, const QString &pwd)
{
    QString error;
    loginChk(userLogin, error);
    pwdChk(pwd, error);
    if(!error.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), error);
        return;
    }
    Online->sendToServer("AUTH<|>" + userLogin + "<|>" + sha(pwd));
}

void StartScreen::OnlineAuthResponce(QString authPkg)
{

    if (authPkg.startsWith("ERROR"))
    {
        QMessageBox::critical(this, "result", authPkg);
    }
    else
    {
        onLoggedIn(authPkg);
    }
}

void StartScreen::RegUser(const QString &userName, const QString &userLogin, const QString &pwd)
{
    QString error;
    nameChk(userName, error);
    loginChk(userLogin, error);
    pwdChk(pwd, error);
    if(!error.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), error);
        return;
    }
    Online->sendToServer("REG_USER<|>" + userName + "<|>" + userLogin + "<|>" + sha(pwd));
}

void StartScreen::RegUserResponce(QString regPkg)
{
    if (regPkg.startsWith("ERROR"))
    {
        QMessageBox::critical(this, "result", regPkg);
    }
    else
    {
        onLoggedIn(regPkg);
    }
}

void StartScreen::classConnections()
{
    connect(ui->LoginWidget, &LoginForm::sig_registerRequested, this, &StartScreen::setRegistrationForm);
    connect(ui->RegWidget, &RegForm::sig_loginRequested, this, &StartScreen::setLoginForm);

    connect(ui->RegWidget, &RegForm::sig_rejected, this, &StartScreen::onRejectRequested);
    connect(ui->LoginWidget, &LoginForm::sig_rejected, this, &StartScreen::onRejectRequested);

    connect(ui->LoginWidget, &LoginForm::sig_reqOnlineAuth, this, &StartScreen::OnlineAuthRequest);
    connect(Online, &QNet::authResponce, this, &StartScreen::OnlineAuthResponce);

    connect(ui->RegWidget, &RegForm::sig_regUser, this, &StartScreen::RegUser);
    connect(Online, &QNet::regResponce, this, &StartScreen::RegUserResponce);
}

void StartScreen::loginChk(const QString &login, QString &err)
{
    if(!lengthChk(login))
        err += tr("Incorrect Login, must be more than 4 characters") + "\n";
    if(!nonLatinChk(login))
        err += tr("Incorrect Login, use latin characters") + "\n";
    if(!noSpacesChk(login))
        err += tr("Incorrect Login, don't use space") + "\n";
}

void StartScreen::pwdChk(const QString &pwd, QString &err)
{
    if(!lengthChk(pwd))
        err += tr("Incorrect Login, must be more than 4 characters") + "\n";
    if(!nonLatinChk(pwd))
        err += tr("Incorrect Login, use latin characters") + "\n";
    if(!noSpacesChk(pwd))
        err += tr("Incorrect Login, don't use space") + "\n";
}

void StartScreen::nameChk(const QString &name, QString &err)
{
    if(!lengthChk(name))
        err += tr("Incorrect Name, must be more than 4 characters") + "\n";
    if(!noSpacesChk(name))
        err += tr("Incorrect Name, don't use space") + "\n";
}

QString StartScreen::userName() const
{
    return m_userName;
}

QString StartScreen::userUUID() const
{
    return m_userUUID;
}

QNet *StartScreen::getOnline() const
{
    return Online;
}
