#ifndef QNET_H
#define QNET_H

#include <QFile>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QMessageBox>
#include <QRegularExpression>

class QNet : public QObject
{
    Q_OBJECT
public:
    explicit QNet(QObject *parent = nullptr);
    void sendToServer(const QString& str);
    QTcpSocket *getSocket() const;
    void cutRequest(QString &request);

signals:
    void addUser(QString& usrPkg);
    void authResponce(QString authPkg);
    void regResponce(QString regPkg);
    void updateUsrBase();
    void updateAMBase();
    void updatePMBase();
    void addAllMsg(QString& msgPkg);
    void addPerMsg(QString& msgPkg);

public slots:
    void onGetData();
    void onConnectionLost();
    void sendAMStatus(const QString& pkg);
    void sendPMStatus(const QString& pkg);
private:
    void responseHandler(QString str);
    void getConfig();
private:
    QString delim{"<|>"};
    QTcpSocket * socket;
    QByteArray Data;
    quint16 nextBlockSize;
    QString package;
    QString ip;
    int port;
};

#endif // QNET_H
