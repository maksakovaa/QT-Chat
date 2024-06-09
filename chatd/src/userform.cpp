#include "userform.h"
#include "ui_userform.h"

UserForm::UserForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserForm)
{
    ui->setupUi(this);
}

UserForm::~UserForm()
{
    delete ui;
}

void UserForm::setUserName(QString name)
{
    ui->lblUserName->setText(name);
}

void UserForm::on_BlockUnblockBtn_clicked()
{
    if (status == 0)
    {
        emit updateUsrSatus(uuid, 1);
    }
    else
    {
        emit updateUsrSatus(uuid, 0);
    }
}

void UserForm::setUuid(const QString &newUuid)
{
    uuid = newUuid;
}

void UserForm::setStatus(int newStatus)
{
    status = newStatus;
    if (status == 0)
        ui->BlockUnblockBtn->setText("Block user");
    else
        ui->BlockUnblockBtn->setText("Unblock user");
}

