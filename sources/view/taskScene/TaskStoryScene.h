//
//  TaskStoryScene.h
//  tiegao
//
//  Created by mac on 16-7-8.
//
//

#ifndef __tiegao__TaskStoryScene__
#define __tiegao__TaskStoryScene__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

typedef enum{
    Tag_TS_TouFa = 1,
    Tag_TS_WaiTao,
    Tag_TS_ShangYi,
    Tag_TS_KuZi,
    Tag_TS_WaZi,
    Tag_TS_XieZi,
    Tag_TS_ShiPin,
    Tag_TS_Bao,
    Tag_TS_ZhuangRong,
    Tag_TS_TeXiao,
    Tag_TS_TouFa1 = 30,
    Tag_TS_WaiTao1,
    Tag_TS_ShangYi1,
    Tag_TS_KuZi1,
    Tag_TS_WaZi1,
    Tag_TS_XieZi1,
    Tag_TS_ShiPin1,
    Tag_TS_Bao1,
    Tag_TS_ZhuangRong1,
    Tag_TS_TeXiao1,
    Tag_TS_TouFa2 = 40,
    Tag_TS_WaiTao2,
    Tag_TS_ShangYi2,
    Tag_TS_KuZi2,
    Tag_TS_WaZi2,
    Tag_TS_XieZi2,
    Tag_TS_ShiPin2,
    Tag_TS_Bao2,
    Tag_TS_TouFa3 = 50,
    Tag_TS_WaiTao3,
    Tag_TS_ShangYi3,
    Tag_TS_KuZi3,
    Tag_TS_WaZi3,
    Tag_TS_XieZi3,
    Tag_TS_ShiPin3,
    Tag_TS_Bao3,
    Tag_TS_Bao4 = 60,
    Tag_TS_ShangCheng = 90,
    Tag_TS_BuLiao = 91,
    
    Tag_TS_ShiPin1_1 = 1011,
    Tag_TS_ShiPin1_2,
    Tag_TS_ShiPin1_3,
    Tag_TS_ShiPin1_4,
    Tag_TS_ShiPin1_5,
    Tag_TS_ShiPin1_6,
    Tag_TS_ShiPin1_7,
    Tag_TS_ShiPin1_8,
    Tag_TS_ShiPin1_9,
    Tag_TS_ShiPin2_1 = 2011,
    Tag_TS_ShiPin2_2,
    Tag_TS_ShiPin2_3,
    Tag_TS_ShiPin2_4,
    Tag_TS_ShiPin2_5,
    Tag_TS_ShiPin2_6,
    Tag_TS_ShiPin2_7,
    Tag_TS_ShiPin2_8,
    Tag_TS_ShiPin2_9,
    Tag_TS_ShiPin3_1 = 3011,
    Tag_TS_ShiPin3_2,
    Tag_TS_ShiPin3_3,
    Tag_TS_ShiPin3_4,
    Tag_TS_ShiPin3_5,
    Tag_TS_ShiPin3_6,
    Tag_TS_ShiPin3_7,
    Tag_TS_ShiPin3_8,
    Tag_TS_ShiPin3_9,
}TS_Enum;

class TaskStoryScene : public CCLayer
{
public:
    TaskStoryScene();
    ~TaskStoryScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(TaskStoryScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();
    
public:
    
    void creat_view();
    
    void creat_Man();
    void initClothes();
    
    void backCallBack(CCObject* pSender);
    
public:
    
    CCDictionary* allClothesDic;
    
    CCMenuItem* backItem;
    
    //    CCSprite* roomSpr;
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
    
    CCArray* taskArr;
    
    int OpenToWhichOne;
    int taskPhase;
    int taskIndex;
    
    
    
};
#endif /* defined(__tiegao__TaskStoryScene__) */
