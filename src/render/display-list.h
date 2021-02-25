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
    const int32_t kDLMaxLen = 20;
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
                //list_.push_back((DisplayListCommand*)GM.memory().render_mem_.allocT<T>());
                //return *(static_cast<T*>(list_.back()));
                LEEP_CORE_ASSERT(i_ < kDLMaxLen, "DisplayList full");
                list_[i_++] = GM.memory().renderq_.commandAlloc<T>();
                return *((T*)list_[i_ - 1]);
            }
            //const std::list<std::unique_ptr<DisplayListCommand>>& command_list() const;
            DisplayListCommand* command_list() const;
            int32_t commandListCount() const;
            void submit();

        private:
            //std::list<std::unique_ptr<DisplayListCommand>> command_list_;
            DisplayListCommand *list_[kDLMaxLen];
            int32_t i_;
    };
}

#endif // __LEEP_DISPLAY_LIST_H__