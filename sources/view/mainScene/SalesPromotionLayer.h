//
//  SalesPromotionLayer.h
//  tiegao
//
//  Created by mac on 17-3-8.
//
//

#ifndef __tiegao__SalesPromotionLayer__
#define __tiegao__SalesPromotionLayer__

#include <iostream>
#include "cocos2d.h"
#include "AHMessageBox2.h"

USING_NS_CC;

class SalesPromotionLayer
: public CCLayer
, public AHMessageBox2Delegate
{
public:
    ~SalesPromotionLayer();
    CREATE_FUNC(SalesPromotionLayer);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void message_box_did_selected_button2(AHMessageBox2* box, AH_BUTTON_TYPE2 button_type, AH_BUTTON_TAGS2 button_tag);
    
public:
    CCDictionary* nowClothesTemp;
    CCSprite* kuangSpr;
    
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
    int clthedPhase;
    
    CCMenu* buyMenu;
    
    CCLabelTTF* timeLabel;
    
    CC_SYNTHESIZE(int, _hour, Hour);
    CC_SYNTHESIZE(int, _minute, Minute);
    CC_SYNTHESIZE(int, _second, Second);
    
public:
    
    void creat_View();
    void creat_Man();
    void initClothes();
    void creat_Clothes();
    void save_Clothes();
    bool judgeClothes();
    void creat_kuang();
    void creat_Clothes(CCSprite* spr, int index);
    void haveClothes(CCSprite* spr, int index);
    int updataClothes(int type);
    void initTime();
    void updateTime(float dt);
    
    void changeCallBack(CCObject* pSender);
    void buyCallBack(CCObject* pSender);
    void _161CallBack(CCObject* pSender);
    
};
#endif /* defined(__tiegao__SalesPromotionLayer__) */
