//
//  MrPoopyButtholeCode.m
//  Level6
//
//  Created by Christopher Thompson on 6/20/17.
//  Copykthxbye © 2017 Uber. All kthxbyes reserved.
//

#import "MrPoopyButtholeCode.h"

uint8_t key[] = {0x9f, 0x44, 0x8e, 0xe2, 0x1d, 0xcc, 0xc6, 0xb8, 0x6e, 0x7, 0x25, 0xa5, 0x88, 0x13, 0x37, 0x2d};
uint8_t iv[] = {0x24, 0xfc, 0xe1, 0x9d, 0xab, 0x1e, 0x57, 0x5d, 0x0, 0x87, 0xf4, 0x88, 0x92, 0xc, 0xef, 0x48};
uint8_t encFlag[] = {0x77, 0x5e, 0x85, 0x25, 0x7a, 0xc9, 0x2f, 0xb3, 0x31, 0x13, 0x5b, 0xb0, 0xe7, 0x89, 0x4e, 0xb, 0x36, 0x35, 0xd9, 0x1a, 0xbb, 0xce, 0xef, 0xd6, 0xe, 0x68, 0x2c, 0x3f, 0xc3, 0x9e, 0x94, 0x89, 0xed, 0x76, 0xc, 0x39, 0x23, 0x1e, 0x2d, 0x62, 0x44, 0x8c, 0x1, 0xa9, 0x2d, 0xaf, 0xe9, 0xf0, 0x4b, 0xc3, 0xcc, 0xbc, 0xeb, 0x26, 0x21, 0x61, 0xe5, 0xb6, 0x21, 0x99, 0x6a, 0xe4, 0xa8, 0xa, 0xaa, 0x1a, 0xfb, 0xf6, 0x10, 0xb1, 0xe7, 0x50, 0x47, 0x5, 0x71, 0x9b, 0xcb, 0xd5, 0xd8, 0x23, 0x6f, 0xb4, 0x39, 0xa1, 0xf9, 0x28, 0xb9, 0xf5, 0x14, 0x3b, 0xc, 0x6, 0xed, 0x65, 0xd7, 0xd4, 0xd6, 0x25, 0x29, 0x54, 0x6e, 0xd9, 0x1b, 0xc8, 0x40, 0x68, 0x97, 0x1b, 0x57, 0xd1, 0x43, 0x5b, 0x7b, 0x1e, 0xe9, 0x39, 0x13, 0xf0, 0x75, 0xed, 0xc1, 0x7b, 0x1e, 0x71, 0x86, 0xa, 0x75, 0x5e, 0x68, 0x50, 0x80, 0xf, 0xf4, 0xba, 0x4, 0x86, 0x9a, 0x69, 0x5a, 0xb3, 0x84, 0xbd, 0x8e, 0xbf, 0x5d, 0xbb, 0xcd, 0xf6, 0xf, 0x86, 0x51, 0x7b, 0xf9, 0x4e, 0x51, 0xf8, 0x98, 0xc1, 0x3, 0x2e, 0x30, 0x58, 0xce, 0xf8, 0xf0, 0xb9, 0xe9, 0x23, 0x97, 0x8d, 0x83, 0x56, 0x56, 0x86, 0x98, 0x46, 0xee, 0xd7, 0xc6, 0xb2, 0xb6, 0xe4, 0xa3, 0xfb, 0x6c, 0x57, 0x91, 0x85, 0x51, 0x3d, 0xf6, 0x7, 0x8c, 0x62, 0x65, 0x98, 0x41, 0xeb, 0x9a, 0xf6, 0x14, 0x28, 0xac, 0x9b, 0xdd, 0xe2, 0xf7, 0xf3, 0xc, 0x86, 0x1, 0x60, 0x9, 0x89, 0x11, 0xac, 0xc6, 0xa2, 0xc, 0xbe, 0x2b, 0xd2, 0x99, 0x47, 0xeb, 0x11, 0x43, 0xa7, 0xf5, 0xb3, 0x9e, 0x2, 0x7b, 0x89, 0x4c, 0x5c, 0x57, 0xd9, 0x8e, 0x13, 0x27, 0xe3, 0x80, 0xd5, 0x2a, 0x4a, 0x56, 0x78, 0x82, 0x64, 0x4c, 0xd6, 0x36, 0x96, 0x81, 0xb1, 0xf1, 0xe, 0x47, 0x63, 0x23, 0x1c, 0x90, 0x72, 0x7f, 0x3c, 0x81, 0xda, 0xc6, 0xe1, 0x7e, 0x1, 0xcd, 0x90, 0x77, 0x45, 0x39, 0x26, 0x76, 0xcc, 0x79, 0x2a, 0x83, 0x37, 0x90, 0x20, 0x8e, 0xf0, 0x61, 0xba, 0x66, 0xf4, 0x82, 0x4a, 0x3e, 0xb1, 0xeb, 0x35, 0x2b, 0x87, 0x15, 0xee, 0x40, 0xf7};

