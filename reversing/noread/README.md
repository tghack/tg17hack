# noread
**Points: 500**

**Author: PewZ**

No read access for you! Can you still get the flag?

`ssh -p 2222 noread@noread.tghack.no`

The password is `I_promise_to_be_nice`.

And here's a hint to save you some trouble:
```
gdb-peda$ checksec
CANARY    : ENABLED
FORTIFY   : ENABLED
NX        : ENABLED
PIE       : ENABLED
RELRO     : FULL
```
