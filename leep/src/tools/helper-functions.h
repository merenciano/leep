#ifndef THE_HELPER_FUNCTIONS_H
#define THE_HELPER_FUNCTIONS_H

#include "core/leep-string.h"

void THE_CreateRenderable(leep::String entity_name,
	leep::String geometry_name,
	leep::String texture_name);
void THE_RemoveEntity(leep::String entity_name);
void THE_SetParent(leep::String entity_name, leep::String parent_name);
void THE_DetachFromParent(leep::String entity_name);
void THE_SetLocation(leep::String entity_name, float x, float y, float z);
void THE_SetSunDirection(float x, float y, float z);
void THE_SetSunIntensity(float intensity);

#endif
