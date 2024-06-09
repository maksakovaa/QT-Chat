#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QWidget>
#include "messagebase.h"

namespace Ui {
class ChatView;
}

class ChatView : public QWidget
{
    Q_OBJECT

public:
    explicit ChatView(QWidget *parent = nullptr);
    ~ChatView();
    void setUBptr(UsrBase* ptr);
    void setAllMBptr(MessageBase* ptr);
    void setPerMBptr(MessageBase* ptr);
    void openChat(const QString& uuid);
signals:
    void openChatList();
    void sendMsg(const QString& message, const QString& msgTo);
    void updAllMsgStatus(const QString& pkg);
    void updPerMsgStatus(const QString& pkg);

public slots:
    void on_backToList_clicked();
    void on_sndMsgButton_clicked();
    void fillAM();
    void fillPM();
private:
    void insertAM(const Message& msg);
    void insertPM(const Message& msg);

private:
    Ui::ChatView *ui;
    UsrBase* UserBase;
    MessageBase* AllMsgBase;
    MessageBase* PerMsgBase;
    QString thisUuid;
    QString currentUUID;
};

#endif // CHATVIEW_H
