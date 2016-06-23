//
//  StrangerScene.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#ifndef StrangerScene_hpp
#define StrangerScene_hpp

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

typedef enum{
    Tag_STRANGER_TouFa = 1,
    Tag_STRANGER_WaiTao,
    Tag_STRANGER_ShangYi,
    Tag_STRANGER_KuZi,
    Tag_STRANGER_WaZi,
    Tag_STRANGER_XieZi,
    Tag_STRANGER_ShiPin,
    Tag_STRANGER_Bao,
    Tag_STRANGER_TouFa1 = 30,
    Tag_STRANGER_WaiTao1,
    Tag_STRANGER_ShangYi1,
    Tag_STRANGER_KuZi1,
    Tag_STRANGER_WaZi1,
    Tag_STRANGER_XieZi1,
    Tag_STRANGER_ShiPin1,
    Tag_STRANGER_Bao1,
    Tag_STRANGER_TouFa2 = 40,
    Tag_STRANGER_WaiTao2,
    Tag_STRANGER_ShangYi2,
    Tag_STRANGER_KuZi2,
    Tag_STRANGER_WaZi2,
    Tag_STRANGER_XieZi2,
    Tag_STRANGER_ShiPin2,
    Tag_STRANGER_Bao2,
    Tag_STRANGER_TouFa3 = 50,
    Tag_STRANGER_WaiTao3,
    Tag_STRANGER_ShangYi3,
    Tag_STRANGER_KuZi3,
    Tag_STRANGER_WaZi3,
    Tag_STRANGER_XieZi3,
    Tag_STRANGER_ShiPin3,
    Tag_STRANGER_Bao3,
    Tag_STRANGER_ShangCheng = 90,
    Tag_STRANGER_BuLiao = 91
}STRANGER_Enum;

class StrangerScene: public BaseScene
{
public:
    ~StrangerScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(StrangerScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    void createView();
    void initStranger();
    
    void btn_share_callback(CCObject* pSender);
    void btn_note_callback(CCObject* pSender);
    void btn_back_callback(CCObject* pSender);
        
public:
    
    CCDictionary* allClothesDic;
    
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
    
    void creat_Man();
    void initClothes();//穿衣服
    
};

#endif /* StrangerScene_hpp */
