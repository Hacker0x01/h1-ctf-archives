#include <jni.h>
#include <string>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define RELEASE 1

extern "C"
{

#define THING_SIZE 32
#define THING1_START THING_SIZE * 0
#define THING2_START THING_SIZE * 1
#define THING3_START THING_SIZE * 2
#define THING4_START THING_SIZE * 3
#define THING5_START THING_SIZE * 4
#define THING6_START THING_SIZE * 5
#define THING7_START THING_SIZE * 6

char bigThing[224] = {0};

void anti_debug();
bool is_frida_server_listening();

void
JNICALL
Java_com_hackerone_candidatevote_MainActivity_aaaaaaaaa(
        JNIEnv *env,
        jclass clazz,
        jstring str) {
    anti_debug();
    const char *tmp = env->GetStringUTFChars(str, NULL);
    memcpy((void *)(bigThing + THING1_START), (void *)tmp, THING_SIZE);
}

void
JNICALL
Java_com_hackerone_candidatevote_CandidateClient_aaaaaa(
        JNIEnv *env,
        jclass clazz,
        jstring str) {
    const char *tmp = env->GetStringUTFChars(str, NULL);
    memcpy((void *)(bigThing + THING2_START), (void *)tmp, THING_SIZE);
}

void
JNICALL
Java_com_hackerone_candidatevote_AntiTamper_aaaaaaaaaaaaaa(
        JNIEnv *env,
        jclass clazz,
        jstring str) {
    const char *tmp = env->GetStringUTFChars(str, NULL);
    memcpy((void *)(bigThing + THING3_START), (void *)tmp, THING_SIZE);
}

void
JNICALL
Java_com_hackerone_candidatevote_AntiTamper_aa(
        JNIEnv *env,
        jclass clazz,
        jstring str) {
    const char *tmp = env->GetStringUTFChars(str, NULL);
    memcpy((void *)(bigThing + THING4_START), (void *)tmp, THING_SIZE);
}

void
JNICALL
Java_com_hackerone_candidatevote_AntiTamper_aaaaaaaaaaaa(
        JNIEnv *env,
        jclass clazz,
        jstring str) {
    is_frida_server_listening();
    const char *tmp = env->GetStringUTFChars(str, NULL);
    memcpy((void *)(bigThing + THING5_START), (void *)tmp, THING_SIZE);
}

void
JNICALL
Java_com_hackerone_candidatevote_CandidateClient_aaaaaaaaaa(
        JNIEnv *env,
        jclass clazz,
        jstring str) {
    const char *tmp = env->GetStringUTFChars(str, NULL);
    memcpy((void *)(bigThing + THING6_START), (void *)tmp, THING_SIZE);
}

void
JNICALL
Java_com_hackerone_candidatevote_MainActivity_aaaa(
        JNIEnv *env,
        jclass clazz,
        jstring str) {
    const char *tmp = env->GetStringUTFChars(str, NULL);
    memcpy((void *)(bigThing + THING7_START), (void *)tmp, THING_SIZE);
}

jstring
JNICALL
Java_com_hackerone_candidatevote_MainActivity_getCreds(
        JNIEnv *env,
        jclass clazz,
        jstring str) {
    char thing[THING_SIZE * 4];
    memcpy((void *)thing, (void *)(bigThing + THING4_START), THING_SIZE * 4);
    return env->NewStringUTF(thing);
}

jstring
JNICALL
Java_com_hackerone_candidatevote_AddCandidateActivity_getJs(
        JNIEnv *env,
        jstring str) {
    char *js = "var a=['aHR0cDovL2xvY2FsaG9zdDo5MDAxL2FkbWlu','c2V0UmVxdWVzdEhlYWRlcg==','QmFzaWMg','aWZvcmdvdDp0aGVwYXNzd29yZA==','c2VuZA==','YXBwbHk=','SUthTWQ=','YWV2QW0=','WUxBRnA=','U01rR2I=','Y29uc29sZQ==','Nnw1fDF8M3wyfDR8MHw4fDc=','b25pSE4=','c3BsaXQ=','ZXhjZXB0aW9u','d2Fybg==','aW5mbw==','ZGVidWc=','ZXJyb3I=','bG9n','dHJhY2U=','cmVzcG9uc2VUZXh0','aGFzT3duUHJvcGVydHk=','cHVzaA==','d1VkUnk=','am9pbg==','Z2V0TmFtZQ==','Z2V0VXJs','PGgxPkNhbmRpZGF0ZTwvaDE+PGgzPk5hbWU6IHt7IC5OYW1lIH19PC9oMz48aW1nIHNyYz0ie3sgLlVybCB9fSIgLz4=','YWRkRXZlbnRMaXN0ZW5lcg==','bG9hZA==','b3Blbg==','R0VU'];(function(c,d){var e=function(f){while(--f){c['push'](c['shift']());}};e(++d);}(a,0x1b2));var b=function(c,d){c=c-0x0;var e=a[c];if(b['initialized']===undefined){(function(){var f;try{var g=Function('return\\x20(function()\\x20'+'{}.constructor(\\x22return\\x20this\\x22)(\\x20)'+');');f=g();}catch(h){f=window;}var i='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=';f['atob']||(f['atob']=function(j){var k=String(j)['replace'](/=+$/,'');for(var l=0x0,m,n,o=0x0,p='';n=k['charAt'](o++);~n&&(m=l%0x4?m*0x40+n:n,l++%0x4)?p+=String['fromCharCode'](0xff&m>>(-0x2*l&0x6)):0x0){n=i['indexOf'](n);}return p;});}());b['base64DecodeUnicode']=function(q){var r=atob(q);var s=[];for(var t=0x0,u=r['length'];t<u;t++){s+='%'+('00'+r['charCodeAt'](t)['toString'](0x10))['slice'](-0x2);}return decodeURIComponent(s);};b['data']={};b['initialized']=!![];}var v=b['data'][c];if(v===undefined){e=b['base64DecodeUnicode'](e);b['data'][c]=e;}else{e=v;}return e;};var e=function(){var f=!![];return function(g,h){var i=f?function(){if(h){var j=h[b('0x0')](g,arguments);h=null;return j;}}:function(){};f=![];return i;};}();var k=e(this,function(){var l={'IKaMd':function m(n,o){return n(o);},'aevAm':function p(q,r){return q+r;},'YLAFp':'return\\x20(function()\\x20','FzUEe':'{}.constructor(\\x22return\\x20this\\x22)(\\x20)','SMkGb':function s(t){return t();}};var u=function(){};var v;try{var w=l[b('0x1')](Function,l[b('0x2')](l[b('0x3')]+l['FzUEe'],');'));v=l[b('0x4')](w);}catch(x){v=window;}if(!v[b('0x5')]){v[b('0x5')]=function(y){var z={'oniHN':b('0x6')};var A=z[b('0x7')][b('0x8')]('|'),B=0x0;while(!![]){switch(A[B++]){case'0':C[b('0x9')]=y;continue;case'1':C[b('0xa')]=y;continue;case'2':C[b('0xb')]=y;continue;case'3':C[b('0xc')]=y;continue;case'4':C[b('0xd')]=y;continue;case'5':C[b('0xe')]=y;continue;case'6':var C={};continue;case'7':return C;case'8':C[b('0xf')]=y;continue;}break;}}(u);}else{v[b('0x5')][b('0xe')]=u;v[b('0x5')]['warn']=u;v[b('0x5')][b('0xc')]=u;v[b('0x5')][b('0xb')]=u;v[b('0x5')][b('0xd')]=u;v[b('0x5')][b('0x9')]=u;v[b('0x5')][b('0xf')]=u;}});k();function D(){document['write'](this[b('0x10')]);}serialize=function(E){var F={'wUdRy':function G(H,I){return H+I;}};var J=[];for(var K in E)if(E[b('0x11')](K)){J[b('0x12')](F[b('0x13')](encodeURIComponent(K)+'=',encodeURIComponent(E[K])));}return J[b('0x14')]('&');};var L=A[b('0x15')]();var M=A[b('0x16')]();var N=b('0x17');var O=serialize({'name':L,'image':M,'t':N});var P=new XMLHttpRequest();P[b('0x18')](b('0x19'),D);P[b('0x1a')](b('0x1b'),b('0x1c'));P[b('0x1d')]('Authorization',b('0x1e')+btoa(b('0x1f')));P[b('0x20')]();";


    return env->NewStringUTF(js);
}

}

#ifdef RELEASE
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
                    // Process has exited for some reason
                    _exit(0);
                }
            }
        }
    } else {
        pthread_t t;

        pthread_create(&t, NULL, monitor_pid, (void *)NULL);
    }
}
bool is_frida_server_listening() {
    struct sockaddr_in sa;

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(27047);
    inet_aton("127.0.0.1", &(sa.sin_addr));

    int sock = socket(AF_INET , SOCK_STREAM , 0);

    if (connect(sock, (struct sockaddr*)&sa, sizeof sa) != -1) {
        exit(0);
    }
    return false;
}
#endif
