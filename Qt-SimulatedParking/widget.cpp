#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    this->resize(160,480);
    this->move(640,0);

    pushButtonUART  = new QPushButton("开始刷卡",this);
    ComboBoxBaud    = new QComboBox(this);
    ComboBoxCOM     = new QComboBox(this);
    ComboBoxDataBits= new QComboBox(this);
    lb1             = new QLabel("端口",this);
    lb2             = new QLabel("波特率",this);
    lb3             = new QLabel("数据位",this);
    lb4             = new QLabel("卡号",this);
    cardnumber      = new QLineEdit(this);

    QStringList list;
    list << "ttySAC1" << "ttySAC2" << "ttySAC3" << "ttySAC4";
    ComboBoxCOM->addItems(list);
    list.clear();
    list << "9600" << "115200";
    ComboBoxBaud->addItems(list);
    list.clear();
    list << "8" << "7" << "6" << "5";
    ComboBoxDataBits->addItems(list);

    lb1->resize(40,30);
    lb2->resize(40,30);
    lb3->resize(40,30);
    lb4->resize(40,30);
    cardnumber->resize(140,40);
    pushButtonUART->resize(100,40);
    ComboBoxCOM->resize(100,30);
    ComboBoxBaud->resize(100,30);
    ComboBoxDataBits->resize(100,30);

    lb1->move(10,10);
    lb2->move(10,50);
    lb3->move(10,90);
    ComboBoxCOM->move(60,10);
    ComboBoxBaud->move(60,50);
    ComboBoxDataBits->move(60,90);
    pushButtonUART->move(30,130);
    lb4->move(10,180);
    cardnumber->move(10,230);

    pushButtonUART->setCheckable(true);

    camera = new mycamera(this);
    camera->start();

    signal(SIGALRM,refresh);
    connect(&t,SIGNAL(timeout()),this,SLOT(detectCard()));
    connect(this, SIGNAL(onDetected()), this, SLOT(getCardNumber()));
    connect(pushButtonUART, SIGNAL(clicked(bool)), this, SLOT(pushbutton_click(bool)));
}

Widget::~Widget()
{
        delete ui;
}

void Widget::pushbutton_click(bool checked)
{
        if(checked)
        {
                qDebug() << __LINE__;
                initTTY();

                // 开关当前处于按下的状态
                pushButtonUART->setText("探测中...");

                // 禁用所有的输入框
                ComboBoxBaud->setEnabled(false);
                ComboBoxCOM->setEnabled(false);
                ComboBoxDataBits->setEnabled(false);

                // 开启定时器，不断探查卡片是否在能量范围之内
                t.start(200);
        }
        else
        {
                qDebug() << __LINE__;
                closeTTY();

                // 开关当前处于弹起的状态
                pushButtonUART->setText("开始刷卡");

                // 启用所有的输入框
                ComboBoxBaud->setEnabled(true);
                ComboBoxCOM->setEnabled(true);
                ComboBoxDataBits->setEnabled(true);

                // 暂停定时器，不询问卡片的状态
                t.stop();
        }
}

void Widget::detectCard()
{
        init_REQUEST();
        char recvinfo[128];

        // 向串口发送指令
        tcflush (fd, TCIFLUSH);
        write(fd, PiccRequest_IDLE, PiccRequest_IDLE[0]);

        usleep(10*1000);

        bzero(recvinfo, 128);
        qDebug() << __LINE__;
        read(fd, recvinfo, 128);
        qDebug() << __LINE__;

        //应答帧状态部分为0 则请求成功
        if(recvinfo[2] == 0x00)
        {
                qDebug() << "detected!";
                emit onDetected();
        }
        else
        {
                qDebug() << "no card...";
        }
}

void Widget::getCardNumber()
{
        static uint32_t oldid, newid;

        // 获取附近卡片的卡号... ...
        oldid = newid==0xFFFFFFFF?oldid:newid;
        newid = get_id(fd);
        qDebug() << "fetch card Number:" << newid;
        if(newid == 0 || newid == 0xFFFFFFFF)
        {
                return;
        }

        camera->catch_sig=true;
        // flag为真意味着：卡片刚放上去
        if(flagxx)
        {
                if(newid == oldid)
                {
                        beep(5, 0.05);
                }
                else
                {
                        cardnumber->setText(QString("%1").arg(newid));
                        beep(1, 0.3);
                }

                flagxx = false;
        }
        qDebug() << "refres alarm";
        alarm(1);
}

void Widget::closeTTY()
{
        ::close(fd);
}

void Widget::initTTY()
{
        // 根据下拉框选项初始化串口
        const char *tty = QString("/dev/%1").arg(ComboBoxCOM->currentText()).toStdString().c_str();
        fd = open(tty, O_RDWR | O_NOCTTY);
        if(fd == -1)
        {
                QMessageBox::critical(this, "错误", strerror(errno));
                exit(0);
        }

        //声明设置串口的结构体
        struct termios config;
        bzero(&config, sizeof(config));

        // 设置无奇偶校验
        // 设置数据位为8位
        // 设置为非规范模式（对比与控制终端）
        config.c_iflag &= ~IXON;
        //
        // 2. 输出：禁用自定义输出模式
        //
        // 3. 控制：数据字符长度为8位
        //         禁用校验码
        config.c_cflag &= ~CSIZE;
        switch(ComboBoxDataBits->currentText().toInt())
        {
        case 5: config.c_cflag |= CS5; break;
        case 6: config.c_cflag |= CS6; break;
        case 7: config.c_cflag |= CS7; break;
        case 8: config.c_cflag |= CS8; break;
        }
        config.c_cflag &= ~PARENB;

        //设置波特率
        switch(ComboBoxBaud->currentText().toInt())
        {
        case 9600  : cfsetspeed(&config, B9600); break;
        case 115200: cfsetspeed(&config, B115200); break;
        }

        // CLOCAL和CREAD分别用于本地连接和接受使能
        // 首先要通过位掩码的方式激活这两个选项。
        config.c_cflag |= CLOCAL | CREAD;

        // 一位停止位
        config.c_cflag &= ~CSTOPB;

        // 可设置接收字符和等待时间，无特殊要求可以将其设置为0
        config.c_cc[VTIME] = 0;
        config.c_cc[VMIN] = 1;

        // 用于清空输入/输出缓冲区
        tcflush (fd, TCIFLUSH);
        tcflush (fd, TCOFLUSH);

        //完成配置后，可以使用以下函数激活串口设置
        tcsetattr(fd, TCSANOW, &config);
}

