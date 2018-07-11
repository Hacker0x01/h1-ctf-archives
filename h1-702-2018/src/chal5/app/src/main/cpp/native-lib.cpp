#include <jni.h>
#include <string>

static char something[512];


/**
 * `decode.c' - b64
 *
 * copyright (c) 2014 joseph werle
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 *  Memory allocation functions to use. You can define b64_malloc and
 * b64_realloc to custom functions if you want.
 */

#ifndef b64_malloc
#  define b64_malloc(ptr) malloc(ptr)
#endif
#ifndef b64_realloc
#  define b64_realloc(ptr, size) realloc(ptr, size)
#endif

/**
 * Base64 index table.
 */

static const char b64_table[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encode `unsigned char *' source with `size_t' size.
 * Returns a `char *' base64 encoded string.
 */

char *
b64_encode(const unsigned char *, size_t);

/**
 * Dencode `char *' source with `size_t' size.
 * Returns a `unsigned char *' base64 decoded string.
 */
unsigned char *
b64_decode(const char *, size_t);

/**
 * Dencode `char *' source with `size_t' size.
 * Returns a `unsigned char *' base64 decoded string + size of decoded string.
 */
unsigned char *
b64_decode_ex(const char *, size_t, size_t *);

#ifdef b64_USE_CUSTOM_MALLOC
extern void* b64_malloc(size_t);
#endif

#ifdef b64_USE_CUSTOM_REALLOC
extern void* b64_realloc(void*, size_t);
#endif

unsigned char *
b64_decode(const char *src, size_t len) {
    return b64_decode_ex(src, len, NULL);
}

unsigned char *
b64_decode_ex(const char *src, size_t len, size_t *decsize) {
    int i = 0;
    int j = 0;
    int l = 0;
    size_t size = 0;
    unsigned char *dec = NULL;
    unsigned char buf[3];
    unsigned char tmp[4];

    // alloc
    dec = (unsigned char *) b64_malloc(1);
    if (NULL == dec) { return NULL; }

    // parse until end of source
    while (len--) {
        // break if char is `=' or not base64 char
        if ('=' == src[j]) { break; }
        if (!(isalnum(src[j]) || '+' == src[j] || '/' == src[j])) { break; }

        // read up to 4 bytes at a time into `tmp'
        tmp[i++] = src[j++];

        // if 4 bytes read then decode into `buf'
        if (4 == i) {
            // translate values in `tmp' from table
            for (i = 0; i < 4; ++i) {
                // find translation char in `b64_table'
                for (l = 0; l < 64; ++l) {
                    if (tmp[i] == b64_table[l]) {
                        tmp[i] = l;
                        break;
                    }
                }
            }

            // decode
            buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
            buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
            buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

            // write decoded buffer to `dec'
            dec = (unsigned char *) b64_realloc(dec, size + 3);
            if (dec != NULL) {
                for (i = 0; i < 3; ++i) {
                    dec[size++] = buf[i];
                }
            } else {
                return NULL;
            }

            // reset
            i = 0;
        }
    }

    // remainder
    if (i > 0) {
        // fill `tmp' with `\0' at most 4 times
        for (j = i; j < 4; ++j) {
            tmp[j] = '\0';
        }

        // translate remainder
        for (j = 0; j < 4; ++j) {
            // find translation char in `b64_table'
            for (l = 0; l < 64; ++l) {
                if (tmp[j] == b64_table[l]) {
                    tmp[j] = l;
                    break;
                }
            }
        }

        // decode remainder
        buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
        buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
        buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

        // write remainer decoded buffer to `dec'
        dec = (unsigned char *) b64_realloc(dec, size + (i - 1));
        if (dec != NULL) {
            for (j = 0; (j < i - 1); ++j) {
                dec[size++] = buf[j];
            }
        } else {
            return NULL;
        }
    }

    // Make sure we have enough space to add '\0' character at end.
    dec = (unsigned char *) b64_realloc(dec, size + 1);
    if (dec != NULL) {
        dec[size] = '\0';
    } else {
        return NULL;
    }

    // Return back the size of decoded string if demanded.
    if (decsize != NULL) {
        *decsize = size;
    }

    return dec;
}

#ifdef __cplusplus
}
#endif

void str_replace(char *target, const char *needle, const char *replacement) {
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
}

extern "C" JNIEXPORT jbyteArray
JNICALL
Java_com_hackerone_mobile_challenge5_PetHandler_censorCats(
        JNIEnv *env,
        jobject clazz, jbyteArray catString) {
    const char *cat_str = (char *)env->GetByteArrayElements(catString, NULL);

    char censored_cats[512];

    memcpy(censored_cats, cat_str, 512 + 8 * 6);

    jbyteArray out = env->NewByteArray(sizeof(censored_cats));
    env->SetByteArrayRegion(out, 0, sizeof(censored_cats), (jbyte *)censored_cats);
    return out;
}

extern "C" JNIEXPORT jbyteArray
JNICALL
Java_com_hackerone_mobile_challenge5_PetHandler_censorDogs(
        JNIEnv *env,
        jobject clazz, jint length, jstring dogString) {
    const char *dog_str = env->GetStringUTFChars(dogString, NULL);

    char *dog_str_dec = (char *)b64_decode(dog_str, strlen(dog_str));

    char censored_dogs[512];
    char new_censored_dogs[512];

    if (strlen(dog_str_dec) > sizeof(censored_dogs)) {
        free(dog_str_dec);
        return NULL;
    }
    strcpy(censored_dogs, dog_str_dec);

    strcpy(something, dog_str_dec);

    str_replace(censored_dogs, "dog", "xxx");

    free(dog_str_dec);

    jbyteArray out = env->NewByteArray(length);
    env->SetByteArrayRegion(out, 0, length, (jbyte *)new_censored_dogs);
    return out;
}

extern "C" JNIEXPORT jlong
JNICALL
Java_com_hackerone_mobile_challenge5_PetHandler_getSomething(JNIEnv *env, jobject clazz) {
    return (jlong)&something;
}

/*
 * Meat Inventory
 *
 * Piece of Meat - Name and weight
 * Meat Cart - Array of Meats
 *
 * * Add Item
 * * Remove Item
 * * Get Items
 * * Move Items

#define MAX_NAME_SIZE = 128;
#define MAX_CART_COUNT = 16;

struct meat {
    char name[MAX_NAME_SIZE];
    size_t weight;
};

struct cart {
    struct meat meat_list[MAX_CART_COUNT];
    size_t meat_count;
};

static struct cart meat_cart;

void addMeat() {
}

void removeMeat() {
}

void viewMeat() {
    return get_meat_output(&meat_cart)
}

void get_meat_output(*struct cart print_cart) {
    jbyteArray output_meat;
    for (size_t i = 0; i < print_cart.meat_count; i++) {
        string = "Name: {}, Weight: {}".format(print_cart.meat_list[i].name, print_cart.meat_list[i].weight);
        output_meat.append(string);
    }
    return output_meat;
}

void condenseMeat(size_t minimum_weight) {
    struct cart new_meat_cart;
    new_meat_cart.meat_count = meat_cart.meat_count;

    for (size_t i = 0; i < meat_cart.meat_count; i++) {
        if (meat_cart.meat_list[i].weight >= minimum_weight) {
            new_meat_cart.meat_list[j] =
        }
    }

    get_meat_output(&new_meat_cart);
}
*/
