# Transmission control writeup

We are given a file with the following data:

```
B: UmVxdWVzdGluZyBzZWN1cmUgY29ubmVjdGlvbg==
A: SW5pdGlhdGluZyBwcm90b2NvbDogQUVTKFNIQTI1NihFQ0RIKSwgQ0JDLCBJVik=
A: TWVzc2FnZSBmb3JtYXQ6IElWICsgQUVTKG1lc3NhZ2Up
A: NDIwMTczNjM1MDY0LCA0NTU4NjA4NzQ4NDA1LCAxNTUxNzQ5NTIyNzE4Nw==
A: MTM4NDczNzExOTEyOTIsIDcxNzgyMzE4NjYzNzU=
B: MTI3NDAxMjcxMDk5OTIsIDI2NTc5OTgzNTE3NTg=
A: MTIxMDgzMTQxMDQyNjksIDc3NTE4MzM2NzUzMjc=
B: VYOMsCq1yqv0J+8UMe0pFaCtUi5EFlrhqKh+Y4bmR1MvoGzgSxAI6ZD8QJV0VFl9
A: atU5UvHp5bRbUMeIiJLVxkdLaGF8tivfSKlGpOJQvLRE/74LKZ9DD42xTE6mwbmk2P7Po2Y2Ryh80aYAJY8atA==
B: grl0/CNUkhes9mdZPsLieUC5/kWx3G0n/yEuQNbE+Ao=
B: IvDZ++ZH9jx3LGCJzkS0TbZYz1N6siy2OD/A0xQbuLk=
A: wXEf37wwVGHuYaoD/nULOoUN7JBA9Kq/rH6Rduj/ZW7oLbUfQcPW/0Zto5rqI0T+
```
We can see that this is base64 encoded by the ='s at the end of the lines. So we decode it:

