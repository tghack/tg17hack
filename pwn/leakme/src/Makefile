CC=gcc
CFLAGS=-Wall -Wextra -std=gnu11

.PHONY: all clean

all: leakme

leakme: leakme.c libc_base.c
	$(CC) $(CLFAGS) $^ -o $@

run: leakme
	socat TCP-LISTEN:4444,fork,reuseaddr EXEC:./leakme,stderr

test: test.c libc_base.c
	$(CC) $(CLFAGS) $^ -o $@

pwn: leakme
	python2 leak.py

clean:
	rm -f leakme
