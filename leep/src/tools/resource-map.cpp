#include "resource-map.h"
#include "render/Crenderer.h"

using namespace leep;

void THE_ResourceMapAddMesh(THE_ResourceMap* rm, leep::String name, leep::String path)
{
	THE_ASSERT(name != "Cube" && "There is already a cube named Cube");
	THE_ASSERT(name != "Sphere" && "There is already a sphere named Sphere");
	THE_ASSERT(name != "Quad" && "There is already a quad named Quad");

	THE_Mesh mesh = THE_CreateMeshFromFile_OBJ(path.c_str());
	if (!rm->meshes.emplace(std::make_pair(name, mesh)).second)
	{
		THE_LOG_ERROR("Mesh %s couldn't be inserted", name.c_str());
	}
}

void THE_ResourceMapAddMesh(THE_ResourceMap* rm, leep::String name, THE_Mesh mesh)
{
	THE_ASSERT(name != "Cube" && "There is already a cube named Cube");
	THE_ASSERT(name != "Sphere" && "There is already a sphere named Sphere");
	THE_ASSERT(name != "Quad" && "There is already a quad named Quad");

	if (!rm->meshes.emplace(std::make_pair(name, mesh)).second)
	{
		THE_LOG_ERROR("Mesh %s couldn't be inserted", name.c_str());
	}
}

void THE_ResourceMapAddTexture(THE_ResourceMap* rm, leep::String name, leep::String path, THE_TexType type)
{
	THE_ASSERT(!path.empty() && "For the creation of empty textures call with size params");

	THE_Texture tex = THE_CreateTexture(path.c_str(), type);
	if (!rm->textures.emplace(std::make_pair(name, tex)).second)
	{
		THE_LOG_ERROR("Texture %s couldn't be inserted", name.c_str());
	}
}

void THE_ResourceMapAddTexture(THE_ResourceMap* rm, leep::String name, u32 width, u32 height, THE_TexType type)
{
	THE_ASSERT(width > 0 && height > 0 && "0,0 size texture is no texture");
	THE_Texture tex = THE_CreateEmptyTexture(width, height, type);
	if (!rm->textures.emplace(std::make_pair(name, tex)).second)
	{
		THE_LOG_ERROR("Texture %s couldn't be inserted", name.c_str());
	}
}
