//
//  MMLocalNotif.h
//  mm3c
//
//  Created by lakkey on 14-8-22.
//
//

#ifndef __mm3c__MMLocalNotif__
#define __mm3c__MMLocalNotif__

#include "cocos2d.h"

USING_NS_CC;

class LocalNotifItem : public CCObject
{
public: // Export
//    void configWithDict(CCDictionary* dic);
    void description();
    
    CC_SYNTHESIZE(int, _index, Index);
    CC_SYNTHESIZE(bool, _enable, Enable);
    CC_SYNTHESIZE_RETAIN(CCArray*, _week, Week);
    CC_SYNTHESIZE(int, _hour, Hour);
    CC_SYNTHESIZE(int, _minute, Minute);
    CC_SYNTHESIZE_RETAIN(CCString*, _title, Title);
    CC_SYNTHESIZE_RETAIN(CCString*, _alertBody, AlertBody);
    CC_SYNTHESIZE_RETAIN(CCString*, _sound, Sound);
    
public:
    ~LocalNotifItem();
    CREATE_FUNC(LocalNotifItem);
    bool init() {return true;}
    
};

#endif /* defined(__mm3c__MMLocalNotif__) */

