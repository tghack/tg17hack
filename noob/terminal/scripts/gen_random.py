import string
import random

charset = "ABCDEFGHIJKLMNOPQRS" + string.digits
flag = "TG17{Let's_s0rt_this_out}"

data = ""
for i in range(970):
    a = ''.join(random.choice(charset) for _ in range(30))
    data += a + "\n"

data += flag + "\n"
for i in range(1029):
    a = ''.join(random.choice(charset) for _ in range(30))
    data += a + "\n"

with open("/home/noob/level5/out.txt", "w") as f:
    f.write(data)
