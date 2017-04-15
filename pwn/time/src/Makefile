CC=gcc
CFLAGS=-Wall -Wextra -std=c99

.PHONY: all run clean
all: time

time: time.c
	$(CC) $(CFLAGS) -o $@ $^

run: time
	./time

clean:
	rm -f time
