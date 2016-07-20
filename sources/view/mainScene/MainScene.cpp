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
#include "AudioManager.h"

//#include "HaoyouRankLayer.h"
#include "Shower.h"

#include "StrangerScene.h"
#include "TotalRankScene.h"
#include "HaoyouScene.h"

#include "DragLayer.h"
#include "PromptLayer.h"

#include "Loading2.h"
#include "NetManager.h"
#include "ConfigManager.h"

#include "MailPanel.h"
#include "OperationPanel.h"
#include "SettingPanel.h"

#include <time.h>

// --------------- test ----------------

MainScene::MainScene(){
    
}
MainScene::~MainScene(){
    CC_SAFE_DELETE(_arrGroup1);
    CC_SAFE_DELETE(_arrGroup2);
    CC_SAFE_DELETE(_arrPlay);
}

bool MainScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    _arrGroup1 = NULL;
    _arrGroup2 = NULL;
    _arrPlay = NULL;
    isrenwuBool = false;
    
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
    
    isEffective = true;
    move_x = 0;
    
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
    
    DragLayer* drag = DragLayer::create();
    
    scene->addChild(layer);
    scene->addChild(drag);
    
    return scene;
}

void MainScene::onEnter(){
    BaseScene::onEnter();
    this->setAccelerometerEnabled(true); // ?
    CCString* strBGM = AUDIO->getCurBGM();
    if (strBGM && strBGM->compare("MAIN") == 0) {
        
    }
    else {
        AUDIO->play_main_bgm();
    }
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_huanzhuangCallBack), "HTTP_FINISHED_400", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_500CallBack), "HTTP_FINISHED_500", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::all_mail_callback_700), "HTTP_FINISHED_700", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_600CallBack), "HTTP_FINISHED_600", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::social_info_callback_800), "HTTP_FINISHED_800", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::rankList_callback_300), "HTTP_FINISHED_300", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::all_friends_callback_806), "HTTP_FINISHED_806", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::update_news_status), "UPDATE_NEWS_STATUS", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::check_begin_position), "TOUCH_BEGIN", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::change_position), "DRAGING", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::setIsEffective), "EFFECTIVE", NULL);
    
    this->update_news_status();
    
    
    this->setArrPlay(this->rand_array(_arrGroup1));
    this->schedule(SEL_SCHEDULE(&MainScene::delayPlay), 7, kCCRepeatForever, 3);
    
