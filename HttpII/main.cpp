#include<iostream>
#include"head.h"
#include"tcpcommunication.h"
#include"tcplisten.h"
#include"httpget.h"
#include"httpsend.h"
#include"spellhtml.h"
#include"Tube.h"
#include"service.h"
#include"threadpool.h"
#include"logpferi.h"
using namespace std;
int main(int argc, char *argv[])
{
    logpferI log("./log/log_http");
    TcpListen lis(8889);
    ThreadPool pool;
    while (true)
    {
        auto ret=lis.acceptConnect();
        TcpCommunication *communication=new
                TcpCommunication(ret.first,ret.second);

        auto task=shared_ptr<Service>(new Service(communication));
        task->setLogPut(&log);
        pool.dispatch(task);
        //task->doit();

    }
    lis.destory();
    //log.stop();

    return 1;
}


int mai1212n()
{
    logpferI log("./log/log_http");
    string info="111111";
    while (true)
    {
        log.addinfo(info);
    }
    return 1;
}
