# You Spin Me Round writeup

Since only the letters are changed in the flag, we guess that a rotational encryption was used.
We see that the two first letters have both been rotated by 13. Rotating the rest of the letters 
by 13, we get:

```
TG17{rotation_cipher_is_a_classical_cipher}
```

