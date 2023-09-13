#ifndef THE_MANAGER_H
#define THE_MANAGER_H

#include "config.h"
#include "tools/resource-map.h"
#include "tools/imgui-tools.h"

extern THE_ResourceMap resource_map;

void THE_InitManager(THE_Config *config);
void THE_NextFrame(void);
void THE_StartFrameTimer(void);
float THE_DeltaTime(void);

#endif

