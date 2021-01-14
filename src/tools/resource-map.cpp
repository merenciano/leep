#include "resource-map.h"

namespace leep
{
    void ResourceMap::addGeometry(std::string name, std::string path)
    {
        bool inserted = geometries_.emplace(std::make_pair(name, Geometry())).second;
        if (inserted)
        {
            if (path == "")
            {
                geometries_[name].createCube();
            }
            else
            {
                geometries_[name].loadObj(path);
            }
        }
        else
        {
            LEEP_CORE_WARNING("There is already a geometry with that name");
        }
    }

    void ResourceMap::addTexture(std::string name, std::string path)
    {
        bool inserted = textures_.emplace(std::make_pair(name, Texture())).second;
        if (inserted)
        {
            textures_[name].create(path);
        }
        else
        {
            LEEP_CORE_WARNING("There is already a texture with that name");
        }
    }

    Geometry ResourceMap::getGeometry(std::string name) const
    {
        return geometries_.at(name);
    }

    Texture ResourceMap::getTexture(std::string name) const
    {
        return textures_.at(name);
    }
}