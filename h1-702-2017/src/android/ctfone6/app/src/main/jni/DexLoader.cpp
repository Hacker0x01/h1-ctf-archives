//
// Created by Christopher Thompson on 6/28/17.
//

#include "DexLoader.h"
#include <jni.h>
#include <vector>
#include "android/log.h"

#include <jni.h>
#include <string>
#include <dlfcn.h>
#include <sys/mman.h>
#include <unistd.h>

#define  LOG_TAG    "DexLoader"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...)
#define LOGE(...)

#define log(FMT, ...) __android_log_print(ANDROID_LOG_VERBOSE, "JDWPFun", FMT, ##__VA_ARGS__)

// Vtable structure. Just to make messing around with it more intuitive

void anti_debug();

struct VT_JdwpAdbState {
    unsigned long x;
    unsigned long y;
    void * JdwpSocketState_destructor;
    void * _JdwpSocketState_destructor;
    void * Accept;
    void * showmanyc;
    void * ShutDown;
    void * ProcessIncoming;
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    void* lib = dlopen("libart.so", RTLD_NOW);
    if (lib == NULL) {
        dlerror();
    } else {

        struct VT_JdwpAdbState *vtable = ( struct VT_JdwpAdbState *)dlsym(lib, "_ZTVN3art4JDWP12JdwpAdbStateE");

        if (vtable == 0) {
        } else {

            unsigned long pagesize = sysconf(_SC_PAGESIZE);
            unsigned long page = (unsigned long)vtable & ~(pagesize-1);

            mprotect((void *)page, pagesize, PROT_READ | PROT_WRITE);
            vtable->ProcessIncoming = vtable->ShutDown;
            mprotect((void *)page, pagesize, PROT_READ);
        }
    }
    return  JNI_VERSION_1_6;
}


#define STRINGIFY(x) #x
#define CLEANUP for (size_t i = 0; i < localRefs.size(); i++) { \
                        env->DeleteLocalRef(localRefs[i]); \
                }
#define CHECK_JNI_ERR(x) \
        if (x == NULL) { \
            LOGE("Unable to find: %s", STRINGIFY(x)); \
            CLEANUP \
            return; \
        } \
        if ((exc = env->ExceptionOccurred())) { \
            env->ExceptionDescribe(); \
            env->ExceptionClear(); \
            LOGE("Error when trying to resolve: %s", STRINGIFY(x)); \
            return; \
        }

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_asdf_MainActivity_flerp(JNIEnv *env) {
    return "nY6FtpPFXnh,yjvc";
}

