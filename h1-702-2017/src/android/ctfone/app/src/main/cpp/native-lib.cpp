// GENERATED STRING OBFUSCATED FILE
// DO NOT EDIT


__attribute__((unused)) static inline char *xor_encrypt(char key, char *string, char *out, int n)
{
    int i;

    if (out != (char *)0 && out[0] != '\x00')
        return out;

    for(i = 0; i < n; i++) {
        out[i] = string[i] ^ key;
    }
    return out;
}
static char dec_str_4[4] __attribute__((unused)) = {'\x00'};
static char enc_str_4[] __attribute__((unused)) = {'\x70', '\x71', '\xe', 0x0};
static char dec_str_3[8] __attribute__((unused)) = {'\x00'};
static char enc_str_3[] __attribute__((unused)) = {'\x5e', '\x49', '\x5d', '\x59', '\x49', '\x5f', '\x58', 0x0};
static char dec_str_2[30] __attribute__((unused)) = {'\x00'};
static char enc_str_2[] __attribute__((unused)) = {'\x5e', '\x52', '\x50', '\x12', '\x55', '\xc', '\xa', '\xd', '\xf', '\x5e', '\x49', '\x5b', '\x12', '\x5e', '\x49', '\x5b', '\x52', '\x53', '\x58', '\x12', '\x6f', '\x58', '\x4c', '\x48', '\x58', '\x4e', '\x49', '\x52', '\x4f', 0x0};
static char dec_str_1[73] __attribute__((unused)) = {'\x00'};
static char enc_str_1[] __attribute__((unused)) = {'\x68', '\xf', '\x6c', '\x7d', '\x6c', '\xc', '\x6f', '\x47', '\x6b', '\x66', '\x5a', '\x71', '\x68', '\x79', '\x6c', '\x71', '\x68', '\x53', '\x4e', '\x50', '\x5a', '\xf', '\x52', '\x4d', '\x69', '\x6a', '\x68', '\x55', '\x68', '\xf', '\x74', '\x67', '\x6a', '\x68', '\x5a', '\x4d', '\x6a', '\x55', '\xe', '\x49', '\x5d', '\x79', '\xf', '\x6b', '\x5f', '\x55', '\x4e', '\x48', '\x64', '\x68', '\x6b', '\x46', '\x70', '\x52', '\x6c', '\x4f', '\x5c', '\x7b', '\x6c', '\x5f', '\x5b', '\x54', '\x7f', '\xb', '\x6f', '\xc', '\x5d', '\x7', '\x6e', '\x6f', '\x51', '\x3', 0x0};
static char dec_str_0[14] __attribute__((unused)) = {'\x00'};
static char enc_str_0[] __attribute__((unused)) = {'\x6f', '\x1a', '\x7b', '\x52', '\x41', '\x52', '\x5b', '\x4', '\x1a', '\x71', '\x5b', '\x56', '\x50', 0x0};
#include <jni.h>
#include <string>
#include "sodium.h"

// repro: go to ctfone/tools
// $ ./generate_flag_four.py cApwN{WELL_THAT_WAS_SUPER_EASY} CAPWN{CRYP706R4PHY_15_H4RD_BR0} cApwN{1_4m_numb3r_7hr33} cApwN{w1nn3r_w1nn3r_ch1ck3n_d1nn3r!}
const unsigned char ciphertext_flag4[] = {72, 4, 109, 180, 140, 138, 109, 87, 76, 197, 75, 65, 209, 220, 178, 192, 144, 29, 226, 107, 133, 21, 37, 253, 145, 31, 98, 0, 69, 169, 84, 90, 133, 117, 165, 252};

extern "C"
jstring
Java_com_h1702ctf_ctfone_MonteCarlo_functionnameLeftbraceOneCommaTwoCommaThreeCommaRightbraceFour(
        JNIEnv *env,
        jobject /* this _unused_*/,
        jstring flagOne,
        jstring flagTwo,
        jstring flagThree) {


    size_t ciphertext_len = sizeof ciphertext_flag4;

    const char *flagOneChars = env->GetStringUTFChars(flagOne, 0);
    const char *flagTwoChars = env->GetStringUTFChars(flagTwo, 0);
    const char *flagThreeChars = env->GetStringUTFChars(flagThree, 0);

    size_t flagOneLen = strlen(flagOneChars);
    size_t flagTwoLen = strlen(flagTwoChars);
    size_t flagThreeLen = strlen(flagThreeChars);

    unsigned char hashOne[crypto_generichash_BYTES];
    unsigned char hashTwo[crypto_generichash_BYTES];
    unsigned char hashThree[crypto_generichash_BYTES];


    crypto_generichash(hashOne, sizeof hashOne,
                       (const unsigned char*)flagOneChars, flagOneLen,
                       NULL, 0);
    crypto_generichash(hashTwo, sizeof hashTwo,
                       (const unsigned char*)flagTwoChars, flagTwoLen,
                       NULL, 0);
    crypto_generichash(hashThree, sizeof hashThree,
                       (const unsigned char*)flagThreeChars, flagThreeLen,
                       NULL, 0);

    // first 12 bits of blake2b hash of flag 1 | first 12 bites of blake2b of flag 2 is the nonce
    unsigned char nonce[crypto_stream_NONCEBYTES];
    memcpy(nonce, hashOne, 12);
    memcpy(&nonce[12], hashTwo, 12);


    // blake2b hash flag 3 is the key -- both 32 bytes
    unsigned char key[crypto_stream_KEYBYTES];
    memcpy(key, hashThree, sizeof key);

    unsigned char decrypted[ciphertext_len + 1];

    crypto_stream_xsalsa20_xor(decrypted, (const unsigned char *) ciphertext_flag4, ciphertext_len, nonce, key);

    decrypted[ciphertext_len] = '\0';

    env->ReleaseStringUTFChars(flagOne, flagOneChars);
    env->ReleaseStringUTFChars(flagOne, flagTwoChars);
    env->ReleaseStringUTFChars(flagOne, flagThreeChars);

    return env->NewStringUTF((const char *) decrypted);
}

extern "C"
jstring
Java_com_h1702ctf_ctfone_Requestor_hName(JNIEnv *env)
{
    char *hname = xor_encrypt(55, enc_str_0, dec_str_0, 13);
    return env->NewStringUTF(hname);
}

// repro: echo "cApwN{1_4m_numb3r_7hr33}" | base64 | base64 | base64
extern "C"
jstring
Java_com_h1702ctf_ctfone_Requestor_hVal(JNIEnv *env)
{
    char *hname = xor_encrypt(62, enc_str_1, dec_str_1, 72);
    return env->NewStringUTF(hname);
}

extern "C"
void
Java_com_h1702ctf_ctfone_ArraysArraysArrays_x(JNIEnv *env)
{
    jclass cls = env->FindClass(xor_encrypt(61, enc_str_2, dec_str_2, 29));
    jmethodID mid = env->GetStaticMethodID(cls, xor_encrypt(44, enc_str_3, dec_str_3, 7), xor_encrypt(88, enc_str_4, dec_str_4, 3));
    if (mid == 0)
        return;
    env->CallStaticVoidMethod(cls, mid);
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    return JNI_VERSION_1_6;
}
