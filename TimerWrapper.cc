#include <sys/time.h>
#include <unistd.h>

#include "TimerWrapper.h"

//////////////////////////
// private data members
//////////////////////////

static unsigned long long startTime       = 0;   // program start time (actual)
static unsigned long long stopTime        = 0;   // breakpoint stop time (actual)
static unsigned long long breakpointTime  = 0;   // accumulated breakpoint time

/////////////////////////////////
// private function prototypes
/////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// returns the actual, raw, unadjusted epoch time in usec
////////////////////////////////////////////////////////////////////////////////
static unsigned long long getActualTimeUsec(void);

////////////////////////////////////////////////////////////////////////////////
// GDB timer callback hooks
////////////////////////////////////////////////////////////////////////////////
static void stopProgram(void);
static void continueProgram(void);

///////////////////////////////
// public functions
///////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// this function has the exact same prototype and usage as the normal system
// gettimeofday function but will adjust the returned values based on any time
// spent at a GDB breakpoint
////////////////////////////////////////////////////////////////////////////////
int getTimeOfDay(struct timeval *tv, struct timezone *tz)
{
  // get current epoch time
  int retCode = gettimeofday(tv, tz);
  // convert to usec and adjust time for the amount we've spent at a breakpoint
  unsigned long long adjustedTime = (tv->tv_sec*USEC_PER_SEC+tv->tv_usec)-breakpointTime;
  // reset adjusted values back into the tv structure
  tv->tv_sec = adjustedTime/USEC_PER_SEC;
  tv->tv_usec = adjustedTime%USEC_PER_SEC;
  return (retCode);
}

////////////////////////////////////////////////////////////////////////////////
// this function will return the number of usec since the epoch adjusted for
// any time spent at a GDB breakpoint
////////////////////////////////////////////////////////////////////////////////
unsigned long long getEpochTimeUsec(void)
{
  // adjust our run time to factor out the time we've spent at breakpoints
  return (getActualTimeUsec()-breakpointTime);
}

////////////////////////////////////////////////////////////////////////////////
// this function will return the number of usec since the epoch adjusted for
// any time spent at a GDB breakpoint
////////////////////////////////////////////////////////////////////////////////
unsigned long long getEpochTimeMsec(void)
{
  // adjust our run time to factor out the time we've spent at breakpoints
  return (getEpochTimeUsec()/USEC_PER_MSEC);
}

////////////////////////////////////////////////////////////////////////////////
// this function will return the number of usec since the epoch adjusted for
// any time spent at a GDB breakpoint
////////////////////////////////////////////////////////////////////////////////
unsigned long long getEpochTimeSec(void)
{
  // adjust our run time to factor out the time we've spent at breakpoints
  return (getEpochTimeUsec()/USEC_PER_SEC);
}

////////////////////////////////////////////////////////////////////////////////
// set the programs start time, this will establish a baseline for all future
// calls to all the below getElapsedTimeXXX functions, this function should be
// called per-the example in the gdbHooksDemo.cc program.  It should be called
// before the 'main' as part of a class bases instantiation.
////////////////////////////////////////////////////////////////////////////////
void setStartTime(void)
{
  startTime = getActualTimeUsec();
}

////////////////////////////////////////////////////////////////////////////////
// returns the program's start time baseline
////////////////////////////////////////////////////////////////////////////////
unsigned long long getStartTime(void)
{
  return (startTime);
}

////////////////////////////////////////////////////////////////////////////////
// this function will return the elapsed program run time in usec since the
// start of the program execution, adjusted for any time spent at a GDB
// breakpoint, in order for this to work correctly, the 'setStartTime' function
// must be called as per the example in the gdbDemoHooks.cc program
////////////////////////////////////////////////////////////////////////////////
unsigned long long getElapsedTimeUsec(void)
{
  // get our epoch time and factor out our program start time for our program elapsed time
  return (getEpochTimeUsec()-startTime);
}

////////////////////////////////////////////////////////////////////////////////
// this function will return the elapsed program run time in msec since the
// start of the program execution, adjusted for any time spent at a GDB
// breakpoint, in order for this to work correctly, the 'setStartTime' function
// must be called as per the example in the gdbDemoHooks.cc program
////////////////////////////////////////////////////////////////////////////////
unsigned long long getElapsedTimeMsec(void)
{
  // get our epoch time and factor out our program start time for our program elapsed time
  return (getElapsedTimeUsec()/USEC_PER_MSEC);
}

////////////////////////////////////////////////////////////////////////////////
// this function will return the elapsed program run time in sec since the
// start of the program execution, adjusted for any time spent at a GDB
// breakpoint, in order for this to work correctly, the 'setStartTime' function
// must be called as per the example in the gdbDemoHooks.cc program
////////////////////////////////////////////////////////////////////////////////
unsigned long long getElapsedTimeSec(void)
{
  // get our epoch time and factor out our program start time for our program elapsed time
  return (getElapsedTimeUsec()/USEC_PER_SEC);
}

///////////////////////////////
// private functions
///////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// get the raw, unadjusted time since the epoch in usec
////////////////////////////////////////////////////////////////////////////////
static unsigned long long getActualTimeUsec(void)
{
  struct timeval timeOfDay;
  gettimeofday(&timeOfDay, NULL);
  // convert to microseconds and return
  return ((unsigned long long)(timeOfDay.tv_sec*USEC_PER_SEC + timeOfDay.tv_usec));
}

/////////////////////////////////
// GDB timer callback hooks
/////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// the next two functions are the GDB callback hooks which get installed
// via the file 'timerHooks' which is loaded at the GDB command line startup
// via the '-x' command line GDB option
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// this is called upon program stop to lay down our actual stop time, i.e.
// when a breakpoint is hit or after a step/next command etc.
////////////////////////////////////////////////////////////////////////////////
static void stopProgram(void)
{
  // lay down the actual time at program stop
  stopTime = getActualTimeUsec();
}

////////////////////////////////////////////////////////////////////////////////
// this is called when the program is continued to accumulate the total time
// spent at a breakpoint, i.e. after a continue, next, step command etc.
////////////////////////////////////////////////////////////////////////////////
static void continueProgram(void)
{
  // accumulate the total time we've spent at breakpoints
  breakpointTime += getActualTimeUsec() - stopTime;
}

