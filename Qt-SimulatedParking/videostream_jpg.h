#ifndef VIDEOSTREAM_JPG_H
#define VIDEOSTREAM_JPG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include <linux/fb.h>
#include <linux/videodev2.h>
#include <linux/input.h>
#include <sys/ioctl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <setjmp.h>
#include <stdint.h>

#include "jpeglib.h"
#include "caminfo.h"

typedef struct abcd
{
    int jpg_width;
    int jpg_height;
}get_data;

uint8_t *shooting(char * jpegdata, int size, uint32_t *fb_mem,int mode,int *ret_width,int *ret_height);
//void camera_init();

#ifdef __cplusplus
}
#endif
#endif // VIDEOSTREAM_JPG_H
