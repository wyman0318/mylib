#include "mycamera.h"

mycamera::mycamera(QObject *parent) : QThread(parent)
{
    catch_sig=false;

    lcd = open("/dev/fb0",O_RDWR);

    //获取硬件参数
    ioctl(lcd,FBIOGET_VSCREENINFO,&lcdinfo);

    width = lcdinfo.xres;
    height= lcdinfo.yres;
    bpp   = lcdinfo.bits_per_pixel;

    //映射显存并清０
    size = width * height * bpp/8;
    fb_mem = (uint32_t *)mmap(NULL, size,PROT_READ|PROT_WRITE, MAP_SHARED, lcd, 0);
    bzero(fb_mem, size);

    //准备摄像头
    cam_fd = open("/dev/video0",O_RDWR);
    get_camcap (cam_fd);
    get_caminfo(cam_fd);
    get_camfmt (cam_fd);

    // 配置摄像头的采集格式为: JPEG
    set_camfmt(cam_fd, "JPEG");

    // 设置即将要申请的摄像头缓存的参数
    nbuf = 3;
    bzero(&reqbuf, sizeof (reqbuf));
    reqbuf.count  = nbuf;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    // 使用该参数reqbuf来申请缓存
    ioctl(cam_fd, VIDIOC_REQBUFS, &reqbuf);

    for(int i=0; i<nbuf; i++)
    {
        bzero(&buffer[i], sizeof(buffer[i]));
        buffer[i].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer[i].memory = V4L2_MEMORY_MMAP;
        buffer[i].index = i;
        ioctl(cam_fd, VIDIOC_QUERYBUF, &buffer[i]);

        // 将驱动中的缓存一一映射到用户空间
        length[i] = buffer[i].length;
        start_buf[i]  = (unsigned char *)mmap(NULL, buffer[i].length, PROT_READ|PROT_WRITE,
                  MAP_SHARED, cam_fd, buffer[i].m.offset);

        ioctl(cam_fd, VIDIOC_QBUF, &buffer[i]);
    }

    // 启动摄像头数据采集
    ioctl(cam_fd, VIDIOC_STREAMON, &vtype);

    bzero(&v4lbuf, sizeof(v4lbuf));
    v4lbuf.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4lbuf.memory= V4L2_MEMORY_MMAP;

}

void mycamera::run()
{
    int i = 0;
    uint8_t *info;
    while(1)
    {
        // 从队列中取出填满数据的缓存
        v4lbuf.index = i%nbuf;
        ioctl(cam_fd , VIDIOC_DQBUF, &v4lbuf);

        if(catch_sig)
        {
            catch_action(i);
            catch_sig=false;
        }

        // 解码、显示
        info = shooting(info_buf=(char *)start_buf[i%3], length[i%3], fb_mem,0,NULL,NULL);

        // 将已经读取过数据的缓存块重新置入队列中
        v4lbuf.index = i%nbuf;
        ioctl(cam_fd , VIDIOC_QBUF, &v4lbuf);

        i++;
    }
}

void mycamera::catch_action(int i)
{
        uint8_t *info;

        int jpg_width=640;
        int jpg_height=480;
        // 准备RGB数据
        info=shooting(info_buf=(char *)start_buf[i%3], length[i%3], fb_mem,1,
                &jpg_width,&jpg_height);

        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);


        // 准备图片文件
        char *jpgname = (char *)calloc(1, 20);
        snprintf(jpgname, 20, "%d.jpg", 138);

        FILE *fp = fopen(jpgname, "w");
        if(fp == NULL)
        {
            printf("创建文件[%s]失败:%s\n", jpgname, strerror(errno));
            return;
        }

        jpeg_stdio_dest(&cinfo, fp);

        cinfo.image_width  = jpg_width;
        cinfo.image_height = jpg_height;
        cinfo.input_components = 3;
        cinfo.in_color_space   = JCS_RGB;

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 80, TRUE);

        jpeg_start_compress(&cinfo, TRUE);

        int row_stride = cinfo.image_width*3;
        JSAMPROW row_pointer[1];

        // 开始编码，并将jpg数据存入指定的图片文件中
//        while(cinfo.next_scanline < cinfo.image_height)
        for(int i=0;i<cinfo.image_height;i++)
        {
            row_pointer[0] = info + i * cinfo.image_width * 3;
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }

        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);

        free(info);
        fclose(fp);
}



