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

  gettimeofday(&actualTimeOfDay, NULL);
  nowtime = actualTimeOfDay.tv_sec;
  nowtm = localtime(&nowtime);
  strftime(tmbuf, sizeof tmbuf, "%m-%d-%Y %H:%M:%S", nowtm);
  snprintf(actualTimeOfDayBuf, sizeof actualTimeOfDayBuf, "%s.%06ld", tmbuf, actualTimeOfDay.tv_usec);

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

int main(int argc, char *argv[])
{
  // set our program start time baseline for elapsed time calculations
  setStartTime();
  for (;;)
  {
    foo();
  }
  return (0);
}
