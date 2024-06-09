#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class RegForm;
}

class RegForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegForm(QWidget *parent = nullptr);
    ~RegForm();

signals:
    void sig_loginRequested();
    void sig_accepted(QString UUID, QString userName);
    void sig_rejected();
    void sig_regUser(const QString& userName, const QString& userLogin, const QString& pwd);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_loginPushButton_clicked();

private:
    Ui::RegForm *ui;
};

#endif // REGISTRATIONFORM_H
