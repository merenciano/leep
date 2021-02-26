#include "geometry.h"
#include "core/common-defs.h"

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
    std::vector<float> vertices({
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
    });

    std::vector<uint32_t> indices({
            0,  2,  1,  2,  0,  3,
            4,  5,  6,  6,  7,  4,
            8,  9, 10, 10, 11,  8,
        13, 12, 14, 12, 15, 14,
        16, 17, 18, 18, 19, 16,
        23, 22, 20, 22, 21, 20,
    });

    vertex_buffer_.create();
    vertex_buffer_.set_data(vertices, BufferType::VERTEX_BUFFER_3P_3N_2UV);
    index_buffer_.create();
    index_buffer_.set_data(indices);
}

void Geometry::createSphere(uint32_t x_segments, uint32_t y_segments)
{
    const float PI = 3.14159265359f;
    std::vector<float> vertex_vector;
    std::vector<uint32_t> index_vector;
    
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

            vertex_vector.emplace_back(px);
            vertex_vector.emplace_back(py);
            vertex_vector.emplace_back(pz);
            vertex_vector.emplace_back(nx);
            vertex_vector.emplace_back(ny);
            vertex_vector.emplace_back(nz);
            vertex_vector.emplace_back(uvx);
            vertex_vector.emplace_back(uvy);
        }
    }

    for (uint32_t y = 0; y < y_segments; ++y)
    {
        for (uint32_t x = 0; x < x_segments; ++x)
        {
            index_vector.push_back((y + 1) * (x_segments + 1) + x);
            index_vector.push_back(y       * (x_segments + 1) + x);
            index_vector.push_back(y       * (x_segments + 1) + x + 1);

            index_vector.push_back((y + 1) * (x_segments + 1) + x);
            index_vector.push_back(y       * (x_segments + 1) + x + 1);
            index_vector.push_back((y + 1) * (x_segments + 1) + x + 1);
        }
    }

    vertex_buffer_.create();
    vertex_buffer_.set_data(vertex_vector, 
        BufferType::VERTEX_BUFFER_3P_3N_2UV);
    index_buffer_.create();
    index_buffer_.set_data(index_vector);
}

void Geometry::createQuad()
{
    std::vector<float> vertices({
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f,
            1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  0.0f,
            1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  1.0f,
    });

    std::vector<uint32_t> indices({
        0,  1,  2,  0,  2,  3,
    });

    vertex_buffer_.create();
    vertex_buffer_.set_data(vertices, BufferType::VERTEX_BUFFER_3P_3N_2UV);
    index_buffer_.create();
    index_buffer_.set_data(indices);
}

