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
        bool inserted =
            textures_.emplace(std::make_pair(name, Texture())).second;
        if (inserted)
        {
            if (path != "")
                textures_[name].create(path, t);
            else
                textures_[name].createEmpty(512, 512, t);
        }
        else
        {
            LEEP_CORE_WARNING("There is already a texture with that name");
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