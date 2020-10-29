#include "myuart.h"

myuart::myuart(QObject *parent) : QThread(parent)
{
    //打开串口
    fd=open_uart();
    //串口初始化
    init_uart(fd);
    //超时时间
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    flag=0;
}
//关闭串口
myuart::~myuart()
{
    close(fd);
}

//打开串口
int myuart::open_uart()
{
    int fd;
    //打开串口的驱动
    fd=open("/dev/ttySAC1",O_NOCTTY|O_NONBLOCK|O_RDWR);

    return fd;
}
//串口初始化
void myuart::init_uart(int fd)
{
    struct termios myios;
    //保存现有串口参数设置
    tcgetattr(fd, &myios);

    //设置串口工作在原始模式--》串口只用于数据的收发，不作为其它功能使用
    myios.c_cflag |= CLOCAL | CREAD;
    cfmakeraw(&myios);

    //设置波特率
    cfsetispeed(&myios, B9600);
    cfsetospeed(&myios, B9600);

    //8位数据位
    myios.c_cflag &= ~CSIZE;
    myios.c_cflag |= CS8;  //CS7    CS6  CS5
    //无奇偶校验
    myios.c_cflag &= ~PARENB;
    //1位停止位
    myios.c_cflag &= ~CSTOPB;

    //刷新串口缓冲区
    myios.c_cc[VTIME] = 0;
    myios.c_cc[VMIN] = 1;
    tcflush(fd, TCIOFLUSH);
    //串口设置使能(让前面六步的配置生效)
    tcsetattr(fd,TCSANOW,&myios);
}
//发送A命令
int myuart::sendARequest(int fd)
{
    unsigned char WBuf[128], RBuf[128];
    //清空
    bzero(WBuf,sizeof(WBuf));
    bzero(RBuf,sizeof(RBuf));

    WBuf[0]=0x07;    //帧长
    WBuf[1]=0x02;    //命令类型
    WBuf[2]=0x41;   //A命令
    WBuf[3]=0x01;   //信息长度
    WBuf[4]=0x52;    //请求模式 ALL
    WBuf[5]=CountBCC(WBuf,WBuf[0]-2);        //检验和
    WBuf[6]=0x03;    //结束符

    fd_set myset;
    int count=0; //请求读写次数
    while(1)
    {
        //添加监听描述符
        FD_ZERO(&myset);
        FD_SET(fd,&myset);
        //刷新缓冲区
        tcflush(fd, TCIOFLUSH);

        //发送数据
        write(fd,WBuf,7);


        //监听串口 ,请求接收从机信息
        int ret = select(fd + 1,&myset, NULL,NULL,&timeout);
        if(ret>0)
        {
            usleep(10000);  //微秒
            //读取信息
            ret = read(fd, RBuf, 8);
            if(ret < 0)
            {
                printf("len = %d, %m\n", ret, errno);
                continue;
            }
            //printf("RBuf[2]:%x\n", RBuf[2]);
            if(RBuf[2] == 0x00)	 	//应答帧状态部分为0 则请求成功
            {
                return 0;
            }
        }
        else if(ret==0)  //超时
        {
            count++;   //3次请求超时后，退出该函数
            if(count == 3)
            {
                count=0;
                return -1;
            }
        }
        else
        {
            perror("select error");
            return -1;
        }
    }

    return 0;
}
//发送B命令 --防碰撞
int myuart::sendBRequest(int fd)
{
    unsigned char WBuf[128], RBuf[128];
    //清空
    bzero(WBuf,sizeof(WBuf));
    memset(RBuf,1,128);;

    WBuf[0]=0x08;    //帧长
    WBuf[1]=0x02;    //命令类型
    WBuf[2]=0x42;   //B命令
    WBuf[3]=0x02;   //信息长度
    WBuf[4]=0x93;	//防碰撞0x93  第一级防碰撞
    WBuf[5]=0x00;	//位计数0
    WBuf[6]=CountBCC(WBuf,WBuf[0]-2);        //检验和
    WBuf[7]=0x03;    //结束符

    fd_set myset;
    int count=0; //请求读写次数

    while(1)
    {
        //添加监听描述符
        FD_ZERO(&myset);
        FD_SET(fd,&myset);
        //刷新缓冲区
        tcflush(fd, TCIOFLUSH);

        //发送数据
        write(fd,WBuf,8);

        //监听串口 ,请求接收从机信息
        int ret = select(fd + 1,&myset, NULL,NULL,&timeout);
        if(ret>0)
        {
            usleep(10000);  //微秒
            //读取信息
            ret = read(fd, RBuf, 8);
            if(ret < 0)
            {
                printf("len = %d, %m\n", ret, errno);  //打印错误信息
                continue;
            }
            //printf("RBuf[2]:%x\n", RBuf[2]);
            if(RBuf[2] == 0x00)	 	//应答帧状态部分为0 则请求成功
            {
                //获取卡号信息
                cardid = (RBuf[7]<<24) | (RBuf[6]<<16) | (RBuf[5]<<8) | RBuf[4];
                return 0;
            }
        }
        else if(ret==0)  //超时
        {
            count++;   //3次请求超时后，退出该函数
            if(count == 3)
            {
                count=0;
                return -1;
            }
        }
        else
        {
            perror("select error");
            return -1;
        }
    }

    return 0;
}
//校验和
unsigned char myuart::CountBCC(unsigned char *buf, int n)
{
    unsigned char bcc=0;
    for(int i=0;i<n;i++)
        bcc ^= *(buf+i);
    return ~(bcc);
}
//线程任务函数
void myuart::run()
{
    //读写操作
    while(1)
    {
        //发送A命令
        int ret = sendARequest(fd);
        if(ret==-1)   //请求超时 重新打开并发送
        {
            usleep(500000);
            close(fd);
            //打开串口文件
            fd = open_uart();
            /*初始化串口*/
            init_uart(fd);
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            continue;
        }
        else if(ret==-2)
        {
            perror("select err");
            return ;
        }
        else   //成功
        {
            printf("发送成功，继续！");
            //发送B命令 和获取卡号
            ret = sendBRequest(fd);

            //若获取的cardid为0，或获取id超时，则需重新发送'A'命令
            if(cardid == 0 || ret == -1)
                continue;
            else if(ret == 0)
            {
                flag=1;
                printf("card ID = %x\n", cardid);  //打印cardid号
                usleep(500000);
                //break;
            }
        }
    }
}
