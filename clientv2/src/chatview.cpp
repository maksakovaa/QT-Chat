#include "chatview.h"
#include "ui_chatview.h"
#include "messageform.h"
#include "permsgform.h"

ChatView::ChatView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatView)
{
    ui->setupUi(this);
}

ChatView::~ChatView()
{
    delete ui;
}

void ChatView::setUBptr(UsrBase *ptr)
{
    UserBase = ptr;
}

void ChatView::setAllMBptr(MessageBase *ptr)
{
    AllMsgBase = ptr;
}

void ChatView::setPerMBptr(MessageBase *ptr)
{
    PerMsgBase = ptr;
}

void ChatView::openChat(const QString &uuid)
{
    thisUuid = uuid;
    ui->MsgListWgt->clear();
    if(uuid == "ALL")
    {
        fillAM();
    }
    else
    {
        fillPM(uuid);
    }
}

void ChatView::on_backToList_clicked()
{
    emit openChatList();
}


void ChatView::on_sndMsgButton_clicked()
{
    emit sendMsg(ui->msgLineEdit->text().trimmed(), thisUuid);
}

void ChatView::fillAM()
{
    for (int i = 0; i < AllMsgBase->getCount(); ++i)
    {
        Message msg = AllMsgBase->getMessage(i);
        insertAM(msg);
    }
}

void ChatView::fillPM(const QString &uuid)
{
    for (int i = 0; i < PerMsgBase->getCount(); ++i)
    {
        Message msg = PerMsgBase->getMessage(i);
        if(msg.get_mfrom() == uuid | msg.get_mto() == uuid)
        {
            insertPM(msg);
        }
    }
}

void ChatView::insertAM(Message &msg)
{
    auto m = new MessageForm;
    m->setMsgFrom(msg.get_mfrom());
    m->setMsgDate(msg.get_mdate());
    m->setMsgText(msg.get_mtext());
    auto listItem = new QListWidgetItem;
    ui->MsgListWgt->addItem(listItem);
    ui->MsgListWgt->setItemWidget(listItem, m);
}

void ChatView::insertPM(Message &msg)
{
    auto m = new PerMsgForm;
    m->setMsgFrom(msg.get_mfrom());
    m->setMsgTo(msg.get_mto());
    m->setMsgDate(msg.get_mdate());
    m->setMsgText(msg.get_mtext());
    auto listItem = new QListWidgetItem;
    ui->MsgListWgt->addItem(listItem);
    ui->MsgListWgt->setItemWidget(listItem, m);
}

