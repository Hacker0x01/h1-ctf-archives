#include <jni.h>
#include <string>
#include <android/log.h>

static uint32_t fnv1a_hash(const unsigned char * cp);

static size_t key_gotten = 0;

#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Add a number of milliseconds to a timeval and replace the
 * existing timeval with the new value.
 */
void timeval_addMsecs(struct timeval *a, uint32_t msecs) {
    int uSecs = (msecs%1000) * 1000 + a->tv_usec;
    a->tv_usec = uSecs % 1000000;
    a->tv_sec += msecs/1000 + uSecs/1000000;
} // addMsecs

/**
 * Convert a timeval into the number of msecs.
 */
uint32_t timeval_toMsecs(struct timeval *a) {
    return a->tv_sec * 1000 + a->tv_usec/1000;
} // timeval_toMsecs


/**
 * Subtract one timeval from another.
 */
struct timeval timeval_sub(struct timeval *a, struct timeval *b) {
    struct timeval result;
    result.tv_sec = a->tv_sec - b->tv_sec;
    result.tv_usec = a->tv_usec - b->tv_usec;
    if (a->tv_usec < b->tv_usec) {
        result.tv_sec -= 1;
        result.tv_usec += 1000000;
    }
    return result;
} // timeval_sub


/*
 * Add one timeval to another.
 */
struct timeval timeval_add(struct timeval *a, struct timeval *b) {
    struct timeval result;
    result.tv_sec = a->tv_sec + b->tv_sec;
    result.tv_usec = a->tv_usec + b->tv_usec;
    if (result.tv_usec >= 1000000) {
        result.tv_sec += 1;
        result.tv_usec -= 1000000;
    }
    return result;
} // timeval_add

/**
 * Return the number of milliseconds until  future time value.
 */
uint32_t timeval_durationFromNow(struct timeval *a) {
    struct timeval b;
    gettimeofday(&b, NULL);
    struct timeval delta = timeval_sub(a, &b);
    if (delta.tv_sec < 0) {
        return 0;
    }
    return timeval_toMsecs(&delta);
    // assuming that a is later than b, then the result is a-b
} // timeval_durationFromNow

void get_key_cooldown() {
    struct timeval t0;
    gettimeofday(&t0, 0);

    timeval_addMsecs(&t0, 10000);

    if (key_gotten > 50) {
        while (timeval_durationFromNow(&t0) != 0) {}
        key_gotten = 0;
    } else {
        key_gotten++;
    }
}

extern "C" JNIEXPORT void
JNICALL
Java_com_hackerone_mobile_challenge2_MainActivity_resetCoolDown(
        JNIEnv *env,
        jobject
    ) {
    key_gotten = 0;
}

extern "C" JNIEXPORT jbyteArray
JNICALL
Java_com_hackerone_mobile_challenge2_MainActivity_getKey(
        JNIEnv *env,
        jobject clazz, jstring pinStr) {
    const char *nativePin = env->GetStringUTFChars(pinStr, 0);

    get_key_cooldown();

    uint32_t key[8] = {0};
    unsigned char tmp[10];

    for (int i = 0; i < strlen(nativePin) * 2; i++) {
        size_t tmp_i = i % strlen(nativePin);

        uint8_t c = (uint8_t) (nativePin[tmp_i] - '0');

        memset(tmp, 10, '\0');

        for (int j = 0; j < c; j++) {
            tmp[j] = (unsigned char)(c + '0');
            tmp[j + 1] = '\0';
        }

        uint32_t hash = fnv1a_hash(tmp);

        key[i % 8] ^= hash;
    }

    env->ReleaseStringUTFChars(pinStr, nativePin);
    jbyteArray out = env->NewByteArray(sizeof(key));
    env->SetByteArrayRegion(out, 0, sizeof(key), (jbyte *)key);
    return out;
}

static uint32_t fnv1a_hash(const unsigned char * cp)
{
    uint32_t hash = 0x811c9dc5;
    while (*cp) {
        hash ^= *cp++;
        hash *= 0x01000193;
    }
    return hash;
}
