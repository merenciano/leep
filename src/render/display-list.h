// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_DISPLAY_LIST_H__
#define __LEEP_DISPLAY_LIST_H__ 1

#include "display-list-command.h"
#include "core/manager.h"
#include "core/memory/memory.h"

#include <memory>
#include <list>

namespace leep
{
    class DisplayList
    {
        public:
            DisplayList();
            DisplayList(DisplayList&) = delete;
            DisplayList(DisplayList&&) = default;
            ~DisplayList();

            /*template<typename T>
            T& addCommand()
            {
                command_list_.push_back(std::make_unique<T>());
                return *(static_cast<T*>(command_list_.back().get()));
            }*/

            template<typename T>
            T& addCommand()
            {
                list_.push_back((DisplayListCommand*)GM.memory().renderq_.allocT<T>());
                return *(static_cast<T*>(list_.back()));
            }
            //const std::list<std::unique_ptr<DisplayListCommand>>& command_list() const;
            const std::list<DisplayListCommand*>& command_list() const;
            void submit();

        private:
            //std::list<std::unique_ptr<DisplayListCommand>> command_list_;
            std::list<DisplayListCommand*> list_;
    };
}

#endif // __LEEP_DISPLAY_LIST_H__