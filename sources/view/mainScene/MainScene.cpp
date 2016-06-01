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

MainScene::MainScene(){
    
}
MainScene::~MainScene(){
    
}

bool MainScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
//    this->setTouchSwallowEnabled(true);
//    this->setTouchMode(kCCTouchesOneByOne);
//    this->setTouchEnabled(true);
    
    this->creat_view();
    
    
    this->schedule(schedule_selector(MainScene::gengxin), .1f);
    
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
    this->setAccelerometerEnabled(true);
    
}

void MainScene::onExit(){
    
    
    
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


bool MainScene::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent){
    
    
    return true;
}

void MainScene::creat_view(){
    
    roomSpr = CCSprite::create("res/pic/mainScene/main_bg.png");
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(roomSpr);
    
    // 活动
    CCSprite* hdSpr1 = CCSprite::create("res/pic/mainScene/main_huodong.png");
    CCSprite* hdSpr2 = CCSprite::create("res/pic/mainScene/main_huodong.png");
    hdSpr2->setScale(1.02f);
    CCMenuItem* huodongItem = CCMenuItemSprite::create(hdSpr1, hdSpr2, this, menu_selector(MainScene::huodongCallBack));
    huodongItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .85f));
    
    // 签到
    CCSprite* qdSpr1 = CCSprite::create("res/pic/mainScene/main_qiandao.png");
    CCSprite* qdSpr2 = CCSprite::create("res/pic/mainScene/main_qiandao.png");
    qdSpr2->setScale(1.02f);
    CCMenuItem* qiandaoItem = CCMenuItemSprite::create(qdSpr1, qdSpr2, this, menu_selector(MainScene::qiandaoCallBack));
    qiandaoItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .75f));
    
    // 邮件
    CCSprite* yjSpr1 = CCSprite::create("res/pic/mainScene/main_youjian.png");
    CCSprite* yjSpr2 = CCSprite::create("res/pic/mainScene/main_youjian.png");
    yjSpr2->setScale(1.02f);
    CCMenuItem* youjianItem = CCMenuItemSprite::create(yjSpr1, yjSpr2, this, menu_selector(MainScene::youjianCallBack));
    youjianItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .85f));
    
    // 任务
    CCSprite* rwSpr1 = CCSprite::create("res/pic/mainScene/main_renwu.png");
    CCSprite* rwSpr2 = CCSprite::create("res/pic/mainScene/main_renwu.png");
    rwSpr2->setScale(1.02f);
    CCMenuItem* renwuItem = CCMenuItemSprite::create(rwSpr1, rwSpr2, this, menu_selector(MainScene::renwuCallBack));
    renwuItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .75f));
    
    // 换装
    CCSprite* hzSpr1 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    CCSprite* hzSpr2 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    hzSpr2->setScale(1.02f);
    CCMenuItem* huanzhuangItem = CCMenuItemSprite::create(hzSpr1, hzSpr2, this, menu_selector(MainScene::huanzhuangCallBack));
    huanzhuangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .65f));
    
    // 排行
    CCSprite* phSpr1 = CCSprite::create("res/pic/mainScene/main_paihang.png");
    CCSprite* phSpr2 = CCSprite::create("res/pic/mainScene/main_paihang.png");
    phSpr2->setScale(1.02f);
    CCMenuItem* paihangItem = CCMenuItemSprite::create(phSpr1, phSpr2, this, menu_selector(MainScene::paihangCallBack));
    paihangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .55f));
    
    CCMenu* menu = CCMenu::create(huodongItem, qiandaoItem, youjianItem, renwuItem, huanzhuangItem, paihangItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    // 剧情
    CCSprite* jqSpr1 = CCSprite::create("res/pic/mainScene/main_juqing.png");
    CCSprite* jqSpr2 = CCSprite::create("res/pic/mainScene/main_juqing.png");
    jqSpr2->setScale(1.02f);
    CCMenuItem* juqingItem = CCMenuItemSprite::create(jqSpr1, jqSpr2, this, menu_selector(MainScene::juqingCallBack));
    juqingItem->setPosition(ccp(DISPLAY->ScreenWidth()* .91f, DISPLAY->ScreenHeight()* .3f));
    
    //日常
    CCSprite* rcSpr1 = CCSprite::create("res/pic/mainScene/main_richang.png");
    CCSprite* rcSpr2 = CCSprite::create("res/pic/mainScene/main_richang.png");
    rcSpr2->setScale(1.02f);
    CCMenuItem* richangItem = CCMenuItemSprite::create(rcSpr1, rcSpr2, this, menu_selector(MainScene::richangCallBack));
    richangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .91f, DISPLAY->ScreenHeight()* .2f));
    
    //设置
    CCSprite* szSpr1 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
    CCSprite* szSpr2 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
    szSpr2->setScale(1.02f);
    CCMenuItem* shezhiItem = CCMenuItemSprite::create(szSpr1, szSpr2, this, menu_selector(MainScene::shezhiCallBack));
    shezhiItem->setPosition(ccp(DISPLAY->ScreenWidth()* .09f, DISPLAY->ScreenHeight()* .05f));
    
    CCMenu* downMenu = CCMenu::create(juqingItem, richangItem, shezhiItem, NULL);
    downMenu->setPosition(CCPointZero);
    this->addChild(downMenu);
    
}

void MainScene::huodongCallBack(CCObject* pSender){
    
}
void MainScene::qiandaoCallBack(CCObject* pSender){
    
}
void MainScene::youjianCallBack(CCObject* pSender){
    
}
void MainScene::renwuCallBack(CCObject* pSender){
    
}
void MainScene::huanzhuangCallBack(CCObject* pSender){
    
}
void MainScene::paihangCallBack(CCObject* pSender){
    
}
void MainScene::juqingCallBack(CCObject* pSender){
    CCScene* scene = QingjingScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void MainScene::richangCallBack(CCObject* pSender){
    
}
void MainScene::shezhiCallBack(CCObject* pSender){
    
}


















