// Lucas Merenciano Martinez <lucasmermar@gmail.com>

#ifndef __LEEP_RENDER_GEOMETRY_H__
#define __LEEP_RENDER_GEOMETRY_H__ 1

#include "buffer.h"
#include "core/leep-string.h"
#include "Crenderer.h"

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
        // Voxel is like a cube but with only position and normal attributes
        void createVoxel();
		void createSphere(uint32_t x_segments, uint32_t y_segments);
        void createQuad();

        /* This method allocates memory using generalAlloc and will be released
         * when the obj is going to be rendered for the first time
         * after OpenGL copies it to the VRAM.
         * Make sure the obj will be renderer before calling this method
         * or free it manually with vertex/index_buffer().freeSystemRamData()
         */
        void loadObj(String path);

        void createMesh(
            float *vertex,
            uint32_t vertex_count,
            uint32_t *index,
            uint32_t index_count);
    
    private:
        Buffer vertex_buffer_;
        Buffer index_buffer_;
    };
}

#endif // __LEEP_RENDER_GEOMETRY_H__