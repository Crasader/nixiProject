//
//  PrivacyPolicyView.hpp
//  mm3c
//
//  Created by mac on 16/4/11.
//
//

#ifndef PrivacyPolicyView_hpp
#define PrivacyPolicyView_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;


class PrivacyPolicyView : public CCLayerColor
{
public:
    PrivacyPolicyView();
    ~PrivacyPolicyView();
    CREATE_FUNC(PrivacyPolicyView);

    void onEnter();
    void onExit();
    
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    
    CCSprite* dkSpr;
    
public:
    void init_view();
    void agreeCallback(CCObject* pSender);
    void cancelCallback(CCObject* pSender);
};


#endif /* PrivacyPolicyView_hpp */
