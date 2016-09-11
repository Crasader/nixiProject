#ifndef __tiegao_LocalNotifMgr__
#define __tiegao_LocalNotifMgr__

#include "cocos2d.h"
#import "LocalNotifDelegate.h"

USING_NS_CC;

class LocalNotifMgr : public CCObject
{
public: // Export
    void addFreeGashaponLN(time_t secondDelta);
    void dropLocalNotificationByName(string name);
    void dropAllLocalNotifications();
    void resetIconBadgeNumber();
    
public:
    ~LocalNotifMgr();
    static LocalNotifMgr* Inst();
    void init();
    
    CC_SYNTHESIZE_RETAIN(CCDictionary*, _temp, Temp);
    
private:
    void nc_gashapon(CCObject* pObj);
//    void _nc_gashapon();
    
private:
    LocalNotifDelegate*     _delegate;
};

#endif /* defined(__mm3c__LocalNotifMgr__) */
