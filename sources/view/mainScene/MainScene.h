//
//  MainScene.h
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#ifndef __tiegao__MainScene__
#define __tiegao__MainScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class MainScene : public BaseScene
{
public:
    MainScene();
    ~MainScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(MainScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();
    
    void didAccelerate( CCAcceleration* pAccelerationValue);
    void gengxin(float dt);
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
    
public:
    
    void creat_view();
    void creat_Man();
    void initClothes();
    
    void huodongCallBack(CCObject* pSender);
    void qiandaoCallBack(CCObject* pSender);
    void youjianCallBack(CCObject* pSender);
    void renwuCallBack(CCObject* pSender);
    void huanzhuangCallBack(CCObject* pSender);
    void _huanzhuangCallBack(CCObject* pSender);
    void paihangCallBack(CCObject* pSender);
    void juqingCallBack(CCObject* pSender);
    void richangCallBack(CCObject* pSender);
    void shezhiCallBack(CCObject* pSender);
    void shouchongCallBack(CCObject* pSender);
    void haoyouCallBack(CCObject* pSender);
    
    void _500CallBack(CCObject* pSender);
    void all_mail_callback_700(CCObject* pObj);
    
public:
    CCDictionary* allClothesDic;
    
    CCSprite* roomSpr;
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
    
    CCPoint playerVelocity;
    
};

#endif /* defined(__tiegao__MainScene__) */
