# gdbHooks
**A small package to show gdb hooks for breakpoint safe system time based service access**

[Overview](#overview)<br>
[Building](#building)<br>
[Setup](#setup)<br>
[CLI Mode](#cliMode)<br>
[Emacs Mode](#emacsMode)<br>

<a name="overview"></a>
### Overview
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

<a name="building"></a>
### Building
To build the demo program, from this directory run:

`$ g++ -g TimerWrapper.cc gdbHooksDemo.cc -o gdbHooksDemo`

<a name="setup"></a>
### Setup
To setup for emacs use, copy the included `.emacs` file to your home directory and edit
the file and edit the file per the instructions in the comments.  Also, place the shell
scripts `gdbCli` and `gdbEmacs` and the `timerHooks` file in other locations as necessary
and edit the `gdbCli` and `gdbEmacs` files and change the location of the `timerHooks` file
as necessary.

<a name="cliMode"></a>
### CLI Mode

This section describes running the demo program in command line mode.  There is
a simple shell script wrapper around the 'gdb' command line program.

Run the program from within gdb in CLI mode as follows:

`$ gdbCli gdbHooksDemo`

Once within the gdb prompt, set a breakpoint at the function 'foo' as follows:

`(gdb) break foo`

Once the breakpoint is set, you can just successively run continue as follows to see the
actual time, and the adjusted time based on the time spent at a breakpoint/stopped state
as shown below.  As can be seen by comparing the actual and adjusted times, regardless of
how much time is spent at a breakpoint, the adjusted time will only reflect the actual
process run time.

```
(gdb) continue
Actual Elapsed Time: 133324184 usec, Adjusted Elapsed Time: 11112 usec
Actual Elapsed Time: 133324 msec, Adjusted Elapsed Time: 11 msec
Actual Elapsed Time: 133 sec, Adjusted Elapsed Time: 0 sec
Actual Time Of Day: 12-22-2019 13:22:29.428603, Adjusted Time Of Day: 12-22-2019 13:20:16.115524

(gdb) continue
Actual Elapsed Time: 135933179 usec, Adjusted Elapsed Time: 1022161 usec
Actual Elapsed Time: 135933 msec, Adjusted Elapsed Time: 1022 msec
Actual Elapsed Time: 135 sec, Adjusted Elapsed Time: 1 sec
Actual Time Of Day: 12-22-2019 13:22:32.037598, Adjusted Time Of Day: 12-22-2019 13:20:17.126578

(gdb) continue
Actual Elapsed Time: 141762466 usec, Adjusted Elapsed Time: 2032749 usec
Actual Elapsed Time: 141762 msec, Adjusted Elapsed Time: 2032 msec
Actual Elapsed Time: 141 sec, Adjusted Elapsed Time: 2 sec
Actual Time Of Day: 12-22-2019 13:22:37.866885, Adjusted Time Of Day: 12-22-2019 13:20:18.137166

(gdb) continue
Actual Elapsed Time: 202398215 usec, Adjusted Elapsed Time: 3046006 usec
Actual Elapsed Time: 202398 msec, Adjusted Elapsed Time: 3046 msec
Actual Elapsed Time: 202 sec, Adjusted Elapsed Time: 3 sec
Actual Time Of Day: 12-22-2019 13:23:38.502634, Adjusted Time Of Day: 12-22-2019 13:20:19.150423

(gdb) continue
Actual Elapsed Time: 226610267 usec, Adjusted Elapsed Time: 4057473 usec
Actual Elapsed Time: 226610 msec, Adjusted Elapsed Time: 4057 msec
Actual Elapsed Time: 226 sec, Adjusted Elapsed Time: 4 sec
Actual Time Of Day: 12-22-2019 13:24:02.714686, Adjusted Time Of Day: 12-22-2019 13:20:20.161889

(gdb) continue
Actual Elapsed Time: 369408255 usec, Adjusted Elapsed Time: 5068584 usec
Actual Elapsed Time: 369408 msec, Adjusted Elapsed Time: 5068 msec
Actual Elapsed Time: 369 sec, Adjusted Elapsed Time: 5 sec
Actual Time Of Day: 12-22-2019 13:26:25.512674, Adjusted Time Of Day: 12-22-2019 13:20:21.173001
```

<a name="emacsMode"></a>
### Emacs Mode
This section describes running programs via the Emacs/GUD GDB graphical front end.  This has
been tested with the following versions of emacs and gdb:

GNU Emacs 24.3.1<br>
GNU gdb (Ubuntu 7.7.1-0ubuntu5~14.04.3) 7.7.1

There is a simple shell script wrapper around the 'gdb' command line program.

Run the program from within gdb in Emacs/GUD mode as the graphical front end as follows:

`$ gdbEmacs gdbHooksDemo`

***IMPORTANT NOTE: When running via the Emacs/GUD based front end DO NOT use any of
the graphical buttons (i.e. 'run', 'continue' etc) for program control and execution.
This is because with the latest Emacs/GUD/GDB intgegration, it uses the gud-mi based
interface, so all of the gdb commands are preceeded with the '-exec-' prefix, i.e.
'-exec-continue' as opposed to just 'continue' etc.  I have not found a way to add
gdb callback hooks for any of the '-exec-' based command formats.  If anyone knows
of a way to do this, I would greatly appreciate any info on this.***

Once emacs is up, go to the gdb command line window and enter the same commands as the above
CLI based example.



