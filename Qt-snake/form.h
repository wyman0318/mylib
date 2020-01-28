#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
    Ui::Form *ui;
    QPushButton *CountBtn;
    QPushButton *SaveBtn;
    QPushButton *ExitBtn;
    QVBoxLayout *vb;

signals:
    void count();
    void save();
    void ret();

private slots:
    void CountEvent();
    void SaveEvent();
    void ExitEvent();
};

#endif // FORM_H
