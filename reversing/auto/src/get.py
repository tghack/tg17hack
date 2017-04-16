import base64
from pwn import *

r = remote("localhost", 5555)
data = r.recvuntil("<3")
data = data.split()[0]

print data
with open("dec", "w+") as f:
    f.write(base64.b64decode(data))

r.sendline("dritt")
