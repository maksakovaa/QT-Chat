#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qnet.h"
#include "messagebase.h"
#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString userUUID, QString userName, QNet* ptr, QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow* createClient();

private slots:
public slots:
    void slotOpenChat(const QString& uuid);
    void slotOpenChatList();
    void slotSendMessage(const QString& message, const QString& uuid);
private:
    Ui::MainWindow *ui;
    QNet* Online;
    UsrBase* UserBase;
    QString m_userUUID;
    QString m_userName;
    MessageBase* AllMsgBase;
    MessageBase* PerMsgBase;
private:
    void initObjects();
    void classConnections();
    void setPtr();
};
#endif // MAINWINDOW_H
