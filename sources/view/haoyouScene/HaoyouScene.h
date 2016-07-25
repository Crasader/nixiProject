//
//  HaoyouScene.h
//  tiegao
//
//  Created by mac on 16-6-18.
//
//

#ifndef __tiegao__HaoyouScene__
#define __tiegao__HaoyouScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

typedef enum{
    Tag_Hy_TouFa = 1,
    Tag_Hy_WaiTao,
    Tag_Hy_ShangYi,
    Tag_Hy_KuZi,
    Tag_Hy_WaZi,
    Tag_Hy_XieZi,
    Tag_Hy_ShiPin,
    Tag_Hy_Bao,
    Tag_Hy_ZhuangRong,
    Tag_Hy_TouFa1 = 30,
    Tag_Hy_WaiTao1,
    Tag_Hy_ShangYi1,
    Tag_Hy_KuZi1,
    Tag_Hy_WaZi1,
    Tag_Hy_XieZi1,
    Tag_Hy_ShiPin1,
    Tag_Hy_Bao1,
    Tag_Hy_ZhuangRong1,
    Tag_Hy_TouFa2 = 40,
    Tag_Hy_WaiTao2,
    Tag_Hy_ShangYi2,
    Tag_Hy_KuZi2,
    Tag_Hy_WaZi2,
    Tag_Hy_XieZi2,
    Tag_Hy_ShiPin2,
    Tag_Hy_Bao2,
    Tag_Hy_TouFa3 = 50,
    Tag_Hy_WaiTao3,
    Tag_Hy_ShangYi3,
    Tag_Hy_KuZi3,
    Tag_Hy_WaZi3,
    Tag_Hy_XieZi3,
    Tag_Hy_ShiPin3,
    Tag_Hy_Bao3,
    Tag_Hy_ShangCheng = 90,
    Tag_Hy_BuLiao = 91
}Hy_Enum;

class HaoyouScene : public BaseScene
{
public:
    HaoyouScene();
    ~HaoyouScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(HaoyouScene);
    
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
    
    void strangers_callback_802(CCObject* pObj);
    void all_message_callback_804(CCObject* pObj);
    void all_friends_callback_806(CCObject* pObj);
    void all_paper_callback_808(CCObject* pObj);
    void rank_list_callback_300(CCObject* pObj);

public:
    
    CCDictionary* allClothesDic;
    
    CCMenu* allMenu;
    
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
    CCSprite* _zrSpr1; // 妆容
    
private:
    void update_news_status();
    
private:
    CCMenuItem*         _xiaoxiItem;
    CCMenuItem*         _zhitiaoItem;
    bool hasFriends;
};
#endif /* defined(__tiegao__HaoyouScene__) */
