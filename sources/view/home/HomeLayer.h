//
//  HomeLayer.h
//  tiegao
//
//  Created by mac on 16-8-3.
//
//

#ifndef __tiegao__HomeLayer__
#define __tiegao__HomeLayer__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
#include "AHMessageBox.h"
#include "HomeTableView.h"

USING_NS_CC;

class HomeLayer
: public BaseScene
, public AHMessageBoxDelegate
{
public:
    
    HomeLayer();
    ~HomeLayer();
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(HomeLayer);
    
public:
    
    bool kuangBool;
    
    CCSprite* _ManSpr;
    CCSprite* _touSpr;
    CCSprite* bgSpr;
    CCSprite* kuangSpr;
    
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
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
    CCSprite* _bSpr4;
    CCSprite* _zrSpr1; // 妆容
    
    CCLabelTTF* bgLabel;
    
    HomeTableView* tabLayer;
    
    CCMenuItem* backItem;
    CCSprite* qiehuanSpr;
    CCMenuItem* qiehuanItem;
    CCMenuItemSprite* item_chat;
    void openChat(CCObject* pSender);
    void displayChatItem();
    
    
    void play_music(float dt);
    
public:
    
    void creat_View();
    void creat_Man();
    void initClothes();//穿衣服
    void updataBg();
    
    void manAction1();
    void manAction2();
    void openTouch(float dt);
    void openButton(float dt);
    void backCallBack(CCObject* pSender);
    void gongsiCallBack(CCObject* pSender);
    void qiehuanCallBack(CCObject* pSender);
    void gameCallBack(CCObject* pSender);
    void gameCallBack2(CCObject* pSender);
    void saveCallBack(CCObject* pSender);
    void huanzhuangCallBack(CCObject* pSender);
    void haoyouCallBack(CCObject* pSender);
    void _huanzhuangCallBack(CCObject* pSender);
    void _500CallBack(CCObject* pSender);
    void _600CallBack(CCObject* pSender);
    void _705CallBack(CCObject* pSender);
    void _800CallBack(CCObject* pSender);
};
#endif /* defined(__tiegao__HomeLayer__) */
