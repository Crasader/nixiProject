//
//  TaskSettlementLayer.h
//  tiegao
//
//  Created by mac on 16-6-29.
//
//

#ifndef __tiegao__TaskSettlementLayer__
#define __tiegao__TaskSettlementLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class TaskSettlementLayer
: public CCLayer
{
public:
    
    TaskSettlementLayer();
    ~TaskSettlementLayer();
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(TaskSettlementLayer);
    
public:
    
    void creat_view();
    void lingquCallBack(CCObject* pSender);
    
    void creat_Man();
    void initClothes();
    
public:
    
    bool lingquBool;
    
    CCDictionary* allClothesDic;
    
    CCSprite* roomSpr;
    CCSprite* taskKuang;
    CCSprite* kuangSpr;
    CCSprite* _ManSpr;
    CCSprite* _touSpr;
    CCSprite* _tfSpr1; // 头发
    CCSprite* _tfSpr2;
    CCSprite* _tfSpr3;
    CCSprite* _wtSpr1; // 外套
    CCSprite* _wtSpr2;
    CCSprite* _wtSpr3;
    CCSprite* _sySpr1; // 上衣
    CCSprite* _sySpr2;
    CCSprite* _sySpr3;
    CCSprite* _kzSpr1; // 裤子
    CCSprite* _kzSpr2;
    CCSprite* _kzSpr3;
    CCSprite* _wzSpr1; // 袜子
    CCSprite* _wzSpr2;
    CCSprite* _wzSpr3;
    CCSprite* _xzSpr1; // 鞋子
    CCSprite* _xzSpr2;
    CCSprite* _xzSpr3;
    CCSprite* _spSpr1; // 饰品
    CCSprite* _spSpr2;
    CCSprite* _spSpr3;
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
};

#endif /* defined(__tiegao__TaskSettlementLayer__) */
