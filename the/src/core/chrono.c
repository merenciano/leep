#include "chrono.h"
#include "definitions.h"
#include <time.h>

/*
	Returns the duration of the crono, if THE_ChronoEnd has been called for the chrono
	it returns end-start. If end has not been called it returns the current duration.
*/
static timespec Duration(THE_Chrono *chrono)
{
	timespec ret;
	timespec tmp;
	if (chrono->end.tv_nsec == 0) {
		clock_gettime(2, &tmp);
	} else {
		tmp = chrono->end;
	}
	ret.tv_sec = tmp.tv_sec - chrono->start.tv_sec;
	ret.tv_nsec = tmp.tv_nsec - chrono->start.tv_nsec;
	return ret;
}

void THE_ChronoStart(THE_Chrono *chrono)
{
	//2 is CLOCK_PROCESS_CPUTIME_ID
	clock_gettime(2, &chrono->start);
	chrono->end.tv_nsec = 0;
}

void THE_ChronoEnd(THE_Chrono *chrono)
{
	THE_ASSERT(chrono->end.tv_nsec == 0, "Calling ChronoEnd before ChronoStart");
	clock_gettime(2, &chrono->end);
}

float THE_ChronoDurationSec(THE_Chrono *c)
{
	return THE_ChronoDurationMS(c) / 1000.0f;
}

float THE_ChronoDurationMS(THE_Chrono *c)
{
	timespec time = Duration(c);
	s64 nsec = time.tv_nsec + time.tv_sec * 1000000000;
	return (float)nsec / 1000000.0f;
}
