#include "net.h"
#include <iostream>

Net::Net(Settings* ptr)
{
    Config = ptr;
    nextBlockSize = 0;
    chatPort = Config->get("chat_port").toInt();
}

void Net::setUserBasePtr(UsrBase* ptr)
{
    UserBase = ptr;
}

void Net::setSqlDbPtr(Database* ptr)
{
    SQL_DataBase = ptr;
}

void Net::setAMBasePtr(MsgBase *ptr)
{
    AllMsgBase = ptr;
}

void Net::setPMBasePtr(MsgBase *ptr)
{
    PerMsgBase = ptr;
}

void Net::startListen()
{
    if(this->listen(QHostAddress::Any, 63540))
    {
        emit logUpdated("[NET] " + tr("Awaiting connections..."));
    }
    else
    {
        emit logUpdated("[NET] " + tr("Error set in wait state"));
    }
    nextBlockSize = 0;
}

void Net::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &Net::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Net::slotCliDisconnect);

    Sockets.push_back(socket);
    emit logUpdated("[NET] " + tr("Client connected with descriptor") + " " + QString::number(socketDescriptor));
    emit updSocketStats();
}

void Net::slotCliDisconnect()
{
    auto client = qobject_cast<QTcpSocket*>(sender());
    emit logUpdated("[NET] " + tr("Client disconnected with descriptor") + " " + QString::number(client->socketDescriptor()));
    Sockets.removeOne(client);
    emit updSocketStats();
}

void Net::preparePackage()
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);
    out << quint16() << package;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
}

void Net::sendRequest(const QString& reqName)
{
    preparePackage();
    for(int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
        emit logUpdated("[NET] " + tr("Request send to") + " " + QString::number(Sockets[i]->socketDescriptor()) + " " + reqName);
    }
}

void Net::sendPersonalRequest(const QString& reqName, QTcpSocket* sock)
{
    preparePackage();
    sock->write(Data);
    emit logUpdated("[NET] " + tr("Request send to") + " " + QString::number(sock->socketDescriptor()) + " " + reqName);
}

void Net::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_7);
    if(in.status() == QDataStream::Ok)
    {
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable() < 2)
                {
                    break;
                }
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }
            in >> request;
            requestHandler(socket);
            nextBlockSize = 0;
            break;
        }
    }
    else
    {
        emit logUpdated("[NET] DataStream " + tr("error"));
    }
}



void Net::requestHandler(QTcpSocket* cliSocket)
{
    if (request.startsWith("AUTH"))
    {
        authClient(cliSocket);
    }
    else if (request.startsWith("REG_USER"))
    {
        regUser(cliSocket);
    }
    else if (request.startsWith("GET_USRBASE"))
    {
        sendUsrBase(cliSocket);
    }
    else if (request.startsWith("GET_PER_MSGBASE"))
    {
        sendMsgBase("PER", cliSocket);
    }
    else if (request.startsWith("GET_ALL_MSGBASE"))
    {
        sendMsgBase("ALL", cliSocket);
    }
    else if (request.startsWith("SND_MSG"))
    {
        regMSG(cliSocket);
    }
    else if (request.startsWith("SET_PMSG_STATUS"))
    {
        setPMStatus();
    }
    else if (request.startsWith("SET_AMSG_STATUS"))
    {
        setAMStatus();
    }
}

void Net::cutRequest(QString &request)
{
    request.remove(0, request.indexOf(delim) + delim.length());
}

void Net::authClient(QTcpSocket* cliSocket)
{
    cutRequest(request);
    emit logUpdated("[NET] " + tr("Auth request acccepted from client: ") + QString::number(socket->socketDescriptor()));
    QString login = request.first(request.indexOf(delim));
    cutRequest(request);
    QString pwd = request;
    bool result = UserBase->authChk(login, pwd);
    if (!result)
    {
        package = "AUTH<|>ERROR: Authentification failed";
        sendPersonalRequest(tr("Authentification failed for client ") + QString::number(socket->socketDescriptor()), cliSocket);
    }
    else
    {
        QString uuid = UserBase->getUUID(login);
        QString userName = UserBase->getUser(uuid).getName();
        package = "AUTH<|>" + userName + "<|>" + uuid;
        sendPersonalRequest(tr("Authentification success for client ") + QString::number(socket->socketDescriptor()), cliSocket);
    }
}

