#include "display-list.h"
#include "core/manager.h"
#include "render/renderer.h"

namespace leep
{
    DisplayList::DisplayList()
    {

    }

    DisplayList::~DisplayList()
    {

    }

    /*const std::list<std::unique_ptr<DisplayListCommand>>& DisplayList::command_list() const
    {
        return command_list_;
    }
*/
    const std::list<DisplayListCommand*>& DisplayList::command_list() const
    {
        return list_;
    }

    void DisplayList::submit()
    {
        //Manager::instance().renderer().addDisplayListToQueue(std::move(*this));
        // TODO: mutex (make a function in renderer and lock there)
        GM.renderer().next_frame_queue_.push_back(std::move(*this));
    }
}