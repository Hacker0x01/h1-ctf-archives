#include <jni.h>
#include <string>

static int seq[] = {6, 3, 5, 7, 1, 0, 9, 4, 2, 8};
static char out[sizeof(seq) + 1] = {'\0'};

extern "C" JNIEXPORT jstring

JNICALL
Java_com_hackerone_mobile_challenge1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "This is the second part: \"_static_\"";
    return env->NewStringUTF(hello.c_str());
}

char cr() {
    return '_';
}

char z() {
    return 'a';
}

char pr() {
    return 'n';
}

char q() {
    return 'd';
}

char m() {
    return '_';
}

char l() {
    return 'c';
}

char t() {
    return 'o';
}

char sr() {
    return 'o';
}

char ir() {
    return 'l';
}

char b() {
    return '}';
}

extern "C" JNIEXPORT void

JNICALL
Java_com_hackerone_mobile_challenge1_MainActivity_oneLastThing(
        JNIEnv *env,
     jobject /* this */) {

    for (int i = 0; i < sizeof(seq); i++) {
        switch (seq[i]) {
            case 0:
                out[seq[i]] = cr();
                break;
            case 1:
                out[seq[i]] = z();
                break;
            case 2:
                out[seq[i]] = pr();
                break;
            case 3:
                out[seq[i]] = q();
                break;
            case 4:
                out[seq[i]] = m();
                break;
            case 5:
                out[seq[i]] = l();
                break;
            case 6:
                out[seq[i]] = t();
                break;
            case 7:
                out[seq[i]] = sr();
                break;
            case 8:
                out[seq[i]] = ir();
                break;
            case 9:
                out[seq[i]] = b();
                break;
        }
    }
}
