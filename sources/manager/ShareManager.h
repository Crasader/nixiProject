//
//  ShareManager.h
//  mm3c
//
//  Created by lakkey on 16-1-15.
//
//

#ifndef __mm3c__ShareManager__
#define __mm3c__ShareManager__

#include "cocos2d.h"
#include "C2DXShareSDK.h"

USING_NS_CC;
using namespace cn::sharesdk;

class ShareManager : public CCObject
{
public: // Export
    void share_pic();
    void test();
    
public:
    ~ShareManager();
    static ShareManager* get_instance();
    void init();
    void config();
    
    void stop_share();
    void did_share_success();
    
    void show_share_menu(CCDictionary* content);
    void shareStatus(float dt);
    
protected:
    void delay_share_success();
    
    void add_event_listener();
    void remove_event_listener();
    
//    void notification_http_error(CCObject* pObj);
    void notification_4500_callback(CCObject* pObj);
};

#endif /* defined(__mm3c__ShareManager__) */
