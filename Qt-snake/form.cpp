#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->resize(120,100);

    vb      = new QVBoxLayout(this);
    SaveBtn = new QPushButton;
    ExitBtn = new QPushButton;
    CountBtn = new QPushButton;

    SaveBtn->resize(this->width()/2,this->height()/4);
    SaveBtn->setText("保存");
    ExitBtn->resize(this->width()/2,this->height()/4);
    ExitBtn->setText("退出");
    CountBtn->resize(this->width()/2,this->height()/4);
    CountBtn->setText("继续");

    vb->addWidget(CountBtn);
    vb->addWidget(SaveBtn);
    vb->addWidget(ExitBtn);

    vb->setSizeConstraint(QLayout::SetFixedSize);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setWindowModality(Qt::WindowModal);

    connect(CountBtn,SIGNAL(clicked(bool)),this,SLOT(CountEvent()));
    connect(ExitBtn,SIGNAL(clicked(bool)),this,SLOT(ExitEvent()));
    connect(SaveBtn,SIGNAL(clicked(bool)),this,SLOT(SaveEvent()));

}

Form::~Form()
{
    delete ui;
}

void Form::CountEvent()
{
    emit count();
    this->close();
}

void Form::SaveEvent()
{
    emit save();
}

void Form::ExitEvent()
{
    emit ret();
    this->close();
}