//    int count = arr->count();
//    for (int i = 0; i < count; i++) {
//        CCSprite* spt = (CCSprite*)arr->objectAtIndex(i);
//        this->play(spt);
//    }
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
    
    playerVelocity.x = playerVelocity.x / 10;
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
    
    _arrGroup1 = CCArray::create();
    _arrGroup1->retain();
    _arrGroup2 = CCArray::create();
    _arrGroup2->retain();
    
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


    //设置
    CCSprite* szSpr1 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
    CCSprite* szSpr2 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
    szSpr2->setScale(1.02f);
    _shezhiItem = CCMenuItemSprite::create(szSpr1, szSpr2, this, menu_selector(MainScene::shezhiCallBack));
    _shezhiItem->setPosition(ccp(DISPLAY->ScreenWidth()* .09f, DISPLAY->ScreenHeight()* .05f));

    
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
    _bird_1->setPosition(ccp(_layer_6->getContentSize().width* .9f, _layer_6->getContentSize().height* .77));//0.25f
    _bird_1->setScale(0.3f);
    _layer_6->addChild(_bird_1);
    
    CCAnimation* animation = CCAnimation::create();
    for (int i = 0; i < 5; i++) {
        CCString* str = CCString::createWithFormat("res/pic/mainScene/bird_%d.png", i + 1);
        animation->addSpriteFrameWithFileName(str->getCString());
    }
    animation->setDelayPerUnit(0.22f);
    animation->setRestoreOriginalFrame(true);
    
    CCAnimate* ac1 = CCAnimate::create(animation);
    CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(ac1, ac1->reverse(), NULL));
    _bird_1->runAction(rep);
    
    _bird_2 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_2->setPosition(ccp(_layer_6->getContentSize().width* .4f, _layer_6->getContentSize().height* .71));
    _bird_2->setScale(0.2f);
    _layer_6->addChild(_bird_2);
    animation->setDelayPerUnit(0.18f);
    CCAnimate* ac2 = CCAnimate::create(animation);
    CCRepeatForever* rep2 = CCRepeatForever::create(CCSequence::create(ac2, ac2->reverse(), NULL));
    _bird_2->runAction(rep2);
    
    _bird_3 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_3->setPosition(ccp(_layer_6->getContentSize().width* .75, _layer_6->getContentSize().height* .75));
    _bird_3->setScale(0.4f);
    _layer_6->addChild(_bird_3);
    animation->setDelayPerUnit(0.19f);
    CCAnimate* ac3 = CCAnimate::create(animation);
    CCRepeatForever* rep3 = CCRepeatForever::create(CCSequence::create(ac3, ac3->reverse(), NULL));
    _bird_3->runAction(rep3);
    
    _bird_4 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_4->setPosition(ccp(_layer_6->getContentSize().width* .85, _layer_6->getContentSize().height* .8));
    _bird_4->setScale(0.3f);
    _layer_6->addChild(_bird_4);
    animation->setDelayPerUnit(0.21f);
    CCAnimate* ac4 = CCAnimate::create(animation);
    CCRepeatForever* rep4 = CCRepeatForever::create(CCSequence::create(ac4, ac4->reverse(), NULL));
    _bird_4->runAction(rep4);
    
    _bird_5 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_5->setPosition(ccp(_layer_6->getContentSize().width* .45f, _layer_6->getContentSize().height* .77));
    _bird_5->setScale(0.4f);
    _layer_6->addChild(_bird_5);
    animation->setDelayPerUnit(0.1f);
    CCAnimate* ac5 = CCAnimate::create(animation);
    CCRepeatForever* rep5 = CCRepeatForever::create(CCSequence::create(ac5, ac5->reverse(), NULL));
    _bird_5->runAction(rep5);
    
    _bird_6 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_6->setPosition(ccp(_layer_6->getContentSize().width + 100, _layer_6->getContentSize().height* .73));
    _bird_6->setScale(0.2f);
    _layer_6->addChild(_bird_6);
    animation->setDelayPerUnit(0.1f);
    CCAnimate* ac6 = CCAnimate::create(animation);
    CCRepeatForever* rep6 = CCRepeatForever::create(CCSequence::create(ac6, ac6->reverse(), NULL));
    _bird_6->runAction(rep6);
    
    _bird_7 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_7->setPosition(ccp(_layer_6->getContentSize().width* .3f, _layer_6->getContentSize().height* .76));
    _bird_7->setScale(0.3f);
    _layer_6->addChild(_bird_7);
    animation->setDelayPerUnit(0.1f);
    CCAnimate* ac7 = CCAnimate::create(animation);
    CCRepeatForever* rep7 = CCRepeatForever::create(CCSequence::create(ac7, ac7->reverse(), NULL));
    _bird_7->runAction(rep7);
    
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
    
    paihang_bar1 = CCSprite::create("res/pic/mainScene/paihang_bar.png");
    paihang_bar1->setPosition(ccp(tower_spr1->getContentSize().width* .5f, tower_spr1->getContentSize().height* .5f));
    tower_spr1->addChild(paihang_bar1);
    
    CCSprite* paihang_bar2 = CCSprite::create("res/pic/mainScene/paihang_bar.png");
    paihang_bar2->setPosition(ccp(tower_spr2->getContentSize().width* .5f, tower_spr2->getContentSize().height* .5f));
    tower_spr2->addChild(paihang_bar2);
    
    tower_spr2->setScale(1.02f);
    CCMenuItem* paihang_Item = CCMenuItemSprite::create(tower_spr1, tower_spr2, this, menu_selector(MainScene::paihangCallBack));
    paihang_Item->setPosition(ccp(_layer_4->getContentSize().width* .6f, _layer_4->getContentSize().height* .78f));
    menu_paihang = CCMenu::create(paihang_Item, NULL);
    menu_paihang->setPosition(CCPointZero);
    _layer_4->addChild(menu_paihang);
    paihang_bar1->setUserObject(ccs("res/pic/mainScene/paihang_bar.png"));
    _arrGroup1->addObject(paihang_bar1);
    
    
    //---活动---
    CCSprite* hd_Spr1 = CCSprite::create("res/pic/mainScene/huodong.png");
    CCSprite* hd_Spr2 = CCSprite::create("res/pic/mainScene/huodong.png");
    
    huodong_bar1 = CCSprite::create("res/pic/mainScene/huodong_bar.png");
    huodong_bar1->setPosition(ccp(hd_Spr1->getContentSize().width* .5f, huodong_bar1->getContentSize().height* .4f));
    hd_Spr1->addChild(huodong_bar1);
    
    CCSprite* huodong_bar2 = CCSprite::create("res/pic/mainScene/huodong_bar.png");
    huodong_bar2->setPosition(ccp(hd_Spr2->getContentSize().width* .5f, huodong_bar2->getContentSize().height* .4f));
    hd_Spr2->addChild(huodong_bar2);
    
    hd_Spr2->setScale(1.02f);
    CCMenuItem* huodong_Item = CCMenuItemSprite::create(hd_Spr1, hd_Spr2, this, menu_selector(MainScene::huodongCallBack));
    huodong_Item->setPosition(ccp(_layer_4->getContentSize().width* .35f, _layer_4->getContentSize().height* .89f));
    
    menu_huodong = CCMenu::create(huodong_Item, NULL);
    menu_huodong->setPosition(CCPointZero);
    _layer_4->addChild(menu_huodong);
    
    huodong_bar1->setUserObject(ccs("res/pic/mainScene/huodong_bar.png"));
    _arrGroup1->addObject(huodong_bar1);
    
    CCMoveBy* mb = CCMoveBy::create(2.5f, CCPoint(0, 8));
    CCMoveBy* mb2 = CCMoveBy::create(0.5f, CCPoint(0, -1.6));
    CCSequence* seq = CCSequence::create(mb, CCDelayTime::create(0.2f), mb->reverse(), mb2, CCDelayTime::create(0.2f), mb2->reverse(), NULL);
    CCRepeatForever* ac_huodong = CCRepeatForever::create(seq);
    menu_huodong->runAction(ac_huodong);
        
    //-----3层背景------
    _layer_3 = CCSprite::create("res/pic/mainScene/three_bg.png");
    _layer_3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_3);
    
    //---日常btn(公司)---
    CCSprite* rc_Spr1 = CCSprite::create("res/pic/taskScene/task_building_3.png");
    company_bar1 = CCSprite::create("res/pic/mainScene/company_bar.png");
    company_bar1->setScale(1.0f/0.55f);
    company_bar1->setPosition(ccp(rc_Spr1->getContentSize().width* .585f, rc_Spr1->getContentSize().height* .28f));
    rc_Spr1->addChild(company_bar1);
    rc_Spr1->setScale(0.550f);
    rc_Spr1->setContentSize(rc_Spr1->getContentSize()* .550f);
    
    CCSprite* rc_Spr2 = CCSprite::create("res/pic/taskScene/task_building_3.png");
    CCSprite* company_bar2 = CCSprite::create("res/pic/mainScene/company_bar.png");
    company_bar2->setScale(1.0f/0.55f);
    company_bar2->setPosition(ccp(rc_Spr2->getContentSize().width* .585f, rc_Spr2->getContentSize().height* .28f));
    rc_Spr2->addChild(company_bar2);
    rc_Spr2->setScale(0.55f* 1.02f);
    rc_Spr2->setContentSize(rc_Spr1->getContentSize()* .550f);
    CCMenuItem* richang_Item = CCMenuItemSprite::create(rc_Spr1, rc_Spr2, this, menu_selector(MainScene::richangCallBack));
    richang_Item->setPosition(ccp(richang_Item->getContentSize().width* .53f, _layer_3->getContentSize().height* .65f));
    menu_richang = CCMenu::create(richang_Item, NULL);
    menu_richang->setPosition(CCPointZero);
    _layer_3->addChild(menu_richang);
    company_bar1->setUserObject(ccs("res/pic/mainScene/company_bar.png"));
    _arrGroup1->addObject(company_bar1);
    
    
    //---社交btn(咖啡厅)---
    CCSprite* hy_Spr1 = CCSprite::create("res/pic/mainScene/coffee_nor.png");
    coffee_bar1 = CCSprite::create("res/pic/mainScene/coffee_bar.png");
    coffee_bar1->setPosition(ccp(hy_Spr1->getContentSize().width* .38f, hy_Spr1->getContentSize().height* .67));
    hy_Spr1->addChild(coffee_bar1);
    CCSprite* hy_Spr2 = CCSprite::create("res/pic/mainScene/coffee_sel.png");
    CCSprite* coffee_bar2 = CCSprite::create("res/pic/mainScene/coffee_bar.png");
    coffee_bar2->setPosition(ccp(hy_Spr2->getContentSize().width* .38f, hy_Spr2->getContentSize().height* .67));
    hy_Spr2->addChild(coffee_bar2);
    hy_Spr2->setScale(1.02f);
    haoyou_Item = CCMenuItemSprite::create(hy_Spr1, hy_Spr2, this, menu_selector(MainScene::haoyouCallBack));
    haoyou_Item->setPosition(ccp(_layer_3->getContentSize().width* .66f, _layer_3->getContentSize().height* .557f));
    menu_haoyou = CCMenu::create(haoyou_Item, NULL);
    menu_haoyou->setPosition(CCPointZero);
    _layer_3->addChild(menu_haoyou);
    coffee_bar1->setUserObject(ccs("res/pic/mainScene/coffee_bar.png"));
    _arrGroup1->addObject(coffee_bar1);
    
    
    //---路灯---
    CCSprite* ludeng_spr = CCSprite::create("res/pic/mainScene/ludeng.png");
    ludeng_spr->setPosition(ccp(_layer_3->getContentSize().width* .08f, _layer_3->getContentSize().height* .512f));
    _layer_3->addChild(ludeng_spr);
    
    //---车---
    car_1 = CCSprite::create("res/pic/mainScene/car_1.png");
    car_1->setScale(0.5f);
    car_1->setPosition(ccp(_layer_3->getContentSize().width* .35f, _layer_3->getContentSize().height* .455f));
    _layer_3->addChild(car_1);
    
    car_2 = CCSprite::create("res/pic/mainScene/car_2.png");
    car_2->setScale(0.5f);
    car_2->setPosition(ccp(_layer_3->getContentSize().width + 100, _layer_3->getContentSize().height* .455f));
    _layer_3->addChild(car_2);
    
    car_3 = CCSprite::create("res/pic/mainScene/car_3.png");
    car_3->setFlipX(true);
    car_3->setScale(0.5f);
    car_3->setPosition(ccp(_layer_3->getContentSize().width* .2f, _layer_3->getContentSize().height* .443f));
    _layer_3->addChild(car_3);
    
    
    //-----2层花园背景-----
    _layer_2 = CCSprite::create("res/pic/mainScene/home_bg.png");
    _layer_2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f - 2));
    this->addChild(_layer_2);
    
    //---邮箱btn---
    CCSprite* mail_box1 = CCSprite::create("res/pic/mainScene/mail_nor.png");
    xinfeng_spr1 = CCSprite::create("res/pic/mainScene/xinfeng.png");
    xinfeng_spr1->setPosition(ccp(mail_box1->getContentSize().width* .6f, mail_box1->getContentSize().height * 0.88));
    mail_box1->addChild(xinfeng_spr1);
    CCSprite* mail_box2 = CCSprite::create("res/pic/mainScene/mail_sel.png");
    CCSprite* xinfeng_spr2 = CCSprite::create("res/pic/mainScene/xinfeng.png");
    xinfeng_spr2->setPosition(ccp(mail_box2->getContentSize().width* .6f, mail_box2->getContentSize().height * 0.88));
    mail_box2->addChild(xinfeng_spr2);
    mail_box2->setScale(1.02f);
    CCMenuItemSprite* youjian_Item = CCMenuItemSprite::create(mail_box1, mail_box2, this, menu_selector(MainScene::youjianCallBack));
    youjian_Item->setPosition(ccp(_layer_2->getContentSize().width* .36f, _layer_2->getContentSize().height* .458f));
    menu_mail = CCMenu::create(youjian_Item, NULL);
    menu_mail->setPosition(CCPointZero);
    _layer_2->addChild(menu_mail);
    
        CCMoveBy* ac_xin_1 = CCMoveBy::create(2.5f, CCPoint(0, 8));
        CCScaleTo* st = CCScaleTo::create(2.5f, 1.05f);
        CCSpawn* spa_xin = CCSpawn::create(ac_xin_1, st, NULL);
        CCScaleTo* st_2 = CCScaleTo::create(2.5f, 1.0f);
        CCMoveBy* ac_xin_2 = CCMoveBy::create(2.5f, CCPoint(0, -8));
        CCSpawn* spa_xin_2 = CCSpawn::create(ac_xin_2, st_2, NULL);
        CCSequence* seq_xin = CCSequence::create(spa_xin, CCDelayTime::create(0.3f), spa_xin_2, CCDelayTime::create(0.3), NULL);
        CCRepeatForever* rep_xin = CCRepeatForever::create(seq_xin);
        
        xinfeng_spr1->runAction(rep_xin);
    
    
    //---家btn---
    CCSprite* home_spr1 = CCSprite::create("res/pic/mainScene/home_nor.png");
    CCSprite* home_bar1 = CCSprite::create("res/pic/mainScene/home_bar.png");
    home_bar1->setPosition(ccp(home_spr1->getContentSize().width* .44f, home_spr1->getContentSize().height* .395f));
    home_spr1->addChild(home_bar1);
    CCSprite* home_spr2 = CCSprite::create("res/pic/mainScene/home_sel.png");
    CCSprite* home_bar2 = CCSprite::create("res/pic/mainScene/home_bar.png");
    home_bar2->setPosition(ccp(home_spr2->getContentSize().width* .44f, home_spr2->getContentSize().height* .395f));
    home_spr2->addChild(home_bar2);
    home_spr2->setScale(1.02f);
    CCMenuItem* home_item = CCMenuItemSprite::create(home_spr1, home_spr2, this, menu_selector(MainScene::homeCallBack));
    home_item->setPosition(ccp(_layer_2->getContentSize().width* .298f, _layer_2->getContentSize().height* .310f));
    menu_home = CCMenu::create(home_item, NULL);
    menu_home->setPosition(CCPointZero);
    _layer_2->addChild(menu_home);
    
    
    //---换装btn---
    CCSprite* hz_Spr1 = CCSprite::create("res/pic/mainScene/shop_nor.png");
    shop_bar1 = CCSprite::create("res/pic/mainScene/shop_bar.png");
    shop_bar1->setPosition(ccp(hz_Spr1->getContentSize().width* .29f, hz_Spr1->getContentSize().height* .35f));
    hz_Spr1->addChild(shop_bar1);
    CCSprite* hz_Spr2 = CCSprite::create("res/pic/mainScene/shop_sel.png");
    CCSprite* shop_bar2 = CCSprite::create("res/pic/mainScene/shop_bar.png");
    shop_bar2->setPosition(ccp(hz_Spr2->getContentSize().width* .29f, hz_Spr2->getContentSize().height* .35f));
    hz_Spr2->addChild(shop_bar2);
    hz_Spr2->setScale(1.02f);
    CCMenuItemSprite* huanzhuang_Item = CCMenuItemSprite::create(hz_Spr1, hz_Spr2, this, menu_selector(MainScene::huanzhuangCallBack));
    huanzhuang_Item->setPosition(ccp(_layer_2->getContentSize().width* .65f, _layer_2->getContentSize().height* .330f));
    menu_hz = CCMenu::create(huanzhuang_Item, NULL);
    menu_hz->setPosition(CCPointZero);
    _layer_2->addChild(menu_hz);
    shop_bar1->setUserObject(ccs("res/pic/mainScene/shop_bar.png"));
    _arrGroup1->addObject(shop_bar1);

    //-----1层路------
    _layer_1 = CCSprite::create("res/pic/mainScene/road.png");
    _layer_1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_1);
    
    //---剧情btn---
    CCSprite* jq_Car1 = CCSprite::create("res/pic/mainScene/car_juqing_1_nor.png");
    juqing_bar1 = CCSprite::create("res/pic/mainScene/juqing_bar.png");
    juqing_bar1->setPosition(ccp(jq_Car1->getContentSize().width* .76f, jq_Car1->getContentSize().height* .27f));
    jq_Car1->addChild(juqing_bar1);
    CCSprite* jq_Car2 = CCSprite::create("res/pic/mainScene/car_juqing_1_sel.png");
    CCSprite* juqing_bar2 = CCSprite::create("res/pic/mainScene/juqing_bar.png");
    juqing_bar2->setPosition(ccp(jq_Car2->getContentSize().width* .76f, jq_Car2->getContentSize().height* .27f));
    jq_Car2->addChild(juqing_bar2);
    jq_Car2->setScale(1.02f);
    CCMenuItem* juqing_Item = CCMenuItemSprite::create(jq_Car1, jq_Car2, this, menu_selector(MainScene::juqingCallBack));
    juqing_bar1->setUserObject(ccs("res/pic/mainScene/juqing_bar.png"));
    _arrGroup1->addObject(juqing_bar1);
    
    menu_car1 = CCMenu::create(juqing_Item, NULL);
    menu_car1->setPosition(ccp(_layer_1->getContentSize().width* .65f, _layer_1->getContentSize().height* .13f));
    _layer_1->addChild(menu_car1);
    
    //---vip剧情btn---
    CCSprite* jq2_Car1 = CCSprite::create("res/pic/mainScene/car_juqing_2_nor.png");
    CCSprite* unknow_bar1 = CCSprite::create("res/pic/mainScene/unknow_bar.png");
    unknow_bar1->setPosition(ccp(jq2_Car1->getContentSize().width* .75f, jq2_Car1->getContentSize().height* .23f));
    jq2_Car1->addChild(unknow_bar1);
    CCSprite* jq2_Car2 = CCSprite::create("res/pic/mainScene/car_juqing_2_sel.png");
    CCSprite* unknow_bar2 = CCSprite::create("res/pic/mainScene/unknow_bar.png");
    unknow_bar2->setPosition(ccp(jq2_Car2->getContentSize().width* .75f, jq2_Car2->getContentSize().height* .23f));
    jq2_Car2->addChild(unknow_bar2);
    jq2_Car2->setScale(1.02f);
    CCMenuItem* juqing2_Item = CCMenuItemSprite::create(jq2_Car1, jq2_Car2, this, menu_selector(MainScene::juqing_vipCallBack));
    
    menu_car2 = CCMenu::create(juqing2_Item, NULL);
    menu_car2->setPosition(ccp(_layer_1->getContentSize().width* .35f, _layer_1->getContentSize().height* .15f));
    _layer_1->addChild(menu_car2);
    
    //-----花------
    _layer_0 = CCSprite::create("res/pic/mainScene/near.png");
    _layer_0->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_0);
    
    
    CCMenu* menu = CCMenu::create(shouchongItem, huodongItem, qiandaoItem, _haoyouItem, huanzhuangItem, paihangItem, _shezhiItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
//    CCSprite* txt_bar = CCSprite::create("res/pic/mainScene/txt_bar.png");
//    txt_bar->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, txt_bar->getContentSize().height* .5f));
//    this->addChild(txt_bar);
//    
//    
//    CCLabelTTF* lab = CCLabelTTF::create("小秘书提醒：公司发展到关键阶段了，云总", DISPLAY->fangzhengFont(), 20, CCSizeMake(txt_bar->getContentSize().width - 10, 25), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
//    lab->setColor(ccc3(155, 84, 46));
//    lab->setPosition(ccp(txt_bar->getContentSize().width* .5f, txt_bar->getContentSize().height* .5f - 3));
//    txt_bar->addChild(lab);
    
}

