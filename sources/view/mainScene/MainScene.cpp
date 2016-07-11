//
//  MainScene.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "MainScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "QingjingScene.h"
#include "TaskScene.h"
#include "ClothesScene.h"

#include "HaoyouRankLayer.h"
#include "StrangerScene.h"
#include "TotalRankScene.h"
#include "HaoyouScene.h"

#include "Loading2.h"
#include "NetManager.h"
#include "ConfigManager.h"

#include "MailPanel.h"
#include "OperationPanel.h"
#include "SettingPanel.h"

#include <time.h>

#include "SpecialManager.h"

#include "StorySettlementOfTheAnimationLayer.h"

MainScene::MainScene(){
    
}
MainScene::~MainScene(){
    
}

bool MainScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
//    _ManSpr = CCSprite::create();
//    this->addChild(_ManSpr, 10);
//    
//    allClothesDic = CONFIG->clothes();// 所有衣服
//    
    this->creat_view();
//    this->creat_Man();
//    this->initClothes();
//    
//    this->schedule(schedule_selector(MainScene::gengxin), .1f);
    
    this->scheduleUpdate();
    
    time_t t;
    struct tm *p;
    t = 1467241111;
    p = gmtime(&t);
    char s[80];
    strftime(s, 80, "%Y-%m-%d %H:%M:%S", p);
    CCLog("<><><><>time == %d: %s\n", (int)t, s);
    
    return true;
}

CCScene* MainScene::scene(){
    CCScene* scene = CCScene::create();
    MainScene* layer = MainScene::create();
    scene->addChild(layer);
    
    return scene;
}


void MainScene::onEnter(){
    BaseScene::onEnter();
    this->setAccelerometerEnabled(true); // ?
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_huanzhuangCallBack), "HTTP_FINISHED_400", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_500CallBack), "HTTP_FINISHED_500", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::all_mail_callback_700), "HTTP_FINISHED_700", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_600CallBack), "HTTP_FINISHED_600", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::social_info_callback_800), "HTTP_FINISHED_800", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::update_news_status), "UPDATE_NEWS_STATUS", NULL);
    
    this->update_news_status();
}

void MainScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    BaseScene::onExit();
}

void MainScene::keyBackClicked(){
    
}


void MainScene::didAccelerate( CCAcceleration* pAccelerationValue){
    float deceleration = 0 ;
    float sensitivity = 500;
    
    playerVelocity.x = playerVelocity.x * deceleration+ pAccelerationValue->x*sensitivity;
    
    playerVelocity.x = playerVelocity.x/ 10;
}

void MainScene::gengxin(float dt){
    if (roomSpr == NULL) {
        return;
    }
    
    CCPoint pos = roomSpr->getPosition();
    pos.x = DISPLAY->ScreenWidth()* 0.5 + playerVelocity.x;
    if (pos.x < DISPLAY->ScreenWidth()* 0.5 - 70) {
        pos.x = DISPLAY->ScreenWidth()* 0.5 - 70;
        playerVelocity = CCPointZero;
    }
    else if (pos.x > DISPLAY->ScreenWidth()* 0.5 + 70){
        pos.x = DISPLAY->ScreenWidth()* 0.5 + 70;
        playerVelocity = CCPointZero;
    }
    roomSpr->runAction(CCMoveTo::create(0.1f, ccp(pos.x, pos.y)));
}

