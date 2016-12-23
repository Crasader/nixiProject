//
//  MainScene.h
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#ifndef __tiegao__MainScene__
#define __tiegao__MainScene__

#include "cocos2d.h"
#include "BaseScene.h"
#include "AHMessageBox.h"

USING_NS_CC;

class MainScene
: public BaseScene
, public AHMessageBoxDelegate
{
public:
    MainScene();
    ~MainScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(MainScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    void didAccelerate( CCAcceleration* pAccelerationValue);
    void gengxin(float dt);
    
    void update_lighting_target(float dt);
    
    void creat_guideBool();
    
//    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    CCPoint cur_pos;
//    CCPoint last_pos;
    float move_x;
    float offset_x;
    bool isMenu;
    bool isMoving;
    bool isEffective;
    bool isOk;
    bool isCanMove;
    bool isrenwuBool;
    bool ishomeBool;
    
public:
    
    void creat_view();
    void creat_Man();
    void initClothes();
    
    CCSprite* _layer_6;
    CCSprite* _river_1;
    CCSprite* _river_2;
    CCSprite* _cloud_1;
    CCSprite* _cloud_2;
    CCSprite* _layer_4;
    CCSprite* _layer_3;
    CCSprite* _layer_2;
    CCSprite* _layer_1;
    CCSprite* _layer_0;
    
    CCSprite* _bird_1;
    CCSprite* _bird_2;
    CCSprite* _bird_3;
    CCSprite* _bird_4;
    CCSprite* _bird_5;
    CCSprite* _bird_6;
    CCSprite* _bird_7;
    
    CCSprite*  tree;
    float    opacity;
    float      sp;
    
    CCSprite* car_1;
    CCSprite* car_2;
    CCSprite* car_3;
    
    CCMenu* menu_paihang;
    CCMenu* menu_huodong;
    CCMenu* menu_richang;
    CCMenu* menu_haoyou;
    CCMenu* menu_mail;
    CCMenu* menu_home;
    CCMenu* menu_hz;
    CCMenu* menu_car1;
    CCMenu* menu_car2;
    
    CCMenuItem* home_item;
    CCMenuItemSprite* huanzhuang_Item;
    
    CCSprite* xinfeng_spr1;
    
    CCSprite* shop_bar1;
    CCSprite* juqing_bar1;
    CCSprite* company_bar1;

    CCSprite* coffee_bar1;
    CCSprite* huodong_bar1;
    CCSprite* paihang_bar1;
    CCSprite* home_bar1;
    CCSprite* unknow_bar1;
    
    CCMenuItem* haoyou_Item;
//    CCMenuItem* item_chat;
    
    CCArray* target_arr;
    
    void check_begin_position(CCObject* pos);
    void change_position(CCObject* pObj);
    void updateLayerPosition(float offset_x);
    void setIsEffective();
    CCActionInterval* getIntervalAction();
    
    
    void huodongCallBack(CCObject* pSender);
    void qiandaoCallBack(CCObject* pSender);
    void youjianCallBack(CCObject* pSender);
    void renwuCallBack(CCObject* pSender);
    void huanzhuangCallBack(CCObject* pSender);
    void _huanzhuangCallBack(CCObject* pSender);
    void paihangCallBack(CCObject* pSender);
    void juqingCallBack(CCObject* pSender);
    void richangCallBack(CCObject* pSender);
    void richangMethods();
    void shezhiCallBack(CCObject* pSender);
//    void shouchongCallBack(CCObject* pSender);
    void haoyouCallBack(CCObject* pSender);
    void energyLargessCallBack(CCObject* pSender);
    void purchaseAchievementCallBack(CCObject* pSender);
    void gashaponCallBack(CCObject* pSender);
    void onEventCallback(CCObject* pSender);
    void mysteryUnlockPrompt(CCObject* pSender);
    void welfareCallBack(CCObject* pSender);
    
    void lingdang_callback(CCObject* pSender);
    void blankCallback();
    
    void juqing_vipCallBack(CCObject* pSender);
    void homeCallBack(CCObject* pSender);
    void update(float dt);
    
    void _500CallBack(CCObject* pSender);
    void _504CallBack(CCObject* pSender);
    void _600CallBack(CCObject* pSender);
    void _704CallBack(CCObject* pSender);
    void all_mail_callback_700(CCObject* pObj);
    void social_info_callback_800(CCObject* pObj);
    void all_friends_callback_806(CCObject* pObj);
    void rankList_callback_300(CCObject* pObj);
    void nc_signin_info_302(CCObject* pObj);
    void nc_gashapon_info_306(CCObject *pObj);
    void nc_recharge_info_304(CCObject *pObj);
    void nc_take_gift_333(CCObject *pObj);
    
    void nc_fetch_mystery_info_610(CCObject* pObj);
    
    void show_guoqing_signin();
    void nc_temp_signin_info_340(CCObject* pObj);
    
    void _905CallBack(CCObject *pObj);
    void isTxt_Bar();
    
    void creat_Exchange();
    void linshiMethod(CCObject *pObj);
    
    void setStartGameData(float dt);
    
public:
    CCDictionary* allClothesDic;
    
    int num_child;
        
    CCSprite* roomSpr;
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
    
    CCPoint playerVelocity;
    
private:
    // 约会 [[
    void showTrystEntrance();
    void onBtnStartTryst();
    void after_fetch_tryst_info_620();
    void after_start_tryst_621();
    void check_tryst_progress();
    // ]]
    
    // 各种检查 [[
    void update_news_status();
    void check_dailysignin();
    void update_dailysignin();
    void check_free_gashapon();
//    void check_first_on();
    CCArray* rand_array(CCArray* arr);
    void play(CCSprite* spt);
    void delayPlay(float dt);
    void checkVersion();
    void check_welfare_new();
    // ]]
private:
    CCMenuItem*         _youjianItem;
    CCMenuItem*         _haoyouItem;
    CCMenuItem*         _shezhiItem;
    CCMenuItem*         _qiandaoItem;
    CCMenuItem*         _welfareItem;
    
    CCMenuItem*         _btnGashapon;
    CCMenuItem*         _btnEnergyLargess;
    
    CCClippingNode*     node;
    CCMenu*             menu;
    bool                isOpen;
    
    CCArray*            _arrGroup1;
    CCArray*            _arrGroup2;
    CC_SYNTHESIZE_RETAIN(CCArray*, _arrPlay, ArrPlay);
};

#endif /* defined(__tiegao__MainScene__) */