CCArray* MainScene::rand_array(CCArray *arr) {
    if (arr == NULL) {
        return NULL;
    }
    
    int count = arr->count();
    CCArray* arrTemp = CCArray::createWithCapacity(count);
    for (int i = 0; i < count; i++) {
        arrTemp->addObject(arr->objectAtIndex(i));
    }
    

    
    return arrTemp;
}

void MainScene::play(CCSprite* spt) {
    CCString* name = (CCString*)spt->getUserObject();
    CCSprite* actionSpt = CCSprite::create(name->getCString());
    
    CCScaleTo* st = NULL;
    CCScaleTo* st1 = NULL;
    
    if (name->compare("res/pic/mainScene/company_bar.png") == 0) {
        actionSpt->setScale(1.0f/0.55f);
        
        st = CCScaleTo::create(0.5f, 1.02f * (1.0f/0.55f));
        st1 = CCScaleTo::create(0.5f, 1.0f * (1.0f/0.55f));
    
    }else{
        st = CCScaleTo::create(0.5f, 1.02f);
        st1 = CCScaleTo::create(0.5f, 1.0f);

    }
    
    actionSpt->setPosition(spt->getPosition());
    spt->getParent()->addChild(actionSpt);
    
    
    
    
    
    
    CCSprite* flower_spr = CCSprite::create("res/pic/mainScene/flower_1.png");
    flower_spr->setPosition(ccp(shop_bar1->getContentSize().width* .5f, shop_bar1->getContentSize().height* .5f));
    actionSpt->addChild(flower_spr);
    
    CCAnimation* animation_flower = CCAnimation::create();
    for (int i = 0; i < 4; i++) {
        CCString* str = CCString::createWithFormat("res/pic/mainScene/flower_%d.png", i + 1);
        animation_flower->addSpriteFrameWithFileName(str->getCString());
    }
    animation_flower->setDelayPerUnit(0.15f);
    animation_flower->setRestoreOriginalFrame(true);
    CCAnimate* animate_flower = CCAnimate::create(animation_flower);
    flower_spr->runAction(CCRepeatForever::create(animate_flower));
    

    actionSpt->runAction(CCSequence::create(st, st1, st, st1, st, st1, st, st1, st, st1, CCRemoveSelf::create(), NULL));
}

