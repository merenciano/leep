#ifndef __CLICKTACLYSM_DUNGEON_GENERATION_SYSTEM_H__
#define __CLICKTACLYSM_DUNGEON_GENERATION_SYSTEM_H__

#include "clicktaclysm.h"
#include "ecs/system.h"

#include "dungeon.h"

namespace clicktaclysm
{
    class DungeonGen : public leep::System
    {
        DungeonGen();
        virtual void executeSystem() const override;
    };
}

#endif  // __CLICKTACLYSM_DUNGEON_GENERATION_SYSTEM_H__