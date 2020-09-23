#ifndef __MOTORET_RENDER_GEOMETRY_H__
#define __MOTORET_RENDER_GEOMETRY_H__ 1

#include "buffer.h"

namespace motoret
{
    class Geometry
    {
    public:
        Geometry();
        Geometry(const Geometry &other);
        Geometry(Geometry &&other) = delete;
        ~Geometry();

        Geometry& operator=(const Geometry &other);

        void createCube();
    
    private:
        Buffer vertex_buffer_;
        Buffer index_buffer_;
    };
}

#endif // __MOTORET_RENDER_GEOMETRY_H__