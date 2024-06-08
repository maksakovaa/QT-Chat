#pragma once
#include "message.h"

class MsgBase: public Base<Message>
{
    Q_OBJECT

public:
    MsgBase() = default;
    ~MsgBase() = default;
    Message splitMsgPkg(QString msgPkg);
    QString packMsgPkg(Message& msgPkg);
    Message getMsg(int id);
    void splitPMStatus(QString& str, int arr[]);
    void splitAMStatus(QString& str, QString arr[]);
private:
    static const int msgAttr{ 7 };
    enum class m_Attr{ indId,
                        indFrom,
                        indTo,
                        indDate,
                        indMsg,
                        indStatus,
                        indFor };
    QString* sepBeg = new QString[msgAttr] ("<id>", "<from>", "<to>", "<date>", "<text>", "<status>", "<for_user>" );
    QString* sepEnd = new QString[msgAttr] ( "</id>", "</from>", "</to>", "</date>", "</text>", "</status>", "</for_user>" );
public:
    QString tagStructParam(m_Attr id, const QString& value);
    QString unTagStructParam(m_Attr id, QString& msgPkg);
};
