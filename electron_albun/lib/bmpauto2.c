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

char *displayfun(char *bmp,int x,int y)
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
printf("%d\n",__LINE__);
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
	bzero(p,lcd_h*lcd_w*lcd_bpp/8);
	char *tmp=p;
	char *retinfo=p;
	char *tmp2=p+(y*800+x)*4;
	//居中处理
	int dele_w=info.width;
	int dele_h=info.height;
	int advan_w=0,advan2_w=0;
	int advan_h=0,advan2_h=0;
	bool add_w=false;
	bool sub_w=false;
	bool add_y=false;
	bool sub_y=false;
	if(lcd_w>dele_w)
	{
		dele_w=lcd_w/(lcd_w-info.width);
		advan_w=lcd_w%(lcd_w-info.width);
		if(advan_w!=0)
		{
			advan2_w=lcd_w%advan_w;
			if(advan2_w!=0)
			{
				advan2_w=lcd_w/advan2_w;
			}
			advan_w=lcd_w/advan_w;
			dele_w+=1;
		}
		add_w=true;
	}
	if(lcd_w<dele_w)
	{
		dele_w=lcd_w/(info.width-lcd_w);
		advan_w=lcd_w%(info.width-lcd_w);
		if(advan_w!=0)
		{
			advan2_w=lcd_w%advan_w;
			if(advan2_w!=0)
			{
				advan2_w=lcd_w/advan2_w;
			}
			advan_w=lcd_w/advan_w;
			dele_w+=1;
		}
		sub_w=true;
	}
	if(lcd_h>dele_h)
	{
		dele_h=lcd_h/(lcd_h-info.height);
		advan_h=lcd_h%(lcd_h-info.height);
		if(advan_h!=0)
		{
			advan2_h=lcd_h%advan_h;
			if(advan2_h!=0)
			{
				advan2_h=lcd_h/advan2_h;
			}
			advan_h=lcd_h/advan_h;
			dele_h+=1;
		}
		add_y=true;
	}
	if(lcd_h<dele_h)
	{
		dele_h=lcd_h/(info.height-lcd_h);
		advan_h=lcd_h%(info.height-lcd_h);
		if(advan_h!=0)
		{
			advan2_h=lcd_h%advan_h;
			if(advan2_h!=0)
			{
				advan2_h=lcd_h/advan2_h;
			}
			advan_h=lcd_h/advan_h;
			dele_h+=1;
		}
		sub_y=true;
	}

	//int w=(lcd_w-x)<info.width?(lcd_w-x):info.width;
	//int h=(lcd_h-y)<info.height?(lcd_h-y):info.height;
	int w=(lcd_w-x);
	int h=(lcd_h-y);
	tmp2=tmp2+lcd_w*(h-1)*4;
	for(int i=0,k=0;i<(h-1);i++,k++)
	{
		//ｂｍｐ只有ｒｇｂ，所以每次只取三位，ｐ要加４
		for(int j=0,z=0;j<w,z<info.width;j++,z++)
		{
			if((j%dele_w==0||j%advan_w==0||j%advan2_w==0)&&add_w)
			{
				z--;
			}
			if((j%dele_w==0||j%advan_w==0||j%advan2_w==0)&&sub_w)
			{
				z++;
			}
			memcpy(tmp2+4*j,rgb+3*z,3);
		}
		//不能被四整除的时候要跳过无用字节，避免读取
		tmp2=tmp2-(lcd_bpp/8)*lcd_w;
		if((k%dele_h==0||k%advan_h==0||k%advan2_h==0)&&add_y)
		{
			continue;
		}
		if((k%dele_h==0||k%advan_h==0||k%advan2_h==0)&&sub_y)
		{
			if(skip!=0)
			{
				rgb=rgb+3*info.width+skip;
			}
			else
				rgb=rgb+3*info.width;
		}
		if(skip!=0)
		{
			rgb=rgb+3*info.width+skip;
		}
		else
			rgb=rgb+3*info.width;
	}
	
	p=tmp;
	free(rgbf);
	close(fd);
	char *ret=calloc(1,480*800*4);
	memcpy(ret,retinfo,4*800*480);
	printf("%d\n",__LINE__);
	//测试
	printf("%d\n",__LINE__);
	return ret;
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
