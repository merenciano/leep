/* Lucas Merenciano Martinez  <lucasmermar@gmail.com> */

#ifndef __LEEP_CORE_GAME_STATE_H__
#define __LEEP_CORE_GAME_STATE_H__

#include "core/config.h"
#include "core/chrono.h"
#include "core/scene.h"

namespace leep
{
    class GameState
    {
        GameData data_;
        LeepConfig config_;
        Scene scene_;
        Chrono frame_chrono_;
    };
}

#endif // __LEEP_CORE_GAME_STATE_H__
