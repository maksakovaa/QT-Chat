#include "qnet.h"

#include <iostream>

QNet::QNet(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::disconnected, this, &QNet::onConnectionLost);
    connect(socket, &QTcpSocket::readyRead, this, &QNet::onGetData);

    socket->connectToHost("127.0.0.1", 63540);
    nextBlockSize =0;
    if(!socket->waitForConnected())
    {
        onConnectionLost();
    }
}

void QNet::onGetData()
{
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
            in >> package;
            responseHandler(package);
            nextBlockSize = 0;
        }
    }
    else
    {
        QMessageBox::critical(nullptr, tr("Error"), tr("Error receive data"));
    }
}

void QNet::onConnectionLost()
{
    QMessageBox::critical(nullptr, "Error", "Failed connect to server!\nCheck is server run and restart application");
    exit(0);
}

void QNet::cutRequest(QString &request)
{

    request.remove(0, request.indexOf(delim) + delim.length());
}

void QNet::responseHandler(QString str)
{
    if (str.startsWith("ADD_USER"))
    {
        cutRequest(str);
        emit addUser(str);
    }
    else if (str.startsWith("AUTH"))
    {
        cutRequest(str);
        emit authResponce(str);
    }
    else if (str.startsWith("REG_USER"))
    {
        cutRequest(str);
        emit regResponce(str);
    }
    else if (str == "UPD_USERBASE")
    {
        emit updateUsrBase();
    }
    else if (str.startsWith("ADD_ALL_MSG"))
    {
        cutRequest(str);
        emit addAllMsg(str);
    }
    else if (str.startsWith("UPD_MSGBASE"))
    {

    }
}

QTcpSocket *QNet::getSocket() const
{
    return socket;
}

void QNet::sendToServer(const QString &str)
{
    socket->waitForBytesWritten();
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);
    out << quint16() << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
}
