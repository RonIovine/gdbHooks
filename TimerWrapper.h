#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////
//
// TimerWrapper.h
//
// This timer wrapper module will encapsulate access to the system get time
// functions and will adjust the value returned for any time spent stopped
// at a GDB breakpoint.  This will allow for a timer based process to be run
// in the GDB debugger while still maintaining all the timing characteristics
// of the process when it is running in normal operational mode.  All calls
// to get the system time from the process must be done via these functions.
//
// When running within GDB, GDB must be started with the timerHooks file to
// install the proper GDB callback hooks for timer management.  Here is an
// example of of onvoking GDB for your target process and timerHooks:
//
// $ gdb myProcess -x timerHooks
//
// The timerHooks file is provided with this package.  See the README file
// that is included in this package for a full description of the components,
// building, running etc.
//
////////////////////////////////////////////////////////////////////////////////

// time unit conversion constants
#define USEC_PER_SEC 1000000
#define USEC_PER_MSEC 1000

// this function has the exact same prototype and usage as the
// normal system gettimeofday function but will adjust the returned
// values based on any time spent at a GDB breakpoint
int getTimeOfDay(struct timeval *tv, struct timezone *tz);

// this function will return the number of usec since the epoch
// adjusted for any time spent at a GDB breakpoint
unsigned long long getEpochTimeUsec(void);

// this function will return the number of usec since the epoch
// adjusted for any time spent at a GDB breakpoint
unsigned long long getEpochTimeMsec(void);

// this function will return the number of usec since the epoch
// adjusted for any time spent at a GDB breakpoint
unsigned long long getEpochTimeSec(void);

// set the programs start time, this will establish a baseline
// for all future calls to all the below getElapsedTimeXXX
// functions, this function should be called as the very first
// thing in the program's 'main', if it is called at other
// places in the program's execution, it will re-establish the
// baseline for all future elapsed time calculations
void setStartTime(void);

// returns the program's start time baseline
unsigned long long getStartTime(void);

// this function will return the elapsed program run time in
// usec since the start of the program execution, adjusted
// for any time spent at a GDB breakpoint, in order for this
// to work correctly, the 'setStartTime' function must be
// called as the very first thing done in the program's 'main'
unsigned long long getElapsedTimeUsec(void);

// this function will return the elapsed program run time in
// msec since the start of the program execution, adjusted
// for any time spent at a GDB breakpoint, in order for this
// to work correctly, the 'setStartTime' function must be
// called as the very first thing done in the program's 'main'
unsigned long long getElapsedTimeMsec(void);

// this function will return the elapsed program run time in
// sec since the start of the program execution, adjusted
// for any time spent at a GDB breakpoint, in order for this
// to work correctly, the 'setStartTime' function must be
// called as the very first thing done in the program's 'main'
unsigned long long getElapsedTimeSec(void);

