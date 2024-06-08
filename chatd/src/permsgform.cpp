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
