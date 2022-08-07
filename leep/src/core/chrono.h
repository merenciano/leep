// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_CHRONO_H__
#define __LEEP_CORE_CHRONO_H__ 1

#include "Cdefinitions.h"
#include <chrono>
#include <time.h>

typedef struct THE_Chrono
{
    timespec start;
    timespec end;
} THE_Chrono;

void THE_ChronoStart(THE_Chrono *chrono)
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &chrono->start);
    chrono->end.tv_nsec = 0;
}

void THE_ChronoEnd(THE_Chrono *chrono)
{
    THE_ASSERT(chrono->end.tv_nsec == 0, "Calling ChronoEnd before ChronoStart");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &chrono->end);
}

/*
    Returns the duration of the crono, if THE_ChronoEnd has been called for the chrono
    it returns end-start. If end has not been called it returns the current duration.
*/
timespec THE_ChronoDuration(THE_Chrono *chrono)
{
    timespec ret;
    timespec tmp;
    if (chrono->end.tv_nsec == 0) {
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tmp);
    } else {
        tmp = chrono->end;
    }
    ret.tv_sec = tmp.tv_sec - chrono->start.tv_sec;
    ret.tv_nsec = tmp.tv_nsec - chrono->start.tv_nsec;
    return ret;
}
/*
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
*/
#endif // __LEEP_CORE_CHRONO_H__