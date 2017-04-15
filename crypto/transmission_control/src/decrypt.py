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
print(decrypt("atU5UvHp5bRbUMeIiJLVxkdLaGF8tivfSKlGpOJQvLRE/74LKZ9DD42xTE6mwbmk2P7Po2Y2Ryh80aYAJY8atA=="))
print(decrypt("grl0/CNUkhes9mdZPsLieUC5/kWx3G0n/yEuQNbE+Ao="))
print(decrypt("IvDZ++ZH9jx3LGCJzkS0TbZYz1N6siy2OD/A0xQbuLk="))
print(decrypt("wXEf37wwVGHuYaoD/nULOoUN7JBA9Kq/rH6Rduj/ZW7oLbUfQcPW/0Zto5rqI0T+"))

