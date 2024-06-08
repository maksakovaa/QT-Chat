#ifndef MESSAGEFORM_H
#define MESSAGEFORM_H

#include <QWidget>

namespace Ui {
class MessageForm;
}

class MessageForm : public QWidget
{
    Q_OBJECT

public:
    explicit MessageForm(QWidget *parent = nullptr);
    ~MessageForm();
    void setMsgFrom(const QString& from);
    void setMsgDate(const QString& date);
    void setMsgText(const QString& text);

private:
    Ui::MessageForm *ui;
};

#endif // MESSAGEFORM_H