void MainScene::delayPlay(float dt) {
    if (_arrPlay == NULL) {
        unschedule(SEL_SCHEDULE(&MainScene::delayPlay));
        return;
    }
    
    if (_arrPlay->count() > 0) {
        CCSprite* spt = (CCSprite*)_arrPlay->randomObject();
        this->play(spt);
        _arrPlay->removeObject(spt);
    }
    else {
        this->setArrPlay(this->rand_array(_arrGroup1));
    }
}



void MainScene::check_begin_position(CCObject* pos){
    CCPoint last_pos = CCPointFromString(((CCString*)pos)->getCString());
    if (menu_car1->boundingBox().containsPoint(last_pos) ||
        menu_car2->boundingBox().containsPoint(last_pos) ||
        menu_home->boundingBox().containsPoint(last_pos) ||
        menu_hz->boundingBox().containsPoint(last_pos) ||
        menu_mail->boundingBox().containsPoint(last_pos) ||
        menu_richang->boundingBox().containsPoint(last_pos) ||
        menu_haoyou->boundingBox().containsPoint(last_pos) ||
        menu_paihang->boundingBox().containsPoint(last_pos) ||
        menu_huodong->boundingBox().containsPoint(last_pos)) {
        isMenu = true;
    }
}


void MainScene::update(float dt){
    //云
    if (_cloud_1->getPositionX() <= -_cloud_1->getContentSize().width* .5f) {
        _cloud_1->setPositionX(_cloud_2->getPositionX() + _cloud_1->getContentSize().width);
    }else{
        _cloud_1->setPositionX(_cloud_1->getPositionX() - 3 * dt);
    }
    
    if (_cloud_2->getPositionX() <= -_cloud_2->getContentSize().width* .5f) {
        _cloud_2->setPositionX(_cloud_1->getPositionX() + _cloud_1->getContentSize().width);
    }else{
        _cloud_2->setPositionX(_cloud_2->getPositionX() - 3 * dt);
    }
    
    //河流
    if (_river_1->getPositionX() <= -_river_1->getContentSize().width* .5f) {
        _river_1->setPositionX(_river_2->getPositionX() + _river_1->getContentSize().width* .5f + _river_2->getContentSize().width* .5f - 5);
    }else{
        _river_1->setPositionX(_river_1->getPositionX() - 13 * dt);
    }
    
    if (_river_2->getPositionX() <= -_river_2->getContentSize().width* .5f) {
        _river_2->setPositionX(_river_1->getPositionX() + _river_1->getContentSize().width* .5f + _river_2->getContentSize().width* .5f - 5);
    }else{
        _river_2->setPositionX(_river_2->getPositionX() - 13 * dt);
    }
    
    if (_bird_1->getPositionX() <= -100) {
        _bird_1->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_1->setPositionX(_bird_1->getPositionX() - 35 * dt);
    }
    
    if (_bird_2->getPositionX() <= -100) {
        _bird_2->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_2->setPositionX(_bird_2->getPositionX() - 35 * dt);
    }
    
    if (_bird_3->getPositionX() <= -100) {
        _bird_3->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_3->setPositionX(_bird_3->getPositionX() - 35 * dt);
    }
    
    if (_bird_4->getPositionX() <= -100) {
        _bird_4->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_4->setPositionX(_bird_4->getPositionX() - 35 * dt);
    }
    
    if (_bird_5->getPositionX() <= -100) {
        _bird_5->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_5->setPositionX(_bird_5->getPositionX() - 45 * dt);
    }
    
    if (_bird_6->getPositionX() <= -100) {
        _bird_6->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_6->setPositionX(_bird_6->getPositionX() - 55 * dt);
    }
    
    if (_bird_7->getPositionX() <= -100) {
        _bird_7->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_7->setPositionX(_bird_7->getPositionX() - 65 * dt);
    }
    
    if (car_1->getPositionX() <= -200) {
        car_1->setPositionX(_layer_3->getContentSize().width + 100);
    }else{
        car_1->setPositionX(car_1->getPositionX() - 70 * dt);
    }
    
    if (car_2->getPositionX() <= -200) {
        car_2->setPositionX(_layer_3->getContentSize().width + 100);
    }else{
        car_2->setPositionX(car_2->getPositionX() - 70 * dt);
    }
    
    if (car_3->getPositionX() >= 100 + _layer_3->getContentSize().width) {
        car_3->setPositionX(-_layer_3->getContentSize().width - 100);
    }else{
        car_3->setPositionX(car_3->getPositionX() + 70 * dt);
    }
    
    if (isEffective) {
        isOk = true;
    }
}

