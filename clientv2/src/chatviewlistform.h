#ifndef CHATVIEWLISTFORM_H
#define CHATVIEWLISTFORM_H

#include <QWidget>
#include <QListWidget>
#include <QMessageBox>
#include "messagebase.h"

namespace Ui {
class ChatViewListForm;
}

class ChatViewListForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatViewListForm(QWidget *parent = nullptr);
    ~ChatViewListForm();
    void setUsrBasePtr(UsrBase* ptr);
    void setAMBasePtr(MessageBase* ptr);
    void setPMBasePtr(MessageBase* ptr);
    void setCurrentUUID(const QString &newCurrentUUID);

signals:
    void openChat(const QString& uuid);
public slots:
    void slotInsertInList(const User& newUser);
    void updateChatList();
    void slotOpenChat(const QString& uuid);
private:
    Ui::ChatViewListForm *ui;
    UsrBase* UserBase;
    MessageBase* AllMsgBase;
    MessageBase* PerMsgBase;
    QString currentUUID;
};

#endif // CHATVIEWLISTFORM_H
