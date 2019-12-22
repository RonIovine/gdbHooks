# gdbHooks
A small package to show gdb hooks for breakpoint safe system time based service access

This package contians code and examples for building a timer based process
with the capability to be run withing GDB and still maintain all its original
timing characteristics.  It includes a simple module, TimerWrapper.h/cc, that
will encapsulate all access to the system get time functions and will adjust
the value returned for any time spent stopped at a GDB breakpoint.  This module
should be built/linked with an application.  All calls to get the system time
from the process must be done via these functions.

There is a demo program provided that can be run to show the functionality of
this package.  This demo program has a simple infinite loop with a call to a
function, 'foo', that will print out the actual system time and the adusted
time, and then sleep for 1 second.

To build the demo program, from this directory run:

$ g++ -g TimerWrapper.cc gdbHooksDemo.cc -o gdbHooksDemo

Then run the program from within gdb as follows:

$ gdb gdbHooksDemo -x timerHooks

Once within the gdb prompt, set a breakpoint at the function 'foo' as follows:

(gdb) break foo

Once the breakpoint is set, run the program as follows:

Breakpoint 1 at 0x400c77: file gdbHooksDemo.cc, line 11.
(gdb) run
Starting program: /home/riovine/tools/gdbHooks/gdbHooksDemo
#0  foo () at gdbHooksDemo.cc:11
11  {
(gdb)

Once the program is stopped at the breakpoint, you can just successively run continue
as follows to see the actual time, and the adjusted time based on the time spent at
a breakpoint/stopped state as shown below.  As can be seen by comparing the actual and
adjusted times, regardless of how much time is spent at a breakpoint, the adjusted time
will only reflect the actual process run time.

(gdb) continue
#0  <function called from gdb>
Continuing.
Actual Elapsed Time: 133324184 usec, Adjusted Elapsed Time: 11112 usec
Actual Elapsed Time: 133324 msec, Adjusted Elapsed Time: 11 msec
Actual Elapsed Time: 133 sec, Adjusted Elapsed Time: 0 sec
Actual Time Of Day: 12-22-2019 13:22:29.428603, Adjusted Time Of Day: 12-22-2019 13:20:16.115524
#0  foo () at gdbHooksDemo.cc:11
11  {
(gdb) continue
#0  <function called from gdb>
Continuing.
Actual Elapsed Time: 135933179 usec, Adjusted Elapsed Time: 1022161 usec
Actual Elapsed Time: 135933 msec, Adjusted Elapsed Time: 1022 msec
Actual Elapsed Time: 135 sec, Adjusted Elapsed Time: 1 sec
Actual Time Of Day: 12-22-2019 13:22:32.037598, Adjusted Time Of Day: 12-22-2019 13:20:17.126578
#0  foo () at gdbHooksDemo.cc:11
11  {
(gdb) continue
#0  <function called from gdb>
Continuing.
Actual Elapsed Time: 141762466 usec, Adjusted Elapsed Time: 2032749 usec
Actual Elapsed Time: 141762 msec, Adjusted Elapsed Time: 2032 msec
Actual Elapsed Time: 141 sec, Adjusted Elapsed Time: 2 sec
Actual Time Of Day: 12-22-2019 13:22:37.866885, Adjusted Time Of Day: 12-22-2019 13:20:18.137166
#0  foo () at gdbHooksDemo.cc:11
11  {
(gdb) continue
#0  <function called from gdb>
Continuing.
Actual Elapsed Time: 202398215 usec, Adjusted Elapsed Time: 3046006 usec
Actual Elapsed Time: 202398 msec, Adjusted Elapsed Time: 3046 msec
Actual Elapsed Time: 202 sec, Adjusted Elapsed Time: 3 sec
Actual Time Of Day: 12-22-2019 13:23:38.502634, Adjusted Time Of Day: 12-22-2019 13:20:19.150423
#0  foo () at gdbHooksDemo.cc:11
11  {
(gdb) continue
#0  <function called from gdb>
Continuing.
Actual Elapsed Time: 226610267 usec, Adjusted Elapsed Time: 4057473 usec
Actual Elapsed Time: 226610 msec, Adjusted Elapsed Time: 4057 msec
Actual Elapsed Time: 226 sec, Adjusted Elapsed Time: 4 sec
Actual Time Of Day: 12-22-2019 13:24:02.714686, Adjusted Time Of Day: 12-22-2019 13:20:20.161889
#0  foo () at gdbHooksDemo.cc:11
11  {
(gdb)
(gdb) continue
#0  <function called from gdb>
Continuing.
Actual Elapsed Time: 369408255 usec, Adjusted Elapsed Time: 5068584 usec
Actual Elapsed Time: 369408 msec, Adjusted Elapsed Time: 5068 msec
Actual Elapsed Time: 369 sec, Adjusted Elapsed Time: 5 sec
Actual Time Of Day: 12-22-2019 13:26:25.512674, Adjusted Time Of Day: 12-22-2019 13:20:21.173001
#0  foo () at gdbHooksDemo.cc:11
11  {
(gdb)

