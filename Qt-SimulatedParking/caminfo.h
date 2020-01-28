/////////////////////////////////////////////////////
//
//  Copyright(C), 2011-2019, GEC Tech. Co., Ltd.
//  File name: caminfo.h
//
//  Description: 摄像头图像处理功能函数
//  Author: 林世霖
//  微信公众号：秘籍酷
//
//  GitHub: github.com/vincent040   
//  Bug Report: 2437231462@qq.com
//
//////////////////////////////////////////////////////

#ifndef __CAMINFO_H
#define __CAMINFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include <linux/fb.h>
#include <linux/videodev2.h>
#include <linux/input.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <setjmp.h>

void get_caminfo(int camfd);
void get_camfmt(int camfd);
void get_camcap(int camfd);

void set_camfmt(int camfd, char *pixfmt);

#ifdef __cplusplus
}
#endif

#endif
