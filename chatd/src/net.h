#ifndef NET_H
#define NET_H
#include <QTcpServer>
#include <QTcpSocket>
#include "database.h"

class Net : public QTcpServer
{
    Q_OBJECT

public:
    Net(Settings* ptr);
    void startListen();
    void setUserBasePtr(UsrBase* ptr);
    void setSqlDbPtr(Database* ptr);
    void setAMBasePtr(MsgBase* ptr);
    void setPMBasePtr(MsgBase* ptr);
    QVector<QTcpSocket *>& getSockets();
private:
    void preparePackage();
    void sendRequest(const QString& reqName);
    void sendPersonalRequest(const QString& reqName, QTcpSocket* cliSocket);
    void requestHandler(QTcpSocket* socket);
    void cutRequest(QString& req);
    void authClient(QTcpSocket* cliSocket);
    void sendUsrBase(QTcpSocket* cliSocket);
    void sendMsgBase(const QString& baseType, QTcpSocket* cliSocket);
    void regUser(QTcpSocket* cliSocket);
    void regMSG(QTcpSocket* cliSocket);
    void delUsr();
    void setPMStatus();
    void setAMStatus();
signals:
    void logUpdated(const QString& entry);
    void updSocketStats();

private slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotCliDisconnect();
    void slotReadyRead();
public slots:
    void updateUsrBase();
    void sendAMtoCli(Message& newMsg, QTcpSocket* cliSocket);
    void sendPMtoCli(Message& newMsg, QTcpSocket* cliSocket);
    void updateAllMsgBase();
    void updatePerMsgBase();

private:
    QTcpSocket *socket;
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    quint16 nextBlockSize;
    quint16 chatPort;
    QString delim = "<|>";
    QString request, package;
    Settings* Config;
    UsrBase* UserBase;
    Database* SQL_DataBase;
    MsgBase* AllMsgBase;
    MsgBase* PerMsgBase;
};

#endif // NET_H
