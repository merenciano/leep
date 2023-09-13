#ifndef THE_ENTITY_H
#define THE_ENTITY_H

#include "render/renderer.h"

typedef struct {
	struct mat4 transform;
	THE_Mesh mesh;
	THE_Material mat;
} THE_Entity;

THE_Entity *THE_EntityCreate(void);
THE_Entity *THE_GetEntities(void);
int32_t THE_EntitiesSize(void);
void THE_RenderEntities(THE_Entity *entities, int32_t count);

#endif

