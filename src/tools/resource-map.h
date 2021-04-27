#ifndef __LEEP_TOOLS_RESOURCE_MAP_H__
#define __LEEP_TOOLS_RESOURCE_MAP_H__

#include "render/geometry.h"
#include "render/texture.h"
#include "core/memory/buddy-alloc-stl.h"

#include <string>
#include <unordered_map>
#include <future>

namespace leep
{
    struct FuTexture
    {
        std::future<void> fut_;
        Texture tex_;
    };

    class ResourceMap
    {
    public:
        ResourceMap() = default;
        ~ResourceMap() = default; // TODO: Remove resources from the GPU
        ResourceMap(const ResourceMap &rm) = delete;
        ResourceMap(ResourceMap &&rm) = delete;

        void addGeometry(std::string name, std::string path);
        void addGeometry(std::string name, Geometry geometry);
        void addTexture(std::string name, std::string path, TexType type);
        void addTextureAsync(std::string name, std::string path, TexType);
        void addTexture(std::string name, Texture texture);
        void addTexture(std::string name, float width, float height, TexType type);
        Geometry getGeometry(std::string name) const;
        Texture getTexture(std::string name) const;

        std::unordered_map<
            std::string,
            Geometry,
            std::hash<std::string>,
            std::equal_to<std::string>,
            BuddySTL<std::pair<const std::string, Geometry>>> geometries_;
        std::unordered_map<
            std::string,
            FuTexture,
            std::hash<std::string>,
            std::equal_to<std::string>,
            BuddySTL<std::pair<const std::string, FuTexture>>> textures_;
    };
}

#endif // __LEEP_TOOLS_RESOURCE_MAP_H__