JNIEXPORT void JNICALL
Java_com_example_asdf_MainActivity_doSomethingCool(JNIEnv *env, jobject context) {
    jthrowable exc;
    std::vector<jobject> localRefs;
    // Finding classes
    jclass contextClass = env->GetObjectClass(context);
    CHECK_JNI_ERR(contextClass);
    jclass dexClassLoaderClass = env->FindClass("dalvik/system/DexClassLoader");
    CHECK_JNI_ERR(dexClassLoaderClass);
    jclass fileClass = env->FindClass("java/io/File");
    CHECK_JNI_ERR(fileClass);
    jclass classClass = env->FindClass("java/lang/Class");
    CHECK_JNI_ERR(classClass);

    // Finding methods
    jmethodID fileConstructor = env->GetMethodID(fileClass, "<init>",
                                                 "(Ljava/io/File;Ljava/lang/String;)V");
    CHECK_JNI_ERR(fileConstructor);
    jmethodID dexClassLoaderConstructor = env->GetMethodID(dexClassLoaderClass, "<init>",
                                                           "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
    CHECK_JNI_ERR(dexClassLoaderConstructor);

    jmethodID getDirMethod = env->GetMethodID(contextClass, "getDir",
                                              "(Ljava/lang/String;I)Ljava/io/File;");
    CHECK_JNI_ERR(getDirMethod);
    jmethodID getAbsolutePathMethod = env->GetMethodID(fileClass, "getAbsolutePath",
                                                       "()Ljava/lang/String;");
    CHECK_JNI_ERR(getAbsolutePathMethod);
    jmethodID getClassLoaderMethod = env->GetMethodID(contextClass, "getClassLoader",
                                                      "()Ljava/lang/ClassLoader;");
    CHECK_JNI_ERR(getClassLoaderMethod);

    jmethodID loadClassMethod = env->GetMethodID(dexClassLoaderClass, "loadClass",
                                                 "(Ljava/lang/String;)Ljava/lang/Class;");
    CHECK_JNI_ERR(loadClassMethod);

    jmethodID newInstanceMethod = env->GetMethodID(classClass, "newInstance",
                                                   "()Ljava/lang/Object;");
    CHECK_JNI_ERR(newInstanceMethod);

    anti_debug();

    // Calling methods
    jstring outDexStr = env->NewStringUTF("dex");
    localRefs.push_back(outDexStr);

    jobject optimizedDexPath = env->CallObjectMethod(context, getDirMethod, outDexStr, 0);
    CHECK_JNI_ERR(optimizedDexPath);

    jstring internalDexPathStr = env->NewStringUTF("outdex");
    localRefs.push_back(internalDexPathStr);

    jobject internalDexStoragePath = env->CallObjectMethod(context, getDirMethod, outDexStr, 0);
    CHECK_JNI_ERR(internalDexStoragePath);

    jstring internalDexStr = env->NewStringUTF("something.jar");
    localRefs.push_back(internalDexStr);

    jobject internalDexPath = env->NewObject(fileClass, fileConstructor, internalDexStoragePath,
                                             internalDexStr);
    localRefs.push_back(internalDexPath);

    jobject internalDexAbsolutePath = env->CallObjectMethod(internalDexPath, getAbsolutePathMethod);
    CHECK_JNI_ERR(internalDexAbsolutePath);
    jobject optimizedDexAbsolutePath = env->CallObjectMethod(optimizedDexPath, getAbsolutePathMethod);
    CHECK_JNI_ERR(optimizedDexAbsolutePath);

    jobject classLoader = env->CallObjectMethod(context, getClassLoaderMethod);
    CHECK_JNI_ERR(classLoader);

    jobject dexClassLoader = env->NewObject(dexClassLoaderClass, dexClassLoaderConstructor,
                                            internalDexAbsolutePath, optimizedDexAbsolutePath, NULL,
                                            classLoader);
    CHECK_JNI_ERR(dexClassLoader);
    localRefs.push_back(dexClassLoader);

    jstring classStr = env->NewStringUTF("com.example.something.IReallyHaveNoIdea");
    localRefs.push_back(classStr);

    jobject libraryProviderObject = env->CallObjectMethod(dexClassLoader, loadClassMethod,
                                                          classStr);
    CHECK_JNI_ERR(libraryProviderObject);

    jobject newLibraryProviderObject = env->CallObjectMethod(libraryProviderObject,
                                                             newInstanceMethod);
    CHECK_JNI_ERR(newLibraryProviderObject);

    jclass libraryProviderClass = env->GetObjectClass(newLibraryProviderObject);
    CHECK_JNI_ERR(libraryProviderClass);

    jmethodID getOffMyCaseMethod = env->GetMethodID(libraryProviderClass, "getOffMyCase",
                                                        "(Landroid/content/Context;Ljava/lang/String;)V");
    CHECK_JNI_ERR(getOffMyCaseMethod);

    jstring assetStr = env->NewStringUTF("secretasset");
    localRefs.push_back(assetStr);

    env->CallVoidMethod(newLibraryProviderObject, getOffMyCaseMethod, context, assetStr);

    CLEANUP
}
}

#include <sys/ptrace.h>
#include <sys/wait.h>

static int child_pid;

void *monitor_pid(void *) {
    int status;
    waitpid(child_pid, &status, 0);
    _exit(0);
}

void anti_debug() {
    child_pid = fork();
    if (child_pid == 0) {
        int ppid = getppid();
        int status;

        if (ptrace(PTRACE_ATTACH, ppid, NULL, NULL) == 0) {
            waitpid(ppid, &status, 0);
            ptrace(PTRACE_CONT, ppid, NULL, NULL);
            while (waitpid(ppid, &status, 0)) {
                if (WIFSTOPPED(status)) {
                    ptrace(PTRACE_CONT, ppid, NULL, NULL);
                } else {
                    _exit(0);
                }
            }
        }

    } else {
        pthread_t t;
        pthread_create(&t, NULL, monitor_pid, (void *)NULL);
    }
}
