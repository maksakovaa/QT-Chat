#include "usrbased.h"

User UsrBase::getUser(int id)
{
    return base->at(id);
}

User UsrBase::getUser(const QString& uuid)
{
    for (User account: *base)
    {
        if (account.getUUID() == uuid)
        {
            return account;
        }
    }
    return base->at(0);
}

QString UsrBase::packUsrPkg(User& account)
{
    QString result = tagStructParam(m_Attr::indUUID, account.getUUID());
    result += tagStructParam(m_Attr::indName, account.getName());
    result += tagStructParam(m_Attr::indLogin, account.getLogin());
    result += tagStructParam(m_Attr::indPwd, account.getPwd());
    result += tagStructParam(m_Attr::indDel, QString::number(account.getStatus()));
    return result;
}

void UsrBase::splitChgPwd(QString& str, QString arr[])
{
    arr[0] = unTag(m_Attr::indUUID, str);
    arr[1] = unTag(m_Attr::indPwd, str);
}

QString UsrBase::tagStructParam(m_Attr id, QString value)
{
    QString result = sepBeg[static_cast<int>(id)];
    result += value;
    result += sepEnd[static_cast<int>(id)];
    return result;
}

QString UsrBase::unTag(m_Attr id, QString& str)
{
    int i = static_cast<int>(id);
    QString result = str.sliced(str.indexOf(sepBeg[i]) + sepBeg[i].size(), str.indexOf(sepEnd[i]) - sepBeg[i].size());
    str.remove(0, str.indexOf(sepEnd[i])+sepEnd[i].size());
    return result;
}

bool UsrBase::authChk(const QString &login, const QString &pwd)
{
    for(auto usr: *base)
    {
        if(usr.getLogin() == login)
        {
            if(usr.getPwd() == pwd)
            {
                return true;
            }
        }
    }
    return false;
}

bool UsrBase::userExists(User newUser)
{
    for (auto usr: *base)
    {
        if(usr.getLogin() == newUser.getLogin())
        {
            return true;
        }
    }
    return false;
}

const QString &UsrBase::getUUID(const QString &login)
{
    for (auto usr: *base)
    {
        if(usr.getLogin() == login)
        {
            return usr.getUUID();
        }
    }
}
