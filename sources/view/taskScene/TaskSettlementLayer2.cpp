//
//  TaskSettlementLayer2.cpp
//  tiegao
//
//  Created by mac on 16-7-15.
//
//

#include "TaskSettlementLayer2.h"
#include "DataManager.h"
#include "Loading.h"
#include "ConfigManager.h"
#include "TaskScene.h"
#include "ClothesScene.h"
#include "DisplayManager.h"
#include "SpecialManager.h"


TaskSettlementLayer2::~TaskSettlementLayer2(){
    
}
TaskSettlementLayer2* TaskSettlementLayer2::create(int rating, int coin, bool isPhaseUP){
    TaskSettlementLayer2* rtn = new TaskSettlementLayer2();
    if (rtn && rtn->init(rating, coin, isPhaseUP)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}
bool TaskSettlementLayer2::init(int rating, int coin, bool isPhaseUP){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    _rating = rating;
    _coin = coin;
    _isPhaseUP = isPhaseUP;
    
    lingquBool = false;
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    this->creat_view();
    this->creat_Man();
    this->initClothes();
    
    return true;
}
void TaskSettlementLayer2::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    
    SPECIAL->showPetal2At(this, DISPLAY->center(), 1);
}
void TaskSettlementLayer2::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void TaskSettlementLayer2::will_number_scroll(){
    
}
void TaskSettlementLayer2::did_number_stoped(){
    
}

void TaskSettlementLayer2::will_number_scroll2(){
    
}
void TaskSettlementLayer2::did_number_stoped2(){
    
}

void TaskSettlementLayer2::will_number_scroll3(){
    
}
void TaskSettlementLayer2::did_number_stoped3(){
    
}

void TaskSettlementLayer2::will_number_scroll4(){
    
}
void TaskSettlementLayer2::did_number_stoped4(){
    
}

void TaskSettlementLayer2::keyBackClicked(){
    
}
bool TaskSettlementLayer2::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent){
    if (!lingquBool) {
        // 给个提示
    }else{
//        exit();
    }
    
    return true;
}

void TaskSettlementLayer2::creat_view(){
    CCSprite* bgSpr1 = CCSprite::create("res/pic/taskSettlement/ts_bg1.png");
    bgSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr1);
    
    // 上条
    tiaoSpr = CCSprite::create("res/pic/taskSettlement/ts_tiao.png");
    tiaoSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .86f));
    this->addChild(tiaoSpr, 15);
    
    CCSprite* done_text = CCSprite::create("res/pic/taskSettlement/ts_done_text.png");
    done_text->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .63f));
    tiaoSpr->addChild(done_text);
    
    
    CCString* renStr = CCString::createWithFormat("res/pic/taskSettlement/ts_ren%d.png", 1);
    renSpr = CCSprite::create(renStr->getCString());
