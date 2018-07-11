#include <jni.h>
#include <string>
#include <sys/prctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/system_properties.h>
#include <fcntl.h>
#define ALWAYS_INLINE inline __attribute__((always_inline))
#define DONT_INLINE __attribute__ ((noinline))
#define NO_RETURN __attribute__ ((noreturn))


#include <android/log.h>
#define LOGD(LOG_TAG, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

//#define DEBUG 1

#ifdef DEBUG
#define TRACE(...)  LOGD("BOOYA", __VA_ARGS__)
#else
#define TRACE(...)
#endif

static bool has_setdumpable = false;

ALWAYS_INLINE
static void my_abort() {
//    asm volatile(
//    "ldr r0,=0xdeadbeef"
//    );
    //abort();
//    volatile uintptr_t p = 0xdeadbeef;
//    *((char*)(p)) = 36;

    // didn't fail,
    raise(SIGSEGV);
}

ALWAYS_INLINE
//DONT_INLINE
static void good_abort_show_flag() {
    TRACE("SHOWING GOOD ABORT!");
    // This dumps the key into the stack of a segfault dump

    // setup registers for failure
    asm volatile(
            "ldr r0,=0x5f53d58f;"
            "ldr r3,=0x5f53d58f;"
            "add r0, r0, r3;"       // bea7ab1e
            "ldr r1,=0x7d670f2a;"
            "ldr r3,=0x7d670f2b;"
            "add r1, r1, r3;"       // face1e55
            "ldr r2,=0x6d3d5d2f;"
            "ldr r3,=0x6d3d5d2f;"
            "add r2, r2, r3;"       // da7aba5e
    );

    // fail by jumping to bad location
    asm volatile(
            "ldr r3,=0x6f56dd5f;"
            "ldr lr,=0x6f56dd5f;"  // 0xdeadbabe (even address for alignment)
            "add lr,lr,r3;"
            "bx lr;"
    );

    // setup stack
    // lol overflow this shit to load the secrets there
//    uint32_t stack_overflower[1] = {0};
//    int overflow_count = 10;
//    for (int i = 0; i < overflow_count; i++) {
//        stack_overflower[i] = 0xdeadbaad;
//    }

    // abort at 0xDEADBEEF
    // NOTE: This doesn't work, compiler optimizes this weirdly, even with -O0 for some reason, causes
    // some wierdness
//    uintptr_t p = 0xdeadbeef;
//    *((char*)(p)) = 36;
//
//    if ( p ) {
//        first = p + 1;
//        second = first + p;
//    }
}

ALWAYS_INLINE
static bool is_present(char *pathname) {
    return (access(pathname, F_OK) == -1);
}

//
// Yup, this is just a small subset for a good reason.  We don't
// want to burn anything, this is a "most likely to hit" list
// but nowhere near what it should be in our real root detection
//
char *g_su_paths[] = {
        "/su/bin/su",
        "/system/bin/daemonsu",
        "/system/xbin/daemonsu",
        "/sbin/su",
        "/system/bin/su",
        "/system/xbin/su",
        "/data/local/xbin/su",
        "/data/local/bin/su",
        "/system/sd/xbin/su",
        "/system/bin/failsafe/su",
        "/data/local/su"
};

ALWAYS_INLINE
static bool is_rooted() {

    for (int i = 0; i < (sizeof(g_su_paths)/sizeof(char*)); i++) {
        TRACE("Checking %s", g_su_paths[i]);
        if ( is_present(g_su_paths[i])) {
            TRACE("Rooted, found %s", g_su_paths[i]);
            return true;
        }
    }

    // if we didn't find anything, not rooted
    TRACE("Not rooted");
    return false;
}

ALWAYS_INLINE
static bool is_debugger_attached() {
    const int bufsize = 1024;
    char filename[bufsize];
    char line[bufsize];
    int pid = getpid();
    sprintf(filename, "/proc/%d/status", pid);
    FILE* fd = fopen(filename, "r");
    if (fd != NULL)
    {
        while (fgets(line, bufsize, fd))
        {
            if (strncmp(line, "TracerPid", 9) == 0)
            {
                int statue = atoi(&line[10]);
                TRACE("%s", line);
                if (statue != 0)
                {
                    TRACE("be attached !! kill %d", pid);
                    fclose(fd);
                    return true;
                }
                break;
            }
        }
        fclose(fd);
    } else
    {
        TRACE("open %s fail...", filename);
        return false;
    }
    return false;
}

ALWAYS_INLINE
static void disable_dumpable() {
    if ( (prctl(PR_GET_DUMPABLE) != 0) ) {
        // if we've already set dumpable and someone tried to set it back, well just abort bro
        if (has_setdumpable) {
            my_abort();
        }
        // if this is the first time, then set it to not dumpable
        prctl(PR_SET_DUMPABLE, 0, 0, 0, 0);
        has_setdumpable = true;
    }
}

