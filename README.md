Notes for the RTEMS-moxie port
==============================

This file contains notes for the ongoing port of NetHack to RTEMS / Moxie.

The goal is to move the port into the regular NetHack build infrastructure so
it can be cleanly folded into the main NetHack codebase. 

In the meantime, some files contain hardcoded changes instead of #ifdefs, etc.

NOTE: Some Makefiles still have hardcoded paths to my personal toolchain
      directories.

RTEMS ncurses
-------------
The RTEMS-moxie port of NetHack depends on RTEMS ncurses that can be found here:

git clone git://git.rtems.org/rtems-addon-packages.git
cd rtems-addon-packages/ncurses-5.9
RTEMS_MAKEFILE_PATH=/home/lagerstrom/moxie-cores/tools/root/usr/moxie-rtems/marin/ CC=moxie-rtems-gcc RTEMS_BSP=marin RTEMS_CPU=moxie make -f ../RTEMS_Makefiles/Makefile.ncurses

This will install RTEMS ncurses into your toolchain.


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
* Generic toolchain paths in Makefiles
* Add timed delay
* Add documentation on how to build the executable
* Compile game data instead of using precompiled datafiles from the MIPS .deb file
* Use the standard directory locations instead of "."
* Compress game data using XZ
* Build ncurses with "-Os" (optimized for size)
* Support for saving/restoring games
* Support for a high score file


Bugs
====
* Intermittent tty output garbage (e.g., when using HJKL for extended move)

