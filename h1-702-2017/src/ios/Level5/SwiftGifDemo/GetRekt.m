//
//  GetRekt.c
//  SwiftGif
//
//  Created by Christopher Thompson on 6/6/17.
//  Copyright © 2017 Arne Bahlo. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "GetRekt.h"
#include </usr/include/sys/ptrace.h>
#import <sys/sysctl.h>
#import <sys/stat.h>
#import <unistd.h>

static int letter_0[15] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0 };
static int letter_1[15] = { 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0 };
static int letter_2[15] = { 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0 };
static int letter_3[15] = { 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0 };
static int letter_4[15] = { 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1 };
static int letter_5[15] = { 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0 };
static int letter_6[15] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 };
static int letter_7[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 };
static int letter_8[15] = { 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0 };
static int letter_9[15] = { 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0 };
static int letter_10[15] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0 };
static int letter_11[15] = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0 };
static int letter_12[15] = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0 };
static int letter_13[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 };
static int letter_14[15] = { 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0 };
static int letter_15[15] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0 };
static int letter_16[15] = { 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0 };
static int letter_17[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 };
static int letter_18[15] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0 };
static int letter_19[15] = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0 };
static int letter_20[15] = { 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0 };
static int letter_21[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 };
static int letter_22[15] = { 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0 };
static int letter_23[15] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0 };
static int letter_24[15] = { 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0 };
static int letter_25[15] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0 };
static int letter_26[15] = { 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0 };
static int letter_27[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 };
static int letter_28[15] = { 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0 };
static int letter_29[15] = { 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0 };
static int letter_30[15] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 };
static int letter_31[15] = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0 };
static int letter_32[15] = { 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1 };

static inline void doShift(int amt);

void __attribute__ ((constructor)) _()
{
    letterArray[0] = letter_0;
    letterArray[1] = letter_1;
    letterArray[2] = letter_2;
    letterArray[3] = letter_3;
    letterArray[4] = letter_4;
    letterArray[5] = letter_5;
    letterArray[6] = letter_6;
    letterArray[7] = letter_7;
    letterArray[8] = letter_8;
    letterArray[9] = letter_9;
    letterArray[10] = letter_10;
    letterArray[11] = letter_11;
    letterArray[12] = letter_12;
    letterArray[13] = letter_13;
    letterArray[14] = letter_14;
    letterArray[15] = letter_15;
    letterArray[16] = letter_16;
    letterArray[17] = letter_17;
    letterArray[18] = letter_18;
    letterArray[19] = letter_19;
    letterArray[20] = letter_20;
    letterArray[21] = letter_21;
    letterArray[22] = letter_22;
    letterArray[23] = letter_23;
    letterArray[24] = letter_24;
    letterArray[25] = letter_25;
    letterArray[26] = letter_26;
    letterArray[27] = letter_27;
    letterArray[28] = letter_28;
    letterArray[29] = letter_29;
    letterArray[30] = letter_30;
    letterArray[31] = letter_31;
    letterArray[32] = letter_32;
}

int something(int r, int c) {
    return letterArray[r][c];
}

static inline void doShift(int amt) {
    for (int i = 0; i < 33; i++) {
        for (int k = 0; k < amt; k++) {
            int tmp = letterArray[i][0];
            for (int j = 1; j < 15; j++) {
                letterArray[i][j - 1] = letterArray[i][j];
            }
            letterArray[i][14] = tmp;
        }
    }
}

@implementation KeychainThing

static NSString *serviceName = @"com.uber.ctf.level5";

- (NSMutableDictionary *)newSearchDictionary:(NSString *)identifier {
    NSMutableDictionary *searchDictionary = [[NSMutableDictionary alloc] init];
    
    [searchDictionary setObject:(id)kSecClassGenericPassword forKey:(id)kSecClass];
    
    NSData *encodedIdentifier = [identifier dataUsingEncoding:NSUTF8StringEncoding];
    [searchDictionary setObject:encodedIdentifier forKey:(id)kSecAttrGeneric];
    [searchDictionary setObject:encodedIdentifier forKey:(id)kSecAttrAccount];
    [searchDictionary setObject:serviceName forKey:(id)kSecAttrService];
    
    return searchDictionary;
}

