import string
import random

charset = "ABCDEFGHIJKLMNOPQRS" + string.digits
flag = "TG17{I'm_such_a_unique_snowflake}"

# generate 100 random strings

data = []
for i in range(970):
    data.append(''.join(random.choice(charset) for _ in range(len(flag))))

f = open("/home/noob/level5/uniq.txt", "w+")

pos = 0
for i in range(len(data)):
    if pos == len(data) / 2:
        f.write(flag + "\n")
    for j in range(100):
        f.write(data[i] + "\n")
    pos += 1

f.close()
