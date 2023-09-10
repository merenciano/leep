#ifndef THE_TOOLS_RESOURCE_MAP_H
#define THE_TOOLS_RESOURCE_MAP_H

#include "core/hmap.h"
#include "render/renderertypes.h"

typedef struct {
	THE_HMap *meshes;
	THE_HMap *textures;
} THE_ResourceMap;

void THE_ResourceMapAddMeshFromPath(THE_ResourceMap *rm, const char *name, const char *path);
void THE_ResourceMapAddMesh(THE_ResourceMap *rm, const char *name, THE_Mesh mesh);
THE_Mesh THE_ResourceMapGetMesh(THE_ResourceMap *rm, const char *name);
void THE_ResourceMapAddTextureFromPath(THE_ResourceMap *rm, const char *name, const char *path, THE_TexType type);
void THE_ResourceMapAddTexture(THE_ResourceMap *rm, const char *name, uint32_t width, uint32_t height, THE_TexType type);
THE_Texture THE_ResourceMapGetTexture(THE_ResourceMap *rm, const char *name);

#endif
