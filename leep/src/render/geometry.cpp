#include "geometry.h"
#include "core/common-defs.h"
#include "core/manager.h"
#include "core/memory/memory.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace leep {

Geometry::Geometry()
{

}

Geometry::Geometry(const Geometry &other)
{
    vertex_buffer_ = other.vertex_buffer_;
    index_buffer_ = other.index_buffer_;
}

Geometry::~Geometry()
{

}

Geometry& Geometry::operator=(const Geometry &other)
{
    vertex_buffer_ = other.vertex_buffer_;
    index_buffer_ = other.index_buffer_;
    return *this;
}

Buffer Geometry::vertex_buffer() const
{
    return vertex_buffer_;
}

Buffer Geometry::index_buffer() const
{
    return index_buffer_;
}

void Geometry::createCube()
{
    int32_t i = 0;
    float *vert = (float*)GM.memory().generalAlloc(24*8*sizeof(float));
    uint32_t *ind = (uint32_t*)GM.memory().generalAlloc(36 * sizeof(uint32_t));
    float vertices[] = {
        // positions          // normals           // uv 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  // 4
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  // 8
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 12
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // 16
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, // 20
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    };

    for (float f : vertices)
    {
        vert[i++] = f;
    }

    uint32_t indices[] = {
         0,  2,  1,  2,  0,  3,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        13, 12, 14, 12, 15, 14,
        16, 17, 18, 18, 19, 16,
        23, 22, 20, 22, 21, 20,
    };

    i = 0;
    for (uint32_t idx : indices)
    {
        ind[i++] = idx;
    }

    vertex_buffer_.create();
    vertex_buffer_.set_data(vert, 24*8, BufferType::VERTEX_BUFFER_3P_3N_2UV);
    index_buffer_.create();
    index_buffer_.set_data(ind, 36);
}

void Geometry::createSphere(uint32_t x_segments, uint32_t y_segments)
{
    const float PI = 3.14159265359f;
    // Using the raw alloc because for basic types I dont need to call the constructors
    float *vert = (float*)GM.memory().generalAlloc(((1+x_segments) * (1+y_segments) * 8)*sizeof(float));
    uint32_t *ind = (uint32_t*)GM.memory().generalAlloc((x_segments * y_segments * 6) * sizeof(uint32_t));
    int32_t i = 0;
    
    for (uint32_t y = 0; y <= y_segments; ++y)
    {
        for (uint32_t x = 0; x <= x_segments; ++x)
        {
            float x_segment = (float)x / (float)x_segments;
            float y_segment = (float)y / (float)y_segments;
            float px = std::cos(x_segment * (2.0f * PI)) * std::sin(y_segment * PI);
            float py = std::cos(y_segment * PI);
            float pz = std::sin(x_segment * (2.0f * PI)) * std::sin(y_segment * PI);

            float nx = px;
            float ny = py;
            float nz = pz;

            float uvx = atan2(nx, nz) / (2.0f * PI) + 0.5f;
            float uvy = ny * 0.5f + 0.5f;

            vert[i++] = px;
            vert[i++] = py;
            vert[i++] = pz;
            vert[i++] = nx;
            vert[i++] = ny;
            vert[i++] = nz;
            vert[i++] = uvx;
            vert[i++] = uvy;
        }
    }

    i = 0;
    for (uint32_t y = 0; y < y_segments; ++y)
    {
        for (uint32_t x = 0; x < x_segments; ++x)
        {
            ind[i++] = ((y + 1) * (x_segments + 1) + x);
            ind[i++] = (y       * (x_segments + 1) + x);
            ind[i++] = (y       * (x_segments + 1) + x + 1);
            ind[i++] = ((y + 1) * (x_segments + 1) + x);
            ind[i++] = (y       * (x_segments + 1) + x + 1);
            ind[i++] = ((y + 1) * (x_segments + 1) + x + 1);
        }
    }

    vertex_buffer_.create();
    vertex_buffer_.set_data(vert, (1+x_segments)*(1+y_segments)*8,
        BufferType::VERTEX_BUFFER_3P_3N_2UV);
    index_buffer_.create();
    index_buffer_.set_data(ind, x_segments*y_segments * 6);
}

void Geometry::createQuad()
{
    float *v = (float*)GM.memory().generalAlloc(32 * sizeof(float));
    uint32_t *ind = (uint32_t*)GM.memory().generalAlloc(6 * sizeof(uint32_t));
    float vertices[]{
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  0.0f,
        1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  1.0f,
    };

    float *i = v;
    for (float f : vertices)
    {
        *i++ = f;
    }

    *(ind + 0) = 0;
    *(ind + 1) = 1;
    *(ind + 2) = 2;
    *(ind + 3) = 0;
    *(ind + 4) = 2;
    *(ind + 5) = 3;

    vertex_buffer_.create();
    vertex_buffer_.set_data(v, 32, BufferType::VERTEX_BUFFER_3P_3N_2UV);
    index_buffer_.create();
    index_buffer_.set_data(ind, 6);
}

