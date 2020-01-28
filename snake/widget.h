#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPen>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QTimer>
#include <QList>
#include <QKeyEvent>
#include <QEvent>
#include <QDebug>
#include <form.h>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QByteArray>
#include <mytty.h>
#include <tty.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(int mode=1,QWidget *parent = 0);
    ~Widget();

    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e) override;
    void snake_add();
    void menu_start();
    void add_difficult();

private:
    Ui::Widget *ui;

    bool eaten;
    bool first;
    bool stay;
    bool is_turn;
    int food_x,food_y;
    int snake_x,snake_y;
    int snake_x1,snake_y1;
    int difficluty;
    QPixmap *background;
    QPixmap *food;
    QPixmap *snake;
    int snake_len;
    QTimer snake_timer;
    QTimer food_timer;
    QList<int> change;
    Form *mymenu;
    mytty *myblue;
    int fd;

//signals:
//    void eaten();

private slots:
   // void creatfood();
    void creatsnake();
    void game_count();
    void game_save();
    void LoadEvent();
    void turn_dir(int);
};

#endif // WIDGET_H
