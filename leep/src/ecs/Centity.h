#ifndef THE_ENTITY_H
#define THE_ENTITY_H

#include "render/Crenderer.h"

typedef struct THE_Entity {
	struct mat4 transform;
	THE_Mesh mesh;
	THE_Material mat;
} THE_Entity;

THE_Entity *THE_EntityCreate();
THE_Entity *THE_GetEntities();
s32 THE_EntitiesSize();
void THE_RenderEntities(THE_Entity *entities, s32 count);

#endif