void Geometry::loadObj(String path)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files
    tinyobj::ObjReader reader;

    std::vector<float, BuddySTL<float>> vertices;
    std::vector<uint32_t, BuddySTL<uint32_t>> indices;

    if (!reader.ParseFromFile(std::string(path.c_str()), reader_config))
    {
        if (!reader.Error().empty())
        {
            LEEP_CORE_ERROR("TinyObjReader: {0}", reader.Error());
        }
        return;
    }

    if (!reader.Warning().empty())
    {
        LEEP_CORE_WARNING("TinyObjReader: {0}", reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        int32_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset++];
            float v1[14], v2[14], v3[14];

            v1[0] = attrib.vertices[3*idx.vertex_index+0]; // Cast the 3 to int64_t in case of overflow (that would be a large obj)
            v1[1] = attrib.vertices[3*idx.vertex_index+1];
            v1[2] = attrib.vertices[3*idx.vertex_index+2];
            v1[3] = attrib.normals[3*idx.normal_index+0];
            v1[4] = attrib.normals[3*idx.normal_index+1];
            v1[5] = attrib.normals[3*idx.normal_index+2];
            v1[12] = attrib.texcoords[2*idx.texcoord_index+0];
            v1[13] = attrib.texcoords[2*idx.texcoord_index+1];

            idx = shapes[s].mesh.indices[index_offset++];
            v2[0] = attrib.vertices[3*idx.vertex_index+0];
            v2[1] = attrib.vertices[3*idx.vertex_index+1];
            v2[2] = attrib.vertices[3*idx.vertex_index+2];
            v2[3] = attrib.normals[3*idx.normal_index+0];
            v2[4] = attrib.normals[3*idx.normal_index+1];
            v2[5] = attrib.normals[3*idx.normal_index+2];
            v2[12] = attrib.texcoords[2*idx.texcoord_index+0];
            v2[13] = attrib.texcoords[2*idx.texcoord_index+1];

            idx = shapes[s].mesh.indices[index_offset++];
            v3[0] = attrib.vertices[3*idx.vertex_index+0];
            v3[1] = attrib.vertices[3*idx.vertex_index+1];
            v3[2] = attrib.vertices[3*idx.vertex_index+2];
            v3[3] = attrib.normals[3*idx.normal_index+0];
            v3[4] = attrib.normals[3*idx.normal_index+1];
            v3[5] = attrib.normals[3*idx.normal_index+2];
            v3[12] = attrib.texcoords[2*idx.texcoord_index+0];
            v3[13] = attrib.texcoords[2*idx.texcoord_index+1];
            
            // Calculate tangent and bitangent
            glm::vec3 delta_p1 = glm::vec3(v2[0], v2[1], v2[2]) - glm::vec3(v1[0], v1[1], v1[2]);
            glm::vec3 delta_p2 = glm::vec3(v3[0], v3[1], v3[2]) - glm::vec3(v1[0], v1[1], v1[2]);
            glm::vec2 delta_uv1 = glm::vec2(v2[12], v2[13]) - glm::vec2(v1[12], v1[13]);
            glm::vec2 delta_uv2 = glm::vec2(v3[12], v3[13]) - glm::vec2(v1[12], v1[13]);
            float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
            glm::vec3 tan = (delta_p1 * delta_uv2.y - delta_p2 * delta_uv1.y) * r;
            glm::vec3 bitan = (delta_p2 * delta_uv1.x - delta_p1 * delta_uv2.x) * r;

            v1[6] = tan.x;
            v1[7] = tan.y;
            v1[8] = tan.z;
            v2[6] = tan.x;
            v2[7] = tan.y;
            v2[8] = tan.z;
            v3[6] = tan.x;
            v3[7] = tan.y;
            v3[8] = tan.z;

            v1[9]  = bitan.x;
            v1[10] = bitan.y;
            v1[11] = bitan.z;
            v2[9]  = bitan.x;
            v2[10] = bitan.y;
            v2[11] = bitan.z;
            v3[9]  = bitan.x;
            v3[10] = bitan.y;
            v3[11] = bitan.z;

            for (int32_t i = 0; i < 14; ++i)
            {
                vertices.emplace_back(v1[i]);
            }

            for (int32_t i = 0; i < 14; ++i)
            {
                vertices.emplace_back(v2[i]);
            }

            for (int32_t i = 0; i < 14; ++i)
            {
                vertices.emplace_back(v3[i]);
            }

            indices.emplace_back((uint32_t)indices.size());
            indices.emplace_back((uint32_t)indices.size());
            indices.emplace_back((uint32_t)indices.size());
        }
    }
    int32_t i = 0;
    float *v = (float*)GM.memory().generalAlloc((uint32_t)vertices.size() * sizeof(float));
    uint32_t *ind = (uint32_t*)GM.memory().generalAlloc((uint32_t)indices.size() * sizeof(uint32_t));
    for (float f : vertices)
    {
        v[i++] = f;
    }
    i = 0;
    for (uint32_t u : indices)
    {
        ind[i++] = u;
    }
    
    vertex_buffer_.create();
    index_buffer_.create();
    vertex_buffer_.set_data(v, (uint32_t)vertices.size(), 
        BufferType::VERTEX_BUFFER_3P_3N_3T_3B_2UV);
    index_buffer_.set_data(ind, (uint32_t)indices.size());
}

} // namespace leep
