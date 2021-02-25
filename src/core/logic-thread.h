// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_LOGIC_THREAD_H__
#define __LEEP_CORE_LOGIC_THREAD_H__

#include <thread>
#include <functional>
#include <condition_variable>

namespace leep
{
    typedef std::function<void()> LogicFunction;

    class LogicThread
    {
    public:
        LogicThread(LogicFunction f);
        ~LogicThread();

        void run();
        bool running();

    private:
        std::unique_ptr<std::thread> t_;
        std::condition_variable cv_;
        std::mutex cv_mtx_;
        LogicFunction f_;
        bool terminate_;
        bool run_;
        bool running_;

        void start();
        LogicThread() = delete;
        LogicThread(const LogicThread&) = delete;
        LogicThread& operator=(const LogicThread&) = delete;
    };
}

#endif // __LEEP_CORE_LOGIC_THREAD_H__
