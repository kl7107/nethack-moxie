Notes for the RTEMS-moxie port
==============================

This file contains notes for the ongoing port of NetHack to RTEMS / Moxie.

The goal is to move the port into the regular NetHack build infrastructure so
it can be cleanly folded into the main NetHack codebase. 

In the meantime, some files contain hardcoded changes instead of #ifdefs, etc.

RTEMS ncurses
-------------
The RTEMS-moxie port of NetHack depends on RTEMS ncurses that has been modified
for Moxie.

Game datafiles
--------------
These are normally generated during the compilation of the game.
This is currently not implemented for the Moxie port. Instead precompiled
datafiles were taken from the MIPS .deb file (nethack-common_3.4.3-14_mips.deb).
These were put into a tarfile (./sys/rtems-moxie/dat.tar.xz). 
The "dat.tar" (note: uncompressed) was converted into ./src/fsdata.h using the
./sys/rtems-moxie/tar2h.py script. The tarfile is thus compiled into the game,
and is used to populate the RAM filesystem during boot (in rtems_moxie.c).


TODO
====
* #ifdef all code changes
* Move the Makefiles to the proper place
* Add timed delay
* Add documentation on how to build the executable
* Compile game data instead of using precompiled datafiles from the MIPS .deb file
* Use the standard directory locations instead of "."
* Compress game data using XZ
* Support for saving/restoring games
* Support for a high score file


Bugs
====
* Intermittent tty output garbage (e.g., when using HJKL for extended move)