void MainScene::creat_view(){   
    // 首冲
    CCSprite* scSpr1 = CCSprite::create("res/pic/mainScene/main_chong.png");
    CCSprite* scSpr2 = CCSprite::create("res/pic/mainScene/main_chong.png");
    scSpr2->setScale(1.02f);
    CCMenuItem* shouchongItem = CCMenuItemSprite::create(scSpr1, scSpr2, this, menu_selector(MainScene::shouchongCallBack));
    shouchongItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .63f));

    // 活动
    CCSprite* hdSpr1 = CCSprite::create("res/pic/mainScene/main_huodong.png");
    CCSprite* hdSpr2 = CCSprite::create("res/pic/mainScene/main_huodong.png");
    hdSpr2->setScale(1.02f);
    CCMenuItem* huodongItem = CCMenuItemSprite::create(hdSpr1, hdSpr2, this, menu_selector(MainScene::huodongCallBack));
    huodongItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .54));

    // 签到
    CCSprite* qdSpr1 = CCSprite::create("res/pic/mainScene/main_qiandao.png");
    CCSprite* qdSpr2 = CCSprite::create("res/pic/mainScene/main_qiandao.png");
    qdSpr2->setScale(1.02f);
    CCMenuItem* qiandaoItem = CCMenuItemSprite::create(qdSpr1, qdSpr2, this, menu_selector(MainScene::qiandaoCallBack));
    qiandaoItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .45f));

//    // 邮件
//    CCSprite* yjSpr1 = CCSprite::create("res/pic/mainScene/main_youjian.png");
//    CCSprite* yjSpr2 = CCSprite::create("res/pic/mainScene/main_youjian.png");
//    yjSpr2->setScale(1.02f);
//    _youjianItem = CCMenuItemSprite::create(yjSpr1, yjSpr2, this, menu_selector(MainScene::youjianCallBack));
//    _youjianItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .89f));

    // 好友
    CCSprite* hySpr1 = CCSprite::create("res/pic/mainScene/main_haoyou.png");
    CCSprite* hySpr2 = CCSprite::create("res/pic/mainScene/main_haoyou.png");
    hySpr2->setScale(1.02f);
    _haoyouItem = CCMenuItemSprite::create(hySpr1, hySpr2, this, menu_selector(MainScene::haoyouCallBack));
    _haoyouItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .9f));

    // 换装
    CCSprite* hzSpr1 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    CCSprite* hzSpr2 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    hzSpr2->setScale(1.02f);
    CCMenuItem* huanzhuangItem = CCMenuItemSprite::create(hzSpr1, hzSpr2, this, menu_selector(MainScene::huanzhuangCallBack));
    huanzhuangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .81f));

    // 排行
    CCSprite* phSpr1 = CCSprite::create("res/pic/mainScene/main_paihang.png");
    CCSprite* phSpr2 = CCSprite::create("res/pic/mainScene/main_paihang.png");
    phSpr2->setScale(1.02f);
    CCMenuItem* paihangItem = CCMenuItemSprite::create(phSpr1, phSpr2, this, menu_selector(MainScene::paihangCallBack));
    paihangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .72f));

//    CCMenu* menu = CCMenu::create(shouchongItem, huodongItem, qiandaoItem, _haoyouItem, _youjianItem, huanzhuangItem, paihangItem, NULL);
//    menu->setPosition(CCPointZero);
//    this->addChild(menu);
//
//    // 剧情
//    CCSprite* jqSpr1 = CCSprite::create("res/pic/mainScene/main_juqing.png");
//    CCSprite* jqSpr2 = CCSprite::create("res/pic/mainScene/main_juqing.png");
//    jqSpr2->setScale(1.02f);
//    CCMenuItem* juqingItem = CCMenuItemSprite::create(jqSpr1, jqSpr2, this, menu_selector(MainScene::juqingCallBack));
//    juqingItem->setPosition(ccp(DISPLAY->ScreenWidth()* .91f, DISPLAY->ScreenHeight()* .3f));
//
//    //日常
//    CCSprite* rcSpr1 = CCSprite::create("res/pic/mainScene/main_richang.png");
//    CCSprite* rcSpr2 = CCSprite::create("res/pic/mainScene/main_richang.png");
//    rcSpr2->setScale(1.02f);
//    CCMenuItem* richangItem = CCMenuItemSprite::create(rcSpr1, rcSpr2, this, menu_selector(MainScene::richangCallBack));
//    richangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .91f, DISPLAY->ScreenHeight()* .2f));

    //设置
    CCSprite* szSpr1 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
    CCSprite* szSpr2 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
    szSpr2->setScale(1.02f);
    _shezhiItem = CCMenuItemSprite::create(szSpr1, szSpr2, this, menu_selector(MainScene::shezhiCallBack));
    _shezhiItem->setPosition(ccp(DISPLAY->ScreenWidth()* .09f, DISPLAY->ScreenHeight()* .05f));
