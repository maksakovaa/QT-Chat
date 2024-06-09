#ifndef PERMSGFORM_H
#define PERMSGFORM_H

#include <QWidget>

namespace Ui {
class PerMsgForm;
}

class PerMsgForm : public QWidget
{
    Q_OBJECT

public:
    explicit PerMsgForm(QWidget *parent = nullptr);
    ~PerMsgForm();
    void setMsgFrom(const QString& from);
    void setMsgTo(const QString& to);
    void setMsgDate(const QString& date);
    void setMsgText(const QString& text);
private:
    Ui::PerMsgForm *ui;
};

#endif // PERMSGFORM_H
