#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mycamera.h"
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>
#include <QMessageBox>
#include "iso14443a.h"
#include "rfid.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void initTTY();
    void closeTTY();

signals:
    void onDetected();

private:
    Ui::Widget *ui;

//    QPushButton *startbtn;
    QPushButton *pushButtonUART;
    QLabel      *lb1;
    QLabel      *lb2;
    QLabel      *lb3;
    QLabel      *lb4;
    QLineEdit   *cardnumber;
    QComboBox   *ComboBoxBaud;
    QComboBox   *ComboBoxCOM;
    QComboBox   *ComboBoxDataBits;
    QTimer      t;
    int         fd;
    mycamera *camera;

private slots:
    void detectCard();
    void getCardNumber();
    void pushbutton_click(bool);

};

#endif // WIDGET_H
