//
//  NXClothesScene.h
//  tiegao
//
//  Created by mac on 17-7-4.
//
//

#ifndef __tiegao__NXClothesScene__
#define __tiegao__NXClothesScene__

#include <iostream>
#include "cocos2d.h"
#include "NXBaseScene.h"
#include "NXClothesTableView.h"
#include "AHMessageBox.h"


USING_NS_CC;

#define def_z_order 50

typedef enum{
    Tag_NXCL_TouFa = 1,
    Tag_NXCL_WaiTao,
    Tag_NXCL_ShangYi,
    Tag_NXCL_KuZi,
    Tag_NXCL_TeXiao,
    Tag_NXCL_XieZi,
    Tag_NXCL_ShiPin,
    Tag_NXCL_Bao,
    Tag_NXCL_ZhuangRong,
    
    Tag_NXCL_TouFa1 = 30,
    Tag_NXCL_WaiTao1,
    Tag_NXCL_ShangYi1,
    Tag_NXCL_KuZi1,
    Tag_NXCL_TeXiao1,
    Tag_NXCL_XieZi1,
    Tag_NXCL_ShiPin1,
    Tag_NXCL_Bao1,
    Tag_NXCL_ZhuangRong1,
    
    Tag_NXCL_TouFa2 = 40,
    Tag_NXCL_WaiTao2,
    Tag_NXCL_ShangYi2,
    Tag_NXCL_KuZi2,
    Tag_NXCL_TeXiao2,
    Tag_NXCL_XieZi2,
    Tag_NXCL_ShiPin2,
    Tag_NXCL_Bao2,
    
    Tag_NXCL_TouFa3 = 50,
    Tag_NXCL_WaiTao3,
    Tag_NXCL_ShangYi3,
    Tag_NXCL_KuZi3,
    Tag_NXCL_TeXiao3,
    Tag_NXCL_XieZi3,
    Tag_NXCL_ShiPin3,
    Tag_NXCL_Bao3,
    Tag_NXCL_Bao4 = 60,
    Tag_NXCL_ShangCheng = 90,
    Tag_NXCL_BuLiao = 91,
    
    Tag_NXCL_ShiPin1_1 = 1011,
    Tag_NXCL_ShiPin1_2,
    Tag_NXCL_ShiPin1_3,
    Tag_NXCL_ShiPin1_4,
    Tag_NXCL_ShiPin1_5,
    Tag_NXCL_ShiPin1_6,
    Tag_NXCL_ShiPin1_7,
    Tag_NXCL_ShiPin1_8,
    Tag_NXCL_ShiPin1_9,
    Tag_NXCL_ShiPin2_1 = 2011,
    Tag_NXCL_ShiPin2_2,
    Tag_NXCL_ShiPin2_3,
    Tag_NXCL_ShiPin2_4,
    Tag_NXCL_ShiPin2_5,
    Tag_NXCL_ShiPin2_6,
    Tag_NXCL_ShiPin2_7,
    Tag_NXCL_ShiPin2_8,
    Tag_NXCL_ShiPin2_9,
    Tag_NXCL_ShiPin3_1 = 3011,
    Tag_NXCL_ShiPin3_2,
    Tag_NXCL_ShiPin3_3,
    Tag_NXCL_ShiPin3_4,
    Tag_NXCL_ShiPin3_5,
    Tag_NXCL_ShiPin3_6,
    Tag_NXCL_ShiPin3_7,
    Tag_NXCL_ShiPin3_8,
    Tag_NXCL_ShiPin3_9,
}NXCL_Enum;

