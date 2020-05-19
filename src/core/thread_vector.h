#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>

class ThreadVector
{
    std::vector<std::thread> m_threads;

    public:

    ~ThreadVector()
    {
        Join();
    }

    void Join()
    {
        for(int i=0;i<m_threads.size();i++)
            m_threads[i].join();
            
        m_threads.clear();
    }

    void Add(std::thread thread)
    {   
        m_threads.push_back(std::move(thread));
    }

};

#endif //THREAD_POOL_H