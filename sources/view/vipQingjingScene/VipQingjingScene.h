//
//  VipQingjingScene.h
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#ifndef __tiegao__VipQingjingScene__
#define __tiegao__VipQingjingScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
#include "VipQingjingCoverView.h"
#include "AHMessageBox.h"

USING_NS_CC;

typedef enum{
    Tag_Vip_QJ_TouFa = 1,
    Tag_Vip_QJ_WaiTao,
    Tag_Vip_QJ_ShangYi,
    Tag_Vip_QJ_KuZi,
    Tag_Vip_QJ_WaZi,
    Tag_Vip_QJ_XieZi,
    Tag_Vip_QJ_ShiPin,
    Tag_Vip_QJ_Bao,
    Tag_Vip_QJ_ZhuangRong,
    Tag_Vip_QJ_TouFa1 = 30,
    Tag_Vip_QJ_WaiTao1,
    Tag_Vip_QJ_ShangYi1,
    Tag_Vip_QJ_KuZi1,
    Tag_Vip_QJ_WaZi1,
    Tag_Vip_QJ_XieZi1,
    Tag_Vip_QJ_ShiPin1,
    Tag_Vip_QJ_Bao1,
    Tag_Vip_QJ_ZhuangRong1,
    Tag_Vip_QJ_TouFa2 = 40,
    Tag_Vip_QJ_WaiTao2,
    Tag_Vip_QJ_ShangYi2,
    Tag_Vip_QJ_KuZi2,
    Tag_Vip_QJ_WaZi2,
    Tag_Vip_QJ_XieZi2,
    Tag_Vip_QJ_ShiPin2,
    Tag_Vip_QJ_Bao2,
    Tag_Vip_QJ_TouFa3 = 50,
    Tag_Vip_QJ_WaiTao3,
    Tag_Vip_QJ_ShangYi3,
    Tag_Vip_QJ_KuZi3,
    Tag_Vip_QJ_WaZi3,
    Tag_Vip_QJ_XieZi3,
    Tag_Vip_QJ_ShiPin3,
    Tag_Vip_QJ_Bao3,
    Tag_Vip_QJ_ShangCheng = 90,
    Tag_Vip_QJ_BuLiao = 91
}Vip_QJ_Enum;

class VipQingjingScene
: public BaseScene
, public AHMessageBoxDelegate
{
public:
    VipQingjingScene();
    ~VipQingjingScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(VipQingjingScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
public:
    
    void creat_view();
    
    void creat_Man();
    void initClothes();
    
    void backCallBack(CCObject* pSender);
    void startCallBack(CCObject* pSender);
    void buyCallBack(CCObject* pSender);
    void _509CallBack(CCObject* pSender);
    void _505CallBack(CCObject* pSender);
    void _111CallBack(CCObject* pSender);
    void _113CallBack(CCObject* pSender);
    void jiantou1CallBack(CCObject* pSender);
    void jiantou2CallBack(CCObject* pSender);
    void closeButton();
    void updataButton();
    void updataMan();
    
    void qingjingStatus();

    void quedingCallBack(CCObject* pSender);
    void quxiaoCallBack(CCObject* pSender);

//    void openChat();
//    void displayChatItem();
    
    
    void send111();
    
//    void iOS_buy_109();
    void buy_iOS_story2_515();
    void nc_buy_iOS_story2(CCObject* pSender);
    
public:
    int num_child;
    
    CCDictionary* allClothesDic;
    
    VipQingjingCoverView* qingjingCoverView;
    CCMenuItem* jiantouItem1, *jiantouItem2, *tempItem;
    
//    CCMenuItemSprite* item_chat;
    
    int storyIndex;
    int allNumber;
    int selectedIndex;
    int buyIndex;
    
    bool  theEndBool;
    
    CCMenu* startMenu;
    CCMenuItem* startItem, * buyItem;
    
    CCSprite* roomSpr;
    CCSprite* manSpr;
    CCSprite* lianSpr;
    CCSprite* qingjingKuang;
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
#endif /* defined(__tiegao__VipQingjingScene__) */
