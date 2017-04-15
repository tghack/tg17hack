import gmpy2
from Crypto.Util import number

print "Generating primes!"
while True:
    #n_len = 1024
    n_len = 2048

    p = number.getPrime(n_len)
    q = p + 1
    while number.isPrime(q) != True:
        q += 1

    n = p * q

    #phi = (p - 1) * (q - 1)
    phi = n - (p + q - 1)

    e = 3
    if number.GCD(e, phi) == 1:
        break
    print "gcd != 1"

print "p: {}".format(p)
print "q: {}".format(q)
print "n: {}".format(n)

d = number.inverse(e, phi)
print "d: {}".format(d)

# Fermat's factorization
m = gmpy2.mpz(n)
M = m

gmpy2.get_context().precision=2048
a = int(gmpy2.sqrt(m))

a2 = a*a
b2 = gmpy2.sub(a2,M)

while not(gmpy2.is_square(b2)):
    a = a+1
    b2 = a*a-M

b2 = gmpy2.mpz(b2)
gmpy2.get_context().precision=2048
b = int(gmpy2.sqrt(b2))

q2 = a + b
p2 = a - b
print "factored"
print "p2: {}".format(p2)
print "q2: {}".format(q2)

assert q2 == q
assert p2 == p
