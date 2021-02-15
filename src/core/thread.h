// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_THREAD_H__
#define __LEEP_CORE_THREAD_H__ 1

#include <thread>
#include <functional>

namespace leep
{
    typedef std::function<void()>       Function;
    // You can use the arg for in and out parameters if you want a return
    typedef std::function<void(void*)>  ArgsFunction;

    class Thread
    {
    public:
        Thread(Function f) : t_(f) {}
        Thread(ArgsFunction f, void* a) : t_(f, a) {}
        ~Thread() = default;
        void join() { t_.join(); }

    private:
        Thread();
        std::thread t_;
    };
}

#endif // __LEEP_CORE_THREAD_H__
