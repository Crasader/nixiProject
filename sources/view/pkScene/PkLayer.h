//
//  PkLayer.h
//  tiegao
//
//  Created by mac on 17-1-9.
//
//

#ifndef __tiegao__PkLayer__
#define __tiegao__PkLayer__

#include <iostream>
#include "cocos2d.h"
#include "AHMessageBox.h"
#include "CompetitionComp.h"
#include "FlashNumberLabel.h"
#include "FlashNumberLabel2.h"


USING_NS_CC;

typedef enum{
    Tag_PkLayer_TouFa = 1,
    Tag_PkLayer_WaiTao,
    Tag_PkLayer_ShangYi,
    Tag_PkLayer_KuZi,
    Tag_PkLayer_WaZi,
    Tag_PkLayer_XieZi,
    Tag_PkLayer_ShiPin,
    Tag_PkLayer_Bao,
    Tag_PkLayer_ZhuangRong,
    Tag_PkLayer_TouFa1 = 30,
    Tag_PkLayer_WaiTao1,
    Tag_PkLayer_ShangYi1,
    Tag_PkLayer_KuZi1,
    Tag_PkLayer_WaZi1,
    Tag_PkLayer_XieZi1,
    Tag_PkLayer_ShiPin1,
    Tag_PkLayer_Bao1,
    Tag_PkLayer_ZhuangRong1,
    Tag_PkLayer_TouFa2 = 40,
    Tag_PkLayer_WaiTao2,
    Tag_PkLayer_ShangYi2,
    Tag_PkLayer_KuZi2,
    Tag_PkLayer_WaZi2,
    Tag_PkLayer_XieZi2,
    Tag_PkLayer_ShiPin2,
    Tag_PkLayer_Bao2,
    Tag_PkLayer_TouFa3 = 50,
    Tag_PkLayer_WaiTao3,
    Tag_PkLayer_ShangYi3,
    Tag_PkLayer_KuZi3,
    Tag_PkLayer_WaZi3,
    Tag_PkLayer_XieZi3,
    Tag_PkLayer_ShiPin3,
    Tag_PkLayer_Bao3,
    Tag_PkLayer_ShangCheng = 90,
    Tag_PkLayer_BuLiao = 91
}PkLayer_Enum;

class PkLayer
: public CCLayer
, public AHMessageBoxDelegate
, public FlashNumberLabelDelegate
, public FlashNumberLabel2Delegate
{
public:
    PkLayer();
    ~PkLayer();
    
    virtual bool init();
    static CCScene* scene();
    
    static PkLayer* create_with_Layer(int selfIndex1, int selfIndex2, int opponentIndex1, int opponentIndex2);
    void init_with_Layer(int selfIndex1, int selfIndex2, int opponentIndex1, int opponentIndex2);
    CREATE_FUNC(PkLayer);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual void will_number_scroll(){};
    virtual void did_number_stoped(){};
    
    virtual void will_number_scroll2(){};
    virtual void did_number_stoped2(){};
    
public:
    
    void creatAnimation();
    
    void creat_view();
    void creat_nameKuang();
    void creat_zhufuKuang();
    void creatDuijue();
    void creatJiesuan();
    void jiesuanAnimation(CCSprite* spr, FlashNumberLabel* label, int type);
    void creatJiesuan2();
    void flashNumberAnimation(CCSprite* spr, int type);
    void creatJiesuan3();
    void creatJiesuan4();
    void nextLayer1();
    void nextLayer2();
    
    void creatScoreAnimation1();
    void creatScoreAnimation2();
    void creatScoreAnimation3();
    void creatScoreAnimation4();
    void creatScoreKuang(int type);
    void updateScore(float dt);
    void updateOpacity(float dt);
    
    void creat_Man1();
    void creat_Man2();
    void initClothes(CCClippingNode* _ManSpr, float widthFolt, float heightFloat, float scaleFloat, bool flipxBool, CCDictionary* myClothesTemp);
    
    
    void backCallBack(CCObject* pSender);
    
    
public:
    int num_child;
    
    int temSelfIndex1;
    int temSelfIndex2;
    int temOpponentIndex1;
    int temOpponentIndex2;
    
    int bgIndex;
    int selfScore;
    int opponentScore;
    int tempSelfScore;
    int tempOpponentScore;
    
    int selfIndex1, selfIndex2, selfIndex3, selfIndex4, opponentIndex1, opponentIndex2, opponentIndex3, opponentIndex4;
    
    CCSprite* xingSpr1_1,* xingSpr1_2,* xingSpr1_3;
    int xingIndex1_1, xingIndex1_2, xingIndex1_3;
    bool xingBool1_1, xingBool1_2, xingBool1_3;
    
    CCSprite* jiesuanSpr1,* jiesuanGuangSpr1,* jiesuanQuanSpr,* jiesuanSpr2;
    CCSprite* jiesuanKuangSpr;
    
    
    
    CCDictionary* allClothesDic;
    
    FlashNumberLabel*   scoreLabel1;
    FlashNumberLabel2*  scoreLabel2;
        
    
    CompetitionThemeInfo* themeInfo;
    CompetitionItem* selfItem,* opponentItem;
    
};
#endif /* defined(__tiegao__PkLayer__) */
