// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_CHRONO_H__
#define __LEEP_CORE_CHRONO_H__ 1

#include <chrono>

namespace leep
{
    class Chrono
    {
    public:
        void start() { start_ = std::chrono::high_resolution_clock::now(); }
        void end() { end_ = std::chrono::high_resolution_clock::now(); }
        int64_t duration_micro() { return std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count(); }
        float duration() { return std::chrono::duration<float, std::milli>(end_ - start_).count(); }
        std::chrono::time_point<std::chrono::high_resolution_clock> start_;
        std::chrono::time_point<std::chrono::high_resolution_clock> end_;
    };
}

#endif // __LEEP_CORE_CHRONO_H__