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

    /*const std::list<std::unique_ptr<DisplayListCommand>>& DisplayList::command_list() const
    {
        return command_list_;
    }
*/
    DisplayListCommand* DisplayList::command_list() const
    {
        return list_[0];
    }

    int32_t DisplayList::commandListCount() const
    {
        return i_;
    }

    void DisplayList::submit()
    {
        //Manager::instance().renderer().addDisplayListToQueue(std::move(*this));
        // TODO: mutex (make a function in renderer and lock there)
        //GM.renderer().next_frame_queue_.push_back(std::move(*this));
        GM.memory().renderq_.addDL(this);
        i_ = 0;
#ifdef LEEP_DEBUG
        for (int32_t i = 0; i < kDLMaxLen; ++i)
        {
            list_[i] = nullptr;
        }
#endif
    }
}