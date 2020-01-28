#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "widget.h"
#include <QVBoxLayout>
#include <QPushButton>

namespace Ui {
class mainWindow;
}

class mainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();


signals:
    void loadsig();

private:
    Ui::mainWindow *ui;
    Widget *w;
    //QVBoxLayout *y;
    QPushButton *startbtn;
    QPushButton *loadbtn;

private slots:
    void StartEvent();
    void LoadEvent();
};

#endif // MAINWINDOW_H
