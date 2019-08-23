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
#include "bmp.h"
static int lcd;
static int lcd_w,lcd_h,lcd_bpp;
static char *p;
struct fb_var_screeninfo vsinfo;
static bool first=true;
static bool slide_first=true;
static char *rgb;
static char *rgbf;

void displayslde(char *bmp,int x,int y)
{
	if(slide_first)
	{
		int fd = open(bmp,O_RDWR);
		struct bitmap_header head;
		struct bitmap_info   info;
		bzero(&head,sizeof(head));
		bzero(&info,sizeof(info));
		read(fd,&head,sizeof(head));
		read(fd,&info,sizeof(info));
		//总大小减去头部信息即为ｒｇｂ
		int rgb_size=head.size-sizeof(head)-sizeof(info);
		rgb=calloc(1,rgb_size);
		rgbf=rgb;
		int total=0;
		int n=0;
		while(rgb_size>0)
		{
			n=read(fd,rgb+total,rgb_size);
			rgb_size-=n;
			total+=n;
		}
		slide_first=false;
	}
	rgb=rgbf;
	char *tmp=p;
	tmp=tmp+97*4+4*416*800;
	char *tmp2=tmp;
	int skip=1;
	tmp=tmp+4*x;
	int32_t white=0x00FFFFFF;
	for(int i=0;i<89;i++)
	{
		bzero(tmp2,617);
		for(int j=0;j<617;j++)
		{
			memcpy(tmp2+4*j,&white,sizeof(white));
		}
		tmp2=tmp2-800*4;
	}
	for(int i=0;i<89;i++)
	{
		for(int j=0;j<58;j++)
		{
			memcpy(tmp+4*j,rgb+3*j,3);
		}
		rgb=rgb+3*89+skip;
		tmp=tmp-(lcd_bpp/8)*lcd_w;
	}
}

void displayfun(char *bmp,int x,int y)
{
	int fd = open(bmp,O_RDWR);
	//bmp的头部信息
	struct bitmap_header head;
	struct bitmap_info   info;
	bzero(&head,sizeof(head));
	bzero(&info,sizeof(info));
	read(fd,&head,sizeof(head));
	read(fd,&info,sizeof(info));
	//总大小减去头部信息即为ｒｇｂ
	int rgb_size=head.size-sizeof(head)-sizeof(info);
	char *rgb=calloc(1,rgb_size);
	char *rgbf=rgb;
	int total=0;
	int n=0;
	//计算无用字节，用于后面跳过
	int skip=(info.width*3)%4;
	switch(skip)
	{
		case 0:break;
		case 1:skip=3;break;
		case 2:skip=2;break;
		case 3:skip=1;break;
	}
	//读取ｒｇｂ数据
	while(rgb_size>0)
	{
		n=read(fd,rgb+total,rgb_size);
		rgb_size-=n;
		total+=n;
	}
	if(first)
	{
		lcd=open("/dev/fb0",O_RDWR);
		//获取设备的参数
		bzero(&vsinfo,sizeof(vsinfo));
		ioctl(lcd,FBIOGET_VSCREENINFO,&vsinfo);
		printf("屏幕分辨率：%d x %d\n",vsinfo.xres,vsinfo.yres);
		printf("像素点内部结构\n");
		printf("Ａ偏移量：%d\n",vsinfo.transp.offset);
		printf("Ｒ偏移量：%d\n",vsinfo.red.offset);
		printf("Ｇ偏移量：%d\n",vsinfo.green.offset);
		printf("Ｂ偏移量：%d\n",vsinfo.blue.offset);
		lcd_w=vsinfo.xres;//宽
		lcd_h=vsinfo.yres;//高
		lcd_bpp=vsinfo.bits_per_pixel;//色深

		p=mmap(NULL,lcd_w*lcd_h*lcd_bpp/8,PROT_READ | PROT_WRITE,MAP_SHARED,lcd,0);
		if(p==MAP_FAILED)
		{
			perror("映射内存失败");
			exit(0);
		}	

		first=false;
	}
	bzero(p,lcd_h*lcd_w*lcd_bpp/8); //特制
	char *tmp=p;
	char *tmp2=p+(y*800+x)*4;
	int w=(lcd_w-x)<info.width?(lcd_w-x):info.width;
	int h=(lcd_h-y)<info.height?(lcd_h-y):info.height;
	tmp2=tmp2+lcd_w*(h-1)*4;
	for(int i=0;i<(h-1);i++)
	{
		//ｂｍｐ只有ｒｇｂ，所以每次只取三位，ｐ要加４
		for(int j=0;j<w;j++)
		{
			memcpy(tmp2+4*j,rgb+3*j,3);
		}
		//不能被四整除的时候要跳过无用字节，避免读取
		if(skip!=0)
		{
			rgb=rgb+3*info.width+skip;
		}
		else
			rgb=rgb+3*info.width;
		tmp2=tmp2-(lcd_bpp/8)*lcd_w;
	}
	p=tmp;
	free(rgbf);
	close(fd);
	//munmap(p,lcd_w*lcd_h*lcd_bpp/8);
	//close(lcd);
}

void freelcd(void)
{
	if(!first)
	{
		munmap(p,lcd_w*lcd_h*lcd_bpp/8);
		close(lcd);
	}
	first=true;

}
