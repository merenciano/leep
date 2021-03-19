#include "display-list.h"
#include "core/manager.h"
#include "render/renderer.h"

namespace leep
{
    DisplayList::DisplayList()
    {
        i_ = 0;
    }

    DisplayList::~DisplayList()
    {

    }

    DLComm** DisplayList::command_list() const
    {
        return (DLComm**)list_;
    }

    int32_t DisplayList::commandListCount() const
    {
        return i_;
    }

    void DisplayList::submit()
    {
        // TODO: If necessary: mutex (make a function in renderer and lock there)
        // NOTE: At this moment adding displaylists at the same time from different
        // threads is not supported.
        GM.renderer().rq_.addDL(this);
        // DisplayList can be used again after submit
        i_ = 0;
#ifdef LEEP_DEBUG
        for (int32_t i = 0; i < kDLMaxLen; ++i)
        {
            list_[i] = nullptr;
        }
#endif
    }
}