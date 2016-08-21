//
//  LocalNotifDelegate.cpp
//  mm3c
//
//  Created by lakkey on 14-8-27.
//
//

#include "LocalNotifDelegate.h"
//#include "MMLocalNotif.h"

#pragma mark - Export

void LocalNotifDelegate::addFreeGashaponLN() {
//    NSDateFormatter* dFormatter = [[[NSDateFormatter alloc] init] autorelease];
//    [dFormatter setDateFormat:@"HH:mm:ss"];
//    NSString* strTime = [NSString stringWithFormat:@"%d:%d:00", item->getHour(), item->getMinute()];
//    NSDate* fireDate = [dFormatter dateFromString:strTime];
//    NSString* strSound = [NSString stringWithFormat:@"%s%s", "", item->getSound()->getCString()];
//    //    NSString* alterBody = [NSString stringWithCString:item->getAlertBody()->getCString() encoding:NSUTF8StringEncoding];
//    NSString* alterBody = [NSString stringWithCString:[self getAlertBody:item] encoding:NSUTF8StringEncoding];
//    NSString* alterAction = [NSString stringWithCString:item->getTitle()->getCString() encoding:NSUTF8StringEncoding];
//    UILocalNotification* ln = [[[UILocalNotification alloc] init] autorelease];
//    [ln setFireDate:fireDate];
//    [ln setTimeZone:[NSTimeZone defaultTimeZone]];
//    [ln setRepeatInterval:NSDayCalendarUnit];
//    [ln setSoundName:strSound];
//    [ln setAlertBody:alterBody];
//    [ln setAlertAction:alterAction];
//    [ln setApplicationIconBadgeNumber:1];
//    [ln setAlertLaunchImage:@"29x29.png"];
//    [[UIApplication sharedApplication] scheduleLocalNotification:ln];
}

void LocalNotifDelegate::dropLocalNotification(UILocalNotification* notif) {
    NSDictionary* dic = notif.userInfo;
    NSString* value = [dic objectForKey:NOTIFICATION_KEY_NAME];
    const char* csValue = [value UTF8String];
    this->dropLocalNotificationByName(csValue);
}

void LocalNotifDelegate::dropLocalNotificationByName(string name) {
    if (name.empty() || name.length() == 0) {
        return;
    }
    
    NSArray* array = [[UIApplication sharedApplication] scheduledLocalNotifications];
    for (UILocalNotification* notif in array) {
        NSDictionary* dic = notif.userInfo;
        if (dic != nil) {
            NSString* value = [dic objectForKey:NOTIFICATION_KEY_NAME];
            const char* csValue = [value UTF8String];
            if (name.compare(csValue) == 0) {
                if (notif != nil) {
                    [notif release];
                    notif = nil;
                }
                break;
            }
        }
    }
}

void LocalNotifDelegate::dropAllLocalNotifications() {
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}

void LocalNotifDelegate::resetIconBadgeNumber() {
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
}

#pragma mark -

LocalNotifDelegate::~LocalNotifDelegate()
{
    CCLOG("LocalNotifDelegate::~LocalNotifDelegate()");
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}


bool LocalNotifDelegate::init()
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&LocalNotifDelegate::notificationUpdateLocalNotification), "NOTIFICATION_UPDATE_LOCAL_NOTIF", NULL);
    
    return true;
}


void LocalNotifDelegate::notificationUpdateLocalNotification(cocos2d::CCObject *pObj)
{
    CCLOG("LocalNotifDelegate::notificationUpdateLocalNotification()");
//    MMLocalNotif* localNotif = (MMLocalNotif*)pObj;
//    [_lnm updateWithNotif:localNotif];
}

#pragma makr - Inner

NSDate* LocalNotifDelegate::getLocalDateByNow() {
    NSDate* now = [NSDate date];
    NSTimeZone* timeZone = [NSTimeZone systemTimeZone];
    NSInteger interval = [timeZone secondsFromGMTForDate:now];
    return [now dateByAddingTimeInterval:interval];
}

int LocalNotifDelegate::getWeekIndexFromDate(NSDate* date) {
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

