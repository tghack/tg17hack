import gmpy2
import sys

def fermat_factor(n):
	assert n % 2 != 0  # Odd integers only

	#a = gmpy2.ceil(gmpy2.sqrt(n))
	a = gmpy2.isqrt(n)
	b2 = gmpy2.square(a) - n
	while not is_square(b2):
		a += 1
		b2 = gmpy2.square(a) - n

	factor1 = a + gmpy2.sqrt(b2)
	factor2 = a - gmpy2.sqrt(b2)
	return int(factor1), int(factor2) 

def is_square(n):
	root = gmpy2.sqrt(n)
	return root % 1 == 0  # '4.0' will pass, '4.1212' won't

n = gmpy2.mpz(int(sys.argv[1], 10))
N = n

gmpy2.get_context().precision=2048
a=int(gmpy2.sqrt(n))

a2=a*a
b2=gmpy2.sub(a2,N)

while not(gmpy2.is_square(b2)):
    a=a+1
    b2=a*a-N

b2=gmpy2.mpz(b2)
gmpy2.get_context().precision=2048
b = int(gmpy2.sqrt(b2))

print "q: {}".format(a+b)
print "p: {}".format(a-b)
