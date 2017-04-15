CC=gcc
CFLAGS=-Wall -Wextra -std=gnu99

.PHONY: all clean

all: BIOS

#det heter BIOS som enda et hint til at det er IO-relatert...
BIOS: file.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f BIOS make_me

