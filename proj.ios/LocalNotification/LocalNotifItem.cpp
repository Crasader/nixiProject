//
//  MMLocalNotif.cpp
//  mm3c
//
//  Created by lakkey on 14-8-22.
//
//

#include "LocalNotifItem.h"

//void LocalNotifItem::configWithDict(CCDictionary *dic) {
//    this->setIndex(dic->valueForKey("index")->intValue());
//    this->setEnable(dic->valueForKey("enable")->boolValue());
//    this->setWeek((CCArray*)dic->objectForKey("week"));
//    this->setHour(dic->valueForKey("hour")->intValue());
//    this->setMinute(dic->valueForKey("minu")->intValue());
//    this->setTitle((CCString*)dic->valueForKey("title"));
//    this->setAlertBody((CCString*)dic->valueForKey("alertbody"));
//    this->setSound((CCString*)dic->valueForKey("sound"));
//    CCLOG("setSound: %s", this->getSound()->getCString());
//    //    if (_sound->compare("") == 0) {
//    //        int cur_pet_id = MMDataManager::get_instance()->get_current_manpet_id();
//    //        if (cur_pet_id == 0) {
//    //            this->setSound(ccs("audio/clocksound/clock_101_1_1.wav"));
//    //            MZLog("setSound: %s", this->getSound()->getCString());
//    //        }
//    //        else {
//    //            this->setSound(CCString::createWithFormat("%d", cur_pet_id));
//    //        }
//    //    }
//    this->description();
//}

void LocalNotifItem::description() {
    CCLOG("------------LocalNotifItem::description()-------------");
    CCLOG("index: %d", _index);
    CCLOG("enable: %d", _enable);
    for (int i = 0; i < _week->count(); i++) {
        CCString* weekItem = (CCString*)_week->objectAtIndex(i);
        CCLOG("week %d: %d", i, weekItem->intValue());
    }
    CCLOG("hour: %d", _hour);
    CCLOG("minute: %d", _minute);
    CCLOG("title: %s", _title->getCString());
    CCLOG("alertbody: %s", _alertBody->getCString());
    CCLOG("sound: %s", _sound->getCString());
    CCLOG("------------LocalNotifItem::end-------------");
}

LocalNotifItem::~LocalNotifItem() {
    if (_week) {
        CC_SAFE_DELETE(_week);
        _week = NULL;
    }
    if (_title) {
        CC_SAFE_DELETE(_title);
        _title = NULL;
    }
    if (_alertBody) {
        CC_SAFE_DELETE(_alertBody);
        _alertBody = NULL;
    }
    if (_sound) {
        CC_SAFE_DELETE(_sound);
        _sound = NULL;
    }
}


