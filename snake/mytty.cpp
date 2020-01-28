#include "mytty.h"
#include "tty.h"

mytty::mytty(int fd) : fd2(fd)
{

}

void mytty::run()
{
    char buf[20];
    while(1)
    {
        bzero(buf,20);
        read(fd2,buf,20);
        if(strstr(buf,"up"))
        {
            emit blue_msg(1);
        }
        else if(strstr(buf,"down"))
        {
            emit blue_msg(2);
        }
        else if(strstr(buf,"left"))
        {
            emit blue_msg(3);
        }
        else if(strstr(buf,"right"))
        {
            emit blue_msg(4);
        }
        else if(strstr(buf,"menu"))
        {
            emit blue_msg(5);
        }
        else if(strstr(buf,"file"))
        {
            emit blue_msg(6);
        }
        else
        {
            QString str(buf);
            emit blue_msgc(str);
        }
    }
}
