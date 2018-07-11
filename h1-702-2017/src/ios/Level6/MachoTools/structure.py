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

from binary_data import *


class _ParserState:
    def __init__(self, data, ofs):
        self.data = data
        self.offset = ofs


class Array:
    def __init__(self, state, count, big_endian=False):
        self._state = state
        self.elements = []
        for i in range(0, count):
            self.elements += [Structure(state.data, state, big_endian)]

    def append(self):
        self.elements.append(Structure(self._state.data, self._state))

    def getStart(self):
        start = None
        for i in self.elements:
            if (start is None) or (i.getStart() < start):
                start = i.getStart()
        if start is None:
            return 0
        return start

    def getSize(self):
        start = self.getStart()
        end = None
        for i in self.elements:
            if (end is None) or ((i.getStart() + i.getSize()) > end):
                end = i.getStart() + i.getSize()
        if end is None:
            return 0
        return end - start

    def complete(self):
        for i in self.elements:
            i.complete()

    def __getitem__(self, index):
        return self.elements[index]

    def __len__(self):
        return len(self.elements)


class Structure:
    def __init__(self, data, state=None, big_endian=True):
        self._data = data
        self._state = state
        self._big_endian = big_endian
        if state is None:
            self._state = _ParserState(data, 0)
        self._names = {}
        self._order = []
        self._start = {}
        self._size = {}
        self._type = {}

    def __str__(self):
        format_str = ""
        format_str += "{\n"
        for k in self._order:
            elem = self.__dict__[k]
            elem_format = hex(elem) if type(elem) == int else str(elem)
            format_str += "\t{}: {}\n".format(k, elem_format)
        format_str += "}"
        return format_str

    def get_state(self):
        return self._state

    def seek(self, ofs):
        self._state.offset = ofs

    def struct(self, name, id=None):
        if id is None:
            id = name
        result = Structure(self._data, self._state, self._big_endian)
        self.__dict__[id] = result
        self._names[id] = name
        self._type[id] = "struct"
        self._order += [id]
        return result

    def array(self, count, name, id=None):
        if id is None:
            id = name
        result = Array(self._state, count, self._big_endian)
        self.__dict__[id] = result
        self._names[id] = name
        self._type[id] = "array"
        self._order += [id]
        return result

    def bytes(self, count, name, id=None):
        if id is None:
            id = name
        result = self._data.read(self._state.offset, count)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = count
        self._type[id] = "bytes"
        self._order += [id]
        self._state.offset += count
        return result

    def uint16_le(self, name, id=None):
        if id is None:
            id = name
        result = self._data.read_uint16_le(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 2
        self._type[id] = "uint16_le"
        self._order += [id]
        self._state.offset += 2
        return result

    def uint32_le(self, name, id=None):
        if id is None:
            id = name
        result = self._data.read_uint32_le(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 4
        self._type[id] = "uint32_le"
        self._order += [id]
        self._state.offset += 4
        return result

    def uint64_le(self, name, id=None):
        if id is None:
            id = name
        result = self._data.read_uint64_le(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 8
        self._type[id] = "uint64_le"
        self._order += [id]
        self._state.offset += 8
        return result

    def uint16_be(self, name, id=None):
        if id is None:
            id = name
        result = self._data.read_uint16_be(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 2
        self._type[id] = "uint16_be"
        self._order += [id]
        self._state.offset += 2
        return result

    def uint32_be(self, name, id=None):
        if id is None:
            id = name
        result = self._data.read_uint32_be(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 4
        self._type[id] = "uint32_be"
        self._order += [id]
        self._state.offset += 4
        return result

    def uint64_be(self, name, id=None):
        if id is None:
            id = name
        result = self._data.read_uint64_be(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 8
        self._type[id] = "uint64_be"
        self._order += [id]
        self._state.offset += 8
        return result

    def uint8(self, name, id=None):
        if id is None:
            id = name
        result = None
        if self._big_endian:
            result = self._data.read_uint8_be(self._state.offset)
        else:
            result = self._data.read_uint8_le(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 1
        if self._big_endian:
            self._type[id] = "uint8_be"
        else:
            self._type[id] = "uint8_le"
        self._order += [id]
        self._state.offset += 1
        return result

    def uint16(self, name, id=None):
        if id is None:
            id = name
        result = None
        if self._big_endian:
            result = self._data.read_uint16_be(self._state.offset)
        else:
            result = self._data.read_uint16_le(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 2
        if self._big_endian:
            self._type[id] = "uint16_be"
        else:
            self._type[id] = "uint16_le"
        self._order += [id]
        self._state.offset += 2
        return result

    def uint32(self, name, id=None):
        if id is None:
            id = name
        result = None
        if self._big_endian:
            result = self._data.read_uint32_be(self._state.offset)
        else:
            result = self._data.read_uint32_le(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 4
        if self._big_endian:
            self._type[id] = "uint32_be"
        else:
            self._type[id] = "uint32_le"
        self._order += [id]
        self._state.offset += 4
        return result

    def uint64(self, name, id=None):
        if id is None:
            id = name
        result = None
        if self._big_endian:
            result = self._data.read_uint64_be(self._state.offset)
        else:
            result = self._data.read_uint64_le(self._state.offset)
        self.__dict__[id] = result
        self._names[id] = name
        self._start[id] = self._state.offset
        self._size[id] = 8
        if self._big_endian:
            self._type[id] = "uint64_be"
        else:
            self._type[id] = "uint64_le"
        self._order += [id]
        self._state.offset += 8
        return result

    def getStart(self):
        self.complete()
        start = None
        for i in self._order:
            if (start is None) or (self._start[i] < start):
                start = self._start[i]
        return start

    def getSize(self):
        start = self.getStart()
        end = None
        for i in self._order:
            if (end is None) or ((self._start[i] + self._size[i]) > end):
                end = self._start[i] + self._size[i]
        if end is None:
            return None
        return end - start

    def complete(self):
        for i in self._order:
            if (not self._start.has_key(i)) or (not self._size.has_key(i)):
                self.__dict__[i].complete()
                self._start[i] = self.__dict__[i].getStart()
                self._size[i] = self.__dict__[i].getSize()
