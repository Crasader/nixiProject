//
//  MZIOSStoreUtil.cpp
//  mm3c
//
//  Created by lakkey on 14-7-18.
//
//

#import "MZIOSStoreUtil.h"
#import "platform/ios/CCStoreIOS.h"
#import <StoreKit/SKPaymentTransaction.h>


@implementation MZIOSStoreUtil


+ (const char*)getReceiptByProductId:(CCStorePaymentTransaction *)cctransaction
{
    SKPaymentTransaction* transaction = (SKPaymentTransaction* )cctransaction->getTransactionWrapper()->getTransactionObj();
    NSString* jsonObjectString = [self encode:(uint8_t *)transaction.transactionReceipt.bytes length:transaction.transactionReceipt.length];
//    NSLog(@"jsonObjectString:%@", jsonObjectString);
    return [jsonObjectString UTF8String];
}


+ (NSString *)encode:(const uint8_t *)input length:(NSInteger)length
{
    static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    
    NSMutableData *data = [NSMutableData dataWithLength:((length + 2) / 3) * 4];
    uint8_t *output = (uint8_t *)data.mutableBytes;
    
    for (NSInteger i = 0; i < length; i += 3) {
        NSInteger value = 0;
        for (NSInteger j = i; j < (i + 3); j++) {
            value <<= 8;
            
            if (j < length) {
                value |= (0xFF & input[j]);
            }
        }
        
        NSInteger index = (i / 3) * 4;
        output[index + 0] =                    table[(value >> 18) & 0x3F];
        output[index + 1] =                    table[(value >> 12) & 0x3F];
        output[index + 2] = (i + 1) < length ? table[(value >> 6)  & 0x3F] : '=';
        output[index + 3] = (i + 2) < length ? table[(value >> 0)  & 0x3F] : '=';
    }
    
    return [[[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding] autorelease];
}


@end
