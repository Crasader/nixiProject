//
//  NewSignin7Panel.h
//  tiegao
//
//  Created by mac on 17-3-17.
//
//

#ifndef __tiegao__NewSignin7Panel__
#define __tiegao__NewSignin7Panel__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class NewSignin7Panel : public CCLayer
{
public: // Export
    static void show(CCNode* parent);
    
public:
    ~NewSignin7Panel();
    CREATE_FUNC(NewSignin7Panel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
    
private:
    void creatView();
    void creatNewView();
    void creat_wealth(CCDictionary* dic);
    void creat_clothes(CCArray* arr);
    
    void creatMan();
    void initClothes();
    void creat_Clothes(CCArray* arr);
    int updataClothes(int type);
    
    void signinCallback(CCObject* pSender);
    void jiantou1CallBack(CCObject* pSender);
    void jiantou2CallBack(CCObject* pSender);
    void signin_callback_313();
    
private:
    int num_child;
    CCDictionary* nowClothesTemp;
    
    
    int _signin7Day, _signin7Value;
    int nowPage;
    
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
    CCSprite* _zrSpr1; // 妆容
    
};
#endif /* defined(__tiegao__NewSignin7Panel__) */
