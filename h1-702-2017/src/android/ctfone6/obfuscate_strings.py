#!/usr/bin/env python

import os
import sys
import random
import fnmatch

xor_func = """
__attribute__((unused)) static inline char *xor_encrypt(char key, char *string, char *out, int n)
{
    int i;

    if (out != (char *)0 && out[0] != '\\x00')
        return out;

    for(i = 0; i < n; i++) {
        out[i] = string[i] ^ key;
    }
    return out;
}
"""

def str_xor(key, s):
    return [chr(ord(c) ^ key) for c in s]

def create_xor_string(line, counter):
    str_start = -1
    str_end = -1

    extracted_strings = []

    i = 0
    space_count = 0
    while i < len(line) and line[i] == " ":
        i += 1
        space_count += 1

    # String continues on next line
    if len(line) > 2 and line[-2] == "\"":
        return ([], None, counter)

    while i < len(line):
        if line[i] == '"':
            # If quote is escaped
            if i > 0 and line[i - 1] == "\"" and str_start != i:
                continue
            if -1 == str_start:
                str_start = i + 1
            else:
                str_end = i

                xor_key = random.randint(0x1e, 0x61)

                # skip 0x20, for ascii characters, it will just flip the case
                if xor_key is 0x20:
                    xor_key = 0x21

                extracted_str = str_xor(xor_key, line[str_start:str_end].decode('string_escape'))
                extracted_strings.append((counter, extracted_str))

                insert_string = "xor_encrypt({}, enc_str_{}, dec_str_{}, {})"\
                        .format(xor_key, counter, counter, len(extracted_str))

                i += len(insert_string) - (str_end - str_start)
                line = line[:str_start - 1] + insert_string\
                        + line[str_end + 1:]

                if "[]" in line:
                    line = line.replace("[]", "")\
                               .replace("char const", "char const *")\
                               .replace("const char", "const char *")

                str_start = -1
                str_end = -1
                counter += 1
        i += 1

    return extracted_strings, line, counter

def obfuscate_cpp_files(matches):
    counter = 0
    for match in matches:
        with open(match, "r") as f:
            lines = f.readlines()

        new_lines = []
        extracted_strings = []
        scope_depth = 0
        paren_depth = 0
        combine_line = ""
        for line in lines:
            # Comment, define or include
            if "TRACE" in line or line.strip().startswith("//") or line[0] == "#":
                new_lines.append(line)
                continue

            if "__asm__" in line:
                continue

            # TODO: handle objective-C NSString
            if "@\"" in line:
                new_lines.append(line)
                continue

            if combine_line != "":
                line = combine_line + line[line.find("\"") + 1:]
                combine_line = ""

            if "{" in line:
                scope_depth += 1
            if "}" in line:
                scope_depth -= 1

            new_line = line

            if scope_depth > 0:
                extract_strings, new_line, counter = create_xor_string(line, counter)

                # String continues on next line
                if new_line == None:
                    combine_line = line[:-2]
                    continue

                extracted_strings.extend(extract_strings)

            new_lines.append(new_line)

            if new_line.count("(") > new_line.count(")"):
                paren_depth += 1
            if new_line.count(")") > new_line.count("("):
                paren_depth -= 1

        global_strs = []
        for n, extracted_string in extracted_strings:
            hex_string = ", ".join(["'\\x{}'".format(hex(ord(c))[2:]) for c in extracted_string] + ["0x0"])
            global_enc_str = "static char enc_str_{}[] __attribute__((unused)) = {{{}}};\n".format(n, hex_string)
            global_strs.append(global_enc_str)
            global_dec_str = "static char dec_str_{}[{}] __attribute__((unused)) = {{'\\x00'}};\n".format(n, len(extracted_string) + 1)
            global_strs.append(global_dec_str)

        for global_str in global_strs:
            new_lines.insert(0, global_str)

        if len(global_strs) != 0:
            new_lines.insert(0, xor_func)

        with open(match, "w") as cpp_file:
            cpp_file.write("// GENERATED STRING OBFUSCATED FILE\n")
            cpp_file.write("// DO NOT EDIT\n\n")
            for line in new_lines:
                cpp_file.write(line)
        print "====: ", match

def do_setup2(folder):
    # cleanup first
    if folder is "/" or folder.startswith("/"):
        print "be safe, not sorry, we only support relative paths for now - no foot shooting"
        sys.exit(-1)

    obf_files = []
    for root, dirnames, filenames in os.walk(folder):
        for filename in filenames:
            # skip unit tests
            if 'tests' in filename:
                continue
            if filename.endswith(('.cpp', '.c', '.cxx', '.mm', '.m')) and not filename.endswith(('.obf.cpp', '.obf.c', '.obf.cxx', '.obf.mm', '.obf.m')):
                full_path = os.path.join(root, filename)
                base, ext = os.path.splitext(full_path)
                obf_full_path = full_path.replace(ext, ".obf" + ext)
                if not os.path.exists(obf_full_path) or os.path.getmtime(full_path) > os.path.getmtime(obf_full_path):
                    print "{} is out of date regenerating obfuscated strings...".format(obf_full_path)
                    os.system("rm -f {}".format(obf_full_path))
                    os.system("cp -rf {} {}".format(full_path, obf_full_path))
                    obf_files.append(obf_full_path)
    return obf_files

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Usage: {} <folder>".format(sys.argv[0])
        exit(0)

    folder_name = sys.argv[1]
    print folder_name
    obf_files = do_setup2(folder_name)
    obfuscate_cpp_files(obf_files)

