//
//  ClothesScene.h
//  tiegao
//
//  Created by mac on 16-6-15.
//
//

#ifndef __tiegao__ClothesScene__
#define __tiegao__ClothesScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
#include "PromptLayer.h"

USING_NS_CC;

#define def_z_order 50

typedef enum{
    Tag_GJ_TouFa = 1,
    Tag_GJ_WaiTao,
    Tag_GJ_ShangYi,
    Tag_GJ_KuZi,
    Tag_GJ_WaZi,
    Tag_GJ_XieZi,
    Tag_GJ_ShiPin,
    Tag_GJ_Bao,
    Tag_GJ_TouFa1 = 30,
    Tag_GJ_WaiTao1,
    Tag_GJ_ShangYi1,
    Tag_GJ_KuZi1,
    Tag_GJ_WaZi1,
    Tag_GJ_XieZi1,
    Tag_GJ_ShiPin1,
    Tag_GJ_Bao1,
    Tag_GJ_TouFa2 = 40,
    Tag_GJ_WaiTao2,
    Tag_GJ_ShangYi2,
    Tag_GJ_KuZi2,
    Tag_GJ_WaZi2,
    Tag_GJ_XieZi2,
    Tag_GJ_ShiPin2,
    Tag_GJ_Bao2,
    Tag_GJ_TouFa3 = 50,
    Tag_GJ_WaiTao3,
    Tag_GJ_ShangYi3,
    Tag_GJ_KuZi3,
    Tag_GJ_WaZi3,
    Tag_GJ_XieZi3,
    Tag_GJ_ShiPin3,
    Tag_GJ_Bao3,
    Tag_GJ_ShangCheng = 90,
    Tag_GJ_BuLiao = 91
}GJ_Enum;

class ClothesScene : public BaseScene
{
public:
    
    ClothesScene();
    ~ClothesScene();
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(ClothesScene);
    
public:
    
    int buttonTag;
    
    CCSprite* _ManSpr;
    CCSprite* _touSpr;
    CCSprite* bgSpr;
    CCSprite* clothKuangSpr;
    
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
    
    
    
    CCMenu* allMenu;
    
    PromptLayer* promptLayer;
    
public:
    
    void openTouch(float dt);
    void creat_View();
    void creat_Man();
    void initClothes();//穿衣服
    
    void backCallBack(CCObject* pSender);
    void startCallBack(CCObject* pSender);
    void buttonCallBack(CCObject* pSender);
    void buyCallBack(CCObject* pSender);
};


#endif /* defined(__tiegao__ClothesScene__) */
