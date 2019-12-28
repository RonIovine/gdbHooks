#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>

#include "TimerWrapper.h"

using namespace std;

void foo(void)
{
  struct timeval actualTimeOfDay;
  struct timeval adjustedTimeOfDay;
  time_t nowtime;
  struct tm *nowtm;
  char tmbuf[64];
  char actualTimeOfDayBuf[64];
  char adjustedTimeOfDayBuf[64];

  // get the actual time of day by using the real system gettimeofday function
  gettimeofday(&actualTimeOfDay, NULL);
  nowtime = actualTimeOfDay.tv_sec;
  nowtm = localtime(&nowtime);
  strftime(tmbuf, sizeof tmbuf, "%m-%d-%Y %H:%M:%S", nowtm);
  snprintf(actualTimeOfDayBuf, sizeof actualTimeOfDayBuf, "%s.%06ld", tmbuf, actualTimeOfDay.tv_usec);

  // get the breakpoint adjusted time of day by calling the wrapper funcition getTimeOfDay
  getTimeOfDay(&adjustedTimeOfDay, NULL);
  nowtime = adjustedTimeOfDay.tv_sec;
  nowtm = localtime(&nowtime);
  strftime(tmbuf, sizeof tmbuf, "%m-%d-%Y %H:%M:%S", nowtm);
  snprintf(adjustedTimeOfDayBuf, sizeof adjustedTimeOfDayBuf, "%s.%06ld", tmbuf, adjustedTimeOfDay.tv_usec);

  unsigned long long actualTime = (unsigned long long)(actualTimeOfDay.tv_sec*USEC_PER_SEC + actualTimeOfDay.tv_usec);
  cout<<"Actual Elapsed Time: "<<actualTime-getStartTime()<<" usec, Adjusted Elapsed Time: "<<getElapsedTimeUsec()<<" usec"<<endl;
  cout<<"Actual Elapsed Time: "<<(actualTime-getStartTime())/USEC_PER_MSEC<<" msec, Adjusted Elapsed Time: "<<getElapsedTimeMsec()<<" msec"<<endl;
  cout<<"Actual Elapsed Time: "<<(actualTime-getStartTime())/USEC_PER_SEC<<" sec, Adjusted Elapsed Time: "<<getElapsedTimeSec()<<" sec"<<endl;
  cout<<"Actual Time Of Day: "<<actualTimeOfDayBuf<<", Adjusted Time Of Day: "<<adjustedTimeOfDayBuf<<endl;
  sleep(1);
}

// IMPORTANT!! Create a class that we can instantiate in global space so we can
// lay down our initial startTime before we hit the breakpoint at 'main', which
// will cause us to lay down our initial program startTime, if we don't do this
// and set the initial startTime in 'main', we end up with an initial program
// startTime that is LATER than our first breakpoint stopTime, causing all of
// our subsequent time adjustments to be incorrect
struct SetStartTime {SetStartTime(){setStartTime();};};
SetStartTime startTime;

int main(int argc, char *argv[])
{
  for (;;)
  {
    foo();
  }
  return (0);
}
