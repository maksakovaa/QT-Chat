#ifndef CHATVIEWLISTFORM_H
#define CHATVIEWLISTFORM_H

#include <QWidget>
#include <QListWidget>
#include <QMessageBox>
#include "usrbase.h"
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
signals:
    void openChat(const QString& uuid);
public slots:
    void slotInsertInList(const User& newUser);
    void updateChatList();
    void slotOpenChat(const QString& uuid);

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::ChatViewListForm *ui;
    UsrBase* UserBase;
};

#endif // CHATVIEWLISTFORM_H
