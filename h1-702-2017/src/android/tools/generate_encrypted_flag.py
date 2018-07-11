#!/usr/bin/env python

from salsa20 import XSalsa20_xor
from nacl.hash import blake2b
import sys

if len(sys.argv) < 5:
    print "usage: generate_encrypted.py <input1> <input2> <input3> <flagoutput>"
    exit(-1)

input1 = sys.argv[1]
input2 = sys.argv[2]
input3 = sys.argv[3]
flagoutput = sys.argv[4]

input1hash = blake2b(input1).decode('hex')
input2hash = blake2b(input2).decode('hex')
input3hash = blake2b(input3).decode('hex')

nonce = input1hash[:12] + input2hash[:12]
key = input3hash

print 'nonce: %s' % nonce.encode('hex')
print 'key: %s' % key.encode('hex')

ciphertext_flagoutput = XSalsa20_xor(flagoutput, nonce, key)
print 'char ciphertext_flagoutput[] = %s' % map(ord, ciphertext_flagoutput)
print XSalsa20_xor(ciphertext_flagoutput, nonce, key)
