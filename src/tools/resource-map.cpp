#include "resource-map.h"
#include "render/renderer.h"

namespace leep
{
    void ResourceMap::addGeometry(std::string name, std::string path)
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

    void ResourceMap::addTexture(
            std::string name,
            std::string path,
            TexType t)
    {
        LEEP_CORE_ASSERT(path != "", "For the creation of empty textures call with size params");
        bool inserted =
            textures_.emplace(std::make_pair(name, Texture())).second;
        if (inserted)
        {
            textures_[name].create(path, t);
        }
        else
        {
            LEEP_CORE_WARNING("Texture couldn't be inserted");
        }
    }

    void ResourceMap::addTexture(std::string n, float w, float h, TexType t)
    {
        LEEP_CORE_ASSERT(w > 0.0f && h > 0.0f, "0,0 size texture is no texture");
        bool inserted = textures_.emplace(std::make_pair(n, Texture())).second;
        if (inserted)
        {
            textures_[n].createEmpty(w, h, t);
        }
        else
        {
            LEEP_CORE_WARNING("Texture couldn't be inserted");
        }
    }

    void ResourceMap::addTexture(std::string name, Texture tex)
    {
        bool inserted = textures_.emplace(std::make_pair(name, tex)).second;
        if (!inserted)
        {
            LEEP_CORE_WARNING("Texture couldn't be inserted");
        }
    }

    Geometry ResourceMap::getGeometry(std::string name) const
    {
        if (name == "Cube")
            return Renderer::s_cube;
		if (name == "Sphere")
			return Renderer::s_sphere;
        if (name == "Quad")
            return Renderer::s_quad;

        return geometries_.at(name);
    }

    Texture ResourceMap::getTexture(std::string name) const
    {
        return textures_.at(name);
    }
}