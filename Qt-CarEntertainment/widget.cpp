#include "widget.h"
#include "ui_widget.h"
#include "tty.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->cutMusic->setCheckable(true);
    press = false;
    is_show=false;
    is_finish=true;
    curr=0;
    lenth=0;
    pid=0;

    QDir dir("./music");
    QStringList nameFilters;
    nameFilters << "*.mp3";
    files = dir.entryList(nameFilters,QDir::Files|QDir::Readable,QDir::Name);
    ui->listWidget->addItems(files);
    ui->listWidget->hide();

    QString str;
    foreach (str,files)
    {
        lenth++;
    }

    fd2 = tty_starts();
    ttylink = new mytty(fd2);
    ttylink->start();
    myprocess = new QProcess(this);
    connect(myprocess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(finished()));
    connect(ttylink,SIGNAL(blue_msg(int)),this,SLOT(blue_action(int)));
    connect(ttylink,SIGNAL(blue_msgc(QString)),this,SLOT(show_buf(QString)));

    music_name=QString("%1%2").arg("./music/").arg(files[curr]);
    listm << "-af" << "volume=-10" <<music_name;
    ui->lineEdit->setText(files[curr]);

    ui->vol_ctl->setRange(-20,0);
    ui->vol_ctl->setValue(-10);


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_go_Btn_clicked()
{

    ui->pidnum->setText(QString::number(pid));
    ui->currEd->setText(QString::number(curr));
    ui->filesEd->setText(QString(files[curr]));


    if(pid==0)
    {
        myprocess->close();

        myprocess->start("mplayer",listm);
        ui->lineEdit->setText(QString(listm[2]).split("/").last());
        ui->go_Btn->setText("暂停");
        press=true;
        myprocess->waitForStarted();
        is_finish=true;//防冲突
    }
    else if(!press&&pid!=0)
    {

        const char *c = QString("kill -18 %1").arg(QString::number(pid)).toStdString().c_str();
        system(c);

        ui->go_Btn->setText("暂停");
        press =true;
    }
    else if(press&&pid!=0)
    {
        const char *h = QString("kill -19 %1").arg(QString::number(pid)).toStdString().c_str();
        system(h);
        ui->go_Btn->setText("开始");
        press = false;
    }

    pid = myprocess->pid();
}

void Widget::on_listWidget_itemSelectionChanged()
{
    cur=ui->listWidget->selectedItems();
    music_name=QString("%1%2").arg("./music/").arg(cur[0]->text());
    listm[2] = music_name;
    curr=ui->listWidget->currentRow();
    press=false;
    pid=0;
    this->on_go_Btn_clicked();
}

void Widget::on_qiangzhi_clicked()
{
    system("killall mplayer");
}

void Widget::on_prev_clicked()
{
    if(curr!=0)
    {
        curr -= 1;
    }
    else
    {
        curr = lenth-1;
    }

    music_name=QString("%1%2").arg("./music/").arg(files[curr]);
    listm[2] = music_name;

    press=false;
    is_finish=false;//防止和ｆｉｎｓｈ信号冲突
    pid=0;
    this->on_go_Btn_clicked();

}

void Widget::on_next_clicked()
{
    if(curr!=lenth-1)
    {
        curr += 1;
    }
    else
    {
        curr = 0;
    }

    music_name=QString("%1%2").arg("./music/").arg(files[curr]);
    listm[2] = music_name;

    is_finish=false;
    press=false;
    pid=0;
    this->on_go_Btn_clicked();

}

void Widget::on_cutMusic_clicked()
{
    if(!is_show)
    {
        ui->listWidget->show();
        ui->cutMusic->setText("隐藏列表");
        is_show=true;
    }
    else
    {
        ui->listWidget->hide();
        ui->cutMusic->setText("打开列表选择歌曲");
        is_show=false;
    }
}

void Widget::finished()
{
    if(is_finish)
    {
        on_next_clicked();
    }
}

void Widget::blue_action(int sig)
{
    switch(sig)
    {
    case 1:if(!press) on_go_Btn_clicked();break;
    case 2:if(press) on_go_Btn_clicked();break;
    case 3:on_next_clicked();break;
    case 4:on_prev_clicked();break;
    case 5:on_go_Btn_clicked();break;
    case 6:file_rev(fd2);break;
    }
}

void Widget::show_buf(QString buf)
{
    ui->currEd->setText(buf);
}


void Widget::on_vol_ctl_valueChanged(int value)
{
    listm[1]=QString("volume=%1").arg(QString().number(value));
}
