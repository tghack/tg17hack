import base64

pw = "AAAAAAAAAACejoOChcOGkYWoi8mlnYmT0pFvXmBxcXFjYWd7aW5TfWJ8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=="

pw = base64.b64decode(pw).replace("\00", "")

start = 0xee
dec = ""
for i in range(len(pw)):
    dec += chr((ord(pw[i]) ^ (start + i)) & 0xff)

print "password: {}".format(dec)
