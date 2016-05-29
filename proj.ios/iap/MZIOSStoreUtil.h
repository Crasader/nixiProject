//
//  MZIOSStoreUtil.h
//  mm3c
//
//  Created by lakkey on 14-7-18.
//
//

#ifndef __mm3c__MZIOSStoreUtil__
#define __mm3c__MZIOSStoreUtil__

#include <iostream>
//#import<StoreKit/SKPaymentQueue.h>
#include "store/CCStorePaymentTransaction.h"

USING_NS_CC_EXTRA;

@interface MZIOSStoreUtil : NSObject

+ (const char*)getReceiptByProductId:(CCStorePaymentTransaction *)cctransaction;
+ (NSString *)encode:(const uint8_t *)input length:(NSInteger)length;

@end

#endif /* defined(__mm3c__MZIOSStoreUtil__) */
