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
    void setName(const QString &newName);
    void setUuid(const QString &newUuid);
    void setMsgCount(int count);
    QString getName() const;
    QString getUuid() const;

signals:
    void openChat(const QString& userUuid);
private slots:
    void on_GoToChat_clicked();

private:
    Ui::UserForm *ui;
    QString m_name, m_uuid;
};

#endif // USERFORM_H
