import binascii

start = 0xc0ffee
pw = "passw0rds_s0_fun,no_bruteforce_pls"
enc = ""

for i in range(len(pw)):
    enc += chr((ord(pw[i]) ^ (start + i)) & 0xff)

print "encrypted password: {}".format(binascii.hexlify(enc))

dec_pw = ""
for i in range(len(enc)):
    #print "fuck: {:02x} ^ {:02x}".format(ord(enc[i]), start + i)
    dec_pw += chr((ord(enc[i]) ^ (start + i)) & 0xff)

print "decrypted: {}".format(dec_pw)
assert pw == dec_pw

# encrypt flag with plaintext password
flag = "TG17{no_read_no_write_what_to_doooo}"
enc_flag = ""

for i in range(len(flag)):
    enc_flag += chr((ord(flag[i]) ^ ord(pw[i % len(pw)])) & 0xff)

print "encrypted flag: {}".format(binascii.hexlify(enc_flag))

# test flag decryption
dec_flag = ""

for i in range(len(enc_flag)):
    dec_flag += chr((ord(enc_flag[i]) ^ ord(dec_pw[i % len(dec_pw)])) & 0xff)

print "decrypted flag: {}".format(dec_flag)
assert dec_flag == flag
#":".join("{:02x}".format(ord(c)) for c in s)

print "C stuff"
c_flag = "\\x" + "\\x".join("{:02x}".format(ord(c)) for c in enc_flag)
c_pw = "\\x" + "\\x".join("{:02x}".format(ord(c)) for c in enc)
print "flag: {}".format(c_flag)
print "password: {}".format(c_pw)