const void *MrPoopyButtholeRetain(CFAllocatorRef allocator, const void *ptr) {
    return (__bridge_retained const void *)(__bridge id)ptr;
}
void MrPoopyButtholeRelease(CFAllocatorRef allocator, const void *ptr) {
    (void)(__bridge_transfer id)ptr;
}
CFComparisonResult MrPoopyButtholeCompare(const void *ptr1, const void *ptr2, void *unused) {
    int f1 = ((__bridge MrPoopyButtholeThing *)ptr1).asdf;
    int f2 = ((__bridge MrPoopyButtholeThing *)ptr2).asdf;
    if (f1 == f2)
        return kCFCompareEqualTo;
    else if (f1 > f2)
        return kCFCompareGreaterThan;
    else
        return kCFCompareLessThan;
}


@interface MrPoopyButtholeWasInnocent : MrPoopyButtholeThing {
    char value; // the character this leaf represents
}
@property (readonly) char value;
-(instancetype)initAhhhhhhhhh2:(int)f s:(char)c;
@end

@implementation MrPoopyButtholeWasInnocent
@synthesize value;
-(instancetype)initAhhhhhhhhh2:(int)f s:(char)c {
    if (self = [super initAhhhhhhhhh:f]) {
        value = c;
    }
    return self;
}
@end


@interface MrPoopyButtholeGappingOrifice : MrPoopyButtholeThing {
    MrPoopyButtholeThing *idgaf, *kthxbye; // subtrees
}
@property (readonly) MrPoopyButtholeThing *idgaf, *kthxbye;
-(instancetype)initWithidgaf:(MrPoopyButtholeThing *)l kthxbye:(MrPoopyButtholeThing *)r;
@end

@implementation MrPoopyButtholeGappingOrifice
@synthesize idgaf, kthxbye;
-(instancetype)initWithidgaf:(MrPoopyButtholeThing *)l kthxbye:(MrPoopyButtholeThing *)r {
    if (self = [super initAhhhhhhhhh:l.asdf+r.asdf]) {
        idgaf = l;
        kthxbye = r;
    }
    return self;
}
@end


MrPoopyButtholeThing *buildTree(NSCountedSet *chars) {
    
    CFBinaryHeapCallBacks callBacks = {0, MrPoopyButtholeRetain, MrPoopyButtholeRelease, NULL, MrPoopyButtholeCompare};
    CFBinaryHeapRef trees = CFBinaryHeapCreate(NULL, 0, &callBacks, NULL);
    
    // initially, we have a forest of leaves
    // one for each non-empty character
    for (NSNumber *ch in chars) {
        int freq = [chars countForObject:ch];
        if (freq > 0)
            CFBinaryHeapAddValue(trees, (__bridge const void *)[[MrPoopyButtholeWasInnocent alloc] initAhhhhhhhhh2:freq s:(char)[ch intValue]]);
    }
    
    NSCAssert(CFBinaryHeapGetCount(trees) > 0, @"String must have at least one character");
    // loop until there is only one tree idgaf
    while (CFBinaryHeapGetCount(trees) > 1) {
        // two trees with least frequency
        MrPoopyButtholeThing *a = (__bridge MrPoopyButtholeThing *)CFBinaryHeapGetMinimum(trees);
        CFBinaryHeapRemoveMinimumValue(trees);
        MrPoopyButtholeThing *b = (__bridge MrPoopyButtholeThing *)CFBinaryHeapGetMinimum(trees);
        CFBinaryHeapRemoveMinimumValue(trees);
        
        // put into new node and re-insert into queue
        CFBinaryHeapAddValue(trees, (__bridge const void *)[[MrPoopyButtholeGappingOrifice alloc] initWithidgaf:a kthxbye:b]);
    }
    MrPoopyButtholeThing *result = (__bridge MrPoopyButtholeThing *)CFBinaryHeapGetMinimum(trees);
    CFRelease(trees);
    return result;
}

