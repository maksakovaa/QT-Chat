#ifndef MESSAGE_H
#define MESSAGE_H

#include "functions.h"
#include <QString>

class Message
{
private:
    int m_id;
    QString m_from;
    QString m_to;
    QString m_date;
    QString m_text;
    int m_status;
public:
    Message(const QString& msgFrom, const QString& msgTo, const QString& msgText);
    Message(int id, const QString& msgFrom, const QString& msgTo, const QString& msgDate, const QString& msgText, int msgStatus);
    Message(const Message& other);
    Message& operator= (const Message& other);
    Message(Message&& other);
    Message& operator= (Message&& other);
    int get_mid() const;
    const QString& get_mfrom() const;
    const QString& get_mto() const;
    const QString& get_mdate() const;
    const QString& get_mtext() const;
    int get_mstatus() const;
    void set_read();
};

#endif // MESSAGE_H
