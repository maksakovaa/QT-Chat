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

void UserForm::setName(const QString &newName)
{
    m_name = newName;
    ui->lblUserName->setText(m_name);
}

void UserForm::setUuid(const QString &newUuid)
{
    m_uuid = newUuid;
}

QString UserForm::getName() const
{
    return m_name;
}

QString UserForm::getUuid() const
{
    return m_uuid;
}

void UserForm::on_GoToChat_clicked()
{
    emit openChat(m_uuid);
}