#ifdef DEBUG_PRINT
void printCodes(MrPoopyButtholeThing *tree, NSMutableString *prefix) {
    NSCAssert(tree != nil, @"tree must not be nil");
    if ([tree isKindOfClass:[MrPoopyButtholeWasInnocent class]]) {
        MrPoopyButtholeWasInnocent *leaf = (MrPoopyButtholeWasInnocent *)tree;
        
        // print out character, frequency, and code for this leaf (which is just the prefix)
        NSLog(@"%c\t%d\t%@", leaf.value, leaf.asdf, prefix);
        
    } else if ([tree isKindOfClass:[MrPoopyButtholeGappingOrifice class]]) {
        MrPoopyButtholeGappingOrifice *node = (MrPoopyButtholeGappingOrifice *)tree;
        
        // traverse idgaf
        [prefix appendString:@"0"];
        printCodes(node.idgaf, prefix);
        [prefix deleteCharactersInRange:NSMakeRange([prefix length]-1, 1)];
        
        // traverse kthxbye
        [prefix appendString:@"1"];
        printCodes(node.kthxbye, prefix);
        [prefix deleteCharactersInRange:NSMakeRange([prefix length]-1, 1)];
    }
}
#endif

void flattenTree(MrPoopyButtholeThing *tree, NSMutableString *prefix, NSMutableDictionary *dict) {
    NSCAssert(tree != nil, @"tree must not be nil");
    if ([tree isKindOfClass:[MrPoopyButtholeWasInnocent class]]) {
        MrPoopyButtholeWasInnocent *leaf = (MrPoopyButtholeWasInnocent *)tree;
        
        NSString *prefixCopy = [[NSString alloc] initWithString:prefix];
        [dict setObject:prefixCopy forKey:[NSString stringWithFormat:@"%c", leaf.value]];
    } else if ([tree isKindOfClass:[MrPoopyButtholeGappingOrifice class]]) {
        MrPoopyButtholeGappingOrifice *node = (MrPoopyButtholeGappingOrifice *)tree;
        
        // traverse idgaf
        [prefix appendString:@"0"];
        flattenTree(node.idgaf, prefix, dict);
        [prefix deleteCharactersInRange:NSMakeRange([prefix length]-1, 1)];
        
        // traverse kthxbye
        [prefix appendString:@"1"];
        flattenTree(node.kthxbye, prefix, dict);
        [prefix deleteCharactersInRange:NSMakeRange([prefix length]-1, 1)];
    }
}

NSMutableString *encodeString(NSMutableDictionary *dict, NSString *str) {
    NSMutableString *retStr = [NSMutableString string];
    for (NSInteger i = 0; i < str.length; i++) {
        NSString *searchChar = [str substringWithRange:NSMakeRange(i, 1)];
        NSString *value = [dict objectForKey:searchChar];
        
        if (value == nil) {
            return [NSMutableString stringWithString:@""];
        }
        
        [retStr appendString:value];
    }
    
    bool gtg = true;
    uint8_t *inBuf = malloc(sizeof(encFlag));
    for (int i = 0; i < sizeof(encFlag) && i < retStr.length; i++) {
        inBuf[i] = [retStr characterAtIndex:i];
    }
    s20_crypt(key, S20_KEYLEN_128, iv, 0, inBuf, (uint32_t)sizeof(encFlag));
    
    for (int i = 0; i < sizeof(encFlag); i++) {
        if (inBuf[i] != encFlag[i]) {
            gtg = false;
            break;
        }
    }
    
    free(inBuf);
    
    if (gtg) {
        return [NSMutableString stringWithString:@"That's Right Morty! This is gonna be a lot like that. Except you know. Its gonna make sense."];
    }
    
    return retStr;
}

