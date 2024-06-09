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
        fillPM();
    }
}

void ChatView::on_backToList_clicked()
{
    thisUuid.clear();
    emit openChatList();
}


void ChatView::on_sndMsgButton_clicked()
{
    emit sendMsg(ui->msgLineEdit->text().trimmed(), thisUuid);
    ui->msgLineEdit->clear();
}

void ChatView::fillAM()
{
    if (thisUuid.isEmpty())
    {
        return;
    }
    ui->MsgListWgt->clear();
    for (int i = 0; i < AllMsgBase->getCount(); ++i)
    {
        Message msg = AllMsgBase->getMessage(i);
        if(msg.get_mto() == "ALL" && msg.get_mstatus() == 0)
        {
            AllMsgBase->markMsgRead(i);
            QString request = AllMsgBase->packAllMsgStatus(msg, currentUUID);
            emit updAllMsgStatus(request);
        }
        insertAM(msg);
    }
}

void ChatView::fillPM()
{
    if (thisUuid.isEmpty())
    {
        return;
    }
    ui->MsgListWgt->clear();
    for (int i = 0; i < PerMsgBase->getCount(); ++i)
    {
        Message msg = PerMsgBase->getMessage(i);
        if(msg.get_mfrom() == thisUuid | msg.get_mto() == thisUuid)
        {
            if(msg.get_mstatus() == 0)
            {
                PerMsgBase->markMsgRead(i);
                QString request = PerMsgBase->packPerMsgStatus(msg);
                emit updPerMsgStatus(request);
            }
            insertPM(msg);
        }
    }
}

void ChatView::insertAM(const Message &msg)
{
    auto m = new MessageForm;
    m->setMsgFrom(UserBase->getUserName(msg.get_mfrom()));
    m->setMsgDate(msg.get_mdate());
    m->setMsgText(msg.get_mtext());
    auto listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(0, 65));
    ui->MsgListWgt->addItem(listItem);
    ui->MsgListWgt->setItemWidget(listItem, m);
}

void ChatView::insertPM(const Message &msg)
{
    auto m = new PerMsgForm;
    m->setMsgFrom(UserBase->getUserName(msg.get_mfrom()));
    m->setMsgTo(UserBase->getUserName(msg.get_mto()));
    m->setMsgDate(msg.get_mdate());
    m->setMsgText(msg.get_mtext());
    auto listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(0, 65));
    ui->MsgListWgt->addItem(listItem);
    ui->MsgListWgt->setItemWidget(listItem, m);
}

void ChatView::setCurrentUUID(const QString &newCurrentUUID)
{
    currentUUID = newCurrentUUID;
}

