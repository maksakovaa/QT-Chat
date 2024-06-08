#include "chatviewlistform.h"
#include "ui_chatviewlistform.h"
#include "userform.h"

ChatViewListForm::ChatViewListForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatViewListForm)
{
    ui->setupUi(this);
}

ChatViewListForm::~ChatViewListForm()
{
    delete ui;
}

void ChatViewListForm::setUsrBasePtr(UsrBase* ptr)
{
    UserBase = ptr;
    updateChatList();
}

void ChatViewListForm::slotInsertInList(const User& newUser)
{
    auto u = new UserForm;
    u->setName(newUser.getName());
    u->setUuid(newUser.getUUID());
    connect(u, &UserForm::openChat, this, &ChatViewListForm::slotOpenChat);
    auto listItem = new QListWidgetItem();
    listItem->setIcon(QPixmap("chat_icon.png"));
    ui->listWidget->addItem(listItem);
    ui->listWidget->setItemWidget(listItem, u);
}

void ChatViewListForm::updateChatList()
{
    ui->listWidget->clear();
    auto u = new UserForm;
    u->setName("Main chat");
    u->setUuid("ALL");
    ui->listWidget->setIconSize(QSize( 32, 32 ));
    connect(u, &UserForm::openChat, this, &ChatViewListForm::slotOpenChat);
    auto listItem = new QListWidgetItem();
    listItem->setIcon(QPixmap("chat_icon.png"));
    ui->listWidget->addItem(listItem);
    ui->listWidget->setItemWidget(listItem, u);
}

void ChatViewListForm::slotOpenChat(const QString &uuid)
{
    emit openChat(uuid);
}

void ChatViewListForm::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
}

