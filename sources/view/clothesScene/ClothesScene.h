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
#include "ClothesTableView.h"
#include "AHMessageBox.h"

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
    Tag_GJ_ZhuangRong,
    Tag_GJ_TeXiao,
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
    Tag_GJ_Bao4 = 60,
    Tag_GJ_ShangCheng = 90,
    Tag_GJ_BuLiao = 91,
    
    Tag_GJ_ShiPin1_1 = 1011,
    Tag_GJ_ShiPin1_2,
    Tag_GJ_ShiPin1_3,
    Tag_GJ_ShiPin1_4,
    Tag_GJ_ShiPin1_5,
    Tag_GJ_ShiPin1_6,
    Tag_GJ_ShiPin1_7,
    Tag_GJ_ShiPin1_8,
    Tag_GJ_ShiPin1_9,
    Tag_GJ_ShiPin2_1 = 2011,
    Tag_GJ_ShiPin2_2,
    Tag_GJ_ShiPin2_3,
    Tag_GJ_ShiPin2_4,
    Tag_GJ_ShiPin2_5,
    Tag_GJ_ShiPin2_6,
    Tag_GJ_ShiPin2_7,
    Tag_GJ_ShiPin2_8,
    Tag_GJ_ShiPin2_9,
    Tag_GJ_ShiPin3_1 = 3011,
    Tag_GJ_ShiPin3_2,
    Tag_GJ_ShiPin3_3,
    Tag_GJ_ShiPin3_4,
    Tag_GJ_ShiPin3_5,
    Tag_GJ_ShiPin3_6,
    Tag_GJ_ShiPin3_7,
    Tag_GJ_ShiPin3_8,
    Tag_GJ_ShiPin3_9,
}GJ_Enum;

class ClothesScene
: public BaseScene
, public AHMessageBoxDelegate
{
public:
    
    ClothesScene();
    ~ClothesScene();
    
    virtual bool init();
    
    static ClothesScene* create_with_type(int _type_id, int _task_index, int _task_phase);
    void init_with_type(int _type_id, int _task_index, int _task_phase);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(ClothesScene);
    
public:
    
    ClothesTableView* _delegate;
    
    bool startTask;
    bool animationBool;
    bool zhuangrongBool;
    
    long  changClothesIndex;
    int buttonTag;
    int isClothesType;
    int clothesStatus;
    int task_index;
    int task_phase;
    int tili_AllIndex;
    int _buttonStatus;
    
    CCSprite* _ManSpr;
    CCSprite* _touSpr;
    CCSprite* bgSpr;
    CCSprite* clothKuangSpr;
    CCSprite* box;
    
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
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
    CCSprite* _bSpr4;
    CCSprite* ycSpr1,* ycSpr2,* ycSpr3,* ycSpr4,* ycSpr5,* ycSpr6,* ycSpr7,* ycSpr8,* ycSpr9,* ycSpr10;
    
    CCLayer* _animLayer;
    
    CCMenu* allMenu;
    
    PromptLayer* promptLayer;
    
public:
    
    void openTouch(float dt);
    void creat_View();
    void creat_money();
    void creat_Man();
    void initClothes();//穿衣服
    void ChangeClothes(CCObject* pSender);
    void ChangClothesIndex(CCObject* pSender);
    void ChangeShipin(int clothesId, int sub_part);
    void reductionShangyi();
    void reductionShipin(int index);
    void buttonStatus();
    void crate_Tishi();
    void create_buySuccess();
    void removeAllSpr();
    
    void setShipinTag1(int index, CCSprite* spr);
    void setShipinTag2(int index, CCSprite* spr);
    void setShipinTag3(int index, CCSprite* spr);
    
    void backCallBack(CCObject* pSender);
    void startCallBack(CCObject* pSender);
    void buttonCallBack(CCObject* pSender);
    void buyCallBack(CCObject* pSender);
    void saveCallBack(CCObject* pSender);
    void saveClothesMethods();
    
    int haveEnoughCoin();
    int haveEnoughGold();
    int updataClothes(int type);
    
    int getTaskId(int index);
    
    void showAnimationWithType(int type);
    
private:
    void Http_Finished_401(CCObject* pObj);
    void Http_Finished_601(CCObject* pObj);
    void Http_Finished_602(CCObject* pObj);
};


#endif /* defined(__tiegao__ClothesScene__) */
