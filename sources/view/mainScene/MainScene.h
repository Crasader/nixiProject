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
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    CCPoint cur_pos;
    CCPoint last_pos;
    CCPoint end_pos;
    float offset_x;
    
    
public:
    
    void creat_view();
    void creat_Man();
    void initClothes();
    
    CCSprite* _layer_6;
    CCSprite* _river_1;
    CCSprite* _river_2;
    CCSprite* _cloud_1;
    CCSprite* _cloud_2;
    CCSprite* _layer_4;
    CCSprite* _layer_3;
    CCSprite* _layer_2;
    CCSprite* _layer_1;
    CCSprite* _layer_0;
    
    CCSprite* _bird_1;
    CCSprite* _bird_2;
    CCSprite* _bird_3;
    CCSprite* _bird_4;
    CCSprite* _bird_5;
    CCSprite* _bird_6;
    CCSprite* _bird_7;
    void resetBirdPosition();
    
    
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
    
    void juqing_vipCallBack(CCObject* pSender);
    void homeCallBack(CCObject* pSender);
    void update(float dt);
    
    void _500CallBack(CCObject* pSender);
    void _600CallBack(CCObject* pSender);
    void all_mail_callback_700(CCObject* pObj);
    void social_info_callback_800(CCObject* pObj);
    
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
    CCSprite* _zrSpr1; // 妆容
    
    CCPoint playerVelocity;
    
private:
    void update_news_status();
    
private:
    CCMenuItem*         _youjianItem;
    CCMenuItem*         _haoyouItem;
    CCMenuItem*         _shezhiItem;
};

#endif /* defined(__tiegao__MainScene__) */
