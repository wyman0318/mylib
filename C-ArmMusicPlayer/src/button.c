#include "common.h"
#include "piano.h"
#include "list.h"

#define PIANO 1
#define AUDIO 2

//old是废弃接口
int buttonfun(int old) //60 280
{
	//打开触摸屏设备文件
	int tp = open("/dev/input/event0",O_RDWR);
	struct input_event buf;

	bool xdone = false;
	bool ydone = true;
	bool press = false;
	bool home = false;
	press=true;
	int x_end=0,y_end=0;
	int new = 0;
	while(1)
	{
		//非阻塞读取触摸事件
		bzero(&buf,sizeof(buf));
		read(tp,&buf,sizeof(buf));

		//松开的话恢复对应键位为非按压图片
		if(buf.type==EV_KEY&&buf.code==BTN_TOUCH&&buf.value==0)
		{
			piano_off(new);
		}

		//绝对坐标事件，读取x和y的值
		if(buf.type==EV_ABS)
		{
			if(buf.code==ABS_X&&ydone)
			{
				x_end = buf.value*800/1024;
				xdone = true;
				ydone = false;
			}
			if(buf.code==ABS_Y&&xdone)
			{
				y_end = buf.value*480/600;
				xdone = false;
				ydone = true;
			}
		}
		
		//当x和y都读取到之后判断是否符合对应的坐标
		if(x_end>0&&y_end>0)
		{
			printf("x:%d ",x_end);
			printf("y:%d\n",y_end);
			if(x_end>=40&&x_end<=755&&y_end>=100&&y_end<=380)
			{
				//获得的坐标减去坐标非钢琴键位的横坐标，再除去每个键位
				//的固定横坐标，就是对应的钢琴键位
				x_end = (x_end-40)/65+1;
				printf("%d\n",x_end);
				if(x_end!=new)
				{
					//保证杀死后台播放器
					system("killall madplay");
					pid_t tid;
					
					//子进程进行播放操作，父进程不进入
					tid=fork();
					if(tid==0)
					{
						pid_t tid2;
						//再创建多进程，子进程显示图片，父进程播放音乐
						tid2=fork();
						if(tid2==0)
						{
							piano_off(new);
							piano_on(x_end);
							exit(0);
						}
						else if(tid2>0)
						{
							char *str=calloc(1,10);
							char *d=calloc(1,10);
							strcpy(d,"mp3/d");
							sprintf(str,"%d",x_end);
							strcat(d,str);
							strcat(d,".mp3");

							execlp("madplay","madplay",d,NULL);
						}

					}
					new=x_end;
				}
			}
			//“home”功能，回主菜单
			else if(x_end>=700&&y_end<100)
			{
				return 1;	
			}
			x_end=0;
			y_end=0;
		}
	}

}

int button_init(void)
{
	int tp = open("/dev/input/event0",O_RDWR);
	struct input_event buf;

	bool xdone = false;
	int x_end=0;
	soft_init();

	while(1)
	{
		bzero(&buf,sizeof(buf));
		read(tp,&buf,sizeof(buf));

		if(buf.type==EV_KEY&&buf.code==BTN_TOUCH&&buf.value==0)
		{
			if(x_end>=0&&x_end<400)
			{
				return AUDIO;
			}
			if(x_end>=400&&x_end<=800)
			{
				return PIANO;
			}
		}

		if(buf.type==EV_ABS&&buf.code==ABS_X)
		{
			x_end = buf.value*800/1024;
		}
	}
}

int audio_fun(void)
{
	int tp = open("/dev/input/event0",O_RDWR);
	struct input_event buf;

	bool xdone = false;
	bool ydone = true;
	bool press = false;
	bool start = false,stop = true;
	//bool prev = false,next = false;
	int x_end=0,y_end=0;
	int new = 0;

	//创建链表头
	linklist *list = creatlist();

	struct stat info;
	bzero(&info,sizeof(info));

	//获取文件夹指针
	DIR *dp = opendir("audio");
	//更换操作目录
	chdir("audio");
	struct dirent *ep;
	while(1)
	{
		//获取目标文件夹下的文件
		ep = readdir(dp);
		
		//没有则终止
		if(ep == NULL)
			break;

		//过滤掉非mp3文件
		if(!(strstr(ep->d_name,".mp3")!=NULL))
		{
			continue;
		}
		
		//将mp3文件名作为链表结点的数据域插入到链表中
		bzero(&info,sizeof(info));
		stat(ep->d_name,&info);
		linklist *new = creatnode(ep->d_name);
		printf("%s\n",ep->d_name);
		insert(list,new);
	}

	linklist *using = list->next;

	pid_t pid = 0;
	int flag=0;

	while(1)
	{
		bzero(&buf,sizeof(buf));
		read(tp,&buf,sizeof(buf));


		if(buf.type==EV_KEY&&buf.code==BTN_TOUCH&&buf.value>0)
		{
			press=true;
		}

		if(buf.type==EV_KEY&&buf.code==BTN_TOUCH&&buf.value==0)
		{
			press=false;
		}

		if(buf.type==EV_ABS)
		{
			if(buf.code==ABS_X&&ydone)
			{
				x_end = buf.value*800/1024;
				xdone = true;
				ydone = false;
			}
			if(buf.code==ABS_Y&&xdone)
			{
				y_end = buf.value*480/600;
				xdone = false;
				ydone = true;
			}
		}

		if(x_end>0&&y_end>0&&!press)
		{
			printf("x:%d\n",x_end);
			printf("y:%d\n",y_end);

			if(x_end>700&&y_end<100)
			{
				return 1;
			}
			else if(y_end>300&&y_end<420)
			{
				if(x_end>340&&x_end<460)
				{
					if(!start)
					{
						audiostop();
						start = true;
						stop = false;
						printf("%s\n",using->name);
						
						//如果是被暂停的，则向子进程发送继续信号
						if(pid > 0)
						{
							flag = kill(pid,SIGCONT);
						}
						//如果还没有开始，则创建进程播放当前音乐
						else if(pid == 0)
						{
							pid = fork();
							if(pid == 0)
							{
								execlp("madplay","madplay",using->name,NULL);
							}
						}
					}
					else if(start)
					{
						//暂停操作
						audiostart();
						start = false;
						stop = true;
						flag = kill(pid,SIGSTOP);
					}

					else
					{
						printf("未知错误!\n");
					}

					if(flag == -1)//音乐播放完了
					{
						flag =0;
						start = false;
						stop = true;
						pid = 0;
					}
				}

				else if((x_end>160&&x_end<280)||(x_end>520&&x_end<640))
				{
					//下一首和上一首的操作，遍历链表，播放
					system("killall madplay");
					if(x_end>160&&x_end<280)
					{
						using = using -> prev;
						if(using == list)
						{
							using = using -> prev;
						}
					}


					if(x_end>520&&x_end<640)
					{

						using = using -> next;
						if(using == list)
						{
							using = using -> next;
						}
					}
					audiostop();
					start = true;
					stop = false;
					pid = fork();
					if(pid == 0)
					{
						execlp("madplay","madplay",using->name,NULL);
					}
				}
			}
			x_end=0;
			y_end=0;
		}


	}
}