```
B: Requesting secure connection
A: Initiating protocol: AES(SHA256(ECDH), CBC, IV)
A: Message format: IV + AES(message)
A: 420173635064, 4558608748405, 15517495227187
A: 13847371191292, 7178231866375
B: 12740127109992, 2657998351758
A: 12108314104269, 7751833675327
B: U\x83\x8c\xb0*\xb5\xca\xab\xf4'\xef\x141\xed)\x15\xa0\xadR.D\x16Z\xe1\xa8\xa8~c\x86\xe6GS/\xa0l\xe0K\x10\x08\xe9\x90\xfc@\x95tTY}
A: j\xd59R\xf1\xe9\xe5\xb4[P\xc7\x88\x88\x92\xd5\xc6GKha|\xb6+\xdfH\xa9F\xa4\xe2P\xbc\xb4D\xff\xbe\x0b)\x9fC\x0f\x8d\xb1LN\xa6\xc1\xb9\xa4\xd8\xfe\xcf\xa3f6G(|\xd1\xa6\x00%\x8f\x1a\xb4
B: \x82\xb9t\xfc#T\x92\x17\xac\xf6gY>\xc2\xe2y@\xb9\xfeE\xb1\xdcm'\xff!.@\xd6\xc4\xf8\n
B: "\xf0\xd9\xfb\xe6G\xf6<w,`\x89\xceD\xb4M\xb6X\xcfSz\xb2,\xb68?\xc0\xd3\x14\x1b\xb8\xb9
A: \xc1q\x1f\xdf\xbc0Ta\xeea\xaa\x03\xfeu\x0b:\x85\r\xec\x90@\xf4\xaa\xbf\xac~\x91v\xe8\xffen\xe8-\xb5\x1fA\xc3\xd6\xffFm\xa3\x9a\xea#D\xfe
```
Ok, so we see that some of the messages makes sense, but some does not. The last messages are probably encrypted, so we will keep them as base64 for now since it is easier to handle them this way. This gives us the following transmission:
```
B: Requesting secure connection
A: Initiating protocol: AES(SHA256(ECDH), CBC, IV)
A: Message format: IV + AES(message)
A: 420173635064, 4558608748405, 15517495227187
A: 13847371191292, 7178231866375
B: 12740127109992, 2657998351758
A: 12108314104269, 7751833675327
B: VYOMsCq1yqv0J+8UMe0pFaCtUi5EFlrhqKh+Y4bmR1MvoGzgSxAI6ZD8QJV0VFl9
A: atU5UvHp5bRbUMeIiJLVxkdLaGF8tivfSKlGpOJQvLRE/74LKZ9DD42xTE6mwbmk2P7Po2Y2Ryh80aYAJY8atA==
B: grl0/CNUkhes9mdZPsLieUC5/kWx3G0n/yEuQNbE+Ao=
B: IvDZ++ZH9jx3LGCJzkS0TbZYz1N6siy2OD/A0xQbuLk=
A: wXEf37wwVGHuYaoD/nULOoUN7JBA9Kq/rH6Rduj/ZW7oLbUfQcPW/0Zto5rqI0T+
```
Since we are pro and know every abbreviation in the world (or, you know, google it), we can see that the messages are encrypted with AES in CBC mode (cipher block chaining) with a SHA256 hash as key. The hash is a shared secret generated with Elliptic Curve Diffie-Hellman key exchange. The IV is prepended to the encrypted message. 

So how do we attack this? The numbers given must be variables for initializing ECDH. We try with:
```
a = 420173635064
b = 4558608748405
p = 15517495227187
G = [13847371191292, 7178231866375]
publicB = [12740127109992, 2657998351758]
publicA = [12108314104269, 7751833675327]
```
We notice that these numbers are quite small, so it might be possible to calculate the discrete logarithm of the public keys. [sage](http://www.sagemath.org/) is nice for working on elliptic curves:
```
sage: a = 420173635064
sage: b = 4558608748405
sage: p = 15517495227187
sage: E = EllipticCurve(GF(p), [a,b])
sage: G = E(13847371191292, 7178231866375)
sage: publicB = E(12740127109992, 2657998351758)
sage: publicA = E(12108314104269, 7751833675327)
sage: logB = G.discrete_log(publicB)
sage: logB
3011267900038
sage: logA = G.discrete_log(publicA)
sage: logA
545529006017
sage: logA * publicB
(27356762530 : 7671609982327 : 1)
sage: logB * publicA
(27356762530 : 7671609982327 : 1)
```
Success! We now have the shared secret! The x-coordinate is usually used, so we will try that as the key. A little bit of python should do it:
```
import base64
from Crypto.Cipher import AES
from Crypto.Hash import SHA256

secret = "27356762530"
key = SHA256.new()
key.update(secret)

def decrypt(enc):
    enc = base64.b64decode(enc)
    iv = enc[:16]
    cipher = AES.new(key.digest(), AES.MODE_CBC, iv)
    return cipher.decrypt(enc[16:])

print(decrypt("VYOMsCq1yqv0J+8UMe0pFaCtUi5EFlrhqKh+Y4bmR1MvoGzgSxAI6ZD8QJV0VFl9"))
print(decrypt(atU5UvHp5bRbUMeIiJLVxkdLaGF8tivfSKlGpOJQvLRE/74LKZ9DD42xTE6mwbmk2P7Po2Y2Ryh80aYAJY8atA=="))
print(decrypt("grl0/CNUkhes9mdZPsLieUC5/kWx3G0n/yEuQNbE+Ao="))
print(decrypt("IvDZ++ZH9jx3LGCJzkS0TbZYz1N6siy2OD/A0xQbuLk="))
print(decrypt("wXEf37wwVGHuYaoD/nULOoUN7JBA9Kq/rH6Rduj/ZW7oLbUfQcPW/0Zto5rqI0T+"))
```
We know that the IV is 16 bytes because the block size of AES is always 128 bits. Running the script gives us the flag:
```
$ python2 decrypt.py
Send me flag plz
Here you go: TG17{large_primes_are_nice}
Thanks!									
End connection
Connection closed
```
(If you see some random data at the end of the messages after decrypting, it is because AES encrypts messages in fixed size blocks. So the clear text is padded to match the block size before encrypting.)
