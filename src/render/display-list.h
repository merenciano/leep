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

            template<typename T>
            T& addCommand()
            {
                command_list_.push_back(std::make_unique<T>());
                return *(static_cast<T*>(command_list_.back().get()));
            }

            const std::list<std::unique_ptr<DisplayListCommand>>& command_list() const;
            void submit();

        private:
            std::list<std::unique_ptr<DisplayListCommand>> command_list_;
    };
}

#endif // __MOTORET_DISPLAY_LIST_H__