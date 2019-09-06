#include<iostream>
#include"head.h"
#include<thread>
#include<mutex>
using namespace std;
#define FILE_MODE (S_IRUSR||S_IWUSR||S_IRGRP||S_IROTH)

int maisadn()
{
    int read_fd=open("/home/a624762529/Desktop/fifo",O_WRONLY );
    char buf[102]{0};
    while (true)
    {
        int ret=read(read_fd,buf,102);
        cout<<buf<<endl;
        cout<<ret<<endl;
    }
    close(read_fd);

    return 1;
}