ALWAYS_INLINE
static bool is_palindrome( char array[] )
{
    TRACE("Palindrome: %s", array);
    bool isPalindrome = true;
    int size = 0, index = 0, startingPos = 0, count1 = 0;
    // Step 1
    while ( array[size] != '\0' )
    {
        size++;
    }
    char array1[size + 1];
    // Step 2
    while ( index < size )
    {
        while ( (array[index] >= 'A' and array[index] <= 'Z') or
                (array[index] >= 'a' and array[index] <= 'z')    )
            index++;
        for ( int count = startingPos; count < index; count++ )
        {
            array1[count1] = array[count];
            count1++;
        }
        index++;
        startingPos = index;
    }
    array1[count1] = '\0';
    //Step 3
    index = 0;
    while ( index <= (count1 - 1)/2 and isPalindrome )
    {
        if ( array1[index] != array1[count1 - index - 1]      and
             array1[index] != array1[count1 - index - 1] - 32 and
             array1[index] != array1[count1 - index - 1] + 32 and
             array1[index] - 32 != array1[count1 - index - 1] and
             array1[index] + 32 != array1[count1 - index - 1]     )
            isPalindrome = false;
        index++;
    }
    // Step 4
    return isPalindrome;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {

#ifdef ENABLE_CORE_DUMPABLE
    prctl(PR_SET_DUMPABLE, 1);
#else
    // make sure we can't dump
    disable_dumpable();
#endif

    return JNI_VERSION_1_6;
}

extern "C"
void
Java_com_h1702ctf_ctfone5_CruelIntentions_one(
        JNIEnv *env,
        jobject /* this */) {

#ifdef ENABLE_CORE_DUMPABLE
    prctl(PR_SET_DUMPABLE, 1);
#else
    // make sure we can't dump
    disable_dumpable();
#endif
    // initial debugger check
    if (is_debugger_attached()) {
        TRACE("debugger detected, fail out");
        my_abort();
    }

    char* things_for_palindrome[] = {"A but tuba", // 0th element must be a palindrome!!!
                                     "A car, a man, a maraca",
                                     "A dog pities the fool",
                                     "A dog, a plan, a canal: pagoda",
                                     "A dog is no one",
                                     "A dog! A panic in a pagoda!"};
    int pick = 0;
    int setme = 0;

    // root checks
    TRACE("Checking root...");
    if (!is_rooted()) {
        goto check_debugger;
    }
    // if rooted, do some random shit and end up doing nothing
    pick = random() % ((sizeof(things_for_palindrome)/sizeof(char*)));
random_shit:
    if (is_palindrome( things_for_palindrome[pick] )) {
        //goto check_debugger; // uncomment this and comment out next goto if you want to test
        goto do_nothing; // will always get to here
    } else {
        pick = 0; // the 0'th element should always be a palindrome
        goto random_shit;
    }

check_debugger:
    if (is_debugger_attached()) {
        TRACE("debugger detected, fail out");
        my_abort();
    } else {

#define ENABLE_EXTRA_CHECK_HARD_MODE 1

#ifdef ENABLE_EXTRA_CHECK_HARD_MODE
        // ya you have to do something (i.e. this is for all the smartasses that run this on a non-rooted device)
        // basically this is to make sure there's one thing to nop or hook
        // NOTE TO SELF: This might make this level TOOO HARD????  Ask for feedback
        char mobsec_setme_str[PROP_VALUE_MAX] = {'\0'};

        __system_property_get("mobsec.setme", mobsec_setme_str);
        setme = atoi(mobsec_setme_str);
        if (setme == 1) {
            goto good_abort;
        } else {
            goto do_nothing;
        }
#else
        goto good_abort;
#endif
    }

actual_good_abort:
    good_abort_show_flag();
    goto do_nothing;
good_abort:
    goto actual_good_abort;
do_nothing:
    return;
}

#include "sodium.h"

// repro: go to ctf/tools
// $ ./generate_encrypted_flag.py bea7ab1e face1e55 da7aba5e cApwN{sPEaK_FrieNd_aNd_enteR!}
const unsigned char ciphertext_flag[] = {47, 165, 68, 2, 241, 28, 56, 3, 244, 159, 0, 4, 56, 98, 217, 225, 83, 20, 61, 127, 37, 26, 77, 210, 188, 72, 22, 78, 217, 180};

extern "C"
jstring
Java_com_h1702ctf_ctfone5_MainActivity_flag(
        JNIEnv *env,
        jobject, /* this */
        jstring secretOne,
        jstring secretTwo,
        jstring secretThree) {


    size_t ciphertext_len = sizeof ciphertext_flag;

    const char *flagOneChars = env->GetStringUTFChars(secretOne, 0);
    const char *flagTwoChars = env->GetStringUTFChars(secretTwo, 0);
    const char *flagThreeChars = env->GetStringUTFChars(secretThree, 0);

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

    crypto_stream_xsalsa20_xor(decrypted, (const unsigned char *) ciphertext_flag, ciphertext_len, nonce, key);

    decrypted[ciphertext_len] = '\0';

    env->ReleaseStringUTFChars(secretOne, flagOneChars);
    env->ReleaseStringUTFChars(secretOne, flagTwoChars);
    env->ReleaseStringUTFChars(secretOne, flagThreeChars);

    return env->NewStringUTF((const char *) decrypted);

}