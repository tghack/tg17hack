# Insanity Check Writeup

First, turn the string into a binary file:

```
import binascii

s = "\x40\x40\xe3\xc7\xf1\xf7\xc0\xa3\x88\xf1\xa2\x6d\x89\xa2\x6d\x81\x6d\xa2\xa3\xa4\x97\x89\x84\x6d\x86\x93\xf4\x87\xd0\x25"
with open("insanity.bin", "wb+") as f:
    f.write(binascii.unhexlify())
```

Check out the file:
```
$ file insanity.bin
insanity.bin: Non-ISO extended-ASCII text, with no line terminators
```

Hmmm, let's google a bit.

One of the first links on Google when searching for `Non-ISO extended-ASCII text, with no line terminators` is this one:
[non iso extended ascii text](http://superuser.com/questions/669700/non-iso-extended-ascii-text/669729).

One of the answers there gives us a nice way to find and try all available encodings. We can modify it a bit, and simply grep for `TG17{`, which should precede all flags:
```
$ iconv --list | sed 's/\/\/$//' | sort > encodings.list
$ for a in `cat encodings.list`; do
    iconv -f $a -t UTF-8 insanity.bin | grep TG17{ && break; done
  TG17{th1s_is_a_stupid_fl4g}
```
