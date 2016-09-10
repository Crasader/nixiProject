//
//  LocalNotifDelegate.cpp
//  mm3c
//
//  Created by lakkey on 14-8-27.
//
//

#include "LocalNotifDelegate.h"
//#include "MMLocalNotif.h"

static LocalNotifDelegate* _instance = nullptr;

const NSString* NOTIFICATION_KEY_NAME = @"name";

#pragma mark - Export

void LocalNotifDelegate::addFreeGashaponLN(time_t secondDelta) {
    if (secondDelta <= 0) {
        return;
    }
    
    this->dropAllLocalNotifications();
//    // 获得系统时间
//    NSDate* senddate = [NSDate date];
//    // 获得系统日期
//    NSCalendar* cal = [NSCalendar currentCalendar];
//    NSUInteger unitFlags = NSDayCalendarUnit|NSMonthCalendarUnit|NSYearCalendarUnit;
//    NSDateComponents* conponent = [cal components:unitFlags fromDate:senddate];
//    NSInteger year = [conponent year];
//    NSInteger month = [conponent month];
//    NSInteger day = [conponent day];
    //
//    time_t curTS = [[NSDate date] timeIntervalSince1970];
//    NSDate* toDate = [NSDate dateWithTimeIntervalSince1970:(curTS + secondDelta)];
//    NSDateFormatter* dFormatter = [[[NSDateFormatter alloc] init] autorelease];
//    [dFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
//    NSTimeZone* timeZone = [NSTimeZone timeZoneWithName:@"Asia/Shanghai"];
//    [dFormatter setTimeZone:timeZone];
//    NSString* strToDate = [dFormatter stringFromDate:toDate];
    NSLog(@"secondDelta = %ld", secondDelta);
    NSDate *fireDate = [NSDate dateWithTimeIntervalSinceNow:secondDelta];
//    NSDate* fireDate = [dFormatter dateFromString:strToDate];
    NSLog(@"fireDate = %@", fireDate);
    NSString* alterBody = [NSString stringWithCString:"可以免费抽奖啦~!" encoding:NSUTF8StringEncoding];
//    NSString* alterAction = [NSString stringWithCString:"我是标题党" encoding:NSUTF8StringEncoding];

    UILocalNotification* ln = [[[UILocalNotification alloc] init] autorelease];
    [ln setFireDate:fireDate];
    [ln setTimeZone:[NSTimeZone defaultTimeZone]];
    [ln setRepeatInterval:NSCalendarUnitYear];
    [ln setSoundName:UILocalNotificationDefaultSoundName];
    [ln setAlertBody:alterBody];
//    [ln setAlertAction:alterAction];
    [ln setApplicationIconBadgeNumber:1];
    [ln setAlertLaunchImage:@"29x29.png"];
    
    
//    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:@"gashapon", NOTIFICATION_KEY_NAME,nil]; 有问题，重复发送时报错
//    [ln setUserInfo:dict];
    [[UIApplication sharedApplication] scheduleLocalNotification:ln];
}

//void LocalNotifDelegate::dropLocalNotification(UILocalNotification* notif) {
//    NSDictionary* dic = notif.userInfo;
//    NSString* value = [dic objectForKey:NOTIFICATION_KEY_NAME];
//    const char* csValue = [value UTF8String];
//    this->dropLocalNotificationByName(csValue);
//}

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

LocalNotifDelegate* LocalNotifDelegate::Inst() {
    if (_instance == nullptr) {
        _instance = new LocalNotifDelegate();
        _instance->init();
    }
    
    return _instance;
}

void LocalNotifDelegate::init() {
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&LocalNotifDelegate::nc_gashapon), "FREE_GASHAPON", NULL);
}

#pragma makr - Inner

void LocalNotifDelegate::nc_gashapon(CCObject *pObj) {
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&LocalNotifDelegate::_nc_gashapon), this, 1, 0, 0.1, false);
    this->setTemp((CCDictionary*)pObj);
}

void LocalNotifDelegate::_nc_gashapon() {
    CCLOG("LocalNotifDelegate::_nc_gashapon()");
    const CCString* name = (CCString*)this->getTemp()->valueForKey("name");
    CCInteger* nmu = (CCInteger*)this->getTemp()->objectForKey("num");
    this->dropLocalNotificationByName(name->getCString());
    this->addFreeGashaponLN(nmu->getValue());
}

//NSDate* LocalNotifDelegate::getLocalDateByNow() {
//    NSDate* now = [NSDate date];
//    NSTimeZone* timeZone = [NSTimeZone systemTimeZone];
//    NSInteger interval = [timeZone secondsFromGMTForDate:now];
//    return [now dateByAddingTimeInterval:interval];
//}
//
//int LocalNotifDelegate::getWeekIndexFromDate(NSDate* date) {
//    NSCalendar* calendar = [[[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar] autorelease];
//    NSDateComponents* components = [[[NSDateComponents alloc] init] autorelease];
//    NSInteger unitFlags =   NSYearCalendarUnit |
//    NSMonthCalendarUnit |
//    NSDayCalendarUnit |
//    NSWeekdayCalendarUnit |
//    NSHourCalendarUnit |
//    NSMinuteCalendarUnit |
//    NSSecondCalendarUnit;
//    components = [calendar components:unitFlags fromDate:date];
//    int weekday = [components weekday];
//    //    NSLog(@"weekday = %d, hour = %d, minu = %d", weekday, [components hour], [components minute]);
//    return weekday - 1; // sunday:0
//}

