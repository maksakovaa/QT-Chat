#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "msgbased.h"
#include <QTcpSocket>

class Net;

class Database : public QObject
{
    Q_OBJECT

public:
    Database() = default;
    void connect();
    void setConfigPtr(Settings* ptr);
    void setUsrBasePtr(UsrBase* ptr);
    void setAllMsgBasePtr(MsgBase* ptr);
    void setPerMsgBasePtr(MsgBase* ptr);
    void getAllMsgBase();
    void getPerMsgBase();
    void regPrivateMsg(Message& newMsg);
    void regPublicMsg(Message& newMsg);

private:
    void getConfig();
    void sendRequest(const QString& reqName);
    void firstRun();
private:
    QString m_query;
    QSqlDatabase db;
    QString mysql_srv_ip;
    QString mysql_login;
    QString mysql_pass;
    QString mysql_database;
    QString mysql_table_users;
    QString mysql_table_AM;
    QString mysql_table_AMS;
    QString mysql_table_PM;
    Settings* Config;
    UsrBase* UserBase;
    MsgBase* AllMsgBase;
    MsgBase* PerMsgBase;
signals:
    void logUpdated(const QString& entry);
    void usrBaseUpdated();
    void allMsgBaseUpdated();
    void userRegComplete(const QString& userName);
    void updUserBase();
    void sendAMtoCli(Message& newMsg, QTcpSocket* cliSocket);
    void sendPMtoCli(Message& newMsg, QTcpSocket* cliSocket);
    void updatePrivateMsgBaseRequired();
    void updatePublicMsgBaseRequired();

public slots:
    void regUser(User& newUser);
    void getUsrBase();
    void setUserStatus(const QString& uuid, int newStatus);
    void getAllMsgBase(const QString& uuid, QTcpSocket* cliSocket);
    void getPerMsgBase(const QString& uuid, QTcpSocket* cliSocket);

};

#endif // DATABASE_H
