#pragma once
#include "user.h"

class UsrBase: public Base<User>
{
private:
    static const int usrAttr{ 5 };
    enum class m_Attr { indUUID, indName, indLogin, indPwd, indDel };
    QString sepBeg[usrAttr]{ "<uuid>", "<name>", "<login>", "<pwd>", "<deleted>" };
    QString sepEnd[usrAttr] = { "</uuid>", "</name>", "</login>", "</pwd>", "</deleted>" };
    SHA256 sha256;
public:
	UsrBase() = default;
	~UsrBase() = default;
    User splitUsrPkg(QString& usrPkg);
    User getUser(int id);
    User getUser(const QString& uuid);
    QString packUsrPkg(User& account);
    void splitChgPwd(QString& str, QString arr[]);
    QString tagStructParam(m_Attr id, QString value);
    QString unTag(m_Attr id, QString& str);
    bool authChk(const QString& login, const QString& pwd);
    bool userExists(User newUser);
    const QString& getUUID(const QString& login);
};
