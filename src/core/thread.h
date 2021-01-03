// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_THREAD_H__
#define __LEEP_CORE_THREAD_H__ 1

#include <thread>
#include <functional>

namespace leep
{
    typedef std::function<void()> Function;
    class Thread
    {
    public:
        Thread(Function f) : t_(f) {}
        ~Thread() = default;
        void join() { t_.join(); }

    private:
        Thread();
        std::thread t_;
    };
}

#endif // __LEEP_CORE_THREAD_H__
