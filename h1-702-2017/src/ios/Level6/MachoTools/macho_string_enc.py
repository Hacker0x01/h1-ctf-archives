from macho_pass import MachoPass


def keyed_xor(text, xor_key):
    return "".join([chr(ord(x) ^ ord(y)) for x, y in zip(text, xor_key * (len(text) / len(xor_key)))])


class MachoStringEnc(MachoPass):
    def __init__(self, bin_name, key):
        super(MachoStringEnc, self).__init__(bin_name)
        self.key = key

    def enc_arch_strings(self, arch):
        for section in arch.sections:
            if section.name.strip("\x00") != "__cfstring" and \
               section.name.strip("\x00") != "__cstring":
                continue

            section_strings = arch.read(section.addr, section.size)
            enc_strings = keyed_xor(section_strings, self.key)
            arch.write(section.addr, enc_strings)
