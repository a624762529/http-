#ifndef EPOLL_LOOP__H
#define EPOLL_LOOP__H

#include<iostream>
#include"head.h"
#include"http.h"
using namespace std;
#define MAX_ARRY  1024
#define MAX_SIZE   1024
#define SIZE            1024

typedef void (*fp_ptr)(int cfd,int epfd,int event,void *arg);
class ep_info
{
public:
    int fd;        //当前文件描述符
    char buf[SIZE];//buff缓冲区
    int len;       //可写的缓冲区大小
    int stata;     //是否在红黑数上
    fp_ptr pt;     //执行函数
    long long now_time;//最近一次通讯时间
    void *ptr;//当前ptr
public:
    ep_info(){
        this->fd=-1;
        this->len=0;
        this->pt=NULL;
        this->stata=-1;
        this->ptr=NULL;
        memset(buf,sizeof(buf),0);
    }
    void fill_info(int fd,int stata,fp_ptr pt,long long time,void *ptr) ;
};


extern ep_info info_arry[MAX_ARRY+1];

int  get_socket(int port);
void close_fd(int epfd,int cfd);
void do_read_(int cfd_,int epfd,int event,void *arg);
void do_write_(int cfd_,int epfd,int event,void *arg);
void do_accept(int cfd_,int epfd,int event,void *arg);
int  init_listen_info(int  port,int ep_fd);//创建通讯文件描述符  并将至挂在epoll树上
int  find_dex();
void do_read(int cfd, int epfd);
void disconnect      (int cfd, int epfd);
void http_request(const char* request, int cfd);

#endif // EPOLL_LOOP__H
