#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>

Widget::Widget(int mode,QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Widget)
{
        ui->setupUi(this);

        fd=tty_starts();
        mymenu = new Form(this);


        first=true;
        stay=false;
        difficluty=0;
        this->resize(800,480);
        background = new QPixmap(this->size());
        background->fill(Qt::black);

        food = new QPixmap(QSize(20,20));
        food->fill(Qt::red);

        snake = new QPixmap(QSize(20,20));
        snake->fill(Qt::white);
        snake_len=3;//长度３

        //    snake_head.setX(340);  //尾部
        //    snake_head.setY(240);
        snake_x1=340;
        snake_y1=240;


        eaten = true;
        is_turn=true;

        if(mode==2)
                LoadEvent();
        else
        {
            change << 4 << 4 << 4;//初始三个格子都是向右移动
            snake_timer.start(400);
        }
        myblue = new mytty(fd);
        myblue->start();
        connect(&snake_timer,SIGNAL(timeout()),this,SLOT(creatsnake()));
        connect(mymenu,SIGNAL(count()),this,SLOT(game_count()));
        connect(mymenu,SIGNAL(save()),this,SLOT(game_save()));
        connect(mymenu,SIGNAL(ret()),this,SLOT(close()));
        connect(myblue,SIGNAL(blue_msg(int)),this,SLOT(turn_dir(int)));
}

Widget::~Widget()
{
        delete ui;
}

void Widget::paintEvent(QPaintEvent *e)
{
        //如果直接在要绘制的对象中的使用QPainter
        //就直接用当前对象this初始化p即可
        QPainter p(this);

        p.drawPixmap(QPoint(0,0),*background);
}

void Widget::keyPressEvent(QKeyEvent *e)
{
        //    qDebug() << e->key();
        switch(e->key())
        {
        case Qt::Key_Up:
                if(change[0]==2) break;change[0]=1;break;
        case Qt::Key_Down:
                if(change[0]==1) break;change[0]=2;break;
        case Qt::Key_Left:
                if(change[0]==4) break;change[0]=3;break;
        case Qt::Key_Right:
                if(change[0]==3) break;change[0]=4;break;
        case Qt::Key_Space:
                menu_start();break;
        }
}

void Widget::snake_add()
{
        snake_len++;
        change.push_front(change[0]);
        if((snake_len/5>difficluty)&&difficluty<8)
        {
                difficluty++;
                add_difficult();
        }
}

void Widget::menu_start()
{
        snake_timer.stop();
        mymenu->show();
}

void Widget::add_difficult()
{
        snake_timer.stop();
        snake_timer.start(400-50*difficluty);
}

void Widget::creatsnake()
{
        QPainter p;

        background->fill(Qt::black);
        p.begin(background);

        if(eaten)
        {
                for(int i = 0;i<1;i++)
                {
                        food_x = qrand()%780;
                        food_y = qrand()%460;
                        food_x -= food_x%20;
                        food_y -= food_y%20;
                        if(first) //判断是否开始跟开始的蛇重叠
                        {
                                if((food_x==400&&food_y==240)||(food_x==380&&food_y==240)||food_x==360&&food_y==240)
                                {
                                        i--;
                                }
                                else
                                {
                                        first=false;
                                }
                        }
                }
                eaten=false;
        }
        p.drawPixmap(QPoint(food_x,food_y),*food); //保持

        snake_x=snake_x1;
        snake_y=snake_y1;


        for(int i=snake_len-1;i>=0;i--)
        {
                if(!stay)
                {
                        switch(change[i])
                        {
                        case 1:snake_y -= 20;break;
                        case 2:snake_y += 20;break;
                        case 3:snake_x -= 20;break;
                        case 4:snake_x += 20;break;
                        }
                }
                stay=false;
                if(snake_y>=480||snake_y<=-20||snake_x>=800||snake_x<=-20)
                {
                        QMessageBox::information(this,"游戏结束！","失败！",QMessageBox::Ok);
                        this->close();
                }
                p.drawPixmap(QPoint(snake_x,snake_y),*snake);

                is_turn=true;
                if(snake_y==food_y&&snake_x==food_x)
                {
                        eaten = true;
                        is_turn=false;
                        stay=true;
                        snake_add();
                }
                if(i==0&&is_turn)
                {
                        for(int j=snake_len-1;j>0;j--)
                        {
                                change[j]=change[j-1];
                        }
                }
                if(i==snake_len-1)
                {
                        snake_x1=snake_x;
                        snake_y1=snake_y;
                }
        }

        p.end();
        update();
}

void Widget::game_count()
{
        snake_timer.start(400);
}

void Widget::game_save()
{
        QString log;


        QFile file("savelog.txt");
        if(file.exists())
        {
                file.remove();
        }

        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
                QMessageBox::information(this,"打开文件失败",file.errorString());
        }

        log.append(QString("%1\n%2\n").arg(snake_x1).arg(snake_y1));//x,y
        log.append(QString("%1\n").arg(difficluty));//难度
        log.append(QString("%1\n%2\n").arg(food_x).arg(food_y));//食物
        log.append(QString("%1\n").arg(snake_len));//长度
        int i;
        foreach(i,change)
        {
                log.append(QString().number(i));
                log.append("\n");
        }
        QByteArray byt= log.toUtf8();
        file.write(byt);
        file.close();
        QMessageBox::information(this,"保存","保存成功",QMessageBox::Ok);
}

void Widget::LoadEvent()
{
        snake_timer.stop();
        QFile file("savelog.txt");
        if(!file.exists())
        {
                QMessageBox::information(this,"读取","没有检测到存档",QMessageBox::Ok);
                snake_timer.start();
                return;
        }

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
                QMessageBox::information(this,"文件","打开文件失败",file.errorString());
        }
        for(int i=0;!file.atEnd();i++)
        {
                QByteArray line = file.readLine();
                QString str(line);
                qDebug() << str.toInt();
                switch(i)
                {
                case 0:snake_x1=str.toInt();break;
                case 1:snake_y1=str.toInt();break;
                case 2:difficluty=str.toInt();break;
                case 3:food_x=str.toInt();break;
                case 4:food_y=str.toInt();break;
                case 5:snake_len=str.toInt();break;
                default:change.push_back(str.toInt());
                }
        }
        eaten=false;
        first=false;

        add_difficult();
}

void Widget::turn_dir(int data)
{
    switch(data)
    {
    case 1:
            if(change[0]==2) break;change[0]=1;break;
    case 2:
            if(change[0]==1) break;change[0]=2;break;
    case 3:
            if(change[0]==4) break;change[0]=3;break;
    case 4:
            if(change[0]==3) break;change[0]=4;break;
    case 5:
            menu_start();break;
    }
}
