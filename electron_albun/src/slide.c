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
enum {up,down,left,right}; //就是上下左右分别对应0,1,2,3

int slide(void)
{
	int tp=open("/dev/input/event0",O_RDWR);
	struct input_event buf;
	
	bool xdone = false;
	bool ydone = true;
	bool press=false;
	bool xstart=false;
	bool ystart=false;
	int x_start,y_start,x_end,y_end;
	int x_fin,y_fin;
	while(1)
	{
		bzero(&buf,sizeof(buf));
		read(tp,&buf,sizeof(buf));
		
		if(buf.type==EV_KEY&&buf.code==BTN_TOUCH&&buf.value>0)
		{
			press=true;  //表示已经按下且未松开
		}
		
		if(buf.type==EV_ABS)	//判断此事件是不是绝对坐标事件
		{
			if(buf.code==ABS_X&&ydone) //如果已取得y得坐标值则取x得坐标值(开始时从x开始）
			{
				if(press&&!xstart)			//已经按下得状态判断是不是第一次取坐标值
				{
					x_start=buf.value;		//是得话更新第一次坐标值
					xstart=true;			//取得第一次坐标值后以后都不能进入此分支
				}
				x_end=buf.value;			//只要发生绝对坐标事件就更新x坐标值
				xdone=true;
				ydone=false;
			}
			if(buf.code==ABS_Y&&xdone)	//跟上面差不多，这里是y得
			{
				if(press&&!ystart)
				{
					y_start=buf.value;
					ystart=true;
				}
				y_end=buf.value;
				xdone=false;
				ydone=true;
			}
		}
		if(buf.type==EV_KEY&&buf.code==BTN_TOUCH&&buf.value==0)		//判断是否松开
		{
			x_fin=x_end-x_start;					//最后一个更新得坐标值减去初始值
			y_fin=y_end-y_start;					//就能判断是向哪里滑动
			if(abs(x_fin)>=25&&abs(x_fin)>abs(y_fin))//x滑动得距离大于y滑动得距离
			{											//进一步判断是左滑还是右滑
				if(x_fin>=25)
				{
					return right;
				}
				if(x_fin<=-25)
				{
					return left;
				}
			}
			else if(abs(y_fin)>=25&&abs(y_fin)>abs(x_fin))
			{
				if(y_fin>=25)
				{
					return down;
				}
				if(y_fin<=-25)
				{
					return up;
				}
			}
			else 
			{
				return 0;
			}
		}

	}
}
