#ifndef USRBASE_H
#define USRBASE_H

#include "TmplBase.h"
#include "user.h"

class UsrBase: public Base<User>
{
    Q_OBJECT

public:
    UsrBase() = default;
    ~UsrBase() = default;
    User splitUsrPkg(QString& userStr);
    User getUser(int id);
    QString getUserName(const QString& uuid);
    void setCurrentUUID(const QString& uuid);
signals:
    void userAdded(const User& newUser);
    void updateUBinit();
public slots:
    void slotAddNewUser(QString& usrPkg);
    void slotGetUsrBase();
private:
    enum class m_Attr { indUUID, indName, indLogin, indPwd, indDel };
    QString sepBeg[5]{ "<uuid>", "<name>", "<login>", "<pwd>", "<deleted>"};
    QString sepEnd[5] = { "</uuid>", "</name>", "</login>", "</pwd>", "</deleted>"};
    QString currentUUID;
public:
    QString tagStructParam(m_Attr, const QString& value);
    QString unTagStructParam(m_Attr id, QString& value);
};

#endif // USRBASE_H
