CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99 -D_FORTIFY_SOURCE=2 -z relro, now -fstack-protector=strong

.PHONY: all clean

all: server client

server: server.c
	$(CC) $(CFLAGS) $^ -o $@

client: client__FOR_TESTING.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f server client

