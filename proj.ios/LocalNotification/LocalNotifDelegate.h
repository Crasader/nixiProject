//
//  LocalNotifDelegate.h
//  mm3c
//
//  Created by lakkey on 14-8-27.
//
//

#ifndef __mm3c__LocalNotifDelegate__
#define __mm3c__LocalNotifDelegate__

#include "cocos2d.h"

USING_NS_CC;

const NSString* NOTIFICATION_KEY_NAME = @"name";

class LocalNotifDelegate : public CCObject
{
public: // Export
    void addFreeGashaponLN();
    void dropLocalNotification(UILocalNotification* notif);
    void dropLocalNotificationByName(string name);
    void dropAllLocalNotifications();
    void resetIconBadgeNumber();
    
public:
    ~LocalNotifDelegate();
    CREATE_FUNC(LocalNotifDelegate);
    bool init();
    void notificationUpdateLocalNotification(CCObject* pObj);
    
private:
    NSDate* getLocalDateByNow();
    int getWeekIndexFromDate(NSDate* date);
};

#endif /* defined(__mm3c__LocalNotifDelegate__) */
