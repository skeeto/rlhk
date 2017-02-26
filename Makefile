.POSIX:
CC     = cc
CFLAGS = -ansi -pedantic -Os -Wall -Wextra -Wno-unused-function -g3
LDLIBS = -lm
SUFFIX =

.PHONY : all clean
.SUFFIXES: .c $(SUFFIX)

all: demo/game$(SUFFIX) demo/rand$(SUFFIX)

demo/game$(SUFFIX): demo/game.c rlhk_tui.h rlhk_rand.h
demo/rand$(SUFFIX): demo/rand.c rlhk_tui.h rlhk_rand.h

.c$(SUFFIX):
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f demo/game$(SUFFIX) demo/rand$(SUFFIX)