//    
//    CCMenu* downMenu = CCMenu::create(juqingItem, richangItem, NULL);
//    downMenu->setPosition(CCPointZero);
//    this->addChild(downMenu);
    
    
    //new-----------------------------------new
    
    //-----6层天空------
    _layer_6 = CCSprite::create("res/pic/mainScene/far.png");
    _layer_6->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_6);
    
    //-----云------
    _cloud_1 = CCSprite::create("res/pic/mainScene/cloud.png");
    _cloud_1->setPosition(ccp(_layer_6->getContentSize().width* .5f, _layer_6->getContentSize().height - _cloud_1->getContentSize().height* .5f + 50));
    _layer_6->addChild(_cloud_1);
    
    _cloud_2 = CCSprite::create("res/pic/mainScene/cloud.png");
    _cloud_2->setPosition(ccp(_cloud_1->getPositionX() + _cloud_2->getContentSize().width, _layer_6->getContentSize().height - _cloud_1->getContentSize().height* .5f + 50));
    _layer_6->addChild(_cloud_2);
    
    //-----鸟------
    _bird_1 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_1->setPosition(ccp(_layer_6->getContentSize().width* .8f, _layer_6->getContentSize().height* .85));
    _layer_6->addChild(_bird_1);
    CCAnimation* animation = CCAnimation::create();
    for (int i = 0; i < 5; i++) {
        CCString* str = CCString::createWithFormat("res/pic/mainScene/bird_%d.png", i + 1);
        animation->addSpriteFrameWithFileName(str->getCString());
    }
    animation->setDelayPerUnit(2.8f/14.0f);
    animation->setRestoreOriginalFrame(true);
    CCAnimate* ac1 = CCAnimate::create(animation);
    CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(ac1, ac1->reverse(), NULL));
    _bird_1->runAction(rep);
    
    //-----5层河流------
    _river_1 = CCSprite::create("res/pic/mainScene/river.png");
    _river_1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_river_1);
    
    _river_2 = CCSprite::create("res/pic/mainScene/river.png");
    _river_2->setPosition(ccp(_river_1->getPositionX() + _river_2->getContentSize().width - 5, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_river_2);
    
    //-----4层背景-----
    _layer_4 = CCSprite::create("res/pic/mainScene/four_bg.png");
    _layer_4->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f - 1));
    this->addChild(_layer_4);
    
    //---塔(排行榜)---
    CCSprite* tower_spr1 = CCSprite::create("res/pic/mainScene/paihang.png");
    CCSprite* tower_spr2 = CCSprite::create("res/pic/mainScene/paihang.png");
    tower_spr2->setScale(1.02f);
    CCMenuItem* paihang_Item = CCMenuItemSprite::create(tower_spr1, tower_spr2, this, menu_selector(MainScene::paihangCallBack));
    paihang_Item->setPosition(ccp(_layer_4->getContentSize().width* .6f, _layer_4->getContentSize().height* .78f));
    CCSprite* paihang_bar = CCSprite::create("res/pic/mainScene/paihang_bar.png");
    paihang_bar->setPosition(ccp(paihang_Item->getContentSize().width* .5f, paihang_Item->getContentSize().height* .5f));
    paihang_Item->addChild(paihang_bar);
    CCMenu* menu_paihang = CCMenu::create(paihang_Item, NULL);
    menu_paihang->setPosition(CCPointZero);
    _layer_4->addChild(menu_paihang);
    
    //---活动---
    CCSprite* hd_Spr1 = CCSprite::create("res/pic/mainScene/huodong.png");
    CCSprite* hd_Spr2 = CCSprite::create("res/pic/mainScene/huodong.png");
    hd_Spr2->setScale(1.02f);
    CCMenuItem* huodong_Item = CCMenuItemSprite::create(hd_Spr1, hd_Spr2, this, menu_selector(MainScene::huodongCallBack));
    huodong_Item->setPosition(ccp(_layer_4->getContentSize().width* .35f, _layer_4->getContentSize().height* .89f));
    CCSprite* huodong_bar = CCSprite::create("res/pic/mainScene/huodong_bar.png");
    huodong_bar->setPosition(ccp(huodong_Item->getContentSize().width* .5f, huodong_bar->getContentSize().height* .4f));
    huodong_Item->addChild(huodong_bar);
    CCMenu* menu_huodong = CCMenu::create(huodong_Item, NULL);
    menu_huodong->setPosition(CCPointZero);
    _layer_4->addChild(menu_huodong);
        
    //-----3层背景------
    _layer_3 = CCSprite::create("res/pic/mainScene/three_bg.png");
    _layer_3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_3);
    
    //---日常btn(公司)---
    CCSprite* rc_Spr1 = CCSprite::create("res/pic/taskScene/task_building_3.png");
    rc_Spr1->setScale(0.550f);
    rc_Spr1->setContentSize(rc_Spr1->getContentSize()* .550f);
    CCSprite* rc_Spr2 = CCSprite::create("res/pic/taskScene/task_building_3.png");
    rc_Spr2->setScale(0.56);
    rc_Spr2->setContentSize(rc_Spr1->getContentSize()* .550f);
    CCMenuItem* richang_Item = CCMenuItemSprite::create(rc_Spr1, rc_Spr2, this, menu_selector(MainScene::richangCallBack));
    richang_Item->setPosition(ccp(richang_Item->getContentSize().width* .55f, _layer_3->getContentSize().height* .65f));
    CCSprite* company_bar = CCSprite::create("res/pic/mainScene/company_bar.png");
    company_bar->setPosition(ccp(richang_Item->getContentSize().width* .585f, richang_Item->getContentSize().height* .28f));
    richang_Item->addChild(company_bar);
    
    
    //---社交btn(咖啡厅)---
    CCSprite* hy_Spr1 = CCSprite::create("res/pic/mainScene/coffee_nor.png");
    CCSprite* hy_Spr2 = CCSprite::create("res/pic/mainScene/coffee_sel.png");
    CCMenuItemSprite* haoyou_Item = CCMenuItemSprite::create(hy_Spr1, hy_Spr2, this, menu_selector(MainScene::haoyouCallBack));
    haoyou_Item->setPosition(ccp(_layer_3->getContentSize().width* .66f, _layer_3->getContentSize().height* .556f));
    CCSprite* coffee_bar = CCSprite::create("res/pic/mainScene/coffee_bar.png");
    coffee_bar->setPosition(ccp(haoyou_Item->getContentSize().width* .38f, haoyou_Item->getContentSize().height* .67));
    haoyou_Item->addChild(coffee_bar);
    CCMenu* menu_three = CCMenu::create(richang_Item, haoyou_Item, NULL);
    menu_three->setPosition(CCPointZero);
    _layer_3->addChild(menu_three);
    
    //---路灯---
