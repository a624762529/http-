#include "service.h"

void Service::doit()
{
    Task(m_communication);
}

void Service::setInfo(TcpCommunication *communication)
{
    m_communication=communication;
}

bool Service::sendFile(TcpCommunication *communication,string file_path)
{
    int fd=open(file_path.c_str(),O_RDONLY,0777);
    if(fd==-1)
        return false;
    while (true)
    {
        char buf[64*1024]{0};
        int ret=read(fd,buf,64*1024);
        if(ret==0)
            break;
        communication->writeInfo(buf,ret);
    }
    return true;
}

string Service::dealPath(string source)
{
    cout<<"访问的资源路径是"<<source<<endl;
    char path[4096]{};
    if(source=="/")
    {
        return string(getcwd(path,4096))+"/source";
    }
    else if(source=="/favicon.ico")
    {
        return string(getcwd(path,4096))+"/favicon.ico";
    }
    return source;
}

void Service::Task(TcpCommunication *communication)
{
    char buf[64*1024]{};
    int ret=communication->readInfo(buf,sizeof(buf));
    if(ret>0)
    {
        HttpGet http;
        http.InitHttpInfo(buf,ret);
        string resource=dealPath(http.m_source);

        //将申请访问的资源加载到日志上
        m_log->addinfo(resource);

        if((access(resource.c_str(),F_OK))!=-1)
        {
            struct stat st;
            stat(resource.c_str(),&st);
            if(S_ISREG(st.st_mode))
            {
                //普通文件
                httpSend http_send;
                http_send.setInfo
                                (200,"ok",resource,"http/1.1",
                                 http.getSourceType(resource),-1);
                communication->writeInfo(http_send.getHttpHead());

                sendFile(communication,resource);
            }
            // 如果是目录
            else if(S_ISDIR(st.st_mode))
            {
                //发送相应目录

                string   http_head;
                httpSend http_send;
                http_send.setInfo
                                 (200,"ok",resource,"http/1.1",
                                  "text/html; charset=utf-8",-1);

                http_head=http_send.getHttpHead();
                SpellHtml spell;
                string send_data=spell.getDirSendInfo(resource);
                string send_title=spell.getTitle(resource);
                communication->writeInfo(http_head);
                communication->writeInfo(send_title);
                communication->writeInfo(send_data);

            }
        }
    }
    communication->closeCommunication();
    delete communication;
}

Service::Service(TcpCommunication *tcp)
{
    m_communication=tcp;
}

void Service::setLogPut(logpferI *log)
{
    m_log=log;
}
