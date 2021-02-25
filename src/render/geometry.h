// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_GEOMETRY_H__
#define __LEEP_RENDER_GEOMETRY_H__ 1

#include "buffer.h"
#include <string>

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
		void createSphere(uint32_t x_segments, uint32_t y_segments);
        void createQuad();
        void loadObj(std::string path);
    
    private:
        Buffer vertex_buffer_;
        Buffer index_buffer_;
    };
}

#endif // __LEEP_RENDER_GEOMETRY_H__