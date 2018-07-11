# The base macho pass class

from macho import *

class MachoPass(object):
    def __init__(self, bin_name):
        self.bin_name = bin_name

        self.binary_data = None
        with open(self.bin_name, "r") as macho_file:
            self.binary_data = BinaryData(macho_file.read())

        self.macho = None
        self.arches = self.get_macho_type(self.binary_data)

        if self.arches is None:
            exit(1)

        self.is_fat = self.arches is not None

        for arch in self.arches:
            try:
                arch.parse_objc()
            except:
                logging.debug("Had an error parsing objc")
                raise

    def get_macho_type(self, binary_data):
        macho = MachOFile(binary_data)

        if not macho.valid:
            macho = FatMachOFile(binary_data)

            if not macho.valid:
                logging.error("Invalid macho provided")
                return None
            else:
                arches = macho.macho_arches
                self.macho = macho
        else:
            arches = [macho]
            self.macho = macho

        return arches

    def save(self, out_file):
        logging.debug("Saving MachO")
        self.macho.save(out_file)

    def print_classes(self):
        for arch in self.arches:
            for objc_class in arch.classes:
                print objc_class["name"]
                class_data = objc_class["data"]

                for symbol_type in class_data.keys():
                    if symbol_type == "class":
                        continue

                    for symbol in class_data[symbol_type]:
                        print "\t{}: {}".format(hex(symbol.name_ptr), symbol.name)

