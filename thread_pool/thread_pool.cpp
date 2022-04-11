#include <iostream>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>

using namespace std;

class ThreadPool
{
private:
    vector<thread*> threads;
    queue<function<void()> > tasks;
    int size;
    
    mutex lock;

    void takeAndRun(int number_thread)
    {
        while (true)
        {
            this->lock.lock();
            if (this->tasks.size() > 0)
            {
                cout << "START Execution of thread number " << number_thread << endl;
                function<void()> task = this->tasks.front();
                this->tasks.pop();
                this->lock.unlock();
                task();
                this_thread::sleep_for(chrono::milliseconds(1000) );
                cout << "END Execution of thread number " << number_thread << endl;
            } else {
                this->lock.unlock();
            }
        }
    }

    void runThreads()
    {
        for (int i = 0; i < this->size; i++)
        {
            thread *t = new thread([this, i](){this->takeAndRun(i);});
            this->threads.push_back(t);
        }
    }

public:
    ThreadPool(int size)
    {
        this->size = size;
        this->threads = vector<thread*>(this->size);
        thread t([this]()
                 { this->runThreads(); });
        t.join();
    }

    void run(function<void()> task)
    {
        tasks.push(task);
    }
};