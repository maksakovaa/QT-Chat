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

private slots:
    void on_backToList_clicked();
    void on_sndMsgButton_clicked();
private:
    void fillAM();
    void fillPM(const QString& uuid);
    void insertAM(Message& msg);
    void insertPM(Message& msg);
private:
    Ui::ChatView *ui;
    UsrBase* UserBase;
    MessageBase* AllMsgBase;
    MessageBase* PerMsgBase;
    QString thisUuid;
};

#endif // CHATVIEW_H
