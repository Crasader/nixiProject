//
//  MonthCardLayer.h
//  tiegao
//
//  Created by mac on 17-3-16.
//
//

#ifndef __tiegao__MonthCardLayer__
#define __tiegao__MonthCardLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class MonthCardLayer : public CCLayer
{
public:
    MonthCardLayer();
    ~MonthCardLayer();
    
    void onEnter();
    void onExit();
    
    virtual bool init();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(MonthCardLayer);
    
public:
    
    CCSprite* kuangSpr;
    CCSprite* goldTishiSpr;
    CCSprite* moneyTishiSpr;
    
    int num_child;
    
public:
    
    void creat_view();
    
    void goldButtonCallBack(CCObject* pSender);
    void moneyButtonCallBack(CCObject* pSender);
};
#endif /* defined(__tiegao__MonthCardLayer__) */
