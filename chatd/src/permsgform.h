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

private:
    Ui::PerMsgForm *ui;
};

#endif // PERMSGFORM_H