void MainScene::change_position(CCObject* pObj){
    offset_x = ((CCFloat*)pObj)->getValue();
    
    if (isMenu) {
        if (isMoving) {
            this->updateLayerPosition(offset_x);
        }else{
            move_x += offset_x;
            if (fabsf(move_x) >= 30) {
                this->updateLayerPosition(offset_x);
                isMoving = true;
            }
        }
        
    }else{
        this->updateLayerPosition(offset_x);
    }
}

void MainScene::updateLayerPosition(float offset_x){
    isEffective= false;
    isOk = false;
    
    if ((_layer_0->getPositionX() + offset_x >= _layer_0->getContentSize().width* .5f - 5 && offset_x > 0) ||
        (_layer_0->getPositionX() + offset_x <= DISPLAY->ScreenWidth() - _layer_0->getContentSize().width* .5f + 5 && offset_x < 0)) {
        
    }else{
        _layer_0->setPositionX(_layer_0->getPositionX() + offset_x);
        _layer_1->setPositionX(_layer_1->getPositionX() + offset_x);
        _layer_2->setPositionX(_layer_2->getPositionX() + offset_x* .8f);
        _layer_3->setPositionX(_layer_3->getPositionX() + offset_x* .6f);
        _layer_4->setPositionX(_layer_4->getPositionX() + offset_x* .35f);
        _river_1->setPositionX(_river_1->getPositionX() + offset_x* .3f);
        _river_2->setPositionX(_river_2->getPositionX() + offset_x* .3f);
        _layer_6->setPositionX(_layer_6->getPositionX() + offset_x* .15f);
    }
}

