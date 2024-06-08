#include "registrationform.h"
#include "ui_registrationform.h"

RegForm::RegForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegForm)
{
    ui->setupUi(this);
}

RegForm::~RegForm()
{
    delete ui;
}

void RegForm::on_buttonBox_accepted()
{
    if (ui->PasswordEdit->text() == ui->ConfirmPassEdit->text())
    {
        emit sig_regUser(ui->UsrNameEdit->text(), ui->LoginEdit->text(), ui->PasswordEdit->text());
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Confirmation password incorrect"));
        return;
    }
}


void RegForm::on_buttonBox_rejected()
{
    emit sig_rejected();
}


void RegForm::on_loginPushButton_clicked()
{
    emit sig_loginRequested();
}

