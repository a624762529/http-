#ifndef TCPCOMMUNICATION_H
#define TCPCOMMUNICATION_H
#include"head.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

struct InfoPack
{
    int len;
    char data[1];
};


class TcpCommunication
{
public:
    TcpCommunication(int cfd,sockaddr_in addr);
    void closeCommunication();

    int readInfo (char *read_buf,int len);
    int writeInfo(char* write_back,int len);
    int writeInfo(string write_back);
private:
    int                         m_cfd;
    sockaddr_in                 m_addr;
    enum Status
    {
        Read,Write,Free
    };
    int  m_status=Free;
};

#endif // TCPCOMMUNICATION_H
