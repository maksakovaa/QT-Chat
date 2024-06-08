#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardItemModel>
#include "userform.h"
#include "messageform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initObjects();
    storePtr();
    classConnections();
    SQL_DataBase->connect();
    Online->startListen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logEntry(const QString& entry)
{
    ui->LogTextBrowser->append(entry);
}

void MainWindow::updateUsers()
{
    ui->UsersListViewWgt->clear();
    for (int i = 0; i < UserBase->getCount(); ++i)
    {
        auto u = new UserForm;
        connect(u, &UserForm::updateUsrSatus, SQL_DataBase, &Database::setUserStatus);
        u->setUserName(UserBase->getUser(i).getName());
        u->setStatus(UserBase->getUser(i).getStatus());
        u->setUuid(UserBase->getUser(i).getUUID());
        auto listItem = new QListWidgetItem;
        listItem->setSizeHint(QSize(0, 40));
        ui->UsersListViewWgt->addItem(listItem);
        if(UserBase->getUser(i).getStatus() == 0) {
            listItem->setBackground(QColor(100, 165, 22));
        }
        else {
            listItem->setBackground(QColor(170, 0, 0));
        }
        ui->UsersListViewWgt->setItemWidget(listItem, u);
    }
}

void MainWindow::updateAllMsg()
{
    ui->QListAllMessageWgt->clear();
    for (int i = 0; i < AllMsgBase->getCount(); ++i)
    {
        auto u = new MessageForm;
        auto msg = AllMsgBase->getMsg(i);
        u->setMsgFrom(msg.get_mfrom());
        u->setMsgDate(msg.get_mdate());
        u->setMsgText(msg.get_mtext());
        auto listItem = new QListWidgetItem;
        ui->QListAllMessageWgt->addItem(listItem);
        ui->QListAllMessageWgt->setItemWidget(listItem, u);
    }
}

void MainWindow::initObjects()
{
    Config = new Settings;
    Log = new Logger;
    Online = new Net(Config);
    UserBase = new UsrBase;
    AllMsgBase = new MsgBase;
    PerMsgBase = new MsgBase;
    SQL_DataBase = new Database;
}

void MainWindow::classConnections()
{
    connect(Log, &Logger::appendLogEntry, this, &MainWindow::logEntry);
    connect(Online, &Net::logUpdated, Log, &Logger::recLogEntry);
    connect(SQL_DataBase, &Database::logUpdated, Log, &Logger::recLogEntry);

    connect(SQL_DataBase, &Database::usrBaseUpdated, this, &MainWindow::updateUsers);
    connect(SQL_DataBase, &Database::updUserBase, Online, &Net::updateUsrBase);
    connect(SQL_DataBase, &Database::allMsgBaseUpdated, this, &MainWindow::updateAllMsg);

    connect(SQL_DataBase, &Database::sendAMtoCli, Online, &Net::sendAMtoCli);
    connect(SQL_DataBase, &Database::sendPMtoCli, Online, &Net::sendPMtoCli);

    connect(SQL_DataBase, &Database::updatePublicMsgBaseRequired, Online, &Net::updateAllMsgBase);
    connect(SQL_DataBase, &Database::updatePrivateMsgBaseRequired, Online, &Net::updatePerMsgBase);
}

void MainWindow::storePtr()
{
    SQL_DataBase->setConfigPtr(Config);
    SQL_DataBase->setUsrBasePtr(UserBase);
    SQL_DataBase->setAllMsgBasePtr(AllMsgBase);
    SQL_DataBase->setPerMsgBasePtr(PerMsgBase);
    Online->setUserBasePtr(UserBase);
    Online->setSqlDbPtr(SQL_DataBase);
}
