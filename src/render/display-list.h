// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_DISPLAY_LIST_H__
#define __LEEP_DISPLAY_LIST_H__ 1

#include "core/manager.h"
#include "render/renderer.h"

#include <memory>
#include <list>

namespace leep
{
    class DLComm;
    // TODO: Do something with the dlmax len to lower it and
    // in the render system slice them up when reaching that limit
    const int32_t kDLMaxLen = 2000;
    class DisplayList
    {
    public:
        DisplayList();
        DisplayList(DisplayList&) = delete;
        DisplayList(DisplayList&&) = default;
        ~DisplayList();

        template<typename T>
        T& addCommand()
        {
            LEEP_CORE_ASSERT(i_ < kDLMaxLen, "DisplayList full");
            list_[i_++] = GM.renderer().rq_.commandAlloc<T>();
            return *((T*)list_[i_ - 1]);
        }

        DLComm **command_list() const;
        int32_t commandListCount() const;
        void submit();

    private:
        DLComm *list_[kDLMaxLen];
        int32_t i_;
    };
}

#endif // __LEEP_DISPLAY_LIST_H__