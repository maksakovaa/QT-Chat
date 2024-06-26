#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "logger.h"
#include "net.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void logEntry(const QString& entry);
    void updateUsers();
    void updateAllMsg();
    void updatePerMsg();
    void updateOnlineList();
private slots:
    void on_DiconnecBtn_clicked();

private:
    void initObjects();
    void storePtr();
    void classConnections();
private:
    Ui::MainWindow *ui;
    Settings *Config;
    Logger *Log;
    UsrBase *UserBase;
    MsgBase *AllMsgBase;
    MsgBase *PerMsgBase;
    Database *SQL_DataBase;
    Net *Online;
};
#endif // MAINWINDOW_H