void Geometry::loadObj(std::string path)
{
    // TODO: Preallocate std::vector of vertices and indices
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files
    tinyobj::ObjReader reader;

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    if (!reader.ParseFromFile(path, reader_config))
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
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        int32_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + 0];
            Vertex v1, v2, v3;

            v1.p.x = attrib.vertices[3*idx.vertex_index+0]; // Cast the 3 to int64_t in case of overflow (that would be a large obj)
            v1.p.y = attrib.vertices[3*idx.vertex_index+1];
            v1.p.z = attrib.vertices[3*idx.vertex_index+2];
            v1.n.x = attrib.normals[3*idx.normal_index+0];
            v1.n.y = attrib.normals[3*idx.normal_index+1];
            v1.n.z = attrib.normals[3*idx.normal_index+2];
            v1.uv.x = attrib.texcoords[2*idx.texcoord_index+0];
            v1.uv.y = attrib.texcoords[2*idx.texcoord_index+1];

            idx = shapes[s].mesh.indices[index_offset + 1];
            v2.p.x = attrib.vertices[3*idx.vertex_index+0];
            v2.p.y = attrib.vertices[3*idx.vertex_index+1];
            v2.p.z = attrib.vertices[3*idx.vertex_index+2];
            v2.n.x = attrib.normals[3*idx.normal_index+0];
            v2.n.y = attrib.normals[3*idx.normal_index+1];
            v2.n.z = attrib.normals[3*idx.normal_index+2];
            v2.uv.x = attrib.texcoords[2*idx.texcoord_index+0];
            v2.uv.y = attrib.texcoords[2*idx.texcoord_index+1];

            idx = shapes[s].mesh.indices[index_offset + 2];
            v3.p.x = attrib.vertices[3*idx.vertex_index+0];
            v3.p.y = attrib.vertices[3*idx.vertex_index+1];
            v3.p.z = attrib.vertices[3*idx.vertex_index+2];
            v3.n.x = attrib.normals[3*idx.normal_index+0];
            v3.n.y = attrib.normals[3*idx.normal_index+1];
            v3.n.z = attrib.normals[3*idx.normal_index+2];
            v3.uv.x = attrib.texcoords[2*idx.texcoord_index+0];
            v3.uv.y = attrib.texcoords[2*idx.texcoord_index+1];
            
            index_offset += 3;

            // Calc tangent and bitangent
            glm::vec3 delta_p1 = v2.p - v1.p;
            glm::vec3 delta_p2 = v3.p - v1.p;
            glm::vec2 delta_uv1 = v2.uv - v1.uv;
            glm::vec2 delta_uv2 = v3.uv - v1.uv;
            float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
            glm::vec3 tan = (delta_p1 * delta_uv2.y - delta_p2 * delta_uv1.y) * r;
            glm::vec3 bitan = (delta_p2 * delta_uv1.x - delta_p1 * delta_uv2.x) * r;

            v1.t = tan;
            v2.t = tan;
            v3.t = tan;
            v1.b = bitan;
            v2.b = bitan;
            v3.b = bitan;

            vertices.emplace_back(v1.p.x);
            vertices.emplace_back(v1.p.y);
            vertices.emplace_back(v1.p.z);
            vertices.emplace_back(v1.n.x);
            vertices.emplace_back(v1.n.y);
            vertices.emplace_back(v1.n.z);
            vertices.emplace_back(v1.t.x);
            vertices.emplace_back(v1.t.y);
            vertices.emplace_back(v1.t.z);
            vertices.emplace_back(v1.b.x);
            vertices.emplace_back(v1.b.y);
            vertices.emplace_back(v1.b.z);
            vertices.emplace_back(v1.uv.x);
            vertices.emplace_back(v1.uv.y);

            vertices.emplace_back(v2.p.x);
            vertices.emplace_back(v2.p.y);
            vertices.emplace_back(v2.p.z);
            vertices.emplace_back(v2.n.x);
            vertices.emplace_back(v2.n.y);
            vertices.emplace_back(v2.n.z);
            vertices.emplace_back(v2.t.x);
            vertices.emplace_back(v2.t.y);
            vertices.emplace_back(v2.t.z);
            vertices.emplace_back(v2.b.x);
            vertices.emplace_back(v2.b.y);
            vertices.emplace_back(v2.b.z);
            vertices.emplace_back(v2.uv.x);
            vertices.emplace_back(v2.uv.y);

            vertices.emplace_back(v3.p.x);
            vertices.emplace_back(v3.p.y);
            vertices.emplace_back(v3.p.z);
            vertices.emplace_back(v3.n.x);
            vertices.emplace_back(v3.n.y);
            vertices.emplace_back(v3.n.z);
            vertices.emplace_back(v3.t.x);
            vertices.emplace_back(v3.t.y);
            vertices.emplace_back(v3.t.z);
            vertices.emplace_back(v3.b.x);
            vertices.emplace_back(v3.b.y);
            vertices.emplace_back(v3.b.z);
            vertices.emplace_back(v3.uv.x);
            vertices.emplace_back(v3.uv.y);

            indices.emplace_back((uint32_t)indices.size());
            indices.emplace_back((uint32_t)indices.size());
            indices.emplace_back((uint32_t)indices.size());
        }
    }
    vertex_buffer_.create();
    index_buffer_.create();
    vertex_buffer_.set_data(vertices,
        BufferType::VERTEX_BUFFER_3P_3N_3T_3B_2UV);
    index_buffer_.set_data(indices);
}

} // namespace leep
