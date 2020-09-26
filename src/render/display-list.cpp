#include "display-list.h"
#include "manager.h"

namespace motoret
{
    DisplayList::DisplayList()
    {

    }

    DisplayList::DisplayList(DisplayList &&other)
    {
        std::swap(command_list_, other.command_list_);
    }

    DisplayList::~DisplayList()
    {
        command_list_.clear();
    }

    void DisplayList::submit()
    {
        Manager::instance().renderer().addDisplayListToQueue(std::move(*this));
    }
}