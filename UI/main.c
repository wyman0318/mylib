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
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <pthread.h>
#include "bmpfun.h"

int main(int argc, char **argv)
{
	int tp = open("/dev/input/event0",O_RDWR);
	struct input_event buf;

	int x_code=0;
	int y_code=0;
	bool xdone = false;
	bool ydone = true;
	bool press = false;
	bool style = false;
	bool x_first = true;
	bool y_first = true;

	displayfun("first.bmp",0,0);
	displayslde("slide.bmp",0,0);
	while(1)
	{
		bzero(&buf,sizeof(buf));
		read(tp,&buf,sizeof(buf));

		if(buf.type==EV_ABS)
		{
			if(buf.code==ABS_X&&ydone)
			{
				
				if(x_code!=0&&!x_first)
				{
					x_first=false;
				}
				   
				x_code=buf.value*800/1024;
				ydone=false;
				xdone=true;
			}
			if(buf.code==ABS_Y&&xdone)
			{
			
				if(y_code!=0&&!y_first)
				{
					y_first=false;
				}
				   
				y_code=buf.value*480/600;
				ydone=true;
				xdone=false;
			}
		}

		if((x_code>=97&&x_code<=186)&&(y_code>=343&&y_code<=416)&&press&&!style&&x_first&&y_first)
		{
			style=true;
		}


		if(buf.type==EV_KEY&&buf.code==BTN_TOUCH)
		{
			if(buf.value>0)
			{
				press=true;
			}
			if(buf.value==0)
			{
				x_code=0;
				y_code=0;		
				press=false;
				style=false;
				x_first=true;
				y_first=true;
				displayslde("slide.bmp",0,0);
				continue;
			}
		}

		if(style&&press)
		{
			if(x_code<=97)
			{
				displayslde("slide.bmp",0,0);
				continue;
			}
			if(x_code>=700)
			{
				displayslde("slide.bmp",617,0);
				printf("解锁！");
				return 0;
			}
			displayslde("slide.bmp",x_code,0);	
			continue;
		}

	}

	close(tp);
	return 0;
}
