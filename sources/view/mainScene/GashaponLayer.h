//
//  GashaponLayer.h
//  tiegao
//
//  Created by mac on 16-8-11.
//
//

#ifndef __tiegao__GashaponLayer__
#define __tiegao__GashaponLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class GashaponLayer : public CCLayer
{
public:
    ~GashaponLayer();
    CREATE_FUNC(GashaponLayer);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    CCDictionary* allClothesDic;
    CCDictionary* myClothesTemp;
    CCSprite* bgSpr;
    CCArray* suitsArr;
    CCLabelTTF* label2;
    CCLabelTTF* timeLabel;
    CCLabelTTF* oneLabel;
    CCLabelTTF* oneGoldLabel;
    
    CCMenuItem* jiantouItem1,* jiantouItem2;
    
    CCMenuItem* oneItem;
    
    int nowIndex;
    CC_SYNTHESIZE(int, _hour, Hour);
    CC_SYNTHESIZE(int, _minute, Minute);
    CC_SYNTHESIZE(int, _second, Second);
    
    bool freeBool;
    
    CCSprite* diSpr2;
    
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
    CCSprite* _zrSpr1; // 妆容
    
    int num_child;
    
public:
    
    void creat_View();
    void creat_Man();
    void initClothes();
    void updataClothes();
    void initTime();
    void updateTime(float dt);
    void creat_gold(CCMenuItem* item, int index);
    
    void creat_Tishi(CCObject* arr);
    
    void jiantou1CallBack(CCObject* pSender);
    void jiantou2CallBack(CCObject* pSender);
    void updataButton();
    
    void suipianCallBack(CCObject* pSender);
    void oneCallBack(CCObject* pSender);
    void tenCallBack(CCObject* pSender);
    void _307CallBack(CCObject* pSender);
    void _309CallBack(CCObject* pSender);
    
};
#endif /* defined(__tiegao__GashaponLayer__) */
