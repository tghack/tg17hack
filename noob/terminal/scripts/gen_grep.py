import string
import random

charset = string.lowercase + string.digits
flag = "TG17{grepping_all_day}"

data = ""
for i in range(1029):
    a = ''.join(random.choice(charset) for _ in range(len(flag)))
    data += a + "\n"

data += flag + "\n"
for i in range(970):
    a = ''.join(random.choice(charset) for _ in range(len(flag)))
    data += a + "\n"

with open("/home/noob/level5/grep.txt", "w") as f:
    f.write(data)
