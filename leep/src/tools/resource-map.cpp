#include "resource-map.h"
#include "render/renderer.h"
#include "render/Crenderer.h"

namespace leep
{
    void ResourceMap::addGeometry(String name, String path)
    {
        LEEP_ASSERT(name != "Cube", "There is already a cube named Cube");
        LEEP_ASSERT(name != "Sphere", "There is already a sphere named Sphere");
        LEEP_ASSERT(name != "Quad", "There is already a quad named Quad");
        bool inserted = 
            geometries_.emplace(std::make_pair(name, Geometry())).second;
        if (inserted)
        {
            geometries_[name].loadObj(path);
        }
        else
        {
            LEEP_CORE_WARNING("There is already a geometry with that name");
        }
    }

    void ResourceMap::addGeometry(String name, Geometry g)
    {
        LEEP_CHECK_RESOURCE(g.vertex_buffer());
        LEEP_CHECK_RESOURCE(g.index_buffer());
        LEEP_ASSERT(name != "Cube", "There is already a cube named Cube");
        LEEP_ASSERT(name != "Sphere", "There is already a sphere named Sphere");
        LEEP_ASSERT(name != "Quad", "There is already a quad named Quad");
        bool inserted = geometries_.emplace(std::make_pair(name, g)).second;
        if (!inserted)
        {
            LEEP_CORE_WARNING("Geometry couldn't be inserted. Maybe there is already a geometry with that name");
        }
    }

void ResourceMap::addTexture(String name, String path, THE_TexType t)
{
	THE_ASSERT(!path.empty() && "For the creation of empty textures call with size params");

	THE_Texture tex = THE_CreateTexture(path.c_str(), t);
	if (!textures.emplace(std::make_pair(name, tex)).second)
	{
		THE_LOG_ERROR("Texture couldn't be inserted%c", ' ');
	}
}

    void ResourceMap::addTextureAsync(String name,
                                      String path,
                                      TexType t)
    {

    }

void ResourceMap::addTexture(String name, u32 w, u32 h, THE_TexType t)
{
	THE_ASSERT(w > 0 && h > 0 && "0,0 size texture is no texture");
	THE_Texture tex = THE_CreateEmptyTexture(w, h, t);
	if (!textures.emplace(std::make_pair(name, tex)).second)
	{
		THE_LOG_ERROR("Texture couldn't be inserted%c", ' ');
	}
}

    void ResourceMap::addTexture(String name, Texture tex)
    {
        bool inserted = textures_.emplace(std::make_pair(name, FuTexture())).second;
        if (!inserted)
        {
            LEEP_CORE_WARNING("Texture couldn't be inserted");
        }
        else
        {
            textures_.at(name).tex_ = tex;
        }
    }

    Geometry ResourceMap::getGeometry(String name) const
    {
        if (name == "Cube")
            return Renderer::s_cube;
		if (name == "Sphere")
			return Renderer::s_sphere;
        if (name == "Quad")
            return Renderer::s_quad;

        return geometries_.at(name);
    }

    Texture ResourceMap::getTexture(String name) const
    {
        if (textures_.at(name).fut_.valid())
        {
            textures_.at(name).fut_.wait();
        }
        return textures_.at(name).tex_;
    }
}