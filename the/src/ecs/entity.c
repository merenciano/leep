#include "entity.h"
#include "core/definitions.h"
#include "render/rendercommands.h"

static THE_Entity entities[256];
static s32 entities_last;

THE_Entity *THE_EntityCreate()
{
	return entities + entities_last++;
}

THE_Entity *THE_GetEntities()
{
	return entities;
}

s32 THE_EntitiesSize()
{
	return entities_last;
}

void THE_RenderEntities(THE_Entity *entities, s32 count)
{
	THE_ASSERT(entities && count, "Nothing to render.");

	THE_RenderCommand *command_list = THE_AllocateCommand();
	THE_RenderCommand *prev = command_list;
	THE_MaterialSetModel(&(entities->mat), (float*)&(entities->transform));
	prev->data.draw.mat = &(entities->mat);
	prev->data.draw.mesh = entities->mesh;
	prev->data.draw.inst_count = 1;
	prev->execute = THE_DrawExecute;

	for (s32 i = 1; i < count; ++i)
	{
		THE_RenderCommand *com = THE_AllocateCommand();

		THE_MaterialSetModel(&(entities[i].mat), (float*)&(entities[i].transform));
		com->data.draw.mat = &(entities[i].mat);
		com->data.draw.mesh = entities[i].mesh;
		com->data.draw.inst_count = 1;
		com->next = NULL;
		com->execute = THE_DrawExecute;

		prev->next = com;
		prev = com;
	}

	THE_AddCommands(command_list);
}