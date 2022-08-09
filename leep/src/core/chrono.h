#ifndef __THE_CORE_CHRONO_H__
#define __THE_CORE_CHRONO_H__

#include "Cdefinitions.h"
#include <time.h>

typedef struct THE_Chrono
{
	timespec start;
	timespec end;
} THE_Chrono;

void THE_ChronoStart(THE_Chrono *chrono);
void THE_ChronoEnd(THE_Chrono *chrono);

float THE_ChronoDurationSec(THE_Chrono *chrono);
float THE_ChronoDurationMS(THE_Chrono *chrono);

#endif
