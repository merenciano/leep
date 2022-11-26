#ifndef THE_MANAGER_H
#define THE_MANAGER_H

#include "config.h"
#include "tools/resource-map.h"
#include "tools/imgui-tools.h"

extern THE_ResourceMap resource_map;

void THE_Init(THE_Config *config);
void THE_NextFrame();
void THE_StartFrameTimer();
float THE_DeltaTime();

#endif
