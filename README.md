# Roguelike Header Kit (RLHK)

The Roguelike Header Kit is a collection of public domain, portable,
efficient, C header libraries for creating roguelikes. If your goal is
a terminal / console roguelike, then you won't need any additional
libraries — no ncurses, libtcod, etc — and since these are header
libraries it's trivial to integrate without any linking headaches.

## Design Philosophy

RLHK is designed for single-threaded roguelikes that completely block
on user input. While this isn't *strictly* required, should you want
to use multiple threads, me mindful that RLHK functions are neither
thread-safe nor re-entrant.
