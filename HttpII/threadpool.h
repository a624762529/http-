#ifndef THREADPOOL_H
#define THREADPOOL_H
#include"Tube.h"
#include"service.h"
#include<iostream>
#include<atomic>
using namespace std;

class ThreadPool
{
public:
    ThreadPool();
    void initThreads();
    void dispatch(shared_ptr<Service>);
private:
    void loopTask(int index);
private:
    atomic<int>   max_thread;
    atomic<int>   m_tube_size[4];
    Tube          m_tube[4];
    volatile bool m_loop;
    int           m_add_index;
};

#endif // THREADPOOL_H
