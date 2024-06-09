#include "permsgform.h"
#include "ui_permsgform.h"

PerMsgForm::PerMsgForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PerMsgForm)
{
    ui->setupUi(this);
}

PerMsgForm::~PerMsgForm()
{
    delete ui;
}

void PerMsgForm::setMsgFrom(const QString &from)
{
    ui->lblMsgFrom->setText(from);
}

void PerMsgForm::setMsgTo(const QString &to)
{
    ui->lblMsgTo->setText(to);
}

void PerMsgForm::setMsgDate(const QString &date)
{
    ui->lblDate->setText(date);
}

void PerMsgForm::setMsgText(const QString &text)
{
    ui->lblMessage->setText(text);
}
