#!/usr/bin/env python
import sys
import struct

from macho_checksummer import *
from macho_string_enc import *
import logging

logging.basicConfig(level=logging.INFO)

# Checksummer Pass Demo
"""
checksummer = MachoChecksummer(sys.argv[1])
checksummer.print_checksums()

for arch in checksummer.arches:
    text_checksum = checksummer.get_arch_checksum(arch, "__TEXT")

    arch_endian = ">" if arch.big_endian else "<"

    for magic_addr in checksummer.find_arch_magic_values(arch, "I", 0x12345678):
        logging.debug("Changing magic value at {} to {}".format(hex(magic_addr), hex(text_checksum)))
        arch.write(magic_addr, struct.pack(arch_endian + "Q", text_checksum))

    for name, addr in arch.symbols_by_name.iteritems():
        if name == "_its_magic":
            logging.debug("Changing magic value at {} to {}".format(hex(addr), hex(text_checksum)))

            new_num = struct.pack(arch_endian + "Q", text_checksum)
            bytes_written = arch.write(addr, new_num)
            if bytes_written != len(new_num):
                logging.error("Did not write expected number of bytes!")


checksummer.save(sys.argv[1])
"""

checksummer = MachoChecksummer(sys.argv[1])
checksummer.print_checksums()
# Checksummer Pass Demo

# If this key is changed, needs to be updated in StringDecryption.c
# for the demo app
"""
encryption_key = "lookatmeimmmakey123"
string_encryptor = MachoStringEnc(sys.argv[1], encryption_key)

for arch in string_encryptor.arches:
    string_encryptor.enc_arch_strings(arch)

string_encryptor.save(sys.argv[1])
"""
