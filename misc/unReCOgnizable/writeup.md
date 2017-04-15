#Unrecognizable writeup

The picture contains the hex-representation of a binary file. The executable outputs the flag. 

Generate all valid two-byte hex values.
```python
for i in range(256):
    print hex(i)[2:].upper()
```
(`gen_hex.py`)

And store them in `tessdata`.
```
$ python2 gen_hex.py > words
$ sudo cp words /usr/share/tesseract-ocr/tessdata/eng.user-words
```

And set up the `hex` config:
```
load_system_dawg	F
load_freq_dawg		F
user_words_suffix	user-words
user_patterns_suffix	user-patterns
```
(`/usr/share/tesseract-ocr/tessdata/configs/hex`)

Enough config already! Let's run `tesseract`:
`tesseract ocr_shit.png stdout -c tessedit_char_whitelist=0123456789ABCDEF -c load_system_dawg=false -c load_freq_dawg=false hex` 

The OCR reader converts to text which is almost right. We used the python script below to generate an ELF-file of the text. 

```python
import binascii

with open("out.bin", "r") as f:
    data = f.read()

data = data.replace(" ", "").replace("\n", "")

#data += "0"
print data

with open("ocr_binary.elf", "wb+") as f:
    f.write(binascii.unhexlify(data))
```


Great, now we have an executable! Let's try and run it:
```
$ chmod +x ocr_binary.elf
$ ./ocr_binary.elf 
  exec format error: ./ocr_binary.elf
```

Ouch! So there's something wrong with our binary. Looking at the strings, we can see that the OCR tool didn't recognize all the characters properly.

```
$ strings ocr_binary.elf
[...]
ma`n
.`n`t
.f`n`
```

Looks like all the i's have been replaced by backticks. If we replace all the obvious errors in the strings, we can use `objdump` to look at the code.
```
$ objdump -M intel -D ocr_binary.elf | grep -A 8 main
0000000000400576 <main>:
  400576:	55                   	push   rbp
  400577:	48 80 e5 53          	rex.W and bpl,0x53
  40057b:	48 83 ec 18          	sub    rsp,0x18
  40057f:	48 c7 45 e8 68 06 40 	mov    QWORD PTR [rbp-0x18],0x400668
  400586:	00
  400587:	c7 45 e4 00 00 00 00 	mov    DWORD PTR [rbp-0x1c],0x0
  40058e:	eb 21                	jmp    4005b1 <main+0x3b>
  400590:	8b 45 e4             	mov    eax,DWORD PTR [rbp-0x1c]
  400593:	48 63 d0             	movsxd rdx,eax
  400596:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  40059a:	48 01 d0             	add    rax,rdx
  40059d:	0f b6 00             	movzx  eax,BYTE PTR [rax]
  4005a0:	83 f0 05             	xor    eax,0x5
  4005a3:	0f be c0             	movsx  eax,al
  4005a6:	80 c7 e8             	add    bh,0xe8
--
  4005c6:	72 c8                	jb     400590 <main+0x1a>
  4005c8:	bf 0a 00 00 00       	mov    edi,0xa
  4005cd:	e8 6e fe ff ff       	call   400440 <_libc_csu_init+0x38>
  4005d2:	b8 00 00 00 00       	mov    eax,0x0
  4005d7:	48 83 c4 18          	add    rsp,0x18
  4005db:	5b                   	pop    rbx
  4005dc:	5d                   	pop    rbp
  4005dd:	c3                   	ret
  4005de:	66 00 41 57          	data16 add BYTE PTR [rcx+0x57],al
```

We can see some obvious errors in the code, like `rex.W and bpl,0x53`. But most of it looks good. It's most likely a xor-decryption routine, using the string at address `0x400668`.

```
$ strings -t x ocr_binary.elf | grep 668
    668 QB42~5uq4fd`Zg4kdw|Zw6f5bk4q45kx
```

Replace the backtick with i:
```
$ python2 -c 'print "".join(chr(ord(c) ^ 0x5) for c in "QB42~5uq4fdiZg4kdw|Zw6f5bk4q45kx")'
TG17{0pt1cal_b1nary_r3c0gn1t10n}
```

*EDIT:* With newer versions of tesseract, there's no need to do any reversing :)
