#include "geometry.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace leep
{
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
        float vertices_f[] = {
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

        //  I'm lazy and didn't want to rewrite the data above as a Vertex
        Vertex *vertex_ptr = reinterpret_cast<Vertex*>(vertices_f);
        std::vector<Vertex> vertex_vector(vertex_ptr, 24 + vertex_ptr);

        uint32_t indices[] = {
             0,  2,  1,  2,  0,  3,
             4,  5,  6,  6,  7,  4,
             8,  9, 10, 10, 11,  8,
            13, 12, 14, 12, 15, 14,
            16, 17, 18, 18, 19, 16,
            23, 22, 20, 22, 21, 20,
        };

        std::vector<uint32_t> index_vector(std::begin(indices), std::end(indices));

        vertex_buffer_.create(BufferType::VERTEX_BUFFER);
        vertex_buffer_.set_data(vertex_vector);
        index_buffer_.create(BufferType::INDEX_BUFFER);
        index_buffer_.set_data(index_vector);
    }

	void Geometry::createSphere(uint32_t x_segments, uint32_t y_segments)
	{
		const float PI = 3.14159265359f;
		std::vector<Vertex> vertex_vector;
		std::vector<uint32_t> index_vector;
		
		for (uint32_t y = 0; y <= y_segments; ++y)
		{
			for (uint32_t x = 0; x <= x_segments; ++x)
			{
				Vertex v;
				float x_segment = (float)x / (float)x_segments;
				float y_segment = (float)y / (float)y_segments;
				v.px = std::cos(x_segment * (2.0f * PI)) * std::sin(y_segment * PI);
				v.py = std::cos(y_segment * PI);
				v.pz = std::sin(x_segment * (2.0f * PI)) * std::sin(y_segment * PI);

				v.nx = v.px;
				v.ny = v.py;
				v.nz = v.pz;

				v.tx = x / x_segments;
				v.ty = y / y_segments;

				vertex_vector.push_back(v);
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

		vertex_buffer_.create(BufferType::VERTEX_BUFFER);
		vertex_buffer_.set_data(vertex_vector);
		index_buffer_.create(BufferType::INDEX_BUFFER);
		index_buffer_.set_data(index_vector);
	}

    void Geometry::loadObj(std::string path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning;
        std::string error;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, path.c_str());
        if (!warning.empty()) {
            LEEP_CORE_WARNING("TinyObj Warning: {0}\n", warning.c_str());
        }

        if (!error.empty()) {
            LEEP_CORE_ERROR("TinyObj Error: {0}\n", error.c_str());
        }

        LEEP_CORE_ASSERT(ret, "TinyObjLoader Failed");

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex;

                vertex.px = attrib.vertices[3 * index.vertex_index + 0];
                vertex.py = attrib.vertices[3 * index.vertex_index + 1];
                vertex.pz = attrib.vertices[3 * index.vertex_index + 2];

                vertex.nx = attrib.normals[3 * index.normal_index + 0];
                vertex.ny = attrib.normals[3 * index.normal_index + 1];
                vertex.nz = attrib.normals[3 * index.normal_index + 2];

                vertex.tx = attrib.texcoords[2 * index.texcoord_index + 0];
                vertex.ty = attrib.texcoords[2 * index.texcoord_index + 1];
                vertices.push_back(vertex);
                indices.push_back(indices.size());
            }
        }

        vertex_buffer_.create(BufferType::VERTEX_BUFFER);
        index_buffer_.create(BufferType::INDEX_BUFFER);
        vertex_buffer_.set_data(vertices);
        index_buffer_.set_data(indices);
    }
}