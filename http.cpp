#pragma once
#include"http.h"

#include<string.h>
void trit_source(char *p,char *buf1,char *path,char *proth)
{
    decode_str(path, path);
    sscanf(p,"%[^ ] %[^ ] %[^ ]",buf1,path,proth);
    return ;
}

void remove_rn(char *buf,int len)
{
    int a=0;
    while (a<len) {
        if(buf[a]=='\r'||buf[a]=='\n')
            buf[a]=' ';
        a++;
    }
    return;
}


void send_dir(int cfd, const char* dirname)
{
    // 拼一个html页面<table></table>
    char buf[4094] = {0};

    sprintf(buf, "<html><head><title>目录名: %s</title></head>", dirname);
    sprintf(buf+strlen(buf), "<body><h1>当前目录: %s</h1><table>", dirname);

    decode_str(buf,buf);
    write(cfd,buf,strlen(buf));
    memset(buf,0,sizeof(buf));
    // 打开目录
    DIR* dir = opendir(dirname);
    if(dir == NULL)
    {
        perror("opendir error");
        exit(1);
    }

    // 读目录
    char path_[1024]{0};

    struct dirent* ptr = NULL;
    while( (ptr = readdir(dir)) != NULL )
    {
        char* name = ptr->d_name;
        if(strcmp(name,".")==0||strcmp(name,"..")==0)
            continue;
        sprintf(path_,"%s/%s",getcwd(buf,4096),name);;
        cout<<name<<endl;
        struct stat st;
        stat(path_,&st);
        if(S_ISREG(st.st_mode))
        {
            //<a href=file://images/1.jpg>图片</a>
            sprintf(buf ,
                    "<h1><a href=%s>%s </a></h1>",
                    name,name );
        }
        // 如果是目录
        else if(S_ISDIR(st.st_mode))
        {

        }
        send(cfd, buf, strlen(buf), 0);

        memset(buf, 0, sizeof(buf));

    }
    closedir(dir);

}
void send_respond_head(int cfd, int no, const char* desp, const char* type, long len)
{
    char buf[1024] = {0};
    // 状态行

    sprintf(buf, "http/1.1 %d %s\r\n", no, desp);
    send(cfd, buf, strlen(buf), 0);
    // 消息报头
    sprintf(buf, "Content-Type:%s\r\n", type);
    sprintf(buf+strlen(buf), "Content-Length:%ld\r\n", len);
    send(cfd, buf, strlen(buf), 0);
    // 空行
    send(cfd, "\r\n", 2, 0);
}
void send_file(int cfd, const char* filename)
{
    // 打开文件
    int fd = open(filename, O_RDONLY);
    if(fd == -1)
    {
        // show 404
        return;
    }

    // 循环读文件
    char buf[4096] = {0};
    int len = 0;
    while( (len = read(fd, buf, sizeof(buf))) > 0 )
    {
        // 发送读出的数据
        decode_str(buf,buf);
        send(cfd, buf, len, 0);
    }
    if(len == -1)
    {
        perror("read file error");
        exit(1);
    }
close(cfd);
}

// 解析http请求消息的每一行内容
int get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;
    while ((i < size - 1) && (c != '\n'))
    {
        n = recv(sock, &c, 1, 0);
        if (n > 0)
        {
            if (c == '\r')
            {
                n = recv(sock, &c, 1, MSG_PEEK);
                if ((n > 0) && (c == '\n'))
                {
                    recv(sock, &c, 1, 0);
                }
                else
                {
                    c = '\n';
                }
            }
            buf[i] = c;
            i++;
        }
        else
        {
            c = '\n';
        }
    }
    buf[i] = '\0';

    return i;
}

// 16进制数转化为10进制
int hexit(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return 0;
}

/*
*  这里的内容是处理%20之类的东西！是"解码"过程。
*  %20 URL编码中的‘ ’(space)
*  %21 '!' %22 '"' %23 '#' %24 '$'
*  %25 '%' %26 '&' %27 ''' %28 '('......
*  相关知识html中的‘ ’(space)是&nbsp
*/

void encode_str(char* to, int tosize, const char* from)
{
    int tolen;

    for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from)
    {
        if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0)
        {
            *to = *from;
            ++to;
            ++tolen;
        }
        else
        {
            sprintf(to, "%%%02x", (int) *from & 0xff);
            to += 3;
            tolen += 3;
        }
    }
    *to = '\0';
}


void decode_str(char *to, char *from)
{
    for ( ; *from != '\0'; ++to, ++from  )
    {
        if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
        {

            *to = hexit(from[1])*16 + hexit(from[2]);

            from += 2;
        }
        else
        {
            *to = *from;
        }

    }
    *to = '\0';

}

// 通过文件名获取文件的类型
const char *get_file_type(const char *name)
{
    char* dot;

    // 自右向左查找‘.’字符, 如不存在返回NULL
    dot = (char *)strrchr(name, '.');
    if (dot == NULL)
        return "text/plain; charset=utf-8";
    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html; charset=utf-8";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".au") == 0)
        return "audio/basic";
    if (strcmp( dot, ".wav" ) == 0)
        return "audio/wav";
    if (strcmp(dot, ".avi") == 0)
        return "video/x-msvideo";
    if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
        return "video/quicktime";
    if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
        return "video/mpeg";
    if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
        return "model/vrml";
    if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
        return "audio/midi";
    if (strcmp(dot, ".mp3") == 0)
        return "audio/mpeg";
    if (strcmp(dot, ".ogg") == 0)
        return "application/ogg";
    if (strcmp(dot, ".pac") == 0)
        return "application/x-ns-proxy-autoconfig";

    return "text/plain; charset=utf-8";
}