//    CCSprite* ludeng_spr = CCSprite::create("res/pic/mainScene/coffee_nor.png");
//    ludeng_spr->setPosition(ccp(_layer_3->getContentSize().width* .45f, _layer_3->getContentSize().height* .25f));
//    _layer_3->addChild(ludeng_spr);
    
    
    //-----2层花园背景-----
    _layer_2 = CCSprite::create("res/pic/mainScene/home_bg.png");
    _layer_2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f - 2));
    this->addChild(_layer_2);
    
    //---邮箱btn---
    CCSprite* mail_box1 = CCSprite::create("res/pic/mainScene/mail_nor.png");
    CCSprite* mail_box2 = CCSprite::create("res/pic/mainScene/mail_sel.png");
    mail_box2->setScale(1.02f);
    CCMenuItemSprite* youjian_Item = CCMenuItemSprite::create(mail_box1, mail_box2, this, menu_selector(MainScene::youjianCallBack));
    youjian_Item->setPosition(ccp(_layer_2->getContentSize().width* .36f, _layer_2->getContentSize().height* .458f));
    CCSprite* xinfeng_spr = CCSprite::create("res/pic/mainScene/xinfeng.png");
    xinfeng_spr->setPosition(ccp(youjian_Item->getContentSize().width* .6f, youjian_Item->getContentSize().height + 20));
    youjian_Item->addChild(xinfeng_spr);
    
    //---家btn---
    CCSprite* home_spr1 = CCSprite::create("res/pic/mainScene/home_nor.png");
    CCSprite* home_spr2 = CCSprite::create("res/pic/mainScene/home_sel.png");
    home_spr2->setScale(1.02f);
    CCMenuItem* home_item = CCMenuItemSprite::create(home_spr1, home_spr2, this, menu_selector(MainScene::homeCallBack));
    home_item->setPosition(ccp(_layer_2->getContentSize().width* .298f, _layer_2->getContentSize().height* .310f));
    CCSprite* home_bar = CCSprite::create("res/pic/mainScene/home_bar.png");
    home_bar->setPosition(ccp(home_item->getContentSize().width* .44f, home_item->getContentSize().height* .395f));
    home_item->addChild(home_bar);
    
    //---换装btn---
    CCSprite* hz_Spr1 = CCSprite::create("res/pic/mainScene/shop_nor.png");
    CCSprite* hz_Spr2 = CCSprite::create("res/pic/mainScene/shop_sel.png");
    hz_Spr2->setScale(1.02f);
    CCMenuItem* huanzhuang_Item = CCMenuItemSprite::create(hz_Spr1, hz_Spr2, this, menu_selector(MainScene::huanzhuangCallBack));
    huanzhuang_Item->setPosition(ccp(_layer_2->getContentSize().width* .68f, _layer_2->getContentSize().height* .335f));
    CCSprite* shop_bar = CCSprite::create("res/pic/mainScene/shop_bar.png");
    shop_bar->setPosition(ccp(huanzhuang_Item->getContentSize().width* .29f, huanzhuang_Item->getContentSize().height* .35f));
    huanzhuang_Item->addChild(shop_bar);
    CCMenu* menu_3 = CCMenu::create(home_item, huanzhuang_Item, youjian_Item, NULL);
    menu_3->setPosition(CCPointZero);
    _layer_2->addChild(menu_3);
    
    //-----1层路------
    _layer_1 = CCSprite::create("res/pic/mainScene/road.png");
    _layer_1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_1);
    
    //---剧情btn---
    CCSprite* jq_Car1 = CCSprite::create("res/pic/mainScene/car_juqing_1_nor.png");
    CCSprite* jq_Car2 = CCSprite::create("res/pic/mainScene/car_juqing_1_sel.png");
    jq_Car2->setScale(1.02f);
    CCMenuItem* juqing_Item = CCMenuItemSprite::create(jq_Car1, jq_Car2, this, menu_selector(MainScene::juqingCallBack));
    CCSprite* juqing_bar = CCSprite::create("res/pic/mainScene/juqing_bar.png");
    juqing_bar->setPosition(ccp(juqing_Item->getContentSize().width* .76f, juqing_Item->getContentSize().height* .27f));
    juqing_Item->addChild(juqing_bar);
    //    juqing_Item->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .12f));
    CCMenu* menu_car1 = CCMenu::create(juqing_Item, NULL);
    menu_car1->setPosition(ccp(_layer_1->getContentSize().width* .65f, _layer_1->getContentSize().height* .13f));
    _layer_1->addChild(menu_car1);
    
    //---vip剧情btn---
    CCSprite* jq2_Car1 = CCSprite::create("res/pic/mainScene/car_juqing_2_nor.png");
    CCSprite* jq2_Car2 = CCSprite::create("res/pic/mainScene/car_juqing_2_sel.png");
    jq2_Car2->setScale(1.02f);
    CCMenuItem* juqing2_Item = CCMenuItemSprite::create(jq2_Car1, jq2_Car2, this, menu_selector(MainScene::juqing_vipCallBack));
    CCSprite* unknow_bar = CCSprite::create("res/pic/mainScene/unknow_bar.png");
    unknow_bar->setPosition(ccp(juqing2_Item->getContentSize().width* .75f, juqing2_Item->getContentSize().height* .23f));
    juqing2_Item->addChild(unknow_bar);
    //    juqing_Item->setPosition(ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .24f));
    CCMenu* menu_car2 = CCMenu::create(juqing2_Item, NULL);
    menu_car2->setPosition(ccp(_layer_1->getContentSize().width* .35f, _layer_1->getContentSize().height* .15f));
    _layer_1->addChild(menu_car2);
    
    //-----花------
    _layer_0 = CCSprite::create("res/pic/mainScene/near.png");
    _layer_0->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_0);
    
    
    CCMenu* menu = CCMenu::create(shouchongItem, huodongItem, qiandaoItem, _haoyouItem, huanzhuangItem, paihangItem,_shezhiItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
}

