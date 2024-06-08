#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();
signals:
    void sig_registerRequested();
    void sig_accepted(QString UUID, QString userName);
    void sig_rejected();
    void sig_reqOnlineAuth(const QString& userLogin, const QString& pwd);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_RegistrationButton_clicked();

private:
    Ui::LoginForm *ui;
};

#endif // LOGINFORM_H
