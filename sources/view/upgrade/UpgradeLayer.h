//
//  UpgradeLayer.h
//  tiegao
//
//  Created by mac on 16-9-2.
//
//

#ifndef __tiegao__UpgradeLayer__
#define __tiegao__UpgradeLayer__

#include <iostream>
#include "cocos2d.h"


USING_NS_CC;


class UpgradeLayer : public CCLayer
{
public:
    UpgradeLayer();
    ~UpgradeLayer();
    
    void onEnter();
    void onExit();
    
    static UpgradeLayer* create_with_index(int index);
    void init_with_index(int index);
    
    bool init();
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(UpgradeLayer);
    
public:
    
    CCSprite* diaocheSpr2;
    
    bool diaocheBool;
    
public:
    
    void creat_view1();
    void updateDiaoche(float dt);
    void creat_view2();
    
};
#endif /* defined(__tiegao__UpgradeLayer__) */
