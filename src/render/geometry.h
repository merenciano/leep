// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_GEOMETRY_H__
#define __LEEP_RENDER_GEOMETRY_H__ 1

#include "buffer.h"

namespace leep
{
    class Geometry
    {
    public:
        Geometry();
        Geometry(const Geometry &other);
        Geometry(Geometry &&other) = delete;
        ~Geometry();

        Geometry& operator=(const Geometry &other);
        Buffer vertex_buffer() const;
        Buffer index_buffer() const;

        void createCube();
    
    private:
        Buffer vertex_buffer_;
        Buffer index_buffer_;
    };
}

#endif // __LEEP_RENDER_GEOMETRY_H__