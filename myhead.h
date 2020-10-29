#ifndef MYHEAD_H_
#define MYHEAD_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <dirent.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//v4l2摄像头头文件
#include <linux/videodev2.h>

//串口
#include <termios.h>

#include "videodev2.h"
//jpeg库源
#ifdef __cplusplus
extern "C" {
#endif

#include "jpeglib.h"

#ifdef __cplusplus
}
#endif
#endif
