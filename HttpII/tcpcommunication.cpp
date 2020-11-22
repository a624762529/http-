#include "tcpcommunication.h"

TcpCommunication::TcpCommunication(int cfd,sockaddr_in addr)
{
    this->m_cfd=cfd;
    this->m_addr=addr;
    m_status=Status::Free;


    int opt = 1;
    // sockfd为需要端口复用的套接字
    setsockopt(m_cfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

}


int TcpCommunication::readInfo(char *read_buf,int len)
{
    //将相应的数据读取到buf中
    int ret=read(m_cfd,read_buf,len);
    if(ret==0)
    {
        return ret;
    }
    else if(ret>0)
    {
        return ret;
    }
    else
    {
        if(errno==EAGAIN)
        {
            return -1;
        }
        cout<<"read error"<<__LINE__<<endl;
        throw bad_alloc();
    }

}

int TcpCommunication::writeInfo(string write_back)
{
    int ret=write(m_cfd,write_back.c_str(),write_back.size());
    if(ret>0)
    {
        return ret;
    }
    else
    {
        if(errno!=EAGAIN)
        {
            perror("read err");
            cout<<__LINE__<<__FUNCTION__<<endl;
            return -1;
        }
        throw bad_alloc();
    }
    return -1;
}

int TcpCommunication::writeInfo(char* write_back,int len)
{
    int ret=write(m_cfd,write_back,len);
    if(ret>0)
    {
        return ret;
    }
    else
    {
        if(errno!=EAGAIN)
        {
            perror("read err");
            cout<<__LINE__<<__FUNCTION__<<endl;
            return -1;
        }
        throw bad_alloc();
    }
    return -1;
}

void TcpCommunication::closeCommunication()
{
    cout<<"client dis connect"<<endl;
    close(m_cfd);
}


