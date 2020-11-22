#ifndef SERVICE_H
#define SERVICE_H
#include<string>
#include<iostream>
#include"head.h"
#include"tcpcommunication.h"
#include"tcplisten.h"
#include"httpget.h"
#include"httpsend.h"
#include"spellhtml.h"
#include"Tube.h"
#include"logpferi.h"

using namespace std;

class Service:public TubeImple
{
public:
    Service(TcpCommunication *);
    void doit();
    void setInfo(TcpCommunication *communication);
    bool sendFile(TcpCommunication *communication,string file_path);
    string dealPath(string source);
    void Task(TcpCommunication *communication);
    void setLogPut(logpferI     *log);
private:
    TcpCommunication *m_communication;
    logpferI         *m_log;
};

#endif // SERVICE_H
