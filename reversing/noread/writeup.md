# noread writeup

Let's connect and see what we got!

```
$ ssh -p 2222 noread@localhost
noread@localhost's password:
Welcome to noread.

Binary is in /home/noread. Use /tmp to develop code.

Good luck!
noread@05ce088488ec:~$ ls
noread
noread@05ce088488ec:~$ file noread
noread: executable, regular file, no read permission
noread@05ce088488ec:~$ ./noread
You are fantastic
Password please: test
nope :(
$ ls -lah noread
---------x 5 root root 11K Mar  4 10:42 noread
```

Only execute permissions, ouch!

Let's summarize all the information we have:

1. no read permission
2. execute permission
3. PIE

If the file hadn't been a position independent executable (PIE), we could have used available tools to dump the whole binary. However, we don't know the memory location of anything anymore :(

Let's create a test program and check what's going on when running a PIE.
```C
#include <stdio.h>

int main(void)
{
	printf("heeeey\n");
	return 0;
}
```
(`test.c`)


(some output is omitted.)
```
$ gcc test.c -pie -fPIE -o test
$ gdb ./test
gdb-peda$ b main
Breakpoint 1 at 0x555555554754
gdb-peda$ r
Breakpoint 1, 0x0000555555554754 in main ()
gdb-peda$ bt
#0  0x0000555555554754 in main ()
#1  0x00007ffff7a2e830 in __libc_start_main (main=0x555555554750 <main>, argc=0x1,
    argv=0x7fffffffe5d8, init=<optimized out>, fini=<optimized out>,
    rtld_fini=<optimized out>, stack_end=0x7fffffffe5c8) at ../csu/libc-start.c:291
#2  0x0000555555554649 in _start ()
```

We can see that the program has to go through `__libc_start_main` (from, you guessed it, `libc`). This information is useful if we want to trace the program. There's no need to be `root` to trace another process, as long as we control the parent process.

I chose to implement a tiny tracer program that checks the upper 32 bits of the current address of `rip` (the instruction pointer) for any changes. This allows us to pause execution when we hit `main()`. When we have the address of `main()`, we can search backwards in memory for the ELF header.

Here's the code that looks for changes in the address of `rip`:
```C
unsigned long long prev_rip;
prev_rip = regs.rip;
unsigned long long mask = 0xffffffff00000000;

for (;;) {
	if (proc_get_regs(pid, &regs))
		return;

	if ((regs.rip & mask) != (prev_rip & mask)) {
		prev_rip = regs.rip;
		break;
	}

	prev_rip = regs.rip;
	proc_step_and_wait(pid);
}
```

And here's the code that looks for the ELF header:
```C
/* align on page size */
unsigned long long addr;
addr = ((prev_rip + 4096 - 1) / 4096) * 4096;
const char *elf_head = "\x7f\x45\x4c\x46";

for (;;) {
	unsigned long val = proc_read_word(pid, (void *)addr);
	if (memmem(&val, sizeof(unsigned long), elf_head, 4))
		break;

	addr -= 8;
}
```

One thing to note is that the linker (?) only loads the first page of the binary into memory (unless it's very big, maybe?):
```
gdb-peda$ info proc
process 17228
gdb-peda$ cat /proc/17228/maps
555555554000-555555555000 r-xp 00000000 fd:01 801208                     /home/pewz/tghack/test
```

Now we can read one page (4096 bytes) from the process memory and dump it to a file.
Let's transfer the code over and test it on the challenge server:

```
noread@bffac925c2c3:/tmp$ gcc dump.c -o dump
noread@bffac925c2c3:/tmp$ ./dump /home/noread/noread
[+] Initialization done! Entering trace loop
[+] process done with execve!
[+] Job's done!
noread@bffac925c2c3:/tmp$ base64 test.bin
f0VMRgIBAQAAAAAAAAAAAAMAPgABAAAAUAkAAAAAAABAAAAAAAAAAKghAAAAAAAAAAAAAEAAOAAJ
[...]
```

Copy the base64 over to our machine, and lets check it out!

```
$ base64 -D b64 > noread.bin
$ file noread.bin
noread.bin: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, stripped
```

Looks good, but! It doesn't run :( And gdb won't touch it either. Neither will objdump.


Throwing it into IDA should work though, and it does! We can even reverse the password-checking function:
```C
int __fastcall check_password(const char *a1)
{
	char *v1; // rsi@1
	const char *input; // r8@1
	unsigned __int64 v3; // rdx@1
	char *v4; // rdi@1
	unsigned __int64 v5; // kr18_8@1
	int result; // eax@2
	signed int v7; // ecx@3
	__int64 i; // rax@3
	__int64 v9; // rax@6
	char v10[35]; // [sp+5h] [bp-33h]@3
	__int64 v11; // [sp+28h] [bp-10h]@1

	v1 = (char *)-1;
	input = a1;
	v11 = 0x21A8LL;
	v3 = strlen(a1);
	v5 = strlen((const char *)&unk_202020) + 1;
	v4 = (char *)&unk_202020 + v5;
	if ( v3 == v5 - 1 )
	{
		v1 = v10;
		memset(v10, 0, sizeof(v10));
		v4 = (char *)&v11;
		v7 = 0xEE;
		for ( i = 0LL; (signed int)v3 > (signed int)i; ++i )
		{
			LOBYTE(v4) = input[i] ^ v7++;
			v10[i] = (char)v4;
		}
		v9 = 0LL;
		while ( (signed int)v3 > (signed int)v9 )
		{
			LOBYTE(v4) = v10[v9++];
			if ( (_BYTE)v4 != *((_BYTE *)&unk_20201F + v9) )
				goto LABEL_2;
		}
		result = 1;
	}
	else
	{
	LABEL_2:
		result = 0;
	}
	if ( v11 != 0x21A8 )
		result = sub_808((__int64)v4, (__int64)v1, v11 ^ 0x21A8);
	return result;
}
```


Looks like some XOR stuff. However, there's one tiny problem. We don't have access to `unk_20201F`, which is the XOR-ed password.

Let's look at the opcode right there: `48 8D 0D 25 15 20 00`
We can use capstone or something similar to disassemble it:
```
0xaf3:	lea	rcx, qword ptr [rip + 0x201525]
```

Instead of finding the ELF header and then dumping one page, we can find the ELF header and read some bytes at offset `0xaf3 + 0x201525 = 0x202018`.

Change the code that dumps the ELF:
```C
/* get XORed password */
size_t xor_offset = 0x202018;
char buf[64] = { 0 };
proc_read_bytes(pid, (char *)addr + xor_offset, buf, sizeof(buf));
FILE *fp = fopen("password.bin", "wb+");
assert(fp);
fwrite(buf, sizeof(buf), 1, fp);
```

```
$ xxd password.bin
00000000: 0000 0000 0000 0000 9e8e 8382 85c3 8691  ................
00000010: 85a8 8bc9 a59d 8993 d291 6f5e 6071 7171  ..........o^`qqq
00000020: 6361 677b 696e 537d 627c 0000 0000 0000  cag{inS}b|......
00000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................
```

After removing the NULL bytes, we can reverse the encryption routine from the binary:

```python
import base64

pw = "AAAAAAAAAACejoOChcOGkYWoi8mlnYmT0pFvXmBxcXFjYWd7aW5TfWJ8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=="

pw = base64.b64decode(pw).replace("\00", "")

start = 0xee
dec = ""
for i in range(len(pw)):
	dec += chr((ord(pw[i]) ^ (start + i)) & 0xff)

	print "password: {}".format(dec)
```

Run the python script:
```
$ python2 decrypt.py
password: passw0rds_s0_fun,no_bruteforce_pls
```

ssh back into the server and use the new password:
```
$ $ ssh -p 2222 noread@noread.tghack.no
Welcome to noread.

Binary is in /home/noread. Use /tmp to develop code.

Good luck!
$ ./noread
You are fantastic
Password please: passw0rds_s0_fun,no_bruteforce_pls
yay
flag: TG17{no_read_no_write_what_to_doooo}
```
