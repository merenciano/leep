#ifndef __CLICKTACLYSM_DUNGEON_H__
#define __CLICKTACLYSM_DUNGEON_H__

#include <stdint.h>
#include "clicktaclysm.h"

#include "render/buffer.h"

namespace clicktaclysm 
{
    struct Room
    {
        uint16_t id;
        uint8_t x;
        uint8_t z;
        uint8_t sx;
        uint8_t sz;
    };

    class Dungeon
    {
    public:
        Dungeon();
        ~Dungeon();
        // Use areas power of 2 for better optimization
        // (I guess the compiler will optimize the modulo using mask)
        void generate(uint8_t area_x, uint8_t area_z);
        Room *rooms_;
        leep::Buffer ground_buf_;
        int32_t vox_count_;
        int32_t room_count_;
        // TODO: add floors
        //int8_t floors_;
    };
}

#endif // __CLICKTACLYSM_DUNGEON_H__