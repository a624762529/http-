#pragma once
#include"head.h"

class ep_info;
extern ep_info info_arry[MAX_ARRY+1];
int main ()
{

    chdir("/home/a624762529/Desktop/file_source");
    int port=8888;
    int epfd=epoll_create(MAX_ARRY+1);
    int sock_fd=init_listen_info(port,epfd);
    epoll_event ev_arry[MAX_ARRY+1];

    while (true) {
        int ret=epoll_wait(epfd,ev_arry,MAX_ARRY,-1);
        if(ret==-1)
        {
            perror("wait error");
            exit(0);
        }

        for(int a=0;a<ret;a++)
        {
            ep_info* fd_con_info=(ep_info*)ev_arry[a].data.ptr;
            int fd_con=fd_con_info->fd;
            fp_ptr call_back=fd_con_info->pt;
            int fd_event=ev_arry[a].events;

            if(fd_con==sock_fd)
            {
                int index=find_dex();
                do_accept(fd_con,epfd,EPOLLIN,&info_arry[index]);
            }
            else
            {
                if(fd_event==EPOLLIN)
                {
                    //void do_accept(int cfd,,int epfd,int event,void *arg)
                    int cfd=fd_con;
                    call_back(cfd,epfd,fd_event,fd_con_info);
                }
                else if(fd_event==EPOLLOUT)
                {
                    int cfd=fd_con;
                    call_back(cfd,epfd,fd_event,fd_con_info);
                }
            }
        }
    }
    close(sock_fd);

    return 1;
}
