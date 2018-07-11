# Copyright (c) 2012-2015 Rusty Wagner
# All rights reserved.

# Redistribution and use in source and binary forms are permitted
# provided that the above copyright notice and this paragraph are
# duplicated in all such forms and that any documentation,
# advertising materials, and other materials related to such
# distribution and use acknowledge that the software was developed
# by the Rusty Wagner. The name of the
# Rusty Wagner may not be used to endorse or promote products derived
# from this software without specific prior written permission.
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

# For documentation on the Mach-O file format
# it is probably easiest just to read
# http://opensource.apple.com//source/xnu/xnu-1456.1.26/EXTERNAL_HEADERS/mach-o/loader.h

from binary_data import *
from structure import *
import logging
import io

CPU_TYPE_I386 = 7
CPU_ARCH_ABI64 = 0x1000000
CPU_TYPE_X86_64 = CPU_TYPE_I386 | CPU_ARCH_ABI64

CPU_TYPE_ARM = 0xc
CPU_TYPE_ARM64 = CPU_TYPE_ARM | CPU_ARCH_ABI64

CPU_TYPE_ARM = 12

arch_lookup = {
    CPU_TYPE_I386: "x86",
    CPU_TYPE_X86_64: "x86_64",
    CPU_TYPE_ARM: "arm",
    CPU_TYPE_ARM64: "arm64"
}


class MachOParsingException(Exception):
    def __init__(self, *args, **kwargs):
        Exception.__init__(self, *args, **kwargs)


class FatMachOFile(BinaryAccessor):
    def __init__(self, data):
        self.data = data
        self.machs = []
        self.valid = False
        if not self.is_fat():
            return
        try:
            self.tree = Structure(self.data)
            self.header = self.tree.struct("Fat header", "header")

            self.header.uint32_be("magic")

            self.big_endian = self.header.magic == 0xcafebabe

            self.header._big_endian = self.big_endian
            self.header.uint32("nfat_arch")

            self.fat_arches = self.tree.array(self.header.nfat_arch, "fat_arches")
            self.macho_arches = []

            offset = self.header.getSize()
            for i in xrange(0, self.header.nfat_arch):
                arch = self.fat_arches[i]
                arch.seek(offset)
                arch.uint32("cputype")
                arch.uint32("cpusubtype")
                arch.uint32("offset")
                arch.uint32("size")
                arch.uint32("align")

                arch.cpuname = arch_lookup[arch.cputype]

                logging.debug("Found fat arch with cpu type: {}".format(arch.cpuname))

                arch_data = BinaryData(self.data.read(arch.offset, arch.size))

                macho_arch = MachOFile(arch_data)
                macho_arch.offset = arch.offset
                macho_arch.size = arch.size
                self.macho_arches.append(macho_arch)

                offset += arch.getSize()

            self.valid = True
        except:
            self.valid = False
            raise

    def __len__(self):
        max_found = None
        for arch in self.macho_arches:
            for i in arch.segments:
                if ((max_found is None) or ((i.vmaddr + i.vmsize) > max_found)) and (i.vmsize != 0):
                    max_found = i.vmaddr + i.vmsize
        return max_found - self.start()

    def is_fat(self):
        if self.data.read(0, 4) == "\xca\xfe\xba\xbe":
            return True
        if self.data.read(0, 4) == "\xbe\xba\xfe\xca":
            return True
        return False

    def save(self, filename):
        out_data = self.data.data.raw

        for arch in self.macho_arches:
            out_data[arch.offset:arch.offset + arch.size] = arch.data.data.raw

        out = io.open(filename)
        out.write(out_data)


