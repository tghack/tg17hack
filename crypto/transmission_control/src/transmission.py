import base64
from Crypto.Hash import SHA256
import aes

line0 = "Requesting secure connection"
line1 = "Initiating protocol: AES(SHA256(ECDH), CBC, IV)"
line2 = "Message format: IV + AES(message)"
line3 = "420173635064, 4558608748405, 15517495227187"
line4 = "13847371191292, 7178231866375"
line5 = "12740127109992, 2657998351758"
line6 = "12108314104269, 7751833675327"
line7 = "Send me flag plz"
line8 = "Here you go: TG17{large_primes_are_nice}"
line9 = "Thanks!"
line10 = "End connection"
line11 = "Connection closed"

secret = "27356762530"
key = SHA256.new()
key.update(secret)

cipher = aes.AESCipher(key.digest())

print("B: " + base64.b64encode(line0))
print("A: " + base64.b64encode(line1))
print("A: " + base64.b64encode(line2))
print("A: " + base64.b64encode(line3))
print("A: " + base64.b64encode(line4))
print("B: " + base64.b64encode(line5))
print("A: " + base64.b64encode(line6))
print("B: " + cipher.encrypt(line7))
print("A: " + cipher.encrypt(line8))
print("B: " + cipher.encrypt(line9))
print("B: " + cipher.encrypt(line10))
print("A: " + cipher.encrypt(line11))
