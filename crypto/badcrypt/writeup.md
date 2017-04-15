# Badcrypt writeup

Okay! Lets see what we've got here!

```
$ nc [host] 9090

```

So there is no instant output here, then lets try input some random stuff!

```
$ nc [host] 9090
asdølkfo
atdølkfo
```

Well, it is almost the same output. The only difference is the letter `s`. Maybe something happens some other letters aswell? Lets try the alphabet a few times!

```
$ nc [host] 9090
abcdefghijklmnopqrstuvwxyz
absdrfghijklmnopq#tyevwxiz

$ nc [host] 9090
abcdefghijklmnopqrstuvwxyz
abudyfghijklmnopqes#rvwxtz

$ nc [host] 9090
abcdefghijklmnopqrstuvwxyz
abrdsfghtjklmnopqic#uvwxuz

$ nc [host] 9090
abcdefghijklmnopqrstuvwxyz
abedyfghcjklmnopqsi#uvwxiz
```

Now we see there are several letters which do not give correct output. There are two ways of thinking to solve this task, lets start with one of them. The letters that were replaced in the alphabet was `ceirstuy`. Hmm, I wonder if it is possible to make a word out of those letters? Hellz yeah, it is! I got `security` which is highly relevant to the CTF, so I tried entering this with the flag format to the website.

```
TG17{security}
```

But.. Oh noes! It didn't work. So then lets look at the letters that replaced the input letters, `sr#tyeiuc`. There was a hashtag there aswell!

```
TG17{#security}
```

Yeyya! We got the right flag!
