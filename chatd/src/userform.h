#ifndef USERFORM_H
#define USERFORM_H

#include <QWidget>

namespace Ui {
class UserForm;
}

class UserForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserForm(QWidget *parent = nullptr);
    ~UserForm();
    void setUserName(QString name);
    void setStatus(int newStatus);
    void setUuid(const QString &newUuid);

signals:
    void updateUsrSatus(const QString& uuid, int newStatus);
private slots:
    void on_BlockUnblockBtn_clicked();

private:
    Ui::UserForm *ui;
    int status;
    QString uuid;
};

#endif // USERFORM_H
