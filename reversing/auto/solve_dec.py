import angr
import base64
import sys
from pwn import *

def solve_binary(r):
    try:
        data = r.recvuntil("<3")
    except:
        print r.recv()
        sys.exit(0)

    data = data.split()[0]

    print data
    with open("dec", "w+") as f:
        f.write(base64.b64decode(data))

    p = angr.Project("./dec", load_options={"auto_load_libs": False})
    pg = p.factory.path_group()
    #pg.explore(avoid=0x400724, find=0x400718)
    pg.explore(find=lambda p: "You did it!!!" in p.state.posix.dumps(1))
    s = pg.found[0].state
    flag = s.posix.dumps(0)
    flag = flag.split("\x00", 1)[0]
    print "len: {}".format(len(flag))
    print "flag: {}".format(flag)
    r.send(flag)

def main():
    r = remote("localhost", 5555)
    while True:
        solve_binary(r)
        r.recvline()
        res = r.recvline()
        if "Correct" in res:
            print "Solved it!"
        else:
            print "We failed :("
            break

def test():
    pass

if __name__ == "__main__":
    main()
