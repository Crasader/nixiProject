//
//  LocalNotifDelegate.h
//  mm3c
//
//  Created by lakkey on 14-8-27.
//
//

#ifndef __mm3c__LocalNotifDelegate__
#define __mm3c__LocalNotifDelegate__

#import <Foundation/Foundation.h>

@interface LocalNotifDelegate : NSObject

+ (LocalNotifDelegate*)create;

- (void)initialization;

- (void)addFreeGashaponLN:(time_t)secondDelta;

- (void)resetIconBadgeNumber;

- (void)dropAllLocalNotifications;

- (int)getWeekIndexFromDate:(NSDate* )date;

@end

#endif /* defined(__mm3c__LocalNotifDelegate__) */
