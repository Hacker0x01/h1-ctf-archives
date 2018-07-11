
data = ""
with open("flag.txt", "r") as f:
    data = f.read()

letters = [l[1:-1] for l in data.split("---")]

#code = ""
code = "static int *letterArray[{}];\n\n".format(len(letters))
for n, letter in enumerate(letters):
    letter_array = []
    p = letter.replace("\n", "")
    for c in p:
        if c == " ":
            letter_array.append("0")
        else:
            letter_array.append("1")

    letter_array = letter_array[1:] + [letter_array[0]]
    code += "static int letter_{}[15] = {{ {} }};\n".format(n, ", ".join(letter_array))

for n, letter in enumerate(letters):
    code += "letterArray[{}] = letter_{};\n".format(n, n)

    #code += "let letter_{} = [{}]\n".format(n, ", ".join(letter_array))
    #code += "self.letterArray.append(letter_{})\n".format(n)

print code
