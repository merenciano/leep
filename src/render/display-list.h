#ifndef __MOTORET_DISPLAY_LIST_H__
#define __MOTORET_DISPLAY_LIST_H__ 1

#include "display-list-command.h"

#include <memory>
#include <list>

namespace motoret
{
    class DisplayList
    {
        public:
            DisplayList();
            DisplayList(DisplayList &other) = delete;
            DisplayList(DisplayList &&other);
            ~DisplayList();

        private:
            std::list<std::unique_ptr<DisplayListCommand>> command_list_;
    };
}

#endif // __MOTORET_DISPLAY_LIST_H__