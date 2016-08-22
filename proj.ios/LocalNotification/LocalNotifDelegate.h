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


class LocalNotifDelegate : public CCObject
{
public: // Export
    void addFreeGashaponLN(time_t secondDelta);
//    void dropLocalNotification(UILocalNotification* notif);
    void dropLocalNotificationByName(string name);
    void dropAllLocalNotifications();
    void resetIconBadgeNumber();
    
public:
    ~LocalNotifDelegate();
    static LocalNotifDelegate* Inst();
    void init();
    
    CC_SYNTHESIZE_RETAIN(CCDictionary*, _temp, Temp);
    
private:
    void nc_gashapon(CCObject* pObj);
    void _nc_gashapon();
};

#endif /* defined(__mm3c__LocalNotifDelegate__) */
