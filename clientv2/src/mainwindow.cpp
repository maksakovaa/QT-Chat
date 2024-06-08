#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "startscreen.h"

MainWindow::MainWindow(QString userUUID, QString userName, QNet* ptr, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_userUUID(userUUID),
    m_userName(userName)
{
    ui->setupUi(this);
    Online = ptr;
    initObjects();
    classConnections();
    setPtr();
    UserBase->slotGetUsrBase();
    AllMsgBase->getMsgBase(m_userUUID);
    PerMsgBase->getMsgBase(m_userUUID);
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow *MainWindow::createClient()
{
    StartScreen s;
    auto result = s.exec();
    if(result == QDialog::Rejected)
    {
        return nullptr;
    }
    auto w = new MainWindow(s.userUUID(), s.userName(), s.getOnline());
    w->setAttribute(Qt::WA_DeleteOnClose);
    return w;
}

void MainWindow::slotOpenChat(const QString &uuid)
{
    ui->ChatViewWgt->openChat(uuid);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::slotOpenChatList()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::slotSendMessage(const QString &message, const QString &uuid)
{
    Message newMsg(m_userUUID, uuid, message);
    QString msgPkg;
    if (uuid == "ALL")
    {
        msgPkg = AllMsgBase->packMsgPkg(newMsg);
    }
    else
    {
        msgPkg = PerMsgBase->packMsgPkg(newMsg);
    }
    Online->sendToServer("SND_MSG<|>" + msgPkg);
}

void MainWindow::initObjects()
{
    UserBase = new UsrBase;
    AllMsgBase = new MessageBase;
    PerMsgBase = new MessageBase;
}

void MainWindow::classConnections()
{
    connect(Online, &QNet::addUser, UserBase, &UsrBase::slotAddNewUser);
    connect(Online, &QNet::updateUsrBase, UserBase, &UsrBase::slotGetUsrBase);
    connect(UserBase, &UsrBase::userAdded, ui->ChatViewListWgt, &ChatViewListForm::slotInsertInList);
    connect(UserBase, &UsrBase::updateUBinit, ui->ChatViewListWgt, &ChatViewListForm::updateChatList);
    connect(ui->ChatViewListWgt, &ChatViewListForm::openChat, this, &MainWindow::slotOpenChat);
    connect(ui->ChatViewWgt, &ChatView::openChatList, this, &MainWindow::slotOpenChatList);
    connect(Online, &QNet::addAllMsg, AllMsgBase, &MessageBase::addMsg);
    connect(Online, &QNet::addPerMsg, PerMsgBase, &MessageBase::addMsg);
    connect(ui->ChatViewWgt, &ChatView::sendMsg, this, &MainWindow::slotSendMessage);
}

void MainWindow::setPtr()
{
    UserBase->setNetPtr(Online);
    AllMsgBase->setNetPtr(Online);
    PerMsgBase->setNetPtr(Online);
    AllMsgBase->setAllMsgBasePtr(AllMsgBase);
    AllMsgBase->setPerMsgBasePtr(PerMsgBase);
    PerMsgBase->setPerMsgBasePtr(PerMsgBase);
    PerMsgBase->setAllMsgBasePtr(AllMsgBase);
    ui->ChatViewListWgt->setUsrBasePtr(UserBase);
    ui->ChatViewWgt->setAllMBptr(AllMsgBase);
    ui->ChatViewWgt->setPerMBptr(PerMsgBase);
}
