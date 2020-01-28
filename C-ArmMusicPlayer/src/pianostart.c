#include "common.h"
#include "piano.h"

void audio_init(void)
{
	displayfun("bmp/white.bmp",0,0,1);
	displayfun("bmp/start.bmp",340,300,0);
	displayfun("bmp/prev.bmp",160,300,0);
	displayfun("bmp/next.bmp",520,300,0);
	displayfun("bmp/home.bmp",700,0,0);
}

void audiostop(void)
{
	displayfun("/IOT/wyman/program/piano/bmp/stop.bmp",340,300,0);
}

void audiostart(void)
{
	displayfun("/IOT/wyman/program/piano/bmp/start.bmp",340,300,0);
}

void soft_init(void)
{
	displayfun("bmp/soft_init.bmp",0,0,1);
}

int pianostart(void)
{
	displayfun("bmp/background.bmp",0,0,1);
	displayfun("bmp/home.bmp",700,0,0);
	displayfun("bmp/key_off.bmp",40,100,0); //60,65
	displayfun("bmp/key_off.bmp",105,100,0);
	displayfun("bmp/key_off.bmp",170,100,0);
	displayfun("bmp/key_off.bmp",235,100,0);
	displayfun("bmp/key_off.bmp",300,100,0);
	displayfun("bmp/key_off.bmp",365,100,0);
	displayfun("bmp/key_off.bmp",430,100,0);
	displayfun("bmp/key_off.bmp",495,100,0);
	displayfun("bmp/key_off.bmp",560,100,0);
	displayfun("bmp/key_off.bmp",625,100,0);
	displayfun("bmp/key_off.bmp",690,100,0);
	return 1;
}

void piano_on(int num)
{
	switch(num)
	{
		case 1:displayfun("bmp/key_on.bmp",40,100,0);break;
		case 2:displayfun("bmp/key_on.bmp",105,100,0);break;
		case 3:displayfun("bmp/key_on.bmp",170,100,0);break;
		case 4:displayfun("bmp/key_on.bmp",235,100,0);break;
		case 5:displayfun("bmp/key_on.bmp",300,100,0);break;
		case 6:displayfun("bmp/key_on.bmp",365,100,0);break;
		case 7:displayfun("bmp/key_on.bmp",430,100,0);break;
		case 8:displayfun("bmp/key_on.bmp",495,100,0);break;
		case 9:displayfun("bmp/key_on.bmp",560,100,0);break;
		case 10:displayfun("bmp/key_on.bmp",625,100,0);break;
		case 11:displayfun("bmp/key_on.bmp",690,100,0);break;
		default:break;
	}
}

void piano_off(int num)
{
	switch(num)
	{
		case 1:displayfun("bmp/key_off.bmp",40,100,0);break;
		case 2:displayfun("bmp/key_off.bmp",105,100,0);break;
		case 3:displayfun("bmp/key_off.bmp",170,100,0);break;
		case 4:displayfun("bmp/key_off.bmp",235,100,0);break;
		case 5:displayfun("bmp/key_off.bmp",300,100,0);break;
		case 6:displayfun("bmp/key_off.bmp",365,100,0);break;
		case 7:displayfun("bmp/key_off.bmp",430,100,0);break;
		case 8:displayfun("bmp/key_off.bmp",495,100,0);break;
		case 9:displayfun("bmp/key_off.bmp",560,100,0);break;
		case 10:displayfun("bmp/key_off.bmp",625,100,0);break;
		case 11:displayfun("bmp/key_off.bmp",690,100,0);break;
		default:break;
	}
}
