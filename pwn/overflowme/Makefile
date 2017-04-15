CC=gcc
CFLAGS=-Wall -Wextra -std=gnu11

.PHONY: all clean
all: overflowme

overflowme: overflowme.c
	$(CC) $(CFLAGS) $^ -o $@ -DTOKEN="\"TG17{0verfl0ws_are_c00l}\""
	cp $@ run

dummy: overflowme.c
	$(CC) $(CFLAGS) $^ -o $@ -DTOKEN="\"NOPE\""

clean:
	rm -f overflowme dummy