bool MainScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
    last_pos = pTouch->getLocation();
    
    return true;
}

void MainScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    cur_pos= pTouch->getLocation();
    offset_x = cur_pos.x - last_pos.x;
    last_pos = cur_pos;
    
    if ((_layer_0->getPositionX() + offset_x >= _layer_0->getContentSize().width* .5f - 10 && offset_x > 0) ||
        (_layer_0->getPositionX() + offset_x <= DISPLAY->ScreenWidth() - _layer_0->getContentSize().width* .5f + 10 && offset_x < 0)) {

    }else{
        _layer_0->setPositionX(_layer_0->getPositionX() + offset_x);
        _layer_1->setPositionX(_layer_1->getPositionX() + offset_x);
        _layer_2->setPositionX(_layer_2->getPositionX() + offset_x* .8f);
        _layer_3->setPositionX(_layer_3->getPositionX() + offset_x* .6f);
        _layer_4->setPositionX(_layer_4->getPositionX() + offset_x* .4f);
        _river_1->setPositionX(_river_1->getPositionX() + offset_x* .3f);
        _river_2->setPositionX(_river_2->getPositionX() + offset_x* .3f);
        _layer_6->setPositionX(_layer_6->getPositionX() + offset_x* .2f);
    }
//    this->startAnimation();
}

void MainScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

void MainScene::startAnimation(){
    if ((_layer_0->getPositionX() + offset_x >= _layer_0->getContentSize().width* .5f - 10 && offset_x > 0) ||
        (_layer_0->getPositionX() + offset_x <= DISPLAY->ScreenWidth() - _layer_0->getContentSize().width* .5f + 10 && offset_x < 0)) {
        
    }else{
        CCMoveBy* ac0 = CCMoveBy::create(0.01, CCPoint(offset_x, 0));
        CCMoveBy* ac1 = CCMoveBy::create(0.01, CCPoint(offset_x* .98f, 0));
        CCMoveBy* ac2 = CCMoveBy::create(0.01, CCPoint(offset_x* .88f, 0));
        CCMoveBy* ac3 = CCMoveBy::create(0.01, CCPoint(offset_x* .68f, 0));
        CCMoveBy* ac4 = CCMoveBy::create(0.01, CCPoint(offset_x* .48f, 0));
        CCMoveBy* ac5_1 = CCMoveBy::create(0.01, CCPoint(offset_x* .38f, 0));
        CCMoveBy* ac5_2 = CCMoveBy::create(0.01, CCPoint(offset_x* .38f, 0));
        CCMoveBy* ac6 = CCMoveBy::create(0.01, CCPoint(offset_x* .2f, 0));
        
        _layer_0->stopAllActions();
        _layer_1->stopAllActions();
        _layer_2->stopAllActions();
        _layer_3->stopAllActions();
        _layer_4->stopAllActions();
        _river_1->stopAllActions();
        _river_2->stopAllActions();
        _layer_6->stopAllActions();
        
        _layer_0->runAction(ac0);
        _layer_1->runAction(ac1);
        _layer_2->runAction(ac2);
        _layer_3->runAction(ac3);
        _layer_4->runAction(ac4);
        _river_1->runAction(ac5_1);
        _river_2->runAction(ac5_2);
        _layer_6->runAction(ac6);
    }
    
}

