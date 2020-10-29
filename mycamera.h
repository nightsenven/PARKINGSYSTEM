#ifndef MYCAMERA_H
#define MYCAMERA_H
#include "myhead.h"


//定义一个结构体保存缓存地址和大小
struct Cachebuf
{
    void *addr;
    long length;
};
class mycamera
{
public:
    mycamera();
    ~mycamera();
    //初始化
    void cameraInit();
    //开始采集显示视屏流
    void discamera();
    int toRgb(int y, int u, int v);
    //拍照
    void Photoshop(char *jpegpath);
private:
    int cameraid;
    int lcdfd;
    int *lcdmmap;
    enum v4l2_buf_type mytype;
    //定义数组存放转换得到的一帧完整的ARGB数据
    int *lcdbuf;
    struct v4l2_requestbuffers mybuf;
    struct v4l2_buffer newbuf;
    struct v4l2_format myfmt;
    Cachebuf cachebuf[4];
    const int width;
    const int height;
};

#endif // MYCAMERA_H
