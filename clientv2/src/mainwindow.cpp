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
    setPtr();
    UserBase->setCurrentUUID(m_userUUID);
    ui->ChatViewListWgt->setCurrentUUID(m_userUUID);
    ui->ChatViewWgt->setCurrentUUID(m_userUUID);
    classConnections();
    UserBase->slotGetUsrBase();
    slotUpdAllMsgBase();
    slotUpdPerMsgBase();
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
    ui->ChatViewListWgt->updateChatList();
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

void MainWindow::slotUpdAllMsgBase()
{
    AllMsgBase->getMsgBase(m_userUUID);
}

void MainWindow::slotUpdPerMsgBase()
{
    PerMsgBase->getMsgBase(m_userUUID);
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
    connect(AllMsgBase, &MessageBase::newMsgAdded, ui->ChatViewWgt, &ChatView::fillAM);
    connect(AllMsgBase, &MessageBase::newMsgAdded, ui->ChatViewListWgt, &ChatViewListForm::updateChatList);
    connect(PerMsgBase, &MessageBase::newMsgAdded, ui->ChatViewWgt, &ChatView::fillPM);
    connect(PerMsgBase, &MessageBase::newMsgAdded, ui->ChatViewListWgt, &ChatViewListForm::updateChatList);
    connect(Online, &QNet::updateAMBase, this, &MainWindow::slotUpdAllMsgBase);
    connect(Online, &QNet::updatePMBase, this, &MainWindow::slotUpdPerMsgBase);
    connect(ui->ChatViewWgt, &ChatView::updAllMsgStatus, Online, &QNet::sendAMStatus);
    connect(ui->ChatViewWgt, &ChatView::updPerMsgStatus, Online, &QNet::sendPMStatus);
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
    ui->ChatViewWgt->setUBptr(UserBase);
    ui->ChatViewWgt->setAllMBptr(AllMsgBase);
    ui->ChatViewWgt->setPerMBptr(PerMsgBase);
    ui->ChatViewListWgt->setUsrBasePtr(UserBase);
    ui->ChatViewListWgt->setAMBasePtr(AllMsgBase);
    ui->ChatViewListWgt->setPMBasePtr(PerMsgBase);
}
