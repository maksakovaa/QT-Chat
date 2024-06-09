#ifndef MESSAGEBASE_H
#define MESSAGEBASE_H

#include "usrbase.h"
#include "message.h"

class MessageBase: public Base<Message>
{
    Q_OBJECT

public:
    MessageBase() = default;
    ~MessageBase() = default;
    const Message& getMessage(int id);
    void setNetPtr(QNet * ptr);
    void setAllMsgBasePtr(MessageBase* ptr);
    void setPerMsgBasePtr(MessageBase* ptr);
    int countUnreadAllMsg();
    int countUnreadPrivateMsg(const QString& from, const QString& to);
    void markMsgRead(int id);
public slots:
    void addMsg(QString& msgPkg);
    void getMsgBase(const QString& uuid);
signals:
    void newMsgAdded();
private:
    enum class m_Attr {indId, indFrom, indTo, indDate, indMsg, indStatus, indFor};
    QString sepBeg[7] { "<id>", "<from>", "<to>", "<date>", "<text>", "<status>", "<for_user>" };
    QString sepEnd[7] { "</id>", "</from>", "</to>", "</date>", "</text>", "</status>", "</for_user>" };
    QNet* Online;
    MessageBase* AllMsgBase;
    MessageBase* PerMsgBase;
public:
    QString unTag(m_Attr id, QString& value);
    QString tagStructParam(m_Attr id, const QString& value);
    Message splitMsgPkg(QString& msgPkg);
    QString packMsgPkg(Message& msg);
    QString packAllMsgStatus(Message &msg, QString uuid);
    QString packPerMsgStatus(Message &msg);
};

#endif // MESSAGEBASE_H
