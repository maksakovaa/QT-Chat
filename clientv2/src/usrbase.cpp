#include "usrbase.h"

void UsrBase::slotGetUsrBase()
{
    base->clear();
    Online->sendToServer("GET_USRBASE");
    emit updateUBinit();
}

User UsrBase::getUser(int id)
{
    return base->at(id);
}

QString UsrBase::getUserName(const QString &uuid)
{
    for (auto usr: *base)
    {
        if(usr.getUUID() == uuid)
        {
            return usr.getName();
        }
    }
}

void UsrBase::setCurrentUUID(const QString &uuid)
{
    currentUUID = uuid;
}

User UsrBase::splitUsrPkg(QString &userStr)
{
    QString uuid, name, login, pwd;
    int deleted;
    uuid = unTagStructParam(m_Attr::indUUID, userStr);
    name = unTagStructParam(m_Attr::indName, userStr);
    login = unTagStructParam(m_Attr::indLogin, userStr);
    pwd = unTagStructParam(m_Attr::indPwd, userStr);
    deleted = unTagStructParam(m_Attr::indDel, userStr).toInt();
    User newUser(uuid, name, login, pwd, deleted);
    return newUser;
}

void UsrBase::slotAddNewUser(QString& usrPkg)
{
    User newUser = splitUsrPkg(usrPkg);
    if(newUser.getStatus() == 0 && newUser.getUUID() != currentUUID)
    {
        emit userAdded(newUser);
    }
    addInBase(newUser);
}

QString UsrBase::tagStructParam(m_Attr id, const QString &value)
{
    QString result = sepBeg[static_cast<int>(id)];
    result += value;
    result += sepEnd[static_cast<int>(id)];
    return result;
}

QString UsrBase::unTagStructParam(m_Attr id, QString &value)
{
    int i = static_cast<int>(id);
    QString result = value.sliced(value.indexOf(sepBeg[i]) + sepBeg[i].size(), value.indexOf(sepEnd[i]) - sepBeg[i].size());
    value.remove(0, value.indexOf(sepEnd[i])+sepEnd[i].size());
    return result;
}
