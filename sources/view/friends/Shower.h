//
//  Shower.hpp
//  tiegao
//
//  Created by mac on 16/7/19.
//
//

#ifndef Shower_hpp
#define Shower_hpp

#include "cocos2d.h"

USING_NS_CC;

typedef enum{
    Tag_Rank_TouFa = 1,
    Tag_Rank_WaiTao,
    Tag_Rank_ShangYi,
    Tag_Rank_KuZi,
    Tag_Rank_WaZi,
    Tag_Rank_XieZi,
    Tag_Rank_ShiPin,
    Tag_Rank_Bao,
    Tag_Rank_ZhuangRong,
    Tag_Rank_TouFa1 = 30,
    Tag_Rank_WaiTao1,
    Tag_Rank_ShangYi1,
    Tag_Rank_KuZi1,
    Tag_Rank_WaZi1,
    Tag_Rank_XieZi1,
    Tag_Rank_ShiPin1,
    Tag_Rank_Bao1,
    Tag_Rank_ZhuangRong1,
    Tag_Rank_TouFa2 = 40,
    Tag_Rank_WaiTao2,
    Tag_Rank_ShangYi2,
    Tag_Rank_KuZi2,
    Tag_Rank_WaZi2,
    Tag_Rank_XieZi2,
    Tag_Rank_ShiPin2,
    Tag_Rank_Bao2,
    Tag_Rank_TouFa3 = 50,
    Tag_Rank_WaiTao3,
    Tag_Rank_ShangYi3,
    Tag_Rank_KuZi3,
    Tag_Rank_WaZi3,
    Tag_Rank_XieZi3,
    Tag_Rank_ShiPin3,
    Tag_Rank_Bao3,
    Tag_Rank_ShangCheng = 90,
    Tag_Rank_BuLiao = 91
}Rank_Enum;

class Shower : public CCSprite
{
public:
    void ondress(CCDictionary* clothes);
    void blink();
    CC_SYNTHESIZE_RETAIN(CCAnimate*, _blinkAnim, BlinkAnim);
    
public:
    ~Shower();
    CREATE_FUNC(Shower);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
private:
    
    
private:
    CCSprite* _ManSpr;
    CCSprite* _touSpr;
    
    int         _curZRId;
    int         _savedZRId;
    float       _nextInterval;
    
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

#endif /* Shower_hpp */
