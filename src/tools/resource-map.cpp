#include "resource-map.h"
#include "render/renderer.h"

namespace leep
{
    void ResourceMap::addGeometry(std::string name, std::string path)
    {
        LEEP_ASSERT(name != "Cube", "There is already a cube named Cube");
        bool inserted = geometries_.emplace(std::make_pair(name, Geometry())).second;
        if (inserted)
        {
            geometries_[name].loadObj(path);
        }
        else
        {
            LEEP_CORE_WARNING("There is already a geometry with that name");
        }
    }

    void ResourceMap::addTexture(std::string name, std::string path, bool cube)
    {
        bool inserted = textures_.emplace(std::make_pair(name, Texture())).second;
        if (inserted)
        {
            if (cube)
                textures_[name].create(path, true);
            else
                textures_[name].create(path, false);
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
        return geometries_.at(name);
    }

    Texture ResourceMap::getTexture(std::string name) const
    {
        return textures_.at(name);
    }
}