.POSIX:
CC     = cc
CFLAGS = -ansi -pedantic -Os -Wall -Wextra -g3
LDLIBS = -lm

rlhk_demo: rlhk_demo.c rlhk_tui.h rlhk_rand.h
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ rlhk_demo.c $(LDLIBS)

clean:
	rm -f rlhk_demo rlhk_demo.exe
