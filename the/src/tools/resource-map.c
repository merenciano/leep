#include "resource-map.h"
#include "render/renderer.h"

void THE_ResourceMapAddMeshFromPath(THE_ResourceMap *rm, const char *name, const char *path)
{
	THE_Mesh mesh = THE_CreateMeshFromFile_OBJ(path);
	THE_HMapInsert(rm->meshes, name, &mesh);
}

void THE_ResourceMapAddMesh(THE_ResourceMap *rm, const char *name, THE_Mesh mesh)
{
	THE_HMapInsert(rm->meshes, name, &mesh);
}

THE_Mesh THE_ResourceMapGetMesh(THE_ResourceMap *rm, const char *name)
{
	void *result = THE_HMapGet(rm->meshes, name);
	THE_ASSERT(result != THE_HMAP_INVALID_VALUE, "Invalid result");
	return *(THE_Mesh*)result;
}

void THE_ResourceMapAddTextureFromPath(THE_ResourceMap *rm, const char *name, const char *path, THE_TexType type)
{
	THE_ASSERT(path, "For the creation of empty textures call with size params");
	THE_Texture tex = THE_CreateTexture(path, type);
	THE_HMapInsert(rm->textures, name, &tex);
}

void THE_ResourceMapAddTexture(THE_ResourceMap *rm, const char *name, u32 width, u32 height, THE_TexType type)
{
	THE_ASSERT(width > 0 && height > 0, "0,0 size texture is no texture");
	THE_Texture tex = THE_CreateEmptyTexture(width, height, type);
	THE_HMapInsert(rm->textures, name, &tex);
}

THE_Texture THE_ResourceMapGetTexture(THE_ResourceMap *rm, const char *name)
{
	void *result = THE_HMapGet(rm->textures, name);
	THE_ASSERT(result != THE_HMAP_INVALID_VALUE, "Invalid result");
	return *(THE_Texture*)result;
}
