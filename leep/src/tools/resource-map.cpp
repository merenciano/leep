#include "resource-map.h"
#include "render/renderer.h"
#include "render/Crenderer.h"

namespace leep
{
void ResourceMap::addGeometry(String name, String path)
{
	THE_ASSERT(name != "Cube" && "There is already a cube named Cube");
	THE_ASSERT(name != "Sphere" && "There is already a sphere named Sphere");
	THE_ASSERT(name != "Quad" && "There is already a quad named Quad");

	THE_Mesh mesh = THE_CreateMeshFromFile_OBJ(path.c_str());
	if (!meshes.emplace(std::make_pair(name, mesh)).second)
	{
		THE_LOG_ERROR("Mesh %s couldn't be inserted", name.c_str());
	}
}

void ResourceMap::addGeometry(String name, THE_Mesh mesh)
{
	THE_ASSERT(name != "Cube" && "There is already a cube named Cube");
	THE_ASSERT(name != "Sphere" && "There is already a sphere named Sphere");
	THE_ASSERT(name != "Quad" && "There is already a quad named Quad");

	if (!meshes.emplace(std::make_pair(name, mesh)).second)
	{
		THE_LOG_ERROR("Mesh %s couldn't be inserted", name.c_str());
	}
}

void ResourceMap::addTexture(String name, String path, THE_TexType t)
{
	THE_ASSERT(!path.empty() && "For the creation of empty textures call with size params");

	THE_Texture tex = THE_CreateTexture(path.c_str(), t);
	if (!textures.emplace(std::make_pair(name, tex)).second)
	{
		THE_LOG_ERROR("Texture %s couldn't be inserted", name.c_str());
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
		THE_LOG_ERROR("Texture %s couldn't be inserted", name.c_str());
	}
}
}