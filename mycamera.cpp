#include "mycamera.h"

mycamera::mycamera():width(320),height(240)
{
    //初始化摄像头
    this->cameraInit();
    lcdbuf = new int[width*height*4];
}
//析构首尾
mycamera::~mycamera()
{
    close(lcdfd);
    close(cameraid);
}
//初始化摄像头
void mycamera::cameraInit()
{
    //打开摄像头驱动
    cameraid = open("/dev/video7",O_RDWR);
    if(cameraid==-1)
    {
        perror("open camera err");
        return ;
    }
    //打开lcd显示屏
    lcdfd = open("/dev/fb0",O_RDWR);
    if(lcdfd==-1)
    {
        perror("open lcdfd err");
        return ;
    }
    //映射显示屏内存
    lcdmmap = (int *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
    if(lcdmmap==NULL)
    {
        perror("映射lcd失败!\n");
        return ;
    }

    //对驱动进行操作
    //设置采集格式
    bzero(&myfmt,sizeof(myfmt));
    myfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    myfmt.fmt.pix.width = width; //宽
    myfmt.fmt.pix.height = height;  //高
    myfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    ioctl(cameraid,VIDIOC_S_FMT,&myfmt);

    //申请缓存空间
    bzero(&mybuf,sizeof(mybuf));
    mybuf.count=4;
    mybuf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    mybuf.memory=V4L2_MEMORY_MMAP;
    ioctl(cameraid,VIDIOC_REQBUFS,&mybuf);

    //分配缓存空间
    struct v4l2_buffer newbuf;
    int ret;
    for(int i=0;i<4;i++)
    {
        bzero(&newbuf,sizeof(newbuf));
        //分配内存
        newbuf.index=i;
        newbuf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        newbuf.memory=V4L2_MEMORY_MMAP;
        ioctl(cameraid,VIDIOC_QUERYBUF,&newbuf);
        //映射得到地址和大小
        cachebuf[i].length = newbuf.length;
        cachebuf[i].addr=mmap(NULL,newbuf.length,PROT_READ|PROT_WRITE,MAP_SHARED,cameraid,newbuf.m.offset);

        //申请入队
        ret=ioctl(cameraid,VIDIOC_QBUF,&newbuf);
        if(ret==-1)
        {
            perror("入队失败!\n");
            return ;
        }
    }
    //开始采集
    mytype=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret=ioctl(cameraid,VIDIOC_STREAMON,&mytype);
    if(ret==-1)
    {
        perror("采集失败!\n");
        return ;
    }
}
//循环出队入队，，显示视频流显示视频流
void mycamera::discamera()
{
    for(int i=0;i<4;i++)
    {
        bzero(&newbuf,sizeof(newbuf));
        //分配内存
        newbuf.index=i;
        newbuf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        newbuf.memory=V4L2_MEMORY_MMAP;


        //先出队
        int ret=ioctl(cameraid,VIDIOC_DQBUF,&newbuf);
        if(ret==-1)
        {
            perror("入队失败!\n");
            return ;
        }
        //再入队
        ret=ioctl(cameraid,VIDIOC_QBUF,&newbuf);
        if(ret==-1)
        {
            perror("入队失败!\n");
            return ;
        }

        int n=0;
        for(int j=0;j<cachebuf[i].length;j+=4)
        {
            char *p=(char *)(cachebuf[i].addr);
            lcdbuf[n++]	= toRgb(p[j],p[j+1],p[j+3]);
            lcdbuf[n++]	= toRgb(p[j+2],p[j+1],p[j+3]);
        }
    }
    for(int i=0; i<height; i++)
        memcpy(lcdmmap+(i+60)*800+350,&lcdbuf[i*width],width*4);
}
//yuv编码格式转换成rgb编码格式
int mycamera::toRgb(int y,int u,int v)
{
    int r,g,b;
     r = 1164*(y-16)/1000 + 1596*(v-128)/1000;
     g = 1164*(y-16)/1000 + 813*(v-128)/1000 - 391*(u-128)/1000;
     b = 1164*(y-16)/1000 + 2018*(u-128)/1000;
     //    r = y + 1.4075*10000*(v - 128)/10000;

     //    g = y - 0.3455*10000*(u - 128)/10000 - 0.7169*10000*(v - 128)/10000;

     //    b = y + 1.779*10000*(u - 128)/10000;
    if(r>255)
        r=255;
    if(g>255)
        g=255;
    if(b>255)
        b=255;
    if(r<0)
        r=0;
    if(g<0)
        g=0;
    if(b<0)
        b=0;
    return 0x00<<24|r<<16|g<<8|b;
}
//拍照保存
void mycamera::Photoshop(char *jpegpath)
{
    //定义压缩结构体变量和处理错误的结构体变量
    struct jpeg_compress_struct  newjpeg;
    jpeg_create_compress(&newjpeg);   //创建压缩变量
    struct jpeg_error_mgr myerr;
    jpeg_std_error(&myerr);
    //设置参数
    newjpeg.image_width = width;
    newjpeg.image_height = height;
    newjpeg.input_components  = 3;
    newjpeg.in_color_space = JCS_RGB;
    jpeg_set_defaults(&newjpeg);  //设置参数

    //设定编码jpeg压缩质量 //有损压缩
    /*
        jpeg_set_quality(&jpeg, 整数, true);
        参数：第二个参数(压缩质量)-->数字越大，压缩质量越高 70---100之间

     */
    //boolean ok=true;
    jpeg_set_quality(&newjpeg, 85, TRUE);

    //新建一个空白的jpg文件，绑定输出
    char jpegpathbuf[256]={};
    sprintf(jpegpathbuf,"../pic/%s.jpg",jpegpath);
    FILE *infile = fopen(jpegpathbuf,"w+");
    if(infile==NULL)
    {
        printf("创建图片失败！");
        return ;
    }
    jpeg_stdio_dest(&newjpeg,infile);

    //定义数组
    unsigned char *jpegbuf=(unsigned char *)malloc(width*3);
    //开始压缩
    jpeg_start_compress(&newjpeg, TRUE);

    //写入压缩数据
    for(int i=0;i<height;i++)
    {
        memset(jpegbuf,0,width*3);
        jpeg_write_scanlines(&newjpeg,&jpegbuf,1);
        fwrite(jpegbuf,width,1,infile);
    }
    //压缩完毕
    jpeg_finish_compress(&newjpeg);
    //释放资源
    fclose(infile);
    jpeg_destroy_compress(&newjpeg);
}
