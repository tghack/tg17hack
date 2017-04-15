CC = gcc
CFLAGS = -Wall -Wextra -z norelro

.PHONY: all, clean
all: shelf

shelf: shelf.c
	$(CC) $(CFLAGS) -o $@ $^

run: shelf
	./$^

clean:
	rm -f shelf core.*
