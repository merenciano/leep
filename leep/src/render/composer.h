#ifndef __LEEP_RENDER_COMPOSER_H__
#define __LEEP_RENDER_COMPOSER_H__

#include "core/memory/memory.h"
#include "core/iexecutable.h"

#include <list>

namespace leep {
    class Composer
    {
        Composer();
        ~Composer();


        // NOTE(Lucas): I dont know if I should maintain this two
        // addEffect methods or choose one of them.

        // Usage: Create the effect and set its properties
        // and then add it using this method, losing ownership
        // of the pointer (this class will free the effects)
        void addEffect(IExecutable *effect);

        // Usage: Call this method with the proper effect (type)
        // and use the return to set its properties
        template<typename T>
        T& addEffect()
        {
            pp_list_.emplace_back(T());
            return *(*(T*)(pp_list_.back()));
        }

        std::list<IExecutable*> pp_list_; // post process list

    };
}

#endif // __LEEP_RENDER_COMPOSER_H__
