//
//  NXClothesShareLayer.h
//  tiegao
//
//  Created by mac on 17-3-16.
//
//

#ifndef __tiegao__NXClothesShareLayer__
#define __tiegao__NXClothesShareLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

#define def_z_order 50

class NXClothesShareLayer
: public CCLayer
{
public:
    
    NXClothesShareLayer();
    ~NXClothesShareLayer();
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    static cocos2d::CCScene* scene();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(NXClothesShareLayer);
    
public:
    
    CCSprite* bgSpr;
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
    CCSprite* _txSpr1; // 袜子
    CCSprite* _txSpr2;
    CCSprite* _txSpr3;
    CCSprite* _xzSpr1; // 鞋子
    CCSprite* _xzSpr2;
    CCSprite* _xzSpr3;
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
    CCSprite* _bSpr4;
    CCSprite* _zrSpr1; // 妆容
    
public:
    
    void creat_Man();
    void initClothes();//穿衣服

};
#endif /* defined(__tiegao__NXClothesShareLayer__) */
