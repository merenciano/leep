#ifndef THE_CORE_EXECUTION_H
#define THE_CORE_EXECUTION_H

#include "core/config.h"

void THE_Init(THE_Config *cnfg);
void THE_Logic();
void THE_Close();
void THE_Render();
void THE_ShowFrame();

#endif
