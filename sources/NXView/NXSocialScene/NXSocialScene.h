//
//  NXSocialScene.hpp
//  tiegao
//
//  Created by stevenLi on 17/7/10.
//
//

#ifndef NXSocialScene_hpp
#define NXSocialScene_hpp

#include <stdio.h>
#include "NXBaseScene.h"

USING_NS_CC;

typedef enum{
    Tag_SS_TouFa = 1,
    Tag_SS_WaiTao,
    Tag_SS_ShangYi,
    Tag_SS_KuZi,
    Tag_SS_TeXiao,
    Tag_SS_XieZi,
    Tag_SS_ShiPin,
    Tag_SS_Bao,
    Tag_SS_ZhuangRong,
    
    Tag_SS_TouFa1 = 30,
    Tag_SS_WaiTao1,
    Tag_SS_ShangYi1,
    Tag_SS_KuZi1,
    Tag_SS_TeXiao1,
    Tag_SS_XieZi1,
    Tag_SS_ShiPin1,
    Tag_SS_Bao1,
    Tag_SS_ZhuangRong1,
    
    Tag_SS_TouFa2 = 40,
    Tag_SS_WaiTao2,
    Tag_SS_ShangYi2,
    Tag_SS_KuZi2,
    Tag_SS_TeXiao2,
    Tag_SS_XieZi2,
    Tag_SS_ShiPin2,
    Tag_SS_Bao2,
    Tag_SS_TouFa3 = 50,
    Tag_SS_WaiTao3,
    Tag_SS_ShangYi3,
    Tag_SS_KuZi3,
    Tag_SS_TeXiao3,
    Tag_SS_XieZi3,
    Tag_SS_ShiPin3,
    Tag_SS_Bao3,
    Tag_SS_Bao4 = 60,
    Tag_SS_ShangCheng = 90,
    Tag_SS_BuLiao = 91,
    
    Tag_SS_ShiPin1_1 = 1011,
    Tag_SS_ShiPin1_2,
    Tag_SS_ShiPin1_3,
    Tag_SS_ShiPin1_4,
    Tag_SS_ShiPin1_5,
    Tag_SS_ShiPin1_6,
    Tag_SS_ShiPin1_7,
    Tag_SS_ShiPin1_8,
    Tag_SS_ShiPin1_9,
    Tag_SS_ShiPin2_1 = 2011,
    Tag_SS_ShiPin2_2,
    Tag_SS_ShiPin2_3,
    Tag_SS_ShiPin2_4,
    Tag_SS_ShiPin2_5,
    Tag_SS_ShiPin2_6,
    Tag_SS_ShiPin2_7,
    Tag_SS_ShiPin2_8,
    Tag_SS_ShiPin2_9,
    Tag_SS_ShiPin3_1 = 3011,
    Tag_SS_ShiPin3_2,
    Tag_SS_ShiPin3_3,
    Tag_SS_ShiPin3_4,
    Tag_SS_ShiPin3_5,
    Tag_SS_ShiPin3_6,
    Tag_SS_ShiPin3_7,
    Tag_SS_ShiPin3_8,
    Tag_SS_ShiPin3_9,
}SS_Enum;

class NXSocialScene: public NXBaseScene {
    
public:
    NXSocialScene();
    ~NXSocialScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(NXSocialScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();

public:
    
    void creat_view();
    
    void creat_Man();
    void initClothes();
    
    void openCallback();
    void openCallback2();
    void hiddenCallback();
    void hiddenCallback2();
    
    void backCallBack(CCObject* pSender);
    void xiaoxiCallBack(CCObject* pSender);
    void zhitiaoCallBack(CCObject* pSender);
    void haoyouCallBack(CCObject* pSender);
    void strangerCallBack(CCObject* pSender);
    void paihangCallBack(CCObject* pSender);
    void shareCallBack(CCObject* pSender);
    void shareStatus(float dt);
    
    void competition_callback_820(CCObject *pObj);
    void strangers_callback_802(CCObject* pObj);
    void all_message_callback_804(CCObject* pObj);
    void all_friends_callback_806(CCObject* pObj);
    void all_paper_callback_808(CCObject* pObj);
    void rank_list_callback_300(CCObject* pObj);
    
    void openChat();
    void displayChatItem();
    void createChatPanel();
    void _600CallBack(CCObject* pObj);
    void _605CallBack(CCObject* pObj);
    void _321CallBack(CCObject* pObj);
    
public:
    int num_child;
    
    
    CCDictionary* allClothesDic;
    
    CCMenu* allMenu;
    CCMenu* shareMenu;
    CCMenuItem* shareItem;
    
    CCSprite* roomSpr;
    CCSprite* shareBgSpr;
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
    CCSprite* _txSpr1; // 特效
    CCSprite* _xzSpr1; // 鞋子
    CCSprite* _xzSpr2;
    CCSprite* _xzSpr3;
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
    CCSprite* _bSpr4;
    CCSprite* _zrSpr1; // 妆容
    
private:
    void update_news_status();
    void iOS_share_finish(CCObject* pSender);
    
private:
    CCMenuItem*         _xiaoxiItem;
    CCMenuItem*         _zhitiaoItem;
    CCMenuItemSprite*   item_chat;
    
    bool                _isLoadRanklist;
};

#endif /* NXSocialScene_hpp */
