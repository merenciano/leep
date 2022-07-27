#ifndef __LEEP_TOOLS_RESOURCE_MAP_H__
#define __LEEP_TOOLS_RESOURCE_MAP_H__

#include "core/leep-string.h"
#include "render/Crenderertypes.h"
#include "core/memory/buddy-alloc-stl.h"
#include "core/hmap.h"

#include <unordered_map>

struct THE_ResourceMap
{
	THE_ResourceMap() = default;
	~THE_ResourceMap() = default; // TODO: Remove resources from the GPU
	THE_ResourceMap(const THE_ResourceMap& rm) = delete;
	THE_ResourceMap(THE_ResourceMap&& rm) = delete;

	THE_HMap *meshes;

	/*std::unordered_map<
		leep::String,
		THE_Mesh,
		std::hash<leep::String>,
		std::equal_to<leep::String>,
		leep::BuddySTL<std::pair<const leep::String, THE_Mesh>>> meshes;*/
	std::unordered_map<
		leep::String,
		THE_Texture,
		std::hash<leep::String>,
		std::equal_to<leep::String>,
		leep::BuddySTL<std::pair<const leep::String, THE_Texture>>> textures;
};

void THE_ResourceMapAddMesh(THE_ResourceMap* rm, const char *name, const char *path);
void THE_ResourceMapAddMesh(THE_ResourceMap* rm, const char *name, THE_Mesh mesh);
THE_Mesh THE_ResourceMapGetMesh(THE_ResourceMap *rm, const char *name);
void THE_ResourceMapAddTexture(THE_ResourceMap* rm, leep::String name, leep::String path, THE_TexType type);
void THE_ResourceMapAddTexture(THE_ResourceMap* rm, leep::String name, u32 width, u32 height, THE_TexType type);

#endif // __LEEP_TOOLS_RESOURCE_MAP_H__