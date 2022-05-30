#ifndef __LEEP_TOOLS_RESOURCE_MAP_H__
#define __LEEP_TOOLS_RESOURCE_MAP_H__

#include "render/geometry.h"
#include "render/texture.h"
#include "render/Crenderertypes.h"
#include "core/memory/buddy-alloc-stl.h"

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

        void addGeometry(String name, String path);
        void addGeometry(String name, Geometry geometry);
        void addTexture(String name, String path, THE_TexType type);
        void addTextureAsync(String name, String path, TexType);
        void addTexture(String name, Texture texture);
        void addTexture(String name, u32 width, u32 height, THE_TexType type);
        Geometry getGeometry(String name) const;
        Texture getTexture(String name) const;

        std::unordered_map<
            String,
            Geometry,
            std::hash<String>,
            std::equal_to<String>,
            BuddySTL<std::pair<const String, Geometry>>> geometries_;
        std::unordered_map<
            String,
            FuTexture,
            std::hash<String>,
            std::equal_to<String>,
            BuddySTL<std::pair<const String, FuTexture>>> textures_;

	    std::unordered_map<
		    String,
		    THE_Mesh,
		    std::hash<String>,
		    std::equal_to<String>,
		    BuddySTL<std::pair<const String, THE_Mesh>>> meshes;
	    std::unordered_map<
		    String,
		    THE_Texture,
		    std::hash<String>,
		    std::equal_to<String>,
		    BuddySTL<std::pair<const String, THE_Texture>>> textures;
    };
}

#endif // __LEEP_TOOLS_RESOURCE_MAP_H__