# auto writeup

Let's connect and check out what we got:

```
$ nc auto.tghack.no 2270
f0VMRgIBAQAAAAAAAAAAAAIAPgABAAAAQAZAAAAAAABAAAAAAAAAAEgbAAAAAAAAAAAAAEAAOAAJAEAAHwAc
[...]
Fresh base64 for you <3
Password:
Too slow!
```

Well, that's a lot of stuff. The task description says we have to solve 10 binaries, so this needs to be scripted.

But first we can check out one the base64 encoded binaries.

```
$ chmod +x ./dec
$ ltrace ./dec
__libc_start_main(0x400736, 1, 0x7ffe85eb61f8, 0x400800 <unfinished ...>
printf("Password: ")                                  = 10
fgets(Password: test
"test\n", 64, 0x7fdf7f9728e0)                   = 0x7ffe85eb60c0
strcspn("test\n", "\n")                               = 4
strcmp("test", "4Y17QCx")                             = 64
puts("Try again :(\n"Try again :(

)                                = 14
+++ exited (status 0) +++
```

After looking at a few other samples as well, we see that all of them check against a hard-coded password. No fancy XOR-ing etc. There are several ways to solve this challenge, for example by using strings and extracting the correct password.

I chose to solve this with [angr](http://angr.io/), which is a binary analysis framework. We can tell angr to find the correct path through the program, thus giving us the password.

Here's the main code:
```python
p = angr.Project("./dec", load_options={"auto_load_libs": False})
pg = p.factory.path_group()
pg.explore(find=lambda p: "You did it!!!" in p.state.posix.dumps(1))

s = pg.found[0].state
flag = s.posix.dumps(0)
flag = flag.split("\x00", 1)[0]
print "flag: {}".format(flag)
```
(full code [here](solve_dec.py))

We're looking for a path that gives us the string "You did it!!!" in `stdout`, so that we don't depend on hardcoded addresses.

Angr is a little slow, but after around 40-50 seconds, we get the flag:
```
$ python2 solve_dec.py
[...]
Congrats! The flag is TG17{bin4ries_3verywhere_wh4t_t0_d0}
```
