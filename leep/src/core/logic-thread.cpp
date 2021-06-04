#include "logic-thread.h"

namespace leep
{
    LogicThread::LogicThread(LogicFunction f) : f_(f)
    {
        terminate_ = false;
        run_ = false;
        running_ = false;
        t_ = std::make_unique<std::thread>(std::bind(&LogicThread::start,this));
    }

    LogicThread::~LogicThread()
    {
        {
            // The lock has to exit from its scope before the join
            // Learnt that the hard way...
            std::lock_guard<std::mutex> lk(cv_mtx_);
            terminate_ = true;
            cv_.notify_one();
        }
        t_->join();
    }

    void LogicThread::run()
    {
        std::lock_guard<std::mutex> lk(cv_mtx_);
        run_ = true;
        cv_.notify_one();
    }

    bool LogicThread::running()
    {
        return running_;
    }

    void LogicThread::start()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lk(cv_mtx_);
            cv_.wait(lk, [&] { return terminate_ || run_; });
            if (terminate_)
            {
                return;
            }

            run_ = false;
            running_ = true;
            f_();
            running_ = false;
        }
    }
}
