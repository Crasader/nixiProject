//
//  JiesuanLayer.h
//  tiegao
//
//  Created by mac on 17-1-11.
//
//

#ifndef __tiegao__JiesuanLayer__
#define __tiegao__JiesuanLayer__

#include <iostream>
#include "cocos2d.h"
#include "CompetitionComp.h"
#include "FlashNumberLabel.h"


USING_NS_CC;


typedef enum{
    Tag_JSLayer_TouFa = 1,
    Tag_JSLayer_WaiTao,
    Tag_JSLayer_ShangYi,
    Tag_JSLayer_KuZi,
    Tag_JSLayer_WaZi,
    Tag_JSLayer_XieZi,
    Tag_JSLayer_ShiPin,
    Tag_JSLayer_Bao,
    Tag_JSLayer_ZhuangRong,
    Tag_JSLayer_TouFa1 = 30,
    Tag_JSLayer_WaiTao1,
    Tag_JSLayer_ShangYi1,
    Tag_JSLayer_KuZi1,
    Tag_JSLayer_WaZi1,
    Tag_JSLayer_XieZi1,
    Tag_JSLayer_ShiPin1,
    Tag_JSLayer_Bao1,
    Tag_JSLayer_ZhuangRong1,
    Tag_JSLayer_TouFa2 = 40,
    Tag_JSLayer_WaiTao2,
    Tag_JSLayer_ShangYi2,
    Tag_JSLayer_KuZi2,
    Tag_JSLayer_WaZi2,
    Tag_JSLayer_XieZi2,
    Tag_JSLayer_ShiPin2,
    Tag_JSLayer_Bao2,
    Tag_JSLayer_TouFa3 = 50,
    Tag_JSLayer_WaiTao3,
    Tag_JSLayer_ShangYi3,
    Tag_JSLayer_KuZi3,
    Tag_JSLayer_WaZi3,
    Tag_JSLayer_XieZi3,
    Tag_JSLayer_ShiPin3,
    Tag_JSLayer_Bao3,
    Tag_JSLayer_ShangCheng = 90,
    Tag_JSLayer_BuLiao = 91
}JSLayer_Enum;

class JiesuanLayer
: public CCLayer
, public FlashNumberLabelDelegate
{
public:
    JiesuanLayer();
    ~JiesuanLayer();
    
    virtual bool init();
    
    static CCScene* scene();
    static JiesuanLayer* create_with_Layer(int selfTempScore, int opponentTempScore, int index);
    void init_with_Layer(int selfTempScore, int opponentTempScore, int index);
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual void will_number_scroll(){};
    virtual void did_number_stoped(){};
    
    CREATE_FUNC(JiesuanLayer);
    
    
public:
    
    int num_child;
    
    int bgIndex;
    int selfScore;
    int opponentScore;
    
    CCDictionary* allClothesDic;
    CompetitionThemeInfo* themeInfo;
    CompetitionItem* selfItem,* opponentItem;
    
    FlashNumberLabel*   scoreLabel1;
    FlashNumberLabel*   scoreLabel2;
    
public:
    
    void creatAnimation();
    void creat_view();
    void creat_jiesuan();
    void creat_nameKuang();
    void creat_lingqu(CCMenuItem* item, int type);
    
    void creat_Man1();
    void creat_Man2();
    void initClothes(CCClippingNode* _ManSpr, float widthFolt, float heightFloat, float scaleFloat, bool flipxBool, CCDictionary* myClothesTemp);
    
    
    void lingquCallback(CCObject* pSender);
    void lingquCallback2(float dt);
};


#endif /* defined(__tiegao__JiesuanLayer__) */
