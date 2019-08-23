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
#include "jpeglib.h"
#include <setjmp.h>
#include <sys/time.h>
#include "jconfig.h"
#include "jerror.h"
static int lcd;
static bool first=true;
static int lcd_w;
static int lcd_h;
static int lcd_bpp;
static char *lcdmem;

void displayfun(char *jpg,int x,int y)
{
	int fd =open(jpg,O_RDONLY);
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	//	jpgbuffer

	struct stat info;
	bzero(&info,sizeof(info));
	stat(jpg,&info);
	int size=info.st_size;
	
	char *jpg_buffer=calloc(1,size);
	int n=0;
	while(size>0)
	{
		n=read(fd,jpg_buffer+n,size);
		size=size-n;
	}
	cinfo.err=jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	size=info.st_size;
	jpeg_mem_src(&cinfo,jpg_buffer,size);

	int ret = jpeg_read_header(&cinfo,true);
	if(ret!=1)
	{
		fprintf(stderr,"[%d]:jpeg_read_header failed:""%s\n",__LINE__,strerror(errno));
		exit(1);
	}

	jpeg_start_decompress(&cinfo);
	//每一行字节数
	int row_stride=cinfo.output_width * cinfo.output_components;

	unsigned char *rgb_buffer=calloc(1,row_stride*cinfo.output_height);
	char *rgbf=rgb_buffer;

	int line=0;
	while(cinfo.output_scanline<cinfo.output_height)
	{
		unsigned char *buffer_array[1];
		buffer_array[0]=rgb_buffer+(cinfo.output_scanline)*row_stride;
		jpeg_read_scanlines(&cinfo,buffer_array,1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(jpg_buffer);

	int jpg_width=cinfo.output_width;//图宽
	int jpg_height=cinfo.output_height;//图高
	int jpg_ipp=cinfo.output_components;//一个像素字节数
	printf("色深字节数：%d\n",jpg_ipp);

	struct fb_var_screeninfo vsinfo;
	bzero(&vsinfo,sizeof(vsinfo));
	if(first)
	{
		lcd=open("/dev/fb0",O_RDWR);
		ioctl(lcd,FBIOGET_VSCREENINFO,&vsinfo);
		lcd_w=vsinfo.xres;
		lcd_h=vsinfo.yres;
		lcd_bpp=vsinfo.bits_per_pixel;
		lcdmem=mmap(NULL,lcd_w*lcd_h*lcd_bpp/8,PROT_READ | PROT_WRITE,MAP_SHARED,lcd,0);
		if(lcdmem==MAP_FAILED)
		{
			perror("映射内存失败");
			exit(0);
		}
		first=false;
	}
	char *tmp=lcdmem+(y*lcd_w*lcd_bpp/8+x*lcd_bpp/8);

	//居中处理
	int dele_w=jpg_width;
	int dele_h=jpg_height;
	int advan_w=0,advan2_w=0;
	int advan_h=0,advan2_h=0;
	bool add_w=false;
	bool sub_w=false;
	bool add_y=false;
	bool sub_y=false;
	if(lcd_w>dele_w)
	{
		dele_w=lcd_w/(lcd_w-jpg_width);
		advan_w=lcd_w%(lcd_w-jpg_width);
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
		dele_w=lcd_w/(jpg_width-lcd_w);
		advan_w=lcd_w%(jpg_width-lcd_w);
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
		dele_h=lcd_h/(lcd_h-jpg_height);
		advan_h=lcd_h%(lcd_h-jpg_height);
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
		dele_h=lcd_h/(jpg_height-lcd_h);
		advan_h=lcd_h%(jpg_height-lcd_h);
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
	
	int w=lcd_w-x;
	int h=lcd_h-y;
	for(int i=0,k=0;i<h;i++,k++)
	{
		for(int j=0,z=0;j<w;j++,z++)
		{
			if((j%dele_w==0||j%advan_w==0||j%advan2_w==0)&&add_w)
			{
				z--;
			}
			if((j%dele_w==0||j%advan_w==0||j%advan2_w==0)&&sub_w)
			{
				z++;
			}
			//memcpy(tmp+i*lcd_w*4+j*4,rgb_buffer+i*row_stride+j*jpg_ipp,jpg_ipp);
			//memcpy(tmp+i*lcd_w*4+j*4+1,rgb_buffer+i*row_stride+j*3,3);
			memcpy(tmp+(lcd_bpp/8)*j+vsinfo.red.offset/8,rgbf+3*z+0,1);
			memcpy(tmp+(lcd_bpp/8)*j+vsinfo.green.offset/8,rgbf+3*z+1,1);
			memcpy(tmp+(lcd_bpp/8)*j+vsinfo.blue.offset/8,rgbf+3*z+2,1);
		}
		tmp+=lcd_w*(lcd_bpp/8);
		if((k%dele_h==0||k%advan_h==0||k%advan2_h==0)&&add_y)
		{
			continue;
		}
		if((k%dele_h==0||k%advan_h==0||k%advan2_h==0)&&sub_y)
		{
			rgbf+=row_stride;
		}
			rgbf+=row_stride;
	}
	free(rgb_buffer);
	close(fd);
}

void freelcd(void)
{
	if(!first)
	{
		munmap(lcdmem,lcd_w*lcd_h*lcd_bpp/8);
		close(lcd);
	}
	first=true;
}
