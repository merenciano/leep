#ifndef THE_HELPER_FUNCTIONS_H
#define THE_HELPER_FUNCTIONS_H

#include "core/definitions.h"

void THE_CreateEntity(const char *mesh, const char *tex);
void THE_SetLocation(s32 entity_idx, float x, float y, float z);
void THE_SetSunDirection(float x, float y, float z);
void THE_SetSunIntensity(float intensity);

#endif
