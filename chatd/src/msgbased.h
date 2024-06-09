#pragma once
#include "message.h"

class MsgBase: public Base<Message>
{
    Q_OBJECT

private:
    enum class m_Attr{ indId,
                        indFrom,
                        indTo,
                        indDate,
                        indMsg,
                        indStatus,
                        indFor };
public:
    MsgBase() = default;
    ~MsgBase() = default;
    Message splitMsgPkg(QString msgPkg);
    QString packMsgPkg(Message& msgPkg);
    Message getMsg(int id);
    void splitPMStatus(QString& str, int arr[]);
    void splitAMStatus(QString& str, QString arr[]);
    QString tagStructParam(m_Attr id, const QString& value);
    QString unTagStructParam(m_Attr id, QString& msgPkg);
};
