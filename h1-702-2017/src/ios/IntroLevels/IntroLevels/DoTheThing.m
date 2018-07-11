//
//  DoTheThing.m
//  IntroLevels
//
//  Created by Christopher Thompson on 6/12/17.
//  Copyright © 2017 Uber. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonCryptor.h>
#import <CommonCrypto/CommonDigest.h>

#import "IntroLevels-Bridging-Header.h"

NSString *specialSauce = @"bler";

@implementation ZhuLi

@dynamic specialSauce;

+ (NSString *)specialSauce {
    return specialSauce;
}

+ (NSString *)montyCarlo:(NSData *) data {
    /* Returns hexadecimal string of NSData. Empty string if data is empty.   */
    
    const unsigned char *dataBuffer = (const unsigned char *)[data bytes];
    
    if (!dataBuffer)
    return [NSString string];
    
    NSUInteger          dataLength  = [data length];
    NSMutableString     *hexString  = [NSMutableString stringWithCapacity:(dataLength * 2)];
    
    for (int i = 0; i < dataLength; ++i)
    [hexString appendString:[NSString stringWithFormat:@"%02lx", (unsigned long)dataBuffer[i]]];
    
    return [NSString stringWithString:hexString];
}

+(NSString *) doTheThing:(NSString *)level1 flag2:(NSString *)level2 flag3:(NSString *)level3 {
    uint8_t digest[CC_SHA1_DIGEST_LENGTH];
    
    NSData *data = [level1 dataUsingEncoding:NSUTF8StringEncoding];
    
    uint8_t enc[32] = {'\xdd', '\x2a', '\x7a', '\xec', '\xee', '\x8b', '\x7d', '\xec', '\x0e', '\x72', '\x33', '\xc7', '\x1b', '\xe3', '\xf7', '\x50', '\xfc', '\x4b', '\x7a', '\x85', '\x2c', '\xa0', '\xe1', '\x19', '\x7f', '\x54', '\x60', '\xd3', '\x16', '\x6d', '\x62', '\xfd'};
    
    CC_SHA1(data.bytes, data.length, digest);
    
    NSMutableString *level1Hash = [NSMutableString stringWithCapacity:CC_SHA1_DIGEST_LENGTH * 2];
    
    for (int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++)
    {
        [level1Hash appendFormat:@"%02x", digest[i]];
    }
    
    data = [level2 dataUsingEncoding:NSUTF8StringEncoding];
    
    CC_SHA1(data.bytes, data.length, digest);
    
    NSMutableString *level2Hash = [NSMutableString stringWithCapacity:CC_SHA1_DIGEST_LENGTH * 2];
    
    for (int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++)
    {
        [level2Hash appendFormat:@"%02x", digest[i]];
    }
    
    data = [level3 dataUsingEncoding:NSUTF8StringEncoding];
    
    CC_SHA1(data.bytes, data.length, digest);
    
    NSMutableString *level3Hash = [NSMutableString stringWithCapacity:CC_SHA1_DIGEST_LENGTH * 2];
    
    for (int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++)
    {
        [level3Hash appendFormat:@"%02x", digest[i]];
    }
    
    NSString* parts = [NSString stringWithFormat:@"%@%@%@", [level3Hash substringToIndex:5], [level1Hash substringToIndex:4], [level2Hash substringToIndex:5]];
    
    NSString* parts2 = [NSString stringWithFormat:@"%@%@%@", [level1Hash substringFromIndex:36], [level3Hash substringFromIndex:35], [level2Hash substringFromIndex:35]];
    
    NSString* key = [NSString stringWithFormat:@"%@%@%@", parts, [ZhuLi specialSauce], parts2];
    
    char keyPtr[kCCKeySizeAES256 + 1]; // room for terminator (unused)
    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];
    
    size_t bufferSize = 32 + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    
    size_t numBytesDecrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCDecrypt, kCCAlgorithmAES128, kCCOptionPKCS7Padding,
                                          keyPtr, kCCKeySizeAES256,
                                          NULL /* initialization vector (optional) */,
                                          enc, 32, /* input */
                                          buffer, bufferSize, /* output */
                                          &numBytesDecrypted);
    
    if (cryptStatus == kCCSuccess) {
        NSData* decrypted = [NSData dataWithBytesNoCopy:buffer length:numBytesDecrypted];
        NSString* hexData = [ZhuLi montyCarlo:decrypted];
        return hexData;
    }
    
    free(buffer);
    return nil;
}
@end