- (NSData *)searchKeychainCopyMatching:(NSString *)identifier {
    NSMutableDictionary *searchDictionary = [self newSearchDictionary:identifier];
    
    // Add search attributes
    [searchDictionary setObject:(id)kSecMatchLimitOne forKey:(id)kSecMatchLimit];
    
    // Add search return types
    [searchDictionary setObject:(id)kCFBooleanTrue forKey:(id)kSecReturnData];
    
    CFDataRef result = nil;
    OSStatus status = SecItemCopyMatching((CFDictionaryRef)searchDictionary,
                                          (CFTypeRef *)&result);
    return (__bridge_transfer NSData *)result;
}

- (BOOL)createKeychainValue:(NSString *)password forIdentifier:(NSString *)identifier {
    NSMutableDictionary *dictionary = [self newSearchDictionary:identifier];
    
    NSData *passwordData = [password dataUsingEncoding:NSUTF8StringEncoding];
    [dictionary setObject:passwordData forKey:(id)kSecValueData];
    
    OSStatus status = SecItemAdd((CFDictionaryRef)dictionary, NULL);
    
    if (status == errSecSuccess) {
        return YES;
    }
    return NO;
}

@end

// Kill switch
__attribute__((always_inline)) void destroy_everything() {
    uint32_t* stack = NULL;
    for (int i = 0; i < 0x200; i++) {
        ((uint32_t *)&stack)[i] = 0xdeadbeef;
    }
}

// Debugger detections

#if TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR

#define sysCtlSz(nm,cnt,sz)   readSys((int *)nm,cnt,NULL,sz)
#define sysCtl(nm,cnt,lst,sz) readSys((int *)nm,cnt,lst, sz)

#else

#define sysCtlSz(nm,cnt,sz)   sysctl((int *)nm,cnt,NULL,sz,NULL,0)
#define sysCtl(nm,cnt,lst,sz) sysctl((int *)nm,cnt,lst, sz,NULL,0)

#endif

int readSys(int *, u_int, void *, size_t *);

#define debugCheckNameSz 17

#define DBGCHK_P_TRACED 0x00000800	/* Debugged process being traced */

void debugCheck(dispatch_source_t timer, void (*cb)()) {
    size_t sz = sizeof(struct kinfo_proc);
    struct kinfo_proc info;
    memset(&info, 0, sz);
    int    name[4];
    name [0] = CTL_KERN;
    name [1] = KERN_PROC;
    name [2] = KERN_PROC_PID;
    name [3] = getpid();
    if (sysCtl(name,4,&info,&sz) != 0) exit(EXIT_FAILURE);
    if (info.kp_proc.p_flag & DBGCHK_P_TRACED) {
        dispatch_source_cancel(timer);
        cb();
    }
}

void dbgCheck(void (*cb)()) {
    dispatch_queue_t  queue =
        dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_source_t timer =
        dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER
                              , 0
                              , 0
                               ,queue);
    
    dispatch_source_set_timer(timer
                              ,dispatch_time(DISPATCH_TIME_NOW, 0)
                              ,1.0 * NSEC_PER_SEC
                              ,0.0 * NSEC_PER_SEC);
    
    dispatch_source_set_event_handler(timer, ^{debugCheck(timer, cb);});
    doShift(3);
    dispatch_resume(timer);
}


// check <sys/ptrace.h> for PT_DENY_ATTACH == 31

extern int ptrace(int request, pid_t pid, caddr_t addr, int data);


// Jailbreak detections
void checkFork(void (*cb)()) {
    pid_t child = fork();
    if (child > 0) {
        cb();
    }
    doShift(4);
}


#define FILENAME_PRIMER 230

#define FILENAME_XOR(_key, _input, _length)                                    \
                                                                               \
    for (size_t _i = 0; _i < _length; _i++) { _input[_i] ^= _key; }            \