class MachOFile(BinaryAccessor):
    def __init__(self, data):
        self.classes = []
        self.properties = None
        self.ivars = None
        self.methods = None
        self.data = data
        self.valid = False
        self.symbols_by_name = {}
        self.symbols_by_addr = {}
        self.sections_by_name = {}
        self.class_strings = []
        self.method_strings = []
        self.plt = {}
        self.libraries = []
        self.offset = None
        self.size = 0
        if not self.is_macho():
            return

        try:
            self.tree = Structure(self.data)
            self.header = self.tree.struct("Mach-O header", "header")

            self.header.uint32_be("magic")
            self.big_endian = self.header.magic & 0xfeedface == 0xfeedface
            self.header._big_endian = self.big_endian
            self.tree._big_endian = self.big_endian

            if self.big_endian:
                if self.header.magic & 1 == 1:
                    self.bits = 64
                else:
                    self.bits = 32
            else:
                if self.header.magic & 0x01000000 == 0x01000000:
                    self.bits = 64
                else:
                    self.bits = 32

            self.header.uint32("cputype")
            self.header.uint32("cpusubtype")
            self.header.uint32("filetype")
            self.header.uint32("cmds")
            self.header.uint32("cmdsize")
            self.header.uint32("flags")

            if self.bits == 64:
                self.header.uint32("reserved")

            self.symbol_table = None
            self.dynamic_symbol_table = None

            # Parse loader commands
            self.commands = self.tree.array(self.header.cmds, "commands")
            self.segments = []
            self.sections = []
            offset = self.header.getSize()
            for i in xrange(0, self.header.cmds):
                cmd = self.commands[i]
                cmd.seek(offset)

                cmd.uint32("cmd")
                cmd.uint32("size")

                if cmd.cmd == 1:  # SEGMENT
                    cmd.bytes(16, "name")

                    cmd.uint32("vmaddr")
                    cmd.uint32("vmsize")
                    cmd.uint32("fileoff")
                    cmd.uint32("filesize")
                    cmd.uint32("maxprot")
                    cmd.uint32("initprot")
                    cmd.uint32("nsects")
                    cmd.uint32("flags")

                    if cmd.initprot != 0:  # Ignore __PAGE_ZERO or anything like it
                        self.segments.append(cmd)

                    cmd.array(cmd.nsects, "sections")
                    for j in xrange(0, cmd.nsects):
                        section = cmd.sections[j]

                        section.bytes(16, "name")
                        section.name = section.name.strip("\x00")
                        self.sections_by_name[section.name] = section

                        section.bytes(16, "segment")

                        section.uint32("addr")
                        section.uint32("size")
                        section.uint32("offset")
                        section.uint32("align")
                        section.uint32("reloff")
                        section.uint32("nreloc")
                        section.uint32("flags")
                        section.uint32("reserved1")
                        section.uint32("reserved2")

                        self.sections.append(section)

                    for j in xrange(0, cmd.nsects):
                        section = cmd.sections[j]
                        section.array(section.nreloc, "relocs")
                        for k in xrange(0, section.nreloc):
                            reloc = section.relocs[k]
                            reloc.seek(section.reloff + (k * 8))

                            reloc.uint32("addr")
                            reloc.uint32("value")

                elif cmd.cmd == 25:  # SEGMENT_64
                    cmd.bytes(16, "name")

                    cmd.uint64("vmaddr")
                    cmd.uint64("vmsize")
                    cmd.uint64("fileoff")
                    cmd.uint64("filesize")
                    cmd.uint32("maxprot")
                    cmd.uint32("initprot")
                    cmd.uint32("nsects")
                    cmd.uint32("flags")

                    if cmd.initprot != 0:  # Ignore __PAGE_ZERO or anything like it
                        self.segments.append(cmd)

                    cmd.array(cmd.nsects, "sections")
                    for j in xrange(0, cmd.nsects):
                        section = cmd.sections[j]

                        section.bytes(16, "name")
                        section.name = section.name.strip("\x00")
                        self.sections_by_name[section.name] = section

                        section.bytes(16, "segment")

                        section.uint64("addr")
                        section.uint64("size")
                        section.uint32("offset")
                        section.uint32("align")
                        section.uint32("reloff")
                        section.uint32("nreloc")
                        section.uint32("flags")
                        section.uint32("reserved1")
                        section.uint32("reserved2")
                        section.uint32("reserved3")

                        self.sections.append(section)

                    for j in xrange(0, cmd.nsects):
                        section = cmd.sections[j]
                        section.array(section.nreloc, "relocs")
                        for k in xrange(0, section.nreloc):
                            reloc = section.relocs[k]
                            reloc.seek(section.reloff + (k * 8))

                            reloc.uint32("addr")
                            reloc.uint32("value")

                elif cmd.cmd == 5:  # UNIX_THREAD
                    if self.header.cputype == 7:  # x86
                        cmd.uint32_le("flavor")
                        cmd.uint32_le("count")
                        for reg in ["eax", "ebx", "ecx", "edx", "edi", "esi", "ebp", "esp", "ss", "eflags",
                                    "eip", "cs", "ds", "es", "fs", "gs"]:
                            cmd.uint32_le(reg)
                        self.entry_addr = cmd.eip
                    elif self.header.cputype == 0x01000007:  # x86_64
                        cmd.uint32_le("flavor")
                        cmd.uint32_le("count")
                        for reg in ["rax", "rbx", "rcx", "rdx", "rdi", "rsi", "rbp", "rsp", "r8", "r9",
                                    "r10", "r11", "r12", "r13", "r14", "r15", "rip", "rflags", "cs", "fs", "gs"]:
                            cmd.uint64_le(reg)
                        self.entry_addr = cmd.rip
                    elif self.header.cputype == 18:  # PPC32
                        cmd.uint32_be("flavor")
                        cmd.uint32_be("count")
                        for reg in ["srr0", "srr1"] + ["r%d" % i for i in xrange(0, 32)] + ["cr", "xer",
                                                                                            "lr", "ctr", "mq",
                                                                                            "vrsave"]:
                            cmd.uint32_be(reg)
                        self.entry_addr = cmd.srr0
                    elif self.header.cputype == 0x01000012:  # PPC64
                        cmd.uint32_be("flavor")
                        cmd.uint32_be("count")
                        for reg in ["srr0", "srr1"] + ["r%d" % i for i in xrange(0, 32)] + ["cr", "xer",
                                                                                            "lr", "ctr", "mq",
                                                                                            "vrsave"]:
                            cmd.uint64_be(reg)
                        self.entry_addr = cmd.srr0
                    elif self.header.cputype == 12:  # ARM
                        cmd.uint32_le("flavor")
                        cmd.uint32_le("count")
                        for reg in ["r%d" % i for i in xrange(0, 13)] + ["sp", "lr", "pc", "cpsr"]:
                            cmd.uint32_le(reg)
                        self.entry_addr = cmd.pc
                elif cmd.cmd == 2:  # SYMTAB

                    cmd.uint32("symoff")
                    cmd.uint32("nsyms")
                    cmd.uint32("stroff")
                    cmd.uint32("strsize")

                    self.symbol_table = self.tree.array(cmd.nsyms, "symtab")
                    strings = self.data.read(cmd.stroff, cmd.strsize)

                    sym_offset = cmd.symoff
                    for j in xrange(0, cmd.nsyms):
                        entry = self.symbol_table[j]
                        entry.seek(sym_offset)

                        entry.uint32("strx")
                        entry.uint8("type")
                        entry.uint8("sect")
                        entry.uint16("desc")
                        if self.bits == 32:
                            entry.uint32("value")
                        else:
                            entry.uint64("value")

                        str_end = strings.find("\x00", entry.strx)
                        entry.name = strings[entry.strx:str_end]

                        if self.bits == 32:
                            sym_offset += 12
                        else:
                            sym_offset += 16
                elif cmd.cmd == 11:  # DYSYMTAB
                    cmd.uint32("ilocalsym")
                    cmd.uint32("nlocalsym")
                    cmd.uint32("iextdefsym")
                    cmd.uint32("nextdefsym")
                    cmd.uint32("iundefsym")
                    cmd.uint32("nundefsym")
                    cmd.uint32("tocoff")
                    cmd.uint32("ntoc")
                    cmd.uint32("modtaboff")
                    cmd.uint32("nmodtab")
                    cmd.uint32("extrefsymoff")
                    cmd.uint32("nextrefsyms")
                    cmd.uint32("indirectsymoff")
                    cmd.uint32("nindirectsyms")
                    cmd.uint32("extreloff")
                    cmd.uint32("nextrel")
                    cmd.uint32("locreloff")
                    cmd.uint32("nlocrel")

                elif (cmd.cmd & 0x7fffffff) == 0x22:  # DYLD_INFO
                    self.dynamic_symbol_table = cmd

                    cmd.uint32("rebaseoff")
                    cmd.uint32("rebasesize")
                    cmd.uint32("bindoff")
                    cmd.uint32("bindsize")
                    cmd.uint32("weakbindoff")
                    cmd.uint32("weakbindsize")
                    cmd.uint32("lazybindoff")
                    cmd.uint32("lazybindsize")
                    cmd.uint32("exportoff")
                    cmd.uint32("exportsize")

                elif cmd.cmd == 12:
                    cmd.uint32("stroffset")
                    cmd.uint32("timestamp")
                    cmd.uint32("currentversion")
                    cmd.uint32("compatversion")

                    strings = self.data.read(offset, cmd.size)
                    str_end = strings.find("\x00", cmd.stroffset)
                    cmd.name = strings[cmd.stroffset:str_end]

                    self.libraries.append(cmd.name)
                elif cmd.cmd == 20:
                    print "SUP!!!"

                offset += cmd.size

            # Add symbols from symbol table
            if self.symbol_table:
                for i in xrange(0, len(self.symbol_table)):
                    symbol = self.symbol_table[i]

                    # Only use symbols that are within a section
                    if ((symbol.type & 0xe) == 0xe) and (symbol.sect <= len(self.sections)):
                        self.create_symbol(symbol.value, symbol.name)

            # If there is a DYLD_INFO section, parse it and add PLT entries
            if self.dynamic_symbol_table:
                self.parse_dynamic_tables([[self.dynamic_symbol_table.bindoff, self.dynamic_symbol_table.bindsize],
                                           [self.dynamic_symbol_table.lazybindoff,
                                            self.dynamic_symbol_table.lazybindsize]])

            self.tree.complete()
            self.valid = True
        except:
            self.valid = False
            raise MachOParsingException()

    def read_leb128(self, data, ofs):
        value = 0
        shift = 0
        while ofs < len(data):
            cur = ord(data[ofs])
            ofs += 1
            value |= (cur & 0x7f) << shift
            shift += 7
            if (cur & 0x80) == 0:
                break
        return value, ofs

    def parse_objc(self):
        self.method_strings = []
        self.class_strings = []

        ptr_size = self.bits / 8

        if "__objc_methname" in self.sections_by_name.keys():
            methname_section = self.sections_by_name["__objc_methname"]
            data = self.read(methname_section.addr, methname_section.size)
            method_names = data.split("\x00")

            self.method_strings = []
            addr = methname_section.addr
            for method in method_names:
                self.method_strings.append((addr, method))
                addr += len(method) + 1

        if "__objc_classname" in self.sections_by_name.keys():
            classname_section = self.sections_by_name["__objc_classname"]
            data = self.read(classname_section.addr, classname_section.size)
            class_names = data.split("\x00")
            self.class_strings = class_names

            self.class_strings = []
            addr = classname_section.addr
            for class_name in class_names:
                self.class_strings.append((addr, class_name))
                addr += len(class_name) + 1

        if "__cstring" in self.sections_by_name.keys():
            cstring_section = self.sections_by_name["__cstring"]
            data = self.read(cstring_section.addr, cstring_section.size)
            strings = data.split("\x00")
            strings.pop(len(strings) - 1)

            self.strings = []
            addr = cstring_section.addr
            for string in strings:
                self.strings.append((addr, string))
                addr += len(string) + 1

        if "__objc_selrefs" not in self.sections_by_name.keys():
            return False

        selref_section = self.sections_by_name["__objc_selrefs"]
        selref_count = selref_section.size / ptr_size

        selref_section.array(selref_count, "selrefs")

        # Parsing selref section to extract a list of all selectors
        # in macho
        logging.debug("Parsing selrefs...")

        self.selrefs = []
        offset = selref_section.offset
        for i in xrange(0, selref_count):
            selref = selref_section.selrefs[i]
            selref.seek(offset)

            if self.bits == 64:
                selref.uint64("ref")
            else:
                selref.uint32("ref")

            self.selrefs.append((selref.ref, self.read_str(selref.ref)))
            logging.debug("Selref found: {}".format(hex(selref.ref)))
            offset += ptr_size

        if "__objc_classrefs" not in self.sections_by_name.keys():
            return False

        classref_section = self.sections_by_name["__objc_classrefs"]
        classref_count = classref_section.size / ptr_size

        classref_section.array(classref_count, "classrefs")

        # Parsing classref section to extract a list of all classes
        # in macho
        logging.debug("Parsing classrefs...")

        actual_classes = 0
        offset = classref_section.offset
        for i in xrange(0, classref_count):
            classref = classref_section.classrefs[i]
            classref.seek(offset)

            if self.bits == 64:
                classref.uint64("ref")
            else:
                classref.uint32("ref")

            classref.offset = -1
            if classref.ref != 0:
                logging.debug("Classref found: {}".format(hex(classref.ref)))
                classref.offset = self.vmaddr_to_offset(classref.ref)
                if classref.offset != -1:
                    actual_classes += 1

            offset += ptr_size

        if "__objc_data" not in self.sections_by_name.keys():
            return False

        # Iterate over every class that is located inside of the
        # macho and get its information
        data_section = self.sections_by_name["__objc_data"]
        data_section.array(actual_classes, "class_data")

        cur_class = 0
        for i in xrange(0, classref_count):
            classref = classref_section.classrefs[i]
            if classref.offset == -1:
                continue

            class_data = data_section.class_data[cur_class]
            class_data.seek(classref.offset)

            if self.bits == 64:
                class_data.uint64("metaclass")
                class_data.uint64("superclass")
                class_data.uint64("cache")
                class_data.uint64("vtable")
                class_data.uint64("data")
            else:
                class_data.uint32("metaclass")
                class_data.uint32("superclass")
                class_data.uint32("cache")
                class_data.uint32("vtable")
                class_data.uint32("data")

            logging.debug("Class data found: [metaclass: {}, superclass: {}, cache: {}, vtable: {}, data: {}]" \
                          .format(hex(class_data.metaclass), hex(class_data.superclass), hex(class_data.cache), \
                                  hex(class_data.vtable), hex(class_data.data)))
            cur_class += 1

        if "__objc_const" not in self.sections_by_name.keys():
            return False

        # For every class, extract the struct that describes
        # where its various attributes are located
        const_section = self.sections_by_name["__objc_const"]
        const_section.array(actual_classes, "class_const_data")

        for i in xrange(0, actual_classes):
            class_data = data_section.class_data[i]
            data_offset = self.vmaddr_to_offset(class_data.data)

            const_data = const_section.class_const_data[i]
            const_data.seek(data_offset)
            logging.debug("Going to {}...".format(hex(class_data.data)))

            const_data.uint32("flags")
            const_data.uint32("instance_start")
            const_data.uint32("instance_size")

            if self.bits == 64:
                const_data.uint32("reserved")
                const_data.uint64("ivar_layout")
                const_data.uint64("name_ptr")
                const_data.uint64("base_methods_ptr")
                const_data.uint64("base_protocol")
                const_data.uint64("ivars_ptr")
                const_data.uint64("weak_ivar_layout")
                const_data.uint64("base_properties_ptr")
            else:
                const_data.uint32("ivar_layout")
                const_data.uint32("name_ptr")
                const_data.uint32("base_methods_ptr")
                const_data.uint32("base_protocol")
                const_data.uint32("ivars_ptr")
                const_data.uint32("weak_ivar_layout")
                const_data.uint32("base_properties_ptr")

            class_data = {}

            if const_data.base_properties_ptr != 0:
                # Extract the class's properties
                logging.debug("Getting base properties from {}...".format(hex(const_data.base_properties_ptr)))
                ptr_ofs = self.vmaddr_to_offset(const_data.base_properties_ptr)

                if ptr_ofs == -1:
                    continue

                base_properties = const_data.struct("base_properties")
                base_properties.seek(ptr_ofs)

                base_properties.uint32("flags")
                base_properties.uint32("count")

                class_data["properties"] = []

                property_elements = base_properties.array(base_properties.count, "elements")
                for j in xrange(0, base_properties.count):
                    elem = property_elements[j]
                    if self.bits == 64:
                        elem.uint64("name_ptr")
                        elem.uint64("attributes_ptr")
                    else:
                        elem.uint32("name_ptr")
                        elem.uint32("attributes_ptr")

                    elem.name = self.read_str(elem.name_ptr)
                    elem.attributes = self.read_str(elem.attributes_ptr)

                    class_data["properties"].append(elem)

            if const_data.ivars_ptr != 0:
                # Extracting the class's ivars
                logging.debug("Getting ivars...")
                ptr_ofs = self.vmaddr_to_offset(const_data.ivars_ptr)

                if ptr_ofs == -1:
                    continue

                ivars = const_data.struct("ivars")
                ivars.seek(ptr_ofs)

                ivars.uint32("flags")
                ivars.uint32("count")

                class_data["ivars"] = []

                ivar_elements = ivars.array(ivars.count, "elements")
                for j in xrange(0, ivars.count):
                    elem = ivar_elements[j]
                    if self.bits == 64:
                        elem.uint64("count_ptr")
                        elem.uint64("name_ptr")
                        elem.uint64("attributes_ptr")
                    else:
                        elem.uint32("count_ptr")
                        elem.uint32("name_ptr")
                        elem.uint32("attributes_ptr")

                    elem.uint32("num_1")
                    elem.uint32("num_2")

                    elem.name = self.read_str(elem.name_ptr)
                    elem.attributes = self.read_str(elem.attributes_ptr)

                    class_data["ivars"].append(elem)

            if const_data.base_methods_ptr != 0:
                # Extracting the class's methods
                logging.debug("Getting base methods...")
                ptr_ofs = self.vmaddr_to_offset(const_data.base_methods_ptr)

                if ptr_ofs == -1:
                    continue

                base_methods = const_data.struct("base_methods")
                base_methods.seek(ptr_ofs)

                base_methods.uint32("flags")
                base_methods.uint32("count")

                class_data["methods"] = []

                base_methods_elements = base_methods.array(base_methods.count, "elements")
                for j in xrange(0, base_methods.count):
                    elem = base_methods_elements[j]
                    if self.bits == 64:
                        elem.uint64("name_ptr")
                        elem.uint64("types_ptr")
                        elem.uint64("imp")
                    else:
                        elem.uint32("name_ptr")
                        elem.uint32("types_ptr")
                        elem.uint32("imp")

                    elem.name = self.read_str(elem.name_ptr)
                    elem.types = self.read_str(elem.types_ptr)

                    class_data["methods"].append(elem)

            logging.debug("Getting name...")
            const_data.name = self.read_str(const_data.name_ptr)

            class_name = const_data.name
            class_data["class"] = const_data.name_ptr

            # Add class to list of all classes
            self.classes.append({"name": class_name, "data": class_data})

        return True

    def read_str(self, addr):
        ofs = self.vmaddr_to_offset(addr)
        tmp_str = ""

        if ofs == -1:
            return None

        while True:
            c = self.read(addr, 1)
            if c == "\x00":
                break
            tmp_str += c
            addr += 1
        return tmp_str

    def parse_dynamic_tables(self, tables):
        # Interpret DYLD_INFO instructions (not documented by Apple)
        # http://networkpx.blogspot.com/2009/09/about-lcdyldinfoonly-command.html
        ordinal = 0
        segment = 0
        offset = 0
        sym_type = 0
        name = ""

        for table in tables:
            offset = table[0]
            size = table[1]
            opcodes = self.data.read(offset, size)
            i = 0
            while i < len(opcodes):
                opcode = ord(opcodes[i])
                i += 1
                if (opcode >> 4) == 0:
                    continue
                elif (opcode >> 4) == 1:
                    ordinal = opcode & 0xf
                elif (opcode >> 4) == 2:
                    ordinal, i = self.read_leb128(opcodes, i)
                elif (opcode >> 4) == 3:
                    ordinal = -(opcode & 0xf)
                elif (opcode >> 4) == 4:
                    name = ""
                    while i < len(opcodes):
                        ch = opcodes[i]
                        i += 1
                        if ch == '\x00':
                            break
                        name += ch
                elif (opcode >> 4) == 5:
                    sym_type = opcode & 0xf
                elif (opcode >> 4) == 6:
                    addend, i = self.read_leb128(opcodes, i)
                elif (opcode >> 4) == 7:
                    segment = opcode & 0xf
                    offset, i = self.read_leb128(opcodes, i)
                elif (opcode >> 4) == 8:
                    rel, i = self.read_leb128(opcodes, i)
                    offset += rel
                elif (opcode >> 4) >= 9:
                    if (sym_type == 1) and (segment <= len(self.segments)):
                        # Add pointer type entries to the PLT
                        addr = self.segments[segment - 1].vmaddr + offset
                        self.plt[addr] = name
                        self.create_symbol(addr, self.decorate_plt_name(name))
                    if self.bits == 32:
                        offset += 4
                    else:
                        offset += 8
                    if (opcode >> 4) == 10:
                        rel, i = self.read_leb128(opcodes, i)
                        offset += rel
                    elif (opcode >> 4) == 11:
                        offset += (opcode & 0xf) * 4
                    elif (opcode >> 4) == 12:
                        count, i = self.read_leb128(opcodes, i)
                        skip, i = self.read_leb128(opcodes, i)

    def vmaddr_to_section(self, addr):
        found_section = None
        for i in self.sections:
            if ((addr >= i.addr) and (addr < (i.addr + i.size))) and (i.size != 0):
                found_section = i
                break

        if found_section is None:
            logging.debug("Unable to find section for addr: {}".format(hex(addr)))
            return None

        return found_section

    def vmaddr_to_offset(self, addr):
        cur = None
        for i in self.segments:
            if ((addr >= i.vmaddr) and (addr < (i.vmaddr + i.vmsize))) and (i.vmsize != 0):
                cur = i
                break
        if cur is None:
            logging.debug("Unable to find vmaddr: {}".format(hex(addr)))
            return -1

        return addr - cur.vmaddr + cur.fileoff

    def offset_to_vmaddr(self, ofs):
        cur = None
        for i in self.segments:
            if ((ofs >= i.fileoff) and (ofs < (i.fileoff + i.filesize))) and (i.filesize != 0):
                cur = i
                break
        if cur is None:
            logging.debug("Unable to find offset: {}".format(hex(ofs)))
            return -1

        return ofs - cur.fileoff + cur.vmaddr

    def read(self, ofs, len):
        result = ""
        while len > 0:
            cur = None
            for i in self.segments:
                if ((ofs >= i.vmaddr) and (ofs < (i.vmaddr + i.vmsize))) and (i.vmsize != 0):
                    cur = i
            if cur is None:
                break

            prog_ofs = ofs - cur.vmaddr
            mem_len = cur.vmsize - prog_ofs
            file_len = cur.filesize - prog_ofs
            if mem_len > len:
                mem_len = len
            if file_len > len:
                file_len = len

            if file_len <= 0:
                result += "\x00" * mem_len
                len -= mem_len
                ofs += mem_len
                continue

            result += self.data.read(cur.fileoff + prog_ofs, file_len)
            len -= file_len
            ofs += file_len

        return result

    def next_valid_addr(self, ofs):
        result = -1
        for i in self.segments:
            if (i.vmaddr >= ofs) and (i.vmsize != 0) and ((result == -1) or (i.vmaddr < result)):
                result = i.vmaddr
        return result

    def write(self, ofs, data):
        result = 0
        while len(data) > 0:
            cur = None
            for i in self.segments:
                if ((ofs >= i.vmaddr) and (ofs < (i.vmaddr + i.vmsize))) and (i.vmsize != 0):
                    cur = i
            if cur is None:
                break

            prog_ofs = ofs - cur.vmaddr
            mem_len = cur.vmsize - prog_ofs
            file_len = cur.filesize - prog_ofs
            if mem_len > len:
                mem_len = len
            if file_len > len:
                file_len = len

            if file_len <= 0:
                break

            result += self.data.write(cur.fileoff + prog_ofs, data[0:file_len])
            data = data[file_len:]
            ofs += file_len

        return result

    def rename_symbol_by_name(self, target, dest):
        assert len(target) == len(dest)

        if target in self.methods.keys():
            ptr = self.methods[target]
        elif target in self.ivars.keys():
            ptr = self.ivars[target]
        elif target in self.classes.keys():
            ptr = self.classes[target]
        elif target in self.properties.keys():
            ptr = self.properties[target]
        else:
            return False

        return self.rename_symbol(ptr, dest)

    def rename_symbol(self, ptr, dest):
        self.write(ptr, dest)

    def insert(self, ofs, data):
        return 0

    def remove(self, ofs, size):
        return 0

    def save(self, filename):
        self.data.save(filename)

    def start(self):
        result = None
        for i in self.segments:
            if ((result is None) or (i.vmaddr < result)) and (i.vmsize != 0):
                result = i.vmaddr
        return result

    def entry(self):
        if not hasattr(self, "entry_addr"):
            return 0
        return self.entry_addr

    def __len__(self):
        max = None
        for i in self.segments:
            if ((max is None) or ((i.vmaddr + i.vmsize) > max)) and (i.vmsize != 0):
                max = i.vmaddr + i.vmsize
        return max - self.start()

    def is_macho(self):
        if self.data.read(0, 4) == "\xfe\xed\xfa\xce":
            return True
        if self.data.read(0, 4) == "\xfe\xed\xfa\xcf":
            return True
        if self.data.read(0, 4) == "\xce\xfa\xed\xfe":
            return True
        if self.data.read(0, 4) == "\xcf\xfa\xed\xfe":
            return True
        return False

    def architecture(self):
        if self.header.cputype == 7:
            return "x86"
        if self.header.cputype == 0x01000007:
            return "x86_64"
        if self.header.cputype == 12:
            return "arm"
        if self.header.cputype == 18:
            return "ppc"
        if self.header.cputype == 0x01000012:
            return "ppc"
        return None

    def decorate_plt_name(self, name):
        return name + "@PLT"

    def create_symbol(self, addr, name):
        self.symbols_by_name[name] = addr
        self.symbols_by_addr[addr] = name

    def delete_symbol(self, addr, name):
        if name in self.symbols_by_name:
            del (self.symbols_by_name[name])
        if addr in self.symbols_by_addr:
            del (self.symbols_by_addr[addr])

    def find_num(self, struct_pack, num, addr=None):
        if addr is None:
            addr = self.next_valid_addr(0)

        endianess = ">" if self.big_endian else "<"
        while addr != -1:
            data = self.read(addr, 0xfffffffff)
            idx = data.find(struct.pack(endianess + struct_pack, num))
            if idx != -1:
                return addr + idx

            addr += len(data)
            addr = self.next_valid_addr(addr)

        return -1

    def find_str(self, s, addr=None):
        if addr is None:
            addr = self.next_valid_addr(0)

        while addr != -1:
            data = self.read(addr, 0xfffffffff)
            idx = data.find(s)
            if idx != -1:
                return addr + idx

            addr += len(data)
            addr = self.next_valid_addr(addr)

        return -1
