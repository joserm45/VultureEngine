#ifndef _j1PERFTIMER_H_
#define _j1PERFTIMER_H_

#include "Globals.h"
#include "SDL\include\SDL.h"
//#include "p2Defs.h"
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

class j1PerfTimer
{
public:

	// Constructor
	j1PerfTimer();

	void Start();
	double ReadMs() const;
	Uint64 ReadTicks() const;

private:
	Uint64	started_at;
	static Uint64 frequency;
};

#endif //_j1PERFTIMER_H__