void MainScene::update(float dt){
    //云
    if (_cloud_1->getPositionX() <= -_cloud_1->getContentSize().width* .5f) {
        _cloud_1->setPositionX(_cloud_2->getPositionX() + _cloud_1->getContentSize().width);
    }else{
        _cloud_1->setPositionX(_cloud_1->getPositionX() - 0.2);
    }
    
    if (_cloud_2->getPositionX() <= -_cloud_2->getContentSize().width* .5f) {
        _cloud_2->setPositionX(_cloud_1->getPositionX() + _cloud_1->getContentSize().width);
    }else{
        _cloud_2->setPositionX(_cloud_2->getPositionX() - 0.2);
    }
    
    //河流
    if (_river_1->getPositionX() <= -_river_1->getContentSize().width* .5f) {
        _river_1->setPositionX(_river_2->getPositionX() + _river_1->getContentSize().width* .5f + _river_2->getContentSize().width* .5f - 5);
    }else{
        _river_1->setPositionX(_river_1->getPositionX() - 2);
    }
    
    if (_river_2->getPositionX() <= -_river_2->getContentSize().width* .5f) {
        _river_2->setPositionX(_river_1->getPositionX() + _river_1->getContentSize().width* .5f + _river_2->getContentSize().width* .5f - 5);
    }else{
        _river_2->setPositionX(_river_2->getPositionX() - 2);
    }
    
    if (_bird_1->getPositionX() <= -100) {
        _bird_1->setPositionX(_layer_1->getContentSize().width + 100);
    }else{
        _bird_1->setPositionX(_bird_1->getPositionX() - 4);
    }
}

void MainScene::resetBirdPosition(){
    this->setPositionX(DISPLAY->ScreenWidth() + 100);
}

void MainScene::juqing_vipCallBack(CCObject* pSender){
    
}

void MainScene::homeCallBack(CCObject *pSender){
    
}

void MainScene::shouchongCallBack(CCObject* pSender){
    SPECIAL->showStar2At(getParent(), DISPLAY->center(), 1);
}

void MainScene::huodongCallBack(CCObject* pSender){
    OperationPanel* panel = OperationPanel::create();
    panel->show_from(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .85f));
}

void MainScene::qiandaoCallBack(CCObject* pSender){
//    SPECIAL->showPetal2At(this->getScene(), DISPLAY->center(), 1);
    
//    StorySettlementOfTheAnimationLayer* layer = StorySettlementOfTheAnimationLayer::create_with_index(1);
//    this->addChild(layer, 1000);
    CONFIG->getMissionDialog(1, 2);
}

void MainScene::youjianCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->all_mails_700();
}

void MainScene::haoyouCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->social_info_800();
}