//    renSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .19f, DISPLAY->ScreenHeight()* .13f));
    renSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .19f, DISPLAY->ScreenHeight()* .83f));
    renSpr->setScale(1.6f);
    this->addChild(renSpr, 20);
    
    // 下框
    CCSprite* kuangSpr1 = CCSprite::create("res/pic/taskSettlement/ts_kuang1.png");
    kuangSpr1->setPosition(ccp(renSpr->getContentSize().width + kuangSpr1->getContentSize().width* .33f, renSpr->getContentSize().height* .3f));
    renSpr->addChild(kuangSpr1);
    
    
    CCMoveTo* moveTo = CCMoveTo::create(.3f, ccp(DISPLAY->ScreenWidth()* .19f, DISPLAY->ScreenHeight()* .6f));
    CCScaleTo* scaleTo = CCScaleTo::create(.3f, 1.f);
    renSpr->runAction(CCSpawn::create(moveTo, scaleTo, NULL));
    
    
    if (_rating == 5) {
        randIndex1 = 80 + rand()%20;
        randIndex2 = 80 + rand()%20;
        randIndex3 = 80 + rand()%20;
        randIndex4 = 80 + rand()%20;
        tishiStr = CCString::createWithFormat("不错不错.很是大方得体.");
    }else if (_rating == 4) {
        randIndex1 = 60 + rand()%20;
        randIndex2 = 60 + rand()%20;
        randIndex3 = 60 + rand()%20;
        randIndex4 = 60 + rand()%20;
        tishiStr = CCString::createWithFormat("还可以吧.吐槽君还是可以接受的.");
    }else if (_rating == 3) {
        randIndex1 = 40 + rand()%20;
        randIndex2 = 40 + rand()%20;
        randIndex3 = 40 + rand()%20;
        randIndex4 = 40 + rand()%20;
        tishiStr = CCString::createWithFormat("你的审美是那个星系的~!不敢恭维.");
    }else if (_rating == 2) {
        randIndex1 = 20 + rand()%20;
        randIndex2 = 20 + rand()%20;
        randIndex3 = 20 + rand()%20;
        randIndex4 = 20 + rand()%20;
        tishiStr = CCString::createWithFormat("你这样,还处的去门嘛.");
    }else if (_rating == 1) {
        randIndex1 = 2 + rand()%20;
        randIndex2 = 2 + rand()%20;
        randIndex3 = 2 + rand()%20;
        randIndex4 = 2 + rand()%20;
        tishiStr = CCString::createWithFormat("你光着屁股出门都比这一身强.");
    }
    tishiLabel = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 23, CCSizeMake(kuangSpr1->getContentSize().width* .88f, kuangSpr1->getContentSize().height* .5f), kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    tishiLabel->setPosition(ccp(kuangSpr1->getContentSize().width* .5f, kuangSpr1->getContentSize().height* .55f));
    tishiLabel->setColor(ccc3(80, 63, 68));
    kuangSpr1->addChild(tishiLabel);
    
    // 魅力
    CCSprite* taskSpr1 = CCSprite::create("res/pic/taskScene/task_1.png");
    taskSpr1->setPosition(ccp(kuangSpr1->getContentSize().width* .15f, kuangSpr1->getContentSize().height* .45f));
    kuangSpr1->addChild(taskSpr1, 10);
    CCString* taskStr1 = CCString::createWithFormat("%d", 0);
    taskLabel1 = FlashNumberLabel::create_with_atlas("res/pic/baseScene/base_number.png", taskStr1->getCString(), 0, .01f);
    taskLabel1->setAnchorPoint(ccp(0, .5f));
    taskLabel1->set_delegate(this); // 设置代理
    taskLabel1->setPosition(ccp(taskSpr1->getContentSize().width + 5, taskSpr1->getContentSize().height* .45f));
    taskSpr1->addChild(taskLabel1, 1);
    
    // 沟通
    CCSprite* taskSpr2 = CCSprite::create("res/pic/taskScene/task_2.png");
    taskSpr2->setPosition(ccp(kuangSpr1->getContentSize().width* .45f, kuangSpr1->getContentSize().height* .45f));
    kuangSpr1->addChild(taskSpr2, 10);
    CCString* taskStr2 = CCString::createWithFormat("%d", 0);
    taskLabel2 = FlashNumberLabel2::create_with_atlas2("res/pic/baseScene/base_number.png", taskStr2->getCString(), 0, .01f);
    taskLabel2->setAnchorPoint(ccp(0, .5f));
    taskLabel2->set_delegate2(this); // 设置代理
    taskLabel2->setPosition(ccp(taskSpr2->getContentSize().width + 5, taskSpr2->getContentSize().height* .45f));
    taskSpr2->addChild(taskLabel2, 1);
    
    // 业务
    CCSprite* taskSpr3 = CCSprite::create("res/pic/taskScene/task_3.png");
    taskSpr3->setPosition(ccp(kuangSpr1->getContentSize().width* .15f, kuangSpr1->getContentSize().height* .25f));
    kuangSpr1->addChild(taskSpr3, 10);
    CCString* taskStr3 = CCString::createWithFormat("%d", 0);
    taskLabel3 = FlashNumberLabel3::create_with_atlas3("res/pic/baseScene/base_number.png", taskStr3->getCString(), 0, .01f);
    taskLabel3->setAnchorPoint(ccp(0, .5f));
    taskLabel3->set_delegate3(this); // 设置代理
    taskLabel3->setPosition(ccp(taskSpr3->getContentSize().width + 5, taskSpr3->getContentSize().height* .45f));
    taskSpr3->addChild(taskLabel3, 1);
    
    // 运气
    CCSprite* taskSpr4 = CCSprite::create("res/pic/taskScene/task_4.png");
    taskSpr4->setPosition(ccp(kuangSpr1->getContentSize().width* .45f, kuangSpr1->getContentSize().height* .25f));
    kuangSpr1->addChild(taskSpr4, 10);
    CCString* taskStr4 = CCString::createWithFormat("%d", 0);
    taskLabel4 = FlashNumberLabel4::create_with_atlas4("res/pic/baseScene/base_number.png", taskStr4->getCString(), 0, .01f);
    taskLabel4->setAnchorPoint(ccp(0, .5f));
    taskLabel4->set_delegate4(this); // 设置代理
    taskLabel4->setPosition(ccp(taskSpr4->getContentSize().width + 5, taskSpr4->getContentSize().height* .45f));
    taskSpr4->addChild(taskLabel4, 1);
    
    
    taskLabel1->setVisible(false);
    taskLabel2->setVisible(false);
    taskLabel3->setVisible(false);
    taskLabel4->setVisible(false);
    
    //
    coinSpr = CCSprite::create("res/pic/clothesScene/gj_coin.png");
    coinSpr->setPosition(ccp(kuangSpr1->getContentSize().width* .79f, kuangSpr1->getContentSize().height* .55f));
    kuangSpr1->addChild(coinSpr);
    CCString* coinStr = CCString::createWithFormat("x%d", _coin);
    coinLabel = CCLabelTTF::create(coinStr->getCString(), DISPLAY->fangzhengFont(), 25);
    coinLabel->setAnchorPoint(ccp(0, .5f));
    coinLabel->setPosition(ccp(coinSpr->getContentSize().width + 5, coinSpr->getContentSize().height* .4f));
    coinLabel->setColor(ccc3(80, 63, 68));
    coinSpr->addChild(coinLabel);
    
    CCSprite* lingquSpr1 = CCSprite::create("res/pic/taskSettlement/ts_lingqu.png");
    lingquSpr1->setScale(.9f);
    CCSprite* lingquSpr2 = CCSprite::create("res/pic/taskSettlement/ts_lingqu.png");
    lingquSpr2->setScale(.92f);
    lingquItem = CCMenuItemSprite::create(lingquSpr1, lingquSpr2, this, menu_selector(TaskSettlementLayer2::lingquCallBack));
    lingquItem->setPosition(ccp(kuangSpr1->getContentSize().width* .86f, kuangSpr1->getContentSize().height* .28f));
    CCMenu* menu = CCMenu::create(lingquItem, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr1->addChild(menu);
    lingquItem->setEnabled(false);
    
    coinSpr->setVisible(false);
    lingquItem->setVisible(false);
    
    
    
    CCSprite* bgSpr2 = CCSprite::create("res/pic/taskSettlement/ts_bg2.png");
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr2, 20);
}
void TaskSettlementLayer2::lingquCallBack(CCObject* pSender){
    
}

void TaskSettlementLayer2::creat_Man(){
    
}
void TaskSettlementLayer2::initClothes(){
    
}

void TaskSettlementLayer2::initAniamtion(){
    
}
void TaskSettlementLayer2::startAnimation(float dt){
    
}






