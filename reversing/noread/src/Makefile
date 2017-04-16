CC=gcc
CFLAGS=-Wall -Wextra -std=gnu11
CFLAGS+=-O3 # for _FORTIFY_SOURCE=2, code must be compiled with -O1 or higher
CFLAGS+=-fstack-protector-strong # strong stack protection
CFLAGS+=-D_FORTIFY_SOURCE=2 	 # protect functions like strcpy, printf, etc
CFLAGS+=-z relro		 # make .got and .dtors read-only
CFLAGS+=-z now	 	# Full RELRO, .got.plt is also marked RO

.PHONY: all run clean

all: dump noread

dump: dump.c
	$(CC) $(CFLAGS) $^ -o $@

noread: noread.c
	$(CC) $(CFLAGS) -fpic -pie $^ -o $@ -Os -s -ldl

run: dump noread
	./$< noread

dist: noread
	cp noread ../run/

clean:
	rm -vf dump noread
	rm -rf dist
