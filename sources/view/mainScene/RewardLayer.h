//
//  RewardLayer.h
//  tiegao
//
//  Created by mac on 16-8-12.
//
//

#ifndef __tiegao__RewardLayer__
#define __tiegao__RewardLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class RewardLayer : public CCLayerColor
{
public:
    RewardLayer();
    ~RewardLayer();
    
    void onEnter();
    void onExit();
    
    bool init();
    
    static RewardLayer* create_with_index(CCArray* arr);
    void init_with_index(CCArray* arr);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(RewardLayer);
    
public:
    
    CCSprite* bgSpr;
    CCSprite* kuangSpr;
    
    bool touchBool;
    
public:
    
    void openTouch();
    
};
#endif /* defined(__tiegao__RewardLayer__) */
