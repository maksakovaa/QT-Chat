#ifndef USER_H
#define USER_H
#include <QString>

class User
{
private:
    QString m_uuid;
    QString m_name;
    QString m_login;
    QString m_pwd;
    int m_status;
public:
    User(const QString& newName, const QString& newLogin, const QString& newPwd);
    User(const QString& newUUID, const QString& newName, const QString& newLogin, const QString& newPwd, int status);
    User(const User& other);
    User& operator= (const User& other);
    User(User&& other);
    User& operator= (User&& other);
    const QString& getUUID() const;
    const QString& getName() const;
    const QString& getLogin() const;
    const QString& getPwd() const;
    int getStatus() const;
    void setDeleted();
};

#endif // USER_H
