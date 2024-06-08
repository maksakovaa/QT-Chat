#include "messagebase.h"

QString MessageBase::unTag(m_Attr id, QString& str)
{
    int i = static_cast<int>(id);
    QString result = str.sliced(str.indexOf(sepBeg[i]) + sepBeg[i].size(), str.indexOf(sepEnd[i]) - sepBeg[i].size());
    str.remove(0, str.indexOf(sepEnd[i])+sepEnd[i].size());
    return result;
}

QString MessageBase::tagStructParam(m_Attr id, const QString &value)
{
    QString result = sepBeg[static_cast<int>(id)];
    result += value;
    result += sepEnd[static_cast<int>(id)];
    return result;
}

Message MessageBase::splitMsgPkg(QString &msgPkg)
{
    int id, status;
    QString from, to, date, msg;
    id = (unTag(m_Attr::indId, msgPkg)).toInt();
    from = unTag(m_Attr::indFrom, msgPkg);
    to = unTag(m_Attr::indTo, msgPkg);
    date = unTag(m_Attr::indDate, msgPkg);
    msg = unTag(m_Attr::indMsg, msgPkg);
    status = (unTag(m_Attr::indStatus, msgPkg)).toInt();

    Message newMsg(id, from, to, date, msg, status);
    return newMsg;
}

QString MessageBase::packMsgPkg(Message& msg)
{
    QString result = tagStructParam(m_Attr::indId, QString::number(msg.get_mid()));
    result += tagStructParam(m_Attr::indFrom, msg.get_mfrom());
    result += tagStructParam(m_Attr::indTo, msg.get_mto());
    result += tagStructParam(m_Attr::indDate, msg.get_mdate());
    result += tagStructParam(m_Attr::indMsg, msg.get_mtext());
    result += tagStructParam(m_Attr::indStatus, QString::number(msg.get_mstatus()));
    return result;
}

const Message& MessageBase::getMessage(int id)
{
    return base->at(id);
}

void MessageBase::setNetPtr(QNet *ptr)
{
    Online = ptr;
}

void MessageBase::setAllMsgBasePtr(MessageBase *ptr)
{
    AllMsgBase = ptr;
}

void MessageBase::setPerMsgBasePtr(MessageBase *ptr)
{
    PerMsgBase = ptr;
}

void MessageBase::getMsgBase(const QString &uuid)
{
    if(this == AllMsgBase)
    {
        Online->sendToServer("GET_ALL_MSGBASE<|>" + uuid);
    }
    else if (this == PerMsgBase)
    {
        Online->sendToServer("GET_PER_MSGBASE<|>" + uuid);
    }
}

void MessageBase::addMsg(QString &msgPkg)
{
    Message newMsg = splitMsgPkg(msgPkg);
    addInBase(newMsg);
}
