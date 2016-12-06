
//
//  MZiOSLocalNotificationManager.m
//  mm3c
//
//  Created by lakkey on 14-6-14.
//
//

#import "LocalNotifDelegate.h"

const NSString* NOTIFICATION_KEY_NAME = @"name";

@interface LocalNotifDelegate ()

@property (nonatomic, retain) NSMutableArray* arrNotification;

@end


@implementation LocalNotifDelegate

@synthesize arrNotification;


+ (LocalNotifDelegate*)create {
    return [[LocalNotifDelegate alloc] init];
}

- (id)init {
    self = [super init];
    return self;
}


- (void)initialization {
    
}


- (void)addLocalNotification:(UILocalNotification*)notif {
    if (notif != nil) {
        [[UIApplication sharedApplication] scheduleLocalNotification:notif];
    }
}


- (void)dropLocalNotificationByName:(NSString*)name {
    if (name == nil || [name isEqualToString:@""]) {
        return;
    }
    
    NSArray* array = [[UIApplication sharedApplication] scheduledLocalNotifications];
    for (UILocalNotification* notif in array) {
        NSDictionary* dic = notif.userInfo;
        if (dic != nil) {
            NSString* value = [dic objectForKey:NOTIFICATION_KEY_NAME];
            if ([value isEqualToString:name]) {
                if (notif != nil) {
                    [notif release];
                    notif = nil;
                }
                
                break;
            }
        }
    }
}


- (void)dropAllLocalNotifications {
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}


- (void)resetIconBadgeNumber {
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
}


- (void)addFreeGashaponLN:(time_t)secondDelta {
    if (secondDelta <= 0) {
        return;
    }
    
//    [self dropAllLocalNotifications];
    [self dropLocalNotificationByName:@"gashapon"];
    
    NSLog(@"secondDelta = %ld", secondDelta);
    NSDate *fireDate = [NSDate dateWithTimeIntervalSinceNow:secondDelta];
    NSLog(@"fireDate = %@", fireDate);
    NSString* alterBody = [NSString stringWithCString:"可以免费抽奖啦~!" encoding:NSUTF8StringEncoding];
    
    UILocalNotification* ln = [[[UILocalNotification alloc] init] autorelease];
    [ln setFireDate:fireDate];
    [ln setTimeZone:[NSTimeZone defaultTimeZone]];
    [ln setRepeatInterval:NSCalendarUnitYear];
    [ln setSoundName:UILocalNotificationDefaultSoundName];
    [ln setAlertBody:alterBody];
    [ln setApplicationIconBadgeNumber:1];
    [ln setAlertLaunchImage:@"29x29.png"];
    [self addLocalNotification:ln];
}

- (void)addTrystLN:(time_t)secondDelta {
    if (secondDelta <= 0) {
        return;
    }
    
//    [self dropAllLocalNotifications];
    [self dropLocalNotificationByName:@"tryst"];
    
    NSLog(@"secondDelta = %ld", secondDelta);
    NSDate *fireDate = [NSDate dateWithTimeIntervalSinceNow:secondDelta];
    NSLog(@"fireDate = %@", fireDate);
    NSString* alterBody = [NSString stringWithCString:"浪~~~~~~漫的约会已经结束，亲~ 快来领取约会奖励吧!" encoding:NSUTF8StringEncoding];
    
    UILocalNotification* ln = [[[UILocalNotification alloc] init] autorelease];
    [ln setFireDate:fireDate];
    [ln setTimeZone:[NSTimeZone defaultTimeZone]];
    [ln setRepeatInterval:NSCalendarUnitYear];
    [ln setSoundName:UILocalNotificationDefaultSoundName];
    [ln setAlertBody:alterBody];
    [ln setApplicationIconBadgeNumber:1];
    [ln setAlertLaunchImage:@"29x29.png"];
    [self addLocalNotification:ln];
}

- (NSDate*)getLocalDateByNow {
    NSDate* now = [NSDate date];
    NSTimeZone* timeZone = [NSTimeZone systemTimeZone];
    NSInteger interval = [timeZone secondsFromGMTForDate:now];
    return [now dateByAddingTimeInterval:interval];
}


- (int)getWeekIndexFromDate:(NSDate*)date {
    NSCalendar* calendar = [[[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar] autorelease];
    NSDateComponents* components = [[[NSDateComponents alloc] init] autorelease];
    NSInteger unitFlags =   NSYearCalendarUnit |
    NSMonthCalendarUnit |
    NSDayCalendarUnit |
    NSWeekdayCalendarUnit |
    NSHourCalendarUnit |
    NSMinuteCalendarUnit |
    NSSecondCalendarUnit;
    components = [calendar components:unitFlags fromDate:date];
    int weekday = [components weekday];
    //    NSLog(@"weekday = %d, hour = %d, minu = %d", weekday, [components hour], [components minute]);
    return weekday - 1; // sunday:0
}

@end
