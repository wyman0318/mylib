#include "mainwindow.h"
#include "ui_mainwindow.h"

mainWindow::mainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    this->resize(200,120);
    //y = new QVBoxLayout(this);
    startbtn = new QPushButton("开始",this);
    loadbtn  = new QPushButton("读取",this);
    startbtn->resize(100,20);
    loadbtn->resize(100,20);
    startbtn->move(50,20);
    loadbtn->move(50,70);

//    y->addWidget(startbtn);
//    y->addWidget(loadbtn);
    //y->setSizeConstraint(QLayout::SetFixedSize);

    this->setWindowTitle("简易贪吃蛇");

    connect(startbtn,SIGNAL(clicked(bool)),this,SLOT(StartEvent()));
    connect(loadbtn,SIGNAL(clicked(bool)),this,SLOT(LoadEvent()));
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::StartEvent()
{
    this->hide();
    w = new Widget(1);
    w->show();
}

void mainWindow::LoadEvent()
{
    this->hide();
    w= new Widget(2);
    w->show();
}
