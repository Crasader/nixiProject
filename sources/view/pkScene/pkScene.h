//
//  pkScene.h
//  tiegao
//
//  Created by mac on 17-1-6.
//
//

#ifndef __tiegao__pkScene__
#define __tiegao__pkScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
#include "AHMessageBox.h"

USING_NS_CC;

typedef enum{
    Tag_PK_TouFa = 1,
    Tag_PK_WaiTao,
    Tag_PK_ShangYi,
    Tag_PK_KuZi,
    Tag_PK_WaZi,
    Tag_PK_XieZi,
    Tag_PK_ShiPin,
    Tag_PK_Bao,
    Tag_PK_ZhuangRong,
    Tag_PK_TouFa1 = 30,
    Tag_PK_WaiTao1,
    Tag_PK_ShangYi1,
    Tag_PK_KuZi1,
    Tag_PK_WaZi1,
    Tag_PK_XieZi1,
    Tag_PK_ShiPin1,
    Tag_PK_Bao1,
    Tag_PK_ZhuangRong1,
    Tag_PK_TouFa2 = 40,
    Tag_PK_WaiTao2,
    Tag_PK_ShangYi2,
    Tag_PK_KuZi2,
    Tag_PK_WaZi2,
    Tag_PK_XieZi2,
    Tag_PK_ShiPin2,
    Tag_PK_Bao2,
    Tag_PK_TouFa3 = 50,
    Tag_PK_WaiTao3,
    Tag_PK_ShangYi3,
    Tag_PK_KuZi3,
    Tag_PK_WaZi3,
    Tag_PK_XieZi3,
    Tag_PK_ShiPin3,
    Tag_PK_Bao3,
    Tag_PK_ShangCheng = 90,
    Tag_PK_BuLiao = 91
}PK_Enum;

class pkScene
: public BaseScene
, public AHMessageBoxDelegate
{
public:
    pkScene();
    ~pkScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(pkScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
public:
    
    void creat_view();
    void creatAnimation();
    void updateOpacity(float dt);
    
    void creatScore(CCSprite* tishiKuangSpr);
    void creatGuang(CCSprite* dapeiSpr);
    void creatHuangguan(CCSprite* huangguanSpr);
    void creatRenwuKuang();
    void creatName1(CCSprite* kuangSpr);
    void creatName2(CCSprite* kuangSpr);
    void creatKuangButton();
    void creatDuijue();
    void creatZhuti();
    void creatStart();
    void creatZhufu(CCMenuItem *item);
    
    
    void creat_Man1();
    void creat_Man2();
    void initClothes(CCClippingNode* _ManSpr, float widthFolt, float heightFloat, float scaleFloat, bool flipxBool);
    
    
    void backCallBack(CCObject* pSender);
    void huanzhuangCallback(CCObject* pSender);
    void huanhaoCallback(CCObject* pSender);
    void startCallback(CCObject* pSender);
    void zhufuCallback(CCObject* pSender);
    void buttonCallBack(CCObject* pSender);
    
    
public:
    int num_child;
    
    CCDictionary* allClothesDic;
    
    int xingIndex1_1, xingIndex1_2, xingIndex1_3, xingIndex2_1, xingIndex2_2, xingIndex2_3;
    bool xingBool1_1, xingBool1_2, xingBool1_3, xingBool2_1, xingBool2_2, xingBool2_3;
    
    CCSprite* bgSpr;
    CCSprite* kuangSpr1_3;
    CCSprite* kuangSpr2_3;
    CCSprite* xingSpr1_1,* xingSpr1_2,* xingSpr1_3,* xingSpr2_1,* xingSpr2_2,* xingSpr2_3;
    
    CCLabelTTF* slScore;
    CCLabelTTF* pjScore;
    CCLabelTTF* sbScore;
    CCLabelTTF* zhufuLabel;
    
    CCMenuItem *hzItem, *hhItem, *startItem, *zhufuItem;
    
};
#endif /* defined(__tiegao__pkScene__) */
