from macho_pass import MachoPass
from crc32 import crc32
import logging

class MachoChecksummer(MachoPass):
    def __init__(self, bin_name):
        super(MachoChecksummer, self).__init__(bin_name)

    def get_checksum(self, arch, addr, size):
        data = arch.read(addr, size)
        return crc32(0, data)

    def print_checksums(self):
        logging.debug("Calculating checksums...")
        for arch in self.arches:
            print "Arch"
            for segment in arch.segments:
                #print segment.name + ": ", hex(self.get_checksum(arch, segment.vmaddr, segment.vmsize))

                if segment.name.strip("\x00") == "__TEXT":
                    for i in xrange(0, segment.nsects):
                        section = segment.sections[i]
                        #print "\t" + section.name + ":", hex(self.get_checksum(arch, section.addr, section.size))
                        print hex(self.get_checksum(arch, section.addr, section.size)) + ", ",

    def get_arch_checksum(self, arch, segment_name, section_name=None):
        for segment in arch.segments:
            if segment.name.strip("\x00") != segment_name:
                continue

            if section_name is None:
                return self.get_checksum(arch, segment.vmaddr, segment.vmsize)

            for i in xrange(0, segment.nsects):
                section = segment.sections[i]
                if section_name.strip("\x00") == section_name:
                    return self.get_checksum(arch, section.addr, section.size)
            return -1
        return -1

    def find_arch_magic_values(self, arch, struct_pack, s):
        logging.debug("Finding the magic...")

        addr = 0
        while addr != -1:
            addr = arch.find_num(struct_pack, addr)
            yield addr
