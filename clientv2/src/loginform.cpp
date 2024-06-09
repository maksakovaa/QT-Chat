#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_buttonBox_accepted()
{
    emit sig_reqOnlineAuth(ui->LoginEdit->text(), ui->PasswordEdit->text());
}


void LoginForm::on_buttonBox_rejected()
{
    emit sig_rejected();
}


void LoginForm::on_RegistrationButton_clicked()
{
    emit sig_registerRequested();
}


