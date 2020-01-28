#include "videostream_jpg.h"

uint8_t *shooting(char * jpegdata, int size, uint32_t *fb_mem,int mode,int *ret_width,int *ret_height)
{
    // 声明解压缩结构体，以及错误管理结构体
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // 使用缺省的出错处理来初始化解压缩结构体
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // 配置该cinfo，使其从jpg_buffer中读取jpg_size个字节
    // 这些数据必须是完整的JPEG数据
    jpeg_mem_src(&cinfo, jpegdata, size);

    // 读取JPEG文件的头
    jpeg_read_header(&cinfo, true);

    // 开始解压jpeg数据
    jpeg_start_decompress(&cinfo);

    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int pixel_size = cinfo.output_components;
    int row_stride = width * pixel_size;

    // 定义一个存放一行数据的缓冲区
    char * buffer;
    buffer = malloc(row_stride);

    // 以行为单位，将JPEG数据解压出来之后，按照RGBA方式填入fb_mem
    int red  = 0;
    int green= 1;
    int blue = 2;

//    修改部分
    if(mode)
    {
        uint8_t *ret_mem=calloc(1,3*width*height);
        *ret_width=cinfo.output_width;
        *ret_height=cinfo.output_height;

        while (cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&buffer, 1);

            // 将buffer中的24bit的图像数据写入32bit的fb_mem
            int i=0, x;
            for(x=0; x<cinfo.output_width; x++)
            {
                *(ret_mem+(cinfo.output_scanline-1)*cinfo.output_width*3+x*3+2) = buffer[i+blue];
                *(ret_mem+(cinfo.output_scanline-1)*cinfo.output_width*3+x*3+1) = buffer[i+green];
                *(ret_mem+(cinfo.output_scanline-1)*cinfo.output_width*3+x*3) = buffer[i+red];
                i += 3;
            }
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);

        return ret_mem;
    }

    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&buffer, 1);

        // 将buffer中的24bit的图像数据写入32bit的fb_mem
        int i=0, x;
        for(x=0; x<cinfo.output_width; x++)
        {
            *(fb_mem+(cinfo.output_scanline-1)*800+x) =
                    buffer[i+blue] << 0|
                    buffer[i+green]<< 8|
                    buffer[i+red]  <<16;
            i += 3;
        }
    }

    // 释放相关结构体和内存，解压后的图片信息被保留在fb_mem，直接映射到了LCD
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 0;
}

//void camera_init()
//{
//	// ********************* 准备LCD设备 ********************** //
//	int lcd = open("/dev/fb0", O_RDWR);

//	// 获取硬件参数
//	struct fb_var_screeninfo lcdinfo;
//	ioctl(lcd, FBIOGET_VSCREENINFO, &lcdinfo);

//	int width = lcdinfo.xres;
//	int height= lcdinfo.yres;
//	int bpp   = lcdinfo.bits_per_pixel;

//        // 映射显存并清零，得到显存的入口fb_mem
//	uint32_t size = width * height * bpp/8;
//	uint32_t *fb_mem = mmap(NULL, size,
//			PROT_READ|PROT_WRITE, MAP_SHARED, lcd, 0);
//	bzero(fb_mem, size);


//	// ******************* 准备摄像头设备 ********************* //
	
//	// 打开摄像头设备文件并获取其相关信息
//    int cam_fd = open("/dev/video0",O_RDWR);
//	get_camcap (cam_fd);
//	get_caminfo(cam_fd);
//	get_camfmt (cam_fd);

//	// 配置摄像头的采集格式为: JPEG
//        set_camfmt(cam_fd, "JPEG");


//	// 设置即将要申请的摄像头缓存的参数
//	int nbuf = 3;

//	struct v4l2_requestbuffers reqbuf;
//	bzero(&reqbuf, sizeof (reqbuf));
//	reqbuf.count  = nbuf;
//	reqbuf.memory = V4L2_MEMORY_MMAP;
//	reqbuf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;

//	// 使用该参数reqbuf来申请缓存
//	ioctl(cam_fd, VIDIOC_REQBUFS, &reqbuf);

//	// 根据刚刚设置的reqbuf.count的值，来定义相应数量的struct v4l2_buffer
//	// 每一个struct v4l2_buffer对应内核摄像头驱动中的一个缓存
//	struct v4l2_buffer buffer[nbuf];
//	int length[nbuf];
//	unsigned char *start[nbuf];

//	for(int i=0; i<nbuf; i++)
//	{
//		bzero(&buffer[i], sizeof(buffer[i]));
//		buffer[i].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//		buffer[i].memory = V4L2_MEMORY_MMAP;
//		buffer[i].index = i;
//		ioctl(cam_fd, VIDIOC_QUERYBUF, &buffer[i]);

//		// 将驱动中的缓存一一映射到用户空间
//		length[i] = buffer[i].length;
//		start[i]  = mmap(NULL, buffer[i].length, PROT_READ|PROT_WRITE,
//				  MAP_SHARED, cam_fd, buffer[i].m.offset);

//		ioctl(cam_fd, VIDIOC_QBUF, &buffer[i]);
//	}

//	// 启动摄像头数据采集
//	enum v4l2_buf_type vtype= V4L2_BUF_TYPE_VIDEO_CAPTURE;
//	ioctl(cam_fd, VIDIOC_STREAMON, &vtype);

//	struct v4l2_buffer v4lbuf;
//	bzero(&v4lbuf, sizeof(v4lbuf));
//	v4lbuf.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//	v4lbuf.memory= V4L2_MEMORY_MMAP;

//	int i = 0;
//	while(1)
//	{
//		// 从队列中取出填满数据的缓存
//		v4lbuf.index = i%nbuf;
//		ioctl(cam_fd , VIDIOC_DQBUF, &v4lbuf);

//		// 解码、显示
//		shooting(start[i%nbuf], length[i%nbuf], fb_mem);

//	 	// 将已经读取过数据的缓存块重新置入队列中
//		v4lbuf.index = i%nbuf;
//		ioctl(cam_fd , VIDIOC_QBUF, &v4lbuf);

//		i++;
//	}

//	return 0;
//}
