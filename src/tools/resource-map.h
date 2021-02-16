#ifndef __LEEP_TOOLS_RESOURCE_MAP_H__
#define __LEEP_TOOLS_RESOURCE_MAP_H__

#include "render/geometry.h"
#include "render/texture.h"

#include <string>
#include <unordered_map>

namespace leep
{
    class ResourceMap
    {
    public:
        ResourceMap() = default;
        ~ResourceMap() = default; // TODO: Remove resources from the GPU
        ResourceMap(const ResourceMap &rm) = delete;
        ResourceMap(ResourceMap &&rm) = delete;

        void addGeometry(std::string name, std::string path);
        void addTexture(std::string name, std::string path, TexType t);
        Geometry getGeometry(std::string name) const;
        Texture getTexture(std::string name) const;

        std::unordered_map<std::string, Geometry> geometries_;
        std::unordered_map<std::string, Texture> textures_;
    };
}

#endif // __LEEP_TOOLS_RESOURCE_MAP_H__