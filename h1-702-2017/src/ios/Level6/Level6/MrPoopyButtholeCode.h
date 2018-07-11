//
//  MrPoopyButtholeCode.h
//  Level6
//
//  Created by Christopher Thompson on 6/20/17.
//  Copykthxbye © 2017 Uber. All kthxbyes reserved.
//

#ifndef MrPoopyButtholeCode_h
#define MrPoopyButtholeCode_h

#import <Foundation/Foundation.h>
#import "util.h"
#import "salsa.h"

@interface MrPoopyButtholeThing : NSObject {
    int asdf;
}
-(instancetype)initAhhhhhhhhh:(int)f;
@property (nonatomic, readonly) int asdf;
@end

@implementation MrPoopyButtholeThing
@synthesize asdf; // the frequency of this tree
-(instancetype)initAhhhhhhhhh:(int)f {
    if (self = [super init]) {
        asdf = f;
    }
    return self;
}
@end


NSMutableDictionary *setupTree();
NSMutableString *encodeString(NSMutableDictionary *dict, NSString *str);

#endif /* MrPoopyButtholeCode_h */