void MainScene::setIsEffective(){
    isEffective = true;
    isMenu = false;
    isMoving = false;
    move_x = 0;
}

void MainScene::juqing_vipCallBack(CCObject* pSender){
    if (isOk) {
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "敬请期待");
    }
}

void MainScene::homeCallBack(CCObject *pSender){
    if (isOk) {
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "暂未开放");
    }
}

void MainScene::shouchongCallBack(CCObject* pSender){
    if (isOk) {
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "敬请期待");
    }
}

void MainScene::huodongCallBack(CCObject* pSender){
    if (isOk) {
        AUDIO->comfirm_effect();
        OperationPanel* panel = OperationPanel::create();
        panel->show_from(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .85f));
    }
}

void MainScene::qiandaoCallBack(CCObject* pSender){
    AUDIO->shop_effect();
    if (isOk) {
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "敬请期待");
    }
}

void MainScene::youjianCallBack(CCObject* pSender){
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        NET->all_mails_700();
    }
}

void MainScene::haoyouCallBack(CCObject* pSender){
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        NET->social_info_800();
    }
}

void MainScene::social_info_callback_800(CCObject* pObj) {
    LOADING->remove();
    CCScene* scene = HaoyouScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::renwuCallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
}

void MainScene::huanzhuangCallBack(CCObject* pSender){
    if (isOk) {
        if (DATA->getClothes()->has_init_clothes == true) {
            this->_huanzhuangCallBack(pSender);
        }
        else {
            LOADING->show_loading();
            NET->owned_clothes_400();
        }
    }
}

