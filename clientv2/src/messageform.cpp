#include "messageform.h"
#include "ui_messageform.h"

MessageForm::MessageForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MessageForm)
{
    ui->setupUi(this);
}

MessageForm::~MessageForm()
{
    delete ui;
}

void MessageForm::setMsgFrom(const QString &from)
{
    ui->lbFrom->setText(from);
}

void MessageForm::setMsgDate(const QString &date)
{
    ui->lblDate->setText(date);
}

void MessageForm::setMsgText(const QString &text)
{
    ui->lblMessgae->setText(text);
}