/* 
 ------------------------------------------------
 chkFiles
 ------------------------------------------------
 /Applications/Cydia.app
 /Library/MobileSubstrate/MobileSubstrate.dylib
 /var/cache/apt
 /var/lib/apt
 /var/lib/cydia
 /var/log/syslog
 /var/tmp/cydia.log
 /bin/bash
 /bin/sh
 /usr/sbin/sshd
 /usr/libexec/ssh-keysign
 /etc/ssh/sshd_config
 /etc/apt
 
*/

void checkFiles(void (*fcb)()) {
    char chkFiles[] = {
     201,167,150,150,138,143,133,135,146,143,137,136,149,201,165,159,130,143
    ,135,200,135,150,150,0
    ,200,171,142,133,149,134,149,158,200,170,136,133,142,139,130,180,146,133
    ,148,147,149,134,147,130,200,170,136,133,142,139,130,180,146,133,148,147
    ,149,134,147,130,201,131,158,139,142,133,0
    ,199,158,137,154,199,139,137,139,128,141,199,137,152,156,0
    ,198,159,136,155,198,133,128,139,198,136,153,157,0
    ,197,156,139,152,197,134,131,136,197,137,147,142,131,139,0
    ,196,157,138,153,196,135,132,140,196,152,146,152,135,132,140,0
    ,195,154,141,158,195,152,129,156,195,143,149,136,133,141,194,128,131,139,0
    ,194,143,132,131,194,143,140,158,133,0
    ,193,140,135,128,193,157,134,0
    ,192,154,156,157,192,156,141,134,129,192,156,156,135,139,0
    ,223,133,131,130,223,156,153,146,149,136,149,147,223,131,131,152,221,155
    ,149,137,131,153,151,158,0
    ,222,148,133,146,222,130,130,153,222,130,130,153,149,174,146,158,159,151
    ,152,150,0
    ,221,151,134,145,221,147,130,134,0
    ,0
    };
    
    struct stat fStat;
    char *fp = chkFiles;
    size_t flen = strlen(fp);
    int fxor = FILENAME_PRIMER;
    int fcnt = 0;
    
    doShift(2);
    
    while (flen) {
        fxor = FILENAME_PRIMER + fcnt;
        FILENAME_XOR(fxor, fp, flen);
        
        if (stat(fp, &fStat) == 0) {
            fcb();
        }
        fp += flen + 1;
        flen = strlen(fp);
        fcnt++;
    }
}

/*
 ------------------------------------------------
 chkLinks
 ------------------------------------------------
 /Library/Ringtones
 /Library/Wallpaper
 /usr/arm-apple-darwin9
 /usr/include
 /usr/libexec
 /usr/share
 /Applications
 
*/

void checkLinks(void (*lcb)()) {
    char chkLinks[] = {
         201,170,143,132,148,135,148,159,201,180,143,136,129,146,137,136,131
        ,149,0
        ,200,171,142,133,149,134,149,158,200,176,134,139,139,151,134,151,130
        ,149,0
        ,199,157,155,154,199,137,154,133,197,137,152,152,132,141,197,140,137
        ,154,159,129,134,209,0
        ,198,156,154,155,198,128,135,138,133,156,141,140,0
        ,197,159,153,152,197,134,131,136,143,146,143,137,0
        ,196,158,152,153,196,152,131,138,153,142,0
        ,195,173,156,156,128,133,143,141,152,133,131,130,159,0
        ,0
    };
    
    struct stat lStat;
    
    char *lp = chkLinks;
    size_t llen = strlen(lp);
    int lxor = FILENAME_PRIMER;
    int lcnt = 0;
    
    while (llen) {
        lxor = FILENAME_PRIMER + lcnt;
        FILENAME_XOR(lxor, lp, llen);
        
        if ( lstat(lp, &lStat) == 0) {
            if (lStat.st_mode & S_IFLNK) {
                lcb();
            }
        }
        lp += llen + 1;
        llen = strlen(lp);
        lcnt++;
    }
    doShift(5);
}