void MainScene::social_info_callback_800(CCObject* pObj) {
    LOADING->remove();
    CCScene* scene = HaoyouScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::renwuCallBack(CCObject* pSender){
    
}

void MainScene::huanzhuangCallBack(CCObject* pSender){
    if (DATA->getClothes()->has_init_clothes == true) {
        this->_huanzhuangCallBack(pSender);
    }
    else {
        LOADING->show_loading();
        NET->owned_clothes_400();
    }
}
void MainScene::_huanzhuangCallBack(CCObject* pSender){
    
    CCLayer* layer = ClothesScene::create_with_type(2, 0, 0);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void MainScene::paihangCallBack(CCObject* pSender){
    CCLayer* layer = TotalRankScene::create_with_type(1);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
//    CCScene* scene = TotalRankScene::scene(1);
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void MainScene::juqingCallBack(CCObject* pSender){
    if (DATA->getStory()->has_init_story()) {
        this->_500CallBack(NULL);
    }else{
        LOADING->show_loading();
        NET->completed_story_500();
    }
}
void MainScene::_500CallBack(CCObject* pSender){
    CCScene* scene = QingjingScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void MainScene::richangCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->completed_mission_600();
}
void MainScene::_600CallBack(CCObject* pSender){
    LOADING->remove();
    
    CCScene* scene = TaskScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::shezhiCallBack(CCObject* pSender){
    SettingPanel* panel = SettingPanel::create();
    panel->show_from(_shezhiItem->getPosition());
}

void MainScene::creat_Man(){
    float widthFolt = .65f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void MainScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = .65f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    
    for (int i = Tag_Rank_TouFa; i <= Tag_Rank_ZhuangRong; i++) {
        if (i == Tag_Rank_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_Rank_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_Rank_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_Rank_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_Rank_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_Rank_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_Rank_WaiTao1);
                _ManSpr->addChild(_wtSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_Rank_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_Rank_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_Rank_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_Rank_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_Rank_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_Rank_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_Rank_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setScale(scaleFloat);
                _kzSpr1->setFlipX(flipxBool);
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _kzSpr1->setTag(Tag_Rank_KuZi1);
                _ManSpr->addChild(_kzSpr1, 290);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_Rank_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_Rank_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_Rank_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_Rank_WaZi1);
                _ManSpr->addChild(_wzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr1->setTag(Tag_Rank_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_Rank_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_Rank_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_Rank_XieZi1);
                _ManSpr->addChild(_xzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_Rank_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_Rank_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_Rank_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr->addChild(_spSpr1, 50);
                }else{
                    CCDictionary* dic = CONFIG->clothes();// 所有衣服
                    CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                    for (int k = 0; k < clothesArr->count(); k++) {
                        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(k);
                        int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                        if (now_clothes_Id == cloth_id->getValue()) {
                            const CCString* layer1 =  clothDic->valueForKey("layer1");
                            const CCString* layer2 =  clothDic->valueForKey("layer2");
                            const CCString* layer3 =  clothDic->valueForKey("layer3");
                            if (layer1->compare("") != 0) {
                                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_Rank_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_Rank_Bao1);
                _ManSpr->addChild(_bSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_Rank_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_Rank_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_Rank_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_GJ_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_GJ_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _zrSpr1->setTag(Tag_GJ_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}
        
void MainScene::all_mail_callback_700(cocos2d::CCObject *pObj) {
    LOADING->remove();
    MailPanel* panel = MailPanel::create();
    panel->show_from(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .85f));
}

void MainScene::update_news_status() {
#warning "需要更改"
    NewsComp* news = DATA->getNews();
    if (news->mail > 0) {
        CCSprite* spt = CCSprite::create("pic/new.png");
        spt->setPosition(ccp(80, 80));
        _youjianItem->addChild(spt);
    }
    
    if (news->paper + news->message > 0) {
        CCSprite* spt = CCSprite::create("pic/new.png");
        spt->setPosition(ccp(80, 80));
        _haoyouItem->addChild(spt);
    }
}