void Net::regUser(QTcpSocket* cliSocket)
{
    emit logUpdated("[NET] REG_USER " + tr("request accepted from client") + QString::number(cliSocket->socketDescriptor()));
    cutRequest(request);
    QString name = request.first(request.indexOf(delim));
    cutRequest(request);
    QString login = request.first(request.indexOf(delim));
    cutRequest(request);
    QString pwd = request;
    User newUser(name, login, pwd);
    if(UserBase->userExists(newUser))
    {
        package = "REG_USER<|>ERROR: " + tr("user login already exists");
        sendPersonalRequest("[NET] REG_USER ERROR MSG SEND", cliSocket);
    }
    else
    {
        SQL_DataBase->regUser(newUser);
        QString uuid;
        uuid = UserBase->getUUID(login);
        package = "REG_USER" + delim + name + delim + uuid;
        sendPersonalRequest("[NET] REG_USER completed", cliSocket);
    }

}

void Net::sendUsrBase(QTcpSocket* cliSocket)
{
    emit logUpdated("[NET] GET_USRBASE " + tr("request accepted"));
    if (UserBase->getCount() == 0)
    {
        SQL_DataBase->getUsrBase();
    }
    for (int i = 0; i < UserBase->getCount(); i++)
    {
        User account = UserBase->getUser(i);
        package = "ADD_USER<|>" + UserBase->packUsrPkg(account);
        QString reqName = "USRBASE package ";
        sendPersonalRequest(reqName.append(std::to_string(i)), cliSocket);
    }
}

void Net::updateUsrBase()
{
    package = "UPD_USERBASE";
    sendRequest(package);
}

void Net::sendAMtoCli(Message &newMsg, QTcpSocket *cliSocket)
{
    package = "ADD_ALL_MSG" + delim + AllMsgBase->packMsgPkg(newMsg);
    sendPersonalRequest(QString::number(newMsg.get_mid()) +
                        tr("Message package send to ") +
                        QString::number(cliSocket->socketDescriptor()),
                        cliSocket);
}

void Net::sendPMtoCli(Message &newMsg, QTcpSocket *cliSocket)
{
    package = "ADD_PER_MSG" + delim + PerMsgBase->packMsgPkg(newMsg);
    sendPersonalRequest(QString::number(newMsg.get_mid()) +
                        tr("Message package send to ") +
                        QString::number(cliSocket->socketDescriptor()),
                        cliSocket);
}

void Net::updateAllMsgBase()
{
    SQL_DataBase->getAllMsgBase();
    package = "UPD_ALL_MSGBASE";
    sendRequest(package);
}

void Net::updatePerMsgBase()
{
    SQL_DataBase->getPerMsgBase();
    package = "UPD_PER_MSGBASE";
    sendRequest(package);
}

QVector<QTcpSocket *>& Net::getSockets()
{
    return Sockets;
}

void Net::sendMsgBase(const QString &baseType, QTcpSocket* cliSocket)
{
    emit logUpdated("[NET] GET_" + baseType + "_MSGBASE " + tr("request accepted from client") + " " + QString::number(cliSocket->socketDescriptor()));
    cutRequest(request);
    if (baseType.startsWith("ALL"))
    {
        SQL_DataBase->getAllMsgBase(request, cliSocket);
    }
    else if (baseType.startsWith("PER"))
    {
        SQL_DataBase->getPerMsgBase(request, cliSocket);
    }
}

void Net::regMSG(QTcpSocket* cliSocket)
{
    emit logUpdated("[NET] SND_MSG " + tr("request accepted from client") + " " + QString::number(cliSocket->socketDescriptor()));
    cutRequest(request);
    Message newMsg = AllMsgBase->splitMsgPkg(request);
    if (newMsg.get_mto() == "ALL")
    {
        SQL_DataBase->regPublicMsg(newMsg);
    }
    else
    {
        SQL_DataBase->regPrivateMsg(newMsg);
    }
}

void Net::setPMStatus()
{
    emit logUpdated("[NET] SET_PMSG_STATUS request accepted");
    cutRequest(request);
    int arr[2];
    PerMsgBase->splitPMStatus(request, arr);
    SQL_DataBase->updPMStatus(arr[0], arr[1]);
}

void Net::setAMStatus()
{
    emit logUpdated("[NET] SET_AMSG_STATUS request accepted");
    cutRequest(request);
    QString arr[3];
    AllMsgBase->splitAMStatus(request, arr);
    SQL_DataBase->updAMStatus(arr[0], arr[1], arr[2]);
}