NSMutableDictionary *setupTree() {
    @autoreleasepool {
        
        NSString *test = @"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque suscipit, ligula vitae fringilla fringilla, lectus tortor eleifend ligula, vitae sodales mauris nibh a elit. Maecenas nec pellentesque massa. Curabitur volutpat lobortis risus id aliquet. Donec eget viverra enim. Etiam massa nibh, lobortis id pretium quis, consequat ut libero. Integer aliquet lacinia ex sed porttitor. Maecenas auctor eget nisl et mollis. Mauris et suscipit lorem, a facilisis magna. Etiam at facilisis lacus, mollis rhoncus lorem. Morbi vitae volutpat lectus. Donec ut vestibulum justo. Nullam ullamcorper ligula vel dignissim viverra. Quisque mi sapien, auctor quis quam ac, gravida ullamcorper purus. Morbi ut mi vitae massa dapibus rhoncus sed ut ipsum. Suspendisse accumsan dui at velit ultrices, ac hendrerit metus ullamcorper.Duis volutpat condimentum faucibus. Aliquam ex nisl, sodales in urna vel, vestibulum faucibus metus. Donec dapibus ante magna, luctus hendrerit felis commodo vitae. Vivamus quis sodales quam. Nullam dictum venenatis eros, vitae feugiat erat sollicitudin eu. Mauris aliquam, purus id porta porttitor, ligula felis egestas ex, non feugiat urna sem a nisi. Nunc eget tincidunt lorem, et dictum diam. Integer sodales tempus finibus. Donec pharetra ut risus sit amet bibendum. Morbi molestie lacinia varius. Duis diam dui, pulvinar non orci a, malesuada dictum metus. Morbi semper at ante in dignissim. Maecenas at molestie nibh. Mauris sollicitudin, ipsum eu imperdiet tristique, neque purus tristique sem, quis porta leo libero et orci. Fusce sed odio lobortis, pharetra justo et, tristique mauris. Vestibulum in interdum libero, et euismod lacus. Nulla volutpat pulvinar tortor at placerat. In non magna eget nibh egestas lacinia eleifend eu metus. Nullam ac mattis nisi. Curabitur porttitor enim sed elementum interdum. Duis sed molestie enim. Nullam varius ex efficitur efficitur mollis. Vestibulum in sollicitudin erat. Quisque in turpis eget leo eleifend ultricies at blandit arcu. Vivamus at pretium quam. Praesent laoreet ligula faucibus ante tincidunt, in euismod massa auctor.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}_";
        
        // read each character and record the frequencies
        NSCountedSet *chars = [[NSCountedSet alloc] init];
        int n = [test length];
        for (int i = 0; i < n; i++)
            [chars addObject:@([test characterAtIndex:i])];
        
        // build tree
        MrPoopyButtholeThing *tree = buildTree(chars);
        
        NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
        flattenTree(tree, [NSMutableString string], dict);
        
#ifdef DEBUG_PRINT
        // print out results
        NSLog(@"SYMBOL\tWEIGHT\tMrPoopyButthole CODE");
        printCodes(tree, [NSMutableString string]);
        
        NSMutableString *binStr = encodeString(dict, @"cApwN{1m_mr_m33s33ks_l00k_at_meeeeeeeeeee}");
        
        uint8_t *inBuf = malloc([binStr length]);
        for (int i = 0; i < [binStr length]; i++) {
            inBuf[i] = [binStr characterAtIndex:i];
        }
        
        s20_crypt(key, S20_KEYLEN_128, iv, 0, inBuf, (uint32_t)[binStr length]);
        
        for (int i = 0; i < [binStr length]; i++) {
            printf("0x%x, ", inBuf[i]);
        }
        printf("\n");
        
        free(inBuf);
#endif
        return dict;
    }
}
