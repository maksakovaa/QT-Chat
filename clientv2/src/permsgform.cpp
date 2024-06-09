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

void PerMsgForm::setMsgDate(const QString& date)
{
    ui->lblDate->setText(date);
}

void PerMsgForm::setMsgFrom(const QString& from)
{
    ui->lblFrom->setText(from);
}

void PerMsgForm::setMsgText(const QString& text)
{
    ui->lblMessage->setText(text);
}

void PerMsgForm::setMsgTo(const QString& to)
{
    ui->lblTo->setText(to);
}


