#include "common.h"
#include "piano.h"
#include "button.h"

#define PIANO 1
#define AUDIO 2

int main(int argc,char **argv)
{
	int ret = 0;
	int enter;
	while(1)
	{
		//防抖
		usleep(200*1000);
		
		//保证杀死后台播放器
		system("killall madplay");
		ret = 0;
		
		//使工作目录转移到目标文件夹下
		chdir("/IOT/wyman/program/piano/");
		
		//初始界面，选择钢琴和播放器
		enter =button_init(); 
		if(enter == PIANO)
		{
			pianostart();
			ret = buttonfun(0);
			continue;
		}

		else if(enter == AUDIO)
		{
			audio_init();
			ret = audio_fun();
			continue;
		}
	}
	freelcd;
}
