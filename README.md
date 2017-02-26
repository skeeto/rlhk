# Roguelike Header Kit (RLHK)

The Roguelike Header Kit is a collection of public domain, portable,
efficient, C header libraries for creating roguelikes. If your goal is
a terminal / console roguelike, then you won't need any additional
libraries — no ncurses, libtcod, etc — and since these are header
libraries it's trivial to integrate without any linking headaches.

## Supported platforms

* POSIX (ANSI terminal with UTF-8 encoding)
* Win32 console
* MS-DOS (DJGPP, requires [DPMI server][dpmi], e.g. cwsdpmi.exe)

Assuming the proper compilers are available, try out the demo on
different platforms:

    make CC=x86_64-linux-gnu-gcc
    make CC=x86_64-w64-mingw32-gcc
    make CC=i586-msdosdjgpp-gcc

The demo can also be built using Visual Studio's cl.exe, though not
with this Makefile.

## Design Philosophy

RLHK is designed for single-threaded roguelikes that completely block
on user input. While this isn't *strictly* required, should you want
to use multiple threads, me mindful that RLHK functions are neither
thread-safe nor re-entrant.


[dpmi]: http://www.delorie.com/djgpp/v2faq/faq4_4.html
