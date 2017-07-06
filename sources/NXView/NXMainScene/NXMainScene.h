//
//  NXMainScene.h
//  tiegao
//
//  Created by mac on 17-6-29.
//
//

#ifndef __tiegao__NXMainScene__
#define __tiegao__NXMainScene__

#include "cocos2d.h"
#include "NXBaseScene.h"
#include "AHMessageBox.h"

USING_NS_CC;

typedef enum{
    Tag_MS_TouFa = 1,
    Tag_MS_WaiTao,
    Tag_MS_ShangYi,
    Tag_MS_KuZi,
    Tag_MS_WaZi,
    Tag_MS_XieZi,
    Tag_MS_ShiPin,
    Tag_MS_Bao,
    Tag_MS_ZhuangRong,
    Tag_MS_TeXiao,
    Tag_MS_TouFa1 = 30,
    Tag_MS_WaiTao1,
    Tag_MS_ShangYi1,
    Tag_MS_KuZi1,
    Tag_MS_WaZi1,
    Tag_MS_XieZi1,
    Tag_MS_ShiPin1,
    Tag_MS_Bao1,
    Tag_MS_ZhuangRong1,
    Tag_MS_TeXiao1,
    Tag_MS_TouFa2 = 40,
    Tag_MS_WaiTao2,
    Tag_MS_ShangYi2,
    Tag_MS_KuZi2,
    Tag_MS_WaZi2,
    Tag_MS_XieZi2,
    Tag_MS_ShiPin2,
    Tag_MS_Bao2,
    Tag_MS_TouFa3 = 50,
    Tag_MS_WaiTao3,
    Tag_MS_ShangYi3,
    Tag_MS_KuZi3,
    Tag_MS_WaZi3,
    Tag_MS_XieZi3,
    Tag_MS_ShiPin3,
    Tag_MS_Bao3,
    Tag_MS_Bao4 = 60,
    Tag_MS_ShangCheng = 90,
    Tag_MS_BuLiao = 91,
    
    Tag_MS_ShiPin1_1 = 1011,
    Tag_MS_ShiPin1_2,
    Tag_MS_ShiPin1_3,
    Tag_MS_ShiPin1_4,
    Tag_MS_ShiPin1_5,
    Tag_MS_ShiPin1_6,
    Tag_MS_ShiPin1_7,
    Tag_MS_ShiPin1_8,
    Tag_MS_ShiPin1_9,
    Tag_MS_ShiPin2_1 = 2011,
    Tag_MS_ShiPin2_2,
    Tag_MS_ShiPin2_3,
    Tag_MS_ShiPin2_4,
    Tag_MS_ShiPin2_5,
    Tag_MS_ShiPin2_6,
    Tag_MS_ShiPin2_7,
    Tag_MS_ShiPin2_8,
    Tag_MS_ShiPin2_9,
    Tag_MS_ShiPin3_1 = 3011,
    Tag_MS_ShiPin3_2,
    Tag_MS_ShiPin3_3,
    Tag_MS_ShiPin3_4,
    Tag_MS_ShiPin3_5,
    Tag_MS_ShiPin3_6,
    Tag_MS_ShiPin3_7,
    Tag_MS_ShiPin3_8,
    Tag_MS_ShiPin3_9,
}CL_Enum;

class NXMainScene
: public NXBaseScene
, public AHMessageBoxDelegate
{
public:
    NXMainScene();
    ~NXMainScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(NXMainScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    
    
    void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
public:
    
    int num_child;
    int firstX;
    int firstY;
    
    bool leftBool, rightBool;
    
    CCPoint m_touchPoint;
    
    CCMenuItem* xingdongItem, *fanhuiItem;
    CCMenuItem* haoyouItem, *taskItem, *qingjingItem, *shopItem, *pkItem;
    CCSprite* haoyouSpr, *taskSpr, *qingjingSpr, *shopSpr, *pkSpr;
    
    CCSprite* tempSpr;
    CCSprite* huodongKangSpr;
    
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
    
    
public:
    
    void creat_view();
    void creat_Man();
    void initClothes();
    
    void creatHuodong();
    void xingdong_inAction();
    void xingdong_outAction(CCObject* pSender);
    void creatQiu();
    void qiu_inAction();
    void qiu_outAction(CCObject* pSender);
    void openTouch(float dt);
    void randAnimation(float dt);
    void openAnimation(CCSprite* spr, std::string str);
    void closeAnimation();
    void haoyouCallback(CCObject* pSender);
    void pkCallback(CCObject* pSender);
    void qingjingCallback(CCObject* pSender);
    void shopCallback(CCObject* pSender);
    void taskCallback(CCObject* pSender);
    
    
};
#endif /* defined(__tiegao__NXMainScene__) */
