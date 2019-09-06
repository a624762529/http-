#include "epoll_loop_.h"

#include"http.h"


void ep_info::fill_info(int fd, int stata, fp_ptr pt, long long time, void *ptr)
{
    this->fd=fd;
    this->stata=stata;
    this->pt=pt;
    this->ptr=ptr;
    this->now_time=time;
}
ep_info info_arry[MAX_ARRY+1];
int get_socket(int port)
{
    int fd=socket(AF_INET,SOCK_STREAM,0);
    if(fd==-1)
    {
        perror("socket error");
        exit(0);
    }
    sockaddr_in ser;
    memset(&ser,0,sizeof(ser));
    ser.sin_addr.s_addr=htonl(INADDR_ANY);
    ser.sin_family=AF_INET;
    ser.sin_port=htons(port);
    socklen_t len=sizeof(ser);

    int opt=1;
    setsockopt(fd,SOL_SOCKET,
               SO_REUSEADDR|SO_REUSEPORT,
               &opt,sizeof(int));
    bind(fd,(sockaddr*)(&ser),len);
    listen(fd,len);
    return fd;
}

void close_fd(int epfd,int cfd)
{
    cout<<"关闭连接"<<endl;
    epoll_ctl(epfd,EPOLL_CTL_DEL,cfd,NULL);
    close(cfd);
}

void do_read_(int cfd_,int epfd,int event,void *arg)
{

    int cfd=cfd_;
    ep_info *ep=(ep_info *)(arg);
    char       *bf=ep->buf;

    int ret=read(cfd,bf,SIZE);
    if(ret>0)
    {

        ep->len=ret;
        if(ret==-1)
        {
            perror("read error");
            exit(0);
        }
        ep->pt=do_write_;

        epoll_event ev;
        ev.events=EPOLLOUT;
        ev.data.ptr=ep;

        epoll_ctl(epfd,EPOLL_CTL_MOD,cfd,&ev);
    }
    else if(ret==0)
    {
        close_fd(epfd,cfd);
    }
    else
    {
        perror("read file");
        exit(0);
    }
}

void do_write_(int cfd_,int epfd,int event,void *arg)
{

    int cfd=cfd_;
    ep_info *ep=(ep_info *)(arg);
    char       *bf=ep->buf;
    int            len=ep->len;
    char         buf_temp[1024]{0};
    //解析buf中的数据

    //第一步 去除空格
    remove_rn(bf,len);
    //第二部 提取出  GET  路径 以及相应的http协议类型
    //void trit_source(char *p,char *buf1,char *path,char *proth);
    char buf_p[10],path_[1024],proth[20];
    memset(path_,0,sizeof(path_));
    trit_source(bf,buf_p,path_,proth);
    char *path=path_+1;
    //判断是否是GET方法 post直接丢弃
    if(strcmp(buf_p,"GET")==0)
    {
        //进行关于get方法的处理
        char buf_path[1024]{0};
        sprintf(buf_path,"%s/%s",getcwd(buf_temp,SIZE),path);
        struct stat st;
        int ret=stat(buf_path,&st);
        if(ret==-1)
        {
            //文件不存在 404
            ep->stata=-1;
            close_fd(epfd,cfd);
            return ;
        }
        if(S_ISREG(st.st_mode))
        {
            char *p="是普通文件";

            send_respond_head(cfd, 200, "OK", get_file_type(path), -1);
           send_file(cfd,buf_path);
        }
        else if(S_ISDIR(st.st_mode))
        {
            char *p="是文件夹";
            cout<<p<<endl;
            send_respond_head(cfd, 200, "OK", get_file_type(".html"), -1);
            send_dir(cfd,buf_path);

        }

    }
    else
    {
        //反馈404
    }

    memset(bf,0,sizeof(bf));

    ep->stata=-1;
    close_fd(epfd,cfd);
}

void do_accept(int cfd_,int epfd,int event,void *arg)
{
    sockaddr_in cli;
    socklen_t len=sizeof(cli);

    int cfd=accept(cfd_,(sockaddr*)(&cli),&len);
    char ip[64];
    printf("new connect [%s:%d],pos[%d]\n",
           inet_ntoa(cli.sin_addr), ntohs(cli.sin_port) );

    ep_info *ep=(ep_info*)(arg);
    ep->fill_info(cfd,1,do_read_,time(NULL),arg);

    epoll_event ev;
    ev.events=event;
    ev.data.ptr=ep;
    epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
    return;
}

int init_listen_info(int  port,int ep_fd)
{
    int socket_fd=get_socket(port);

    ep_info *ptr_info_acc=&info_arry[MAX_ARRY];
    ptr_info_acc->fill_info(socket_fd,1,do_accept,time(NULL),ptr_info_acc);

    epoll_event ev;
    ev.data.ptr=ptr_info_acc;
    ev.events=EPOLLIN;
    epoll_ctl(ep_fd,EPOLLIN,socket_fd,&ev);

    return socket_fd;
}

int find_dex()
{
    for(int a=0;a<MAX_ARRY+1;a++)
    {
        if(info_arry[a].stata==-1)
        {
            info_arry[a].stata=1;
            return a;
        }
    }
    return MAX_ARRY;
}
