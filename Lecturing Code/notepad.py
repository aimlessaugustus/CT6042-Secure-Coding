from sympy import factorint
from Crypto.Util.number import inverse, long_to_bytes

# Given values
n = 43941819371451617899582143885098799360907134939870946637129466519309346255747

# Factor n
factors = factorint(n)
p, q = factors.keys()
print("Prime factors:")
print("p =", p)
print("q =", q)

# Calculate phi(n)
phi_n = (p - 1) * (q - 1)
print("Phi(n) =", phi_n)

# Calculate private key d
e = 65537
d = inverse(e, phi_n)
print("Private key (d):", d)

c = 9002431156311360251224219512084136121048022631163334079215596223698721862766

plaintext = pow(c, d, n)
flag = long_to_bytes(plaintext)
print(flag.decode())
print("Decrypted Plaintext:", flag)
