#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QObject>
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

#include "jpeglib.h"
#include "caminfo.h"
#include "videostream_jpg.h"
#include "stdint.h"

#include <QThread>
#include <QDebug>


class mycamera : public QThread
{
    Q_OBJECT
public:
    explicit mycamera(QObject *parent = 0);
    void run() override;
    void catch_action(int);
    bool catch_sig;

signals:

private:
    int lcd;
    struct fb_var_screeninfo lcdinfo;
    int width;
    int height;
    int bpp;
    uint32_t size;
    uint32_t *fb_mem;
    int cam_fd;
    int nbuf=3;
    struct v4l2_requestbuffers reqbuf;
    struct v4l2_buffer buffer[3];
    int length[3];
    unsigned char *start_buf[3];
    char *info_buf;
    enum v4l2_buf_type vtype= V4L2_BUF_TYPE_VIDEO_CAPTURE;
    struct v4l2_buffer v4lbuf;
};

#endif // MYCAMERA_H
