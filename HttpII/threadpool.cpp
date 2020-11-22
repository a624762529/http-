#include "threadpool.h"

ThreadPool::ThreadPool()
{
    for(int i=0;i<4;i++)
    {
        m_tube[i].setFreeSize(400);
    }
    initThreads();
    m_loop=true;
}

void ThreadPool::initThreads()
{
    thread th[4];
    for(int i=0;i<4;i++)
    {
        th[i]=std::move(thread(&ThreadPool::loopTask,this,i));
    }

    for(int i=0;i<4;i++)
    {
        th[i].detach();
    }
}

void ThreadPool::dispatch(shared_ptr<Service> task)
{
    static int s_index=0;
    m_tube[s_index].put(task);
    s_index=(s_index+1)%4;
}

void ThreadPool::loopTask(int index)
{
    cout<<index<<"loop"<<endl;
    while (m_loop)
    {
        cout<<"started loop"<<endl;
        auto task=m_tube[index].get();
        if(task==nullptr)
        {
            break;
        }
        cout<<"get task"<<index<<endl;
        task->doit();
    }
}
