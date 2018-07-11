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
import struct
import io
from ctypes import create_string_buffer


class BinaryAccessor:
    def read_uint8(self, ofs):
        return struct.unpack('B', self.read(ofs, 1))[0]

    def read_uint16(self, ofs):
        return struct.unpack('<H', self.read(ofs, 2))[0]

    def read_uint32(self, ofs):
        return struct.unpack('<I', self.read(ofs, 4))[0]

    def read_uint64(self, ofs):
        return struct.unpack('<Q', self.read(ofs, 8))[0]

    def read_uint16_le(self, ofs):
        return struct.unpack('<H', self.read(ofs, 2))[0]

    def read_uint32_le(self, ofs):
        return struct.unpack('<I', self.read(ofs, 4))[0]

    def read_uint64_le(self, ofs):
        return struct.unpack('<Q', self.read(ofs, 8))[0]

    def read_uint16_be(self, ofs):
        return struct.unpack('>H', self.read(ofs, 2))[0]

    def read_uint32_be(self, ofs):
        return struct.unpack('>I', self.read(ofs, 4))[0]

    def read_uint64_be(self, ofs):
        return struct.unpack('>Q', self.read(ofs, 8))[0]

    def read_int8(self, ofs):
        return struct.unpack('b', self.read(ofs, 1))[0]

    def read_int16(self, ofs):
        return struct.unpack('<h', self.read(ofs, 2))[0]

    def read_int32(self, ofs):
        return struct.unpack('<i', self.read(ofs, 4))[0]

    def read_int64(self, ofs):
        return struct.unpack('<q', self.read(ofs, 8))[0]

    def read_uint8_le(self, ofs):
        return struct.unpack('<B', self.read(ofs, 1))[0]

    def read_int16_le(self, ofs):
        return struct.unpack('<h', self.read(ofs, 2))[0]

    def read_int32_le(self, ofs):
        return struct.unpack('<i', self.read(ofs, 4))[0]

    def read_int64_le(self, ofs):
        return struct.unpack('<q', self.read(ofs, 8))[0]

    def read_uint8_be(self, ofs):
        return struct.unpack('>B', self.read(ofs, 1))[0]

    def read_int16_be(self, ofs):
        return struct.unpack('>h', self.read(ofs, 2))[0]

    def read_int32_be(self, ofs):
        return struct.unpack('>i', self.read(ofs, 4))[0]

    def read_int64_be(self, ofs):
        return struct.unpack('>q', self.read(ofs, 8))[0]

    def write_uint8(self, ofs, val):
        return self.write(ofs, struct.pack('B', val)) == 1

    def write_uint16(self, ofs, val):
        return self.write(ofs, struct.pack('<H', val)) == 2

    def write_uint32(self, ofs, val):
        return self.write(ofs, struct.pack('<I', val)) == 4

    def write_uint64(self, ofs, val):
        return self.write(ofs, struct.pack('<Q', val)) == 8

    def write_uint16_le(self, ofs, val):
        return self.write(ofs, struct.pack('<H', val)) == 2

    def write_uint32_le(self, ofs, val):
        return self.write(ofs, struct.pack('<I', val)) == 4

    def write_uint64_le(self, ofs, val):
        return self.write(ofs, struct.pack('<Q', val)) == 8

    def write_uint16_be(self, ofs, val):
        return self.write(ofs, struct.pack('>H', val)) == 2

    def write_uint32_be(self, ofs, val):
        return self.write(ofs, struct.pack('>I', val)) == 4

    def write_uint64_be(self, ofs, val):
        return self.write(ofs, struct.pack('>Q', val)) == 8

    def write_int8(self, ofs, val):
        return self.write(ofs, struct.pack('b', val)) == 1

    def write_int16(self, ofs, val):
        return self.write(ofs, struct.pack('<h', val)) == 2

    def write_int32(self, ofs, val):
        return self.write(ofs, struct.pack('<i', val)) == 4

    def write_int64(self, ofs, val):
        return self.write(ofs, struct.pack('<q', val)) == 8

    def write_int16_le(self, ofs, val):
        return self.write(ofs, struct.pack('<h', val)) == 2

    def write_int32_le(self, ofs, val):
        return self.write(ofs, struct.pack('<i', val)) == 4

    def write_int64_le(self, ofs, val):
        return self.write(ofs, struct.pack('<q', val)) == 8

    def write_int16_be(self, ofs, val):
        return self.write(ofs, struct.pack('>h', val)) == 2

    def write_int32_be(self, ofs, val):
        return self.write(ofs, struct.pack('>i', val)) == 4

    def write_int64_be(self, ofs, val):
        return self.write(ofs, struct.pack('>q', val)) == 8

    def end(self):
        return self.start() + len(self)

    def __str__(self):
        return self.read(0, len(self))

    def __getitem__(self, offset):
        if type(offset) == slice:
            start = offset.start
            end = offset.stop
            if start is None:
                start = self.start()
            if end is None:
                end = self.start() + len(self)
            if end < 0:
                end = self.start() + len(self) + end

            if (offset.step is None) or (offset.step == 1):
                return self.read(start, end - start)
            else:
                result = ""
                for i in xrange(start, end, offset.step):
                    part = self.read(i, 1)
                    if len(part) == 0:
                        return result
                    result += part
                return result

        result = self.read(offset, 1)
        if len(result) == 0:
            raise IndexError
        return result

    def __setitem__(self, offset, value):
        if type(offset) == slice:
            start = offset.start
            end = offset.stop
            if start is None:
                start = self.start()
            if end is None:
                end = self.start() + len(self)
            if end < 0:
                end = self.start() + len(self) + end

            if (offset.step is None) or (offset.step == 1):
                if end < start:
                    return
                if len(value) != (end - start):
                    self.remove(start, end - start)
                    self.insert(start, value)
                else:
                    self.write(start, value)
            else:
                rel_offset = 0
                j = 0
                for i in xrange(start, end, offset.step):
                    if j < len(value):
                        self.write(i + rel_offset, value[j])
                    else:
                        self.remove(i + rel_offset)
                        rel_offset -= 1
        else:
            if self.write(offset, value) == 0:
                raise IndexError

    def __delitem__(self, offset):
        if type(offset) == slice:
            start = offset.start
            end = offset.stop
            if start is None:
                start = self.start()
            if end is None:
                end = self.start() + len(self)
            if end < 0:
                end = self.start() + len(self) + end

            if (offset.step is None) or (offset.step == 1):
                if end < start:
                    return
                self.remove(start, end - start)
            else:
                rel_offset = 0
                for i in xrange(start, end, offset.step):
                    self.remove(i + rel_offset)
                    rel_offset -= 1
        else:
            if self.remove(offset, 1) == 0:
                raise IndexError


class BinaryData(BinaryAccessor):
    def __init__(self, data=""):
        self.data = create_string_buffer(data)
        self.symbols_by_name = {}
        self.symbols_by_addr = {}

    def read(self, ofs, size):
        return self.data[ofs:(ofs + size)]

    def write(self, ofs, data):
        if len(data) == 0:
            return 0
        if ofs + len(data) >= len(self.data):
            return 0
        if ofs < 0:
            return 0

        self.data[ofs:ofs + len(data)] = bytes(data)

        return len(data)

    def save(self, filename):
        f = io.open(filename, 'wb')
        f.write(self.data.raw)
        f.close()

    def start(self):
        return 0

    def __len__(self):
        return len(self.data.value)

    def architecture(self):
        return self.default_arch


class BinaryFile(BinaryData):
    def __init__(self, filename):
        f = io.open(filename, 'rb')
        data = f.read()
        f.close()
        BinaryData.__init__(self, data)
