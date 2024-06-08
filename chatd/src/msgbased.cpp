#include "msgbased.h"
#include <iostream>
Message MsgBase::splitMsgPkg(QString msgPkg)
{
	int id, status;
    QString from, to, date, msg;

    id = (unTagStructParam(m_Attr::indId, msgPkg)).toInt();
    from = unTagStructParam(m_Attr::indFrom, msgPkg);
    to = unTagStructParam(m_Attr::indTo, msgPkg);
    date = unTagStructParam(m_Attr::indDate, msgPkg);
    msg = unTagStructParam(m_Attr::indMsg, msgPkg);
    status = (unTagStructParam(m_Attr::indStatus, msgPkg)).toInt();

	Message newMsg(id, from, to, date, msg, status);
	return newMsg;
}

QString MsgBase::packMsgPkg(Message& msgPkg)
{
    QString result = tagStructParam(m_Attr::indId, QString::number(msgPkg.get_mid()));
	result += tagStructParam(m_Attr::indFrom, msgPkg.get_mfrom());
	result += tagStructParam(m_Attr::indTo, msgPkg.get_mto());
	result += tagStructParam(m_Attr::indDate, msgPkg.get_mdate());
	result += tagStructParam(m_Attr::indMsg, msgPkg.get_mtext());
    result += tagStructParam(m_Attr::indStatus, QString::number(msgPkg.get_mstatus()));
	return result;
}

Message MsgBase::getMsg(int id)
{
	Message temp = base->at(id);
	return temp;
}

void MsgBase::splitPMStatus(QString& str, int arr[])
{
    arr[0] = (unTagStructParam(m_Attr::indId, str)).toInt();
    arr[1] = (unTagStructParam(m_Attr::indStatus, str)).toInt();
}

void MsgBase::splitAMStatus(QString& str, QString arr[])
{
    arr[0] = unTagStructParam(m_Attr::indId, str);
    arr[1] = unTagStructParam(m_Attr::indFor, str);
    arr[2] = unTagStructParam(m_Attr::indStatus, str);
}

QString MsgBase::tagStructParam(m_Attr id, const QString& value)
{
    QString result = sepBeg[static_cast<int>(id)];
	result += value;
	result += sepEnd[static_cast<int>(id)];
	return result;
}

QString MsgBase::unTagStructParam(m_Attr id, QString& msgPkg)
{
    int i = static_cast<int>(id);
    std::cout << sepBeg[i].length();
    QString result = msgPkg.sliced(msgPkg.indexOf(sepBeg[i]) + sepBeg[i].size(), msgPkg.indexOf(sepEnd[i]) - sepBeg[i].size());
    msgPkg.remove(0, msgPkg.indexOf(sepEnd[i])+sepEnd[i].size());
    return result;
}
