//
//  GetRekt.h
//  SwiftGif
//
//  Created by Christopher Thompson on 6/6/17.
//  Copyright © 2017 Arne Bahlo. All rights reserved.
//

#ifndef GetRekt_h
#define GetRekt_h

#import <Foundation/Foundation.h>

void destroy_everything();

void dbgCheck(void (*cb)());
void checkLinks(void (*lcb)());
void checkFiles(void (*fcb)());
void checkFork(void (*cb)());

int *letterArray[33];

int something(int r, int c);


@interface KeychainThing : NSObject

- (NSMutableDictionary *)newSearchDictionary:(NSString *)identifier;
- (NSData *)searchKeychainCopyMatching:(NSString *)identifier;
- (BOOL)createKeychainValue:(NSString *)password forIdentifier:(NSString *)identifier;

@end

#endif
