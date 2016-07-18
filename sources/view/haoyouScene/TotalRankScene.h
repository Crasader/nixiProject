//
//  TotalTotalScene.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#ifndef TotalRankScene_hpp
#define TotalRankScene_hpp

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
#include "NotePanel.h"
#include "TotalRankTableView.h"

USING_NS_CC;

typedef enum{
    Tag_Total_TouFa = 1,
    Tag_Total_WaiTao,
    Tag_Total_ShangYi,
    Tag_Total_KuZi,
    Tag_Total_WaZi,
    Tag_Total_XieZi,
    Tag_Total_ShiPin,
    Tag_Total_Bao,
    Tag_Total_ZhuangRong,
    Tag_Total_TouFa1 = 30,
    Tag_Total_WaiTao1,
    Tag_Total_ShangYi1,
    Tag_Total_KuZi1,
    Tag_Total_WaZi1,
    Tag_Total_XieZi1,
    Tag_Total_ShiPin1,
    Tag_Total_Bao1,
    Tag_Total_ZhuangRong1,
    Tag_Total_TouFa2 = 40,
    Tag_Total_WaiTao2,
    Tag_Total_ShangYi2,
    Tag_Total_KuZi2,
    Tag_Total_WaZi2,
    Tag_Total_XieZi2,
    Tag_Total_ShiPin2,
    Tag_Total_Bao2,
    Tag_Total_TouFa3 = 50,
    Tag_Total_WaiTao3,
    Tag_Total_ShangYi3,
    Tag_Total_KuZi3,
    Tag_Total_WaZi3,
    Tag_Total_XieZi3,
    Tag_Total_ShiPin3,
    Tag_Total_Bao3,
    Tag_Total_ShangCheng = 90,
    Tag_Total_BuLiao = 91
}Total_Enum;

class TotalRankScene: public BaseScene
{
public:
    ~TotalRankScene();
    
    virtual bool init();
    static TotalRankScene* create_with_type(int type);
    void init_with_type(int type);
    CREATE_FUNC(TotalRankScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    void createView();
    void initTotalRank();
    
    void btn_share_callback(CCObject* pSender);
    void btn_note_callback(CCObject* pSender);
    void btn_back_callback(CCObject* pSender);
    
//    void btn_self_callback(CCObject* pSender);
    
    void btn_toBig_callback(CCMenuItem* item);
    void btn_toSmall_callback(int index);
    
    int _curBtn_index;
    
    void small_callback();
    
    CCSprite* getNumSprite(int num);
    
    CCMenuItem* item_first;
    CCMenuItem* item_second;
    CCMenuItem* item_third;
    
    CCMenuItem* item_self;
    
    int rank_self;
    
    int _type;
    
    CCArray* _rankers;
    
public:
    NotePanel* _panel;
    TotalRankTableView* tabLayer;
    
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
    CCSprite* _zrSpr1; // 妆容
    
    void creat_Man();
    void initClothes();//穿衣服
    
    bool enterBool;
    void enterMan();
    void exitMan();
    void removeMan();
    void removeMask();
    
    CCDictionary* myClothesTemp;
};

#endif /* TotalRankScene_hpp */
