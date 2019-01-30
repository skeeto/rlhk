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
    make CC=x86_64-w64-mingw32-gcc SUFFIX=.exe
    make CC=i586-pc-msdosdjgpp-gcc SUFFIX=.exe

The demo can also be built using Visual Studio's cl.exe, though not
with this Makefile.

## Design Philosophy

RLHK is designed for single-threaded roguelikes that completely blocks
on user input. While this isn't *strictly* required, should you want to
use multiple threads be mindful that RLHK functions are neither
thread-safe nor re-entrant.

For maximum portability, RLHK is written in strict ANSI C89 except for
some isolated bits of platform-specific code. RLHK also uses none of
the `stdio.h` functions, meaning your program won't have to link with
these formatted output functions in a static build.

RLHK functions do not make dynamic allocations. Either static storage
is used, or a work buffer must be passed to the function, such as the
case for `rlhk_algo_shortest()`.

RLHK, being a header library, is designed to be embedded within your
program. This reflected through its API, such as not using function
pointers for callbacks. Its functions are intended to inline naturally
with your program's own functions. For example, a modern optimizing
compiler will inline parts of your `rlhk_algo_map_call()` directly
into the library's functions, eliminating many of those function
calls. Think of it like poor-man's templates but with cleaner
semantics and faster compilation.

## Character Set

Any ASCII character can be used directly as-is. For fancier
characters, `rlhk_tui.h` provides a fully portable character set that
work on the more limited Windows console and MS-DOS. It's essentially
just [code page 437][cp437]. Each character is available in the host
system's encoding via the macros named `RLHK_TUI_<unicode_name>`.


[dpmi]: http://www.delorie.com/djgpp/v2faq/faq4_4.html
[cp437]: https://en.wikipedia.org/wiki/Code_page_437
