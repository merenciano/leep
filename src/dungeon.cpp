#include "dungeon.h"

#include "core/manager.h"
#include "core/memory/memory.h"

#include <stdlib.h>
#include <list>

namespace clicktaclysm {
    
    static void PointGen(uint8_t *io_x, uint8_t *io_z)
    {
        uint8_t ax = *io_x;
        uint8_t az = *io_z;

        *io_x = rand() % ax;
        *io_z = rand() % az;
    }

    Dungeon::Dungeon()
    {
        rooms_ = nullptr;
    }

    Dungeon::~Dungeon()
    {
        leep::GM.memory().generalFree(rooms_);
        ground_buf_.release();
    }

    void Dungeon::generate(uint8_t ax, uint8_t az)
    {
        using namespace leep;
        srand(time(NULL));

        std::vector<Room> truerooms;
        
        int32_t amount = (ax >> 2) * (az >> 2);
        int32_t max_size = 32;
        max_size -= 8;

        rooms_ = (Room*)GM.memory().generalAlloc(amount * sizeof(Room));
        std::list<int32_t> indices;

        for (int32_t i = 0; i < amount; ++i)
        {
            rooms_[i].x = ax;
            rooms_[i].z = az;
            rooms_[i].sx = 8 + (rand() % max_size);
            rooms_[i].sz = 8 + (rand() % max_size);
            PointGen(&rooms_[i].x, &rooms_[i].z);
            indices.push_back(i);
        }

        while (!indices.empty())
        {
            Room r = rooms_[indices.front()];
            indices.pop_front();
            auto i = indices.begin();
            while (i != indices.end())
            {
                if (r.x < rooms_[*i].x + rooms_[*i].sx &&
                    r.x + r.sx > rooms_[*i].x &&
                    r.z < rooms_[*i].z + rooms_[*i].sz &&
                    r.z + r.sz > rooms_[*i].z)
                {
                    // There is collision between rooms
                    i = indices.erase(i);
                }
                else
                {
                    ++i;
                }
            }
            truerooms.push_back(r);
        } 
        GM.memory().generalFree(rooms_);
        room_count_ = truerooms.size();
        rooms_ = (Room*)GM.memory().generalAlloc(truerooms.size() * sizeof(Room));
        memcpy(rooms_, truerooms.data(), truerooms.size() * sizeof(Room));


        // Generate ground voxels
        int it1 = 0;
        for (int i = 0; i < room_count_; ++i)
        {
            vox_count_ += rooms_[i].sx * rooms_[i].sz;
        }

        Voxel *ground = (Voxel*)GM.memory().generalAlloc(sizeof(Voxel) * vox_count_);
        Voxel *vit = ground;
        for (int i = 0; i < room_count_; ++i)
        {
            for (int x = rooms_[i].x; x < rooms_[i].x + rooms_[i].sx; ++x)
            {
                for (int z = rooms_[i].z; z < rooms_[i].z + rooms_[i].sz; ++z)
                {
                    *vit = Voxel((float)x, 0.0f, (float)z);
                    vit++;
                }
            }
        }

        ground_buf_.create();
        ground_buf_.set_data((float*)ground, vox_count_ * 3, leep::BufferType::VERTEX_BUFFER_3P);
    }
}