class NXClothesScene
: public NXBaseScene
, public AHMessageBoxDelegate
{
public:
    
    NXClothesScene();
    ~NXClothesScene();
    
    virtual bool init();
    // type_id = clothesStatus - 1:换装任务, 2:纯换装, 3:神秘事件, 4:约会, 5:比拼
    // 公司任务和换装用
    static NXClothesScene* create_with_type(int _type_id, int _task_index, int _task_phase);
    void init_with_type(int _type_id, int _task_index, int _task_phase);
    // 神秘事件专用
    static NXClothesScene* create_with_mystery(int _type_id, const char* category, const char* tishi);
    void init_with_mystery(int _type_id, const char* category, const char* tishi);
    // 约会专用
    static NXClothesScene* create_with_tryst(const char *tishi);
    void init_with_tryst(const char *tishi);
    
    virtual void onEnter();
    virtual void onExitTransitionDidStart();
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(NXClothesScene);
    
public:
    NXClothesTableView* _delegate;
    
    // 神秘任务用
    const char* category;
    const char* tishi;
    
    bool pkTuoguangBool;
    bool startTask;
    bool animationBool;
    bool zhuangrongBool;
    bool renwukuangMethodsBool;
    bool kuziBool;
    bool _manBool;
    bool grayBool;
    
    long  changClothesIndex;
    int buttonTag;
    int isClothesType;
    int clothesStatus;
    int task_index;
    int task_phase;
    int tili_AllIndex;
    int _buttonStatus;
    int tag1;
    int tag2;
    int tag3;
    
    CCSprite* shaixuanSpr;
    CCSprite* yishaixuanSpr;
    CCSprite* tskSpr;
    
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
    CCSprite* _txSpr1; // 特效
    CCSprite* _xzSpr1; // 鞋子
    CCSprite* _xzSpr2;
    CCSprite* _xzSpr3;
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
    CCSprite* _bSpr4;
    CCSprite* _zrSpr1; // 妆容
//    CCSprite* ycSpr1,* ycSpr2,* ycSpr3,* ycSpr4,* ycSpr5,* ycSpr6,* ycSpr7,* ycSpr8,* ycSpr9,* ycSpr10;
    
    CCSprite* tishiSpr;
    
    CCLayer* _animLayer;
    
    CCMenu* buttonMenu;
    CCMenu* shareMenu;
    void openButtonMenu();
    CCMenuItem *startItem, *shopItem, *shareItem, *saveItem;
    
public:
    
    void updataSaveItemStatus();
    void openTouch(float dt);
    void creat_View();
    void creat_ViewMethods(int index);
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
    void creatTuoguang(CCMenuItem* item);
    void tuoguangCallback(CCObject* pSender);
    
    CCSprite* creat_Gray(int type, int index, CCString* spr);
    
    void setShipinTag1(int index, CCSprite* spr);
    void setShipinTag2(int index, CCSprite* spr);
    void setShipinTag3(int index, CCSprite* spr);
    
    void backCallBack(CCObject* pSender);
    void startCallBack(CCObject* pSender);
    void startMethods();
    void buttonCallBack(CCObject* pSender);
    void guideButtonCallBack();
    void buyCallBack(CCObject* pSender);
    void saveCallBack(CCObject* pSender);
    void shopCallback(CCObject* pSender);
    void renwukuangCallBack(CCObject* pSender);
    void renwukuangMethods(int index);
    void saveClothesMethods();
    void shareCallBack(CCObject* pSender);
    void shareStatus(float dt);
    void iOS_share_finish(CCObject* pSender);
    void _321CallBack(CCObject* pSender);
    
    void randomHint(float dt);
    
    int haveEnoughCoin();
    int haveEnoughGold();
    int haveEnoughDebris();
    int updataClothes(int type);
    
    int getTaskId(int index);
    
    void showAnimationWithType(int type);
    
    void go_fitting_room();
    
    void tuoguangConfirmCallBack(CCObject* pSender);
    void tuoguangCancelCallBack(CCObject* pSender);
    bool isThereAClothes();
    
private:
    void Http_Finished_401(CCObject* pObj);
//    void Http_Finished_403(CCObject* pObj);

//    void Http_Finished_601(CCObject* pObj);
    void Http_Finished_603(CCObject* pObj);
    void _605CallBack(CCObject* pObj);
    void _905CallBack(CCObject* pObj);
    void after_commit_mystery_613(CCObject* pObj);
    void after_start_tryst_task_623(CCObject* pObj);
    
    void removeAnimation();
    
    int num_child;
    std::string buyClothesStr;
    void buyClothesMethods();
};
#endif /* defined(__tiegao__NXNXClothesScene__) */