void MainScene::_huanzhuangCallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    CCLayer* layer = ClothesScene::create_with_type(2, 0, 0);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void MainScene::paihangCallBack(CCObject* pSender){
    if (isOk) {
        LOADING->show_loading();
        NET->ranking_list_300();
    }
}

void MainScene::rankList_callback_300(CCObject *pObj){
    NET->all_friends_806();
}

void MainScene::all_friends_callback_806(CCObject *pObj){
    AUDIO->comfirm_effect();
    LOADING->remove();
    CCLayer* layer = TotalRankScene::create_with_type(1);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::juqingCallBack(CCObject* pSender){
    if (isOk) {
        if (DATA->getStory()->has_init_story()) {
            this->_500CallBack(NULL);
        }else{
            LOADING->show_loading();
            NET->completed_story_500();
        }
    }
    
}
void MainScene::_500CallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    if (isrenwuBool) {
        LOADING->show_loading();
        NET->completed_mission_600();
    }else{
        CCScene* scene = QingjingScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}
void MainScene::richangCallBack(CCObject* pSender){
    if (isOk) {
        if (DATA->getStory()->has_init_story()) {
            LOADING->show_loading();
            NET->completed_mission_600();
        }else{
            isrenwuBool = true;
            LOADING->show_loading();
            NET->completed_story_500();
        }
    }
}
void MainScene::_600CallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    LOADING->remove();
    
    DATA->setTaskPhase(DATA->getPlayer()->phase);
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::shezhiCallBack(CCObject* pSender){
    if (isOk) {
        AUDIO->comfirm_effect();
        SettingPanel* panel = SettingPanel::create();
        panel->show_from(_shezhiItem->getPosition());
    }
    
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
    xinfeng_spr1->removeAllChildrenWithCleanup(true);
    LOADING->remove();
    MailPanel* panel = MailPanel::create();
    panel->show_from(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .85f));
}

void MainScene::update_news_status() {
    NewsComp* news = DATA->getNews();
    if (news->mail > 0) {
        CCSprite* spt = CCSprite::create("res/pic/new.png");
        spt->setPosition(ccp(80, 60));
        xinfeng_spr1->addChild(spt);
    }
    
    if (news->paper + news->message > 0) {
        CCSprite* spt = CCSprite::create("res/pic/new.png");
        spt->setPosition(ccp(80, 80));
        _haoyouItem->addChild(spt);
        
        CCSprite* spt2 = CCSprite::create("res/pic/new.png");
        spt2->setPosition(ccp(210, 205));
        haoyou_Item->addChild(spt2);
    }
}


