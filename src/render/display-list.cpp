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

    const std::list<std::unique_ptr<DisplayListCommand>>& DisplayList::command_list() const
    {
        return command_list_;
    }

    void DisplayList::submit()
    {
        LEEP_CORE_ASSERT(command_list_.size() > 0, "Trying to submit an empty display list");
        Manager::instance().renderer().addDisplayListToQueue(std::move(*this));
    }
}