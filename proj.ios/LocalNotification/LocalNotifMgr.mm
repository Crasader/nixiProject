//
//  LocalNotifMgr.cpp
//  mm3c
//
//  Created by lakkey on 14-8-27.
//
//

#include "LocalNotifMgr.h"

static LocalNotifMgr* _instance = nullptr;

#pragma mark - Export

void LocalNotifMgr::addFreeGashaponLN(time_t secondDelta) {
    if (_delegate) {
        [_delegate addFreeGashaponLN:secondDelta];
    }
}

void LocalNotifMgr::addTrystLN(time_t secondDelta) {
    if (_delegate) {
        [_delegate addTrystLN:secondDelta];
    }
}

//void LocalNotifMgr::dropLocalNotification(UILocalNotification* notif) {
//    NSDictionary* dic = notif.userInfo;
//    NSString* value = [dic objectForKey:NOTIFICATION_KEY_NAME];
//    const char* csValue = [value UTF8String];
//    this->dropLocalNotificationByName(csValue);
//}

void LocalNotifMgr::dropLocalNotificationByName(string name) {

}

void LocalNotifMgr::dropAllLocalNotifications() {
    if (_delegate) {
        [_delegate dropAllLocalNotifications];
    }
}

void LocalNotifMgr::resetIconBadgeNumber() {
    if (_delegate) {
        [_delegate resetIconBadgeNumber];
    }
}

#pragma mark -

LocalNotifMgr::~LocalNotifMgr()
{
    CCLOG("LocalNotifMgr::~LocalNotifMgr()");
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

LocalNotifMgr* LocalNotifMgr::Inst() {
    if (_instance == nullptr) {
        _instance = new LocalNotifMgr();
        _instance->init();
    }
    
    return _instance;
}

void LocalNotifMgr::init() {
    _delegate = [LocalNotifDelegate create];
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&LocalNotifMgr::nc_gashapon), "FREE_GASHAPON", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&LocalNotifMgr::nc_tryst), "TRYST_ALERT", NULL);
}

#pragma makr - Inner

//void LocalNotifMgr::nc_gashapon(CCObject *pObj) {
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&LocalNotifMgr::_nc_gashapon), this, 1, 0, 0.1, false);
//    this->setTemp((CCDictionary*)pObj);
//}

void LocalNotifMgr::nc_gashapon(CCObject *pObj) {
    CCLOG("LocalNotifMgr::nc_gashapon()");
    CCDictionary* dic = (CCDictionary*)pObj;
    const CCString* name = (CCString*)dic->valueForKey("name");
    CCInteger* nmu = (CCInteger*)dic->objectForKey("num");
    this->dropLocalNotificationByName(name->getCString());
    this->addFreeGashaponLN(nmu->getValue());
}

void LocalNotifMgr::nc_tryst(CCObject *pObj) {
    CCLOG("LocalNotifMgr::nc_tryst()");
    CCDictionary* dic = (CCDictionary*)pObj;
    const CCString* name = (CCString*)dic->valueForKey("name");
    CCInteger* nmu = (CCInteger*)dic->objectForKey("num");
    this->dropLocalNotificationByName(name->getCString());
    this->addTrystLN(nmu->getValue());
}
