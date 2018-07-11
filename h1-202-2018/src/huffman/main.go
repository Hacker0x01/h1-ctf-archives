package pinkfloyd

import "fmt"

var secreto = []byte{0xd, 0x9, 0x18, 0x2c, 0x48, 0x15, 0x4, 0x5c, 0x12, 0x2, 0x0, 0x6, 0x1c, 0xd, 0x18, 0x3f, 0x6c, 0xe, 0xe, 0x6c, 0x1e, 0x4, 0x11, 0x6, 0x3, 0x0, 0xb, 0x39, 0x41, 0xb, 0x19, 0x41, 0xb, 0x16}
var keyo = []byte{0x6b, 0x65, 0x79, 0x4b, 0x33, 0x79, 0x6b, 0x33, 0x79, 0x6b, 0x65, 0x59}

func e(input, key []byte) (output string) {
  for i := 0; i < len(input); i++ {
    output += string(input[i] ^ key[i % len(key)])
  }
  return output
}

func DarkSideOfTheMoon() {
  fmt.Println(e(secreto, keyo))
}
