#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dirent.h>
#include "slide.h"
#include "list.h"
#include <dlfcn.h>

enum {up,down,left,right};

void timeless(linklist *head,linklist *dontouch)
{
	linklist *tmp=head->next;
	while(tmp!=head)
	{
		if(tmp==dontouch)
		{
			tmp=tmp->next;
			if(tmp==head)
				break;
		}
		tmp->time=tmp->time-1;
		if(tmp->time==0)
		{
			free(tmp->mem);
			tmp->mem=NULL;
		}
		tmp=tmp->next;
	}
	return;
}


//打开设备文件与映射内存
char *opendevice(void)
{
	int lcd=open("/dev/fb0",O_RDWR);
	if(lcd==-1)
	{
		perror("文件出错");
	}
	char *lcdmem=mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcd,0);
	bzero(lcdmem,800*480*4);
	return lcdmem;
}

//根据图片格式自动返回所需的库路径
char *libchange(char *str)
{
	char *p;
	if(strstr(str,".jpg")!=NULL)
	{
		p="/IOT/wyman/program/electron_albun/lib/libjpgauto2.so";
	}
	else if(strstr(str,".bmp")!=NULL)
	{
		p="/IOT/wyman/program/electron_albun/lib/libbmpauto2.so";
	}
	return p;
}

void cache(char *p,char *lcdmem)
{
	bzero(lcdmem,800*480*4);
	if(lcdmem==MAP_FAILED)
	{
		perror("映射出错");
		exit(0);
	}
	
	memcpy(lcdmem,p,480*800*4);
	return;
}

int main(int argc, char **argv)
{
	//确保两个参数
	if(argc!=2)
	{
		printf("参数有误\n");
		exit(0);
	}
	//动态库参数化调用
	void *handle;
	char* (*display)(char *s,int x,int y);
	void (*lcdfree)(void);
	//创建链表
	linklist *list=creatlist();

	//打开目录读取图片
	struct stat info;
	bzero(&info,sizeof(info));

	if(stat(argv[1],&info)==-1)
	{
		perror("获取指定文件失败");
		exit(0);
	}

	if(!S_ISDIR(info.st_mode))
	{
		printf("不是目录\n");
		exit(0);
	}
	//打开设备文件
	char *device=opendevice();
	//打开目录文件之类
	DIR *dp=opendir(argv[1]);
	chdir(argv[1]);
	struct dirent *ep;
	while(1)
	{
		ep=readdir(dp);
		if(ep==NULL)
			break;
		//跳过非ｊｐｇ和ｂｍｐ文件
		if(!(strstr(ep->d_name,".bmp")!=NULL||strstr(ep->d_name,".jpg")!=NULL))
		{
			continue;
		}
		bzero(&info,sizeof(info));
		stat(ep->d_name,&info);
		linklist *new=creatnode(ep->d_name);
		insert(list,new);
	}
	//初始界面显示第一张图片
	linklist *head=list->next;
	handle=dlopen(libchange(head->name),RTLD_NOW);
	if(handle==NULL)
	{
		perror("打开动态库失败");
	}
	display=dlsym(handle,"displayfun");
	lcdfree=dlsym(handle,"freelcd");
	head->mem=display(head->name,0,0);
	printf("%d\n",__LINE__);
	//进入检测左右滑动的循环，来使图片的轮换
	int n=0;
	int time=0;
	while(1)
	{
		n=0;
		n=slide();
		printf("n:%d\n",n);
		if(n==left)
		{
			head=head->prev;
			if(head==list)
			{
				head=list->prev;
			}
		}
		else if(n==right)
		{
			head=head->next;
			if(head==list)
			{
				head=list->next;
			}
		}
		else 
		{
			continue;
		}
		//如果有缓存则直接映射,除此节点外的其他节点计数减少并返回循环
		head->time=10;
		timeless(list,head);//此节点外的节点时间减少

		if(head->mem!=NULL)
		{
			cache(head->mem,device);
			continue;	
		}
		//第一次接触图片的时候识别格式找到对应的动态库，并将对应函数赋予节点
		if(head->display==NULL)
		{
			handle=dlopen(libchange(head->name),RTLD_NOW);
			if(handle==NULL)
			{
				perror("打开动态库失败");
				exit(0);
			}
			head->display=dlsym(handle,"displayfun");
		}
		head->mem=head->display(head->name,0,0);

	}
	munmap(device,4*480*800);
	return 0;
}
