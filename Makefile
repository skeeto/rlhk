.POSIX:
CC = cc
CFLAGS = -ansi -pedantic -Os -Wall -Wextra -g3

rlhk_tui_demo: rlhk_tui_demo.c rlhk_tui.h
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ rlhk_tui_demo.c $(LDLIBS)

clean:
	rm -f rlhk_tui_demo rlhk_tui_demo.exe
