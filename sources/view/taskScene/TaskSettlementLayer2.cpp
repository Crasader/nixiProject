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
#include "QingjingScene.h"


TaskSettlementLayer2::~TaskSettlementLayer2(){
    
}
TaskSettlementLayer2* TaskSettlementLayer2::create(int rating, int coin, int energy, bool isPhaseUP){
    TaskSettlementLayer2* rtn = new TaskSettlementLayer2();
    if (rtn && rtn->init(rating, coin, energy, isPhaseUP)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}
bool TaskSettlementLayer2::init(int rating, int coin, int energy, bool isPhaseUP){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    _rating = rating;
    _coin = coin;
    _energy = energy;
    _isPhaseUP = isPhaseUP;
    
    lingquBool = false;
    logic_open_bool = false;
    logic_end_Bool = false;
    animationBool = false;
    
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
    if (logic_open_bool) {
        if (!logic_end_Bool) {
            if (wordCount < contentLength) {
                wordCount = getContentLength();
            }
        }else{
            if (!lingquBool) {
                // 给个提示
                CCLog("<><><>给个领取提示");
            }else{
                CCLog("<><><>exit()");
                exit();
            }
        }
    }

    return true;
}

void TaskSettlementLayer2::exit() {
    CCLayer* layer = TaskScene::create(_isPhaseUP);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
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
    renSpr->setVisible(false);
    this->addChild(renSpr, 20);
    
    // 下框
    CCSprite* kuangSpr1 = CCSprite::create("res/pic/taskSettlement/ts_kuang1.png");
    kuangSpr1->setPosition(ccp(renSpr->getContentSize().width + kuangSpr1->getContentSize().width* .33f, renSpr->getContentSize().height* .3f));
    renSpr->addChild(kuangSpr1);
    
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TaskSettlementLayer2::updataNumber));
    CCMoveTo* moveTo = CCMoveTo::create(.3f, ccp(DISPLAY->ScreenWidth()* .19f, DISPLAY->ScreenHeight()* .6f));
    CCScaleTo* scaleTo = CCScaleTo::create(.3f, 1.f);
    CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
    renSpr->runAction(CCSequence::create(CCShow::create(), spawn, callFunc, NULL));
    
    
    if (_rating == 5) {
        randIndex1 = 80 + rand()%20;
        randIndex2 = 80 + rand()%20;
        randIndex3 = 80 + rand()%20;
        randIndex4 = 80 + rand()%20;
        tishiStr = CCString::createWithFormat("不错不错.很是大方得体.");
        tishiStr->retain();
    }else if (_rating == 4) {
        randIndex1 = 60 + rand()%20;
        randIndex2 = 60 + rand()%20;
        randIndex3 = 60 + rand()%20;
        randIndex4 = 60 + rand()%20;
        tishiStr = CCString::createWithFormat("还可以吧.吐槽君还是可以接受的.");
        tishiStr->retain();
    }else if (_rating == 3) {
        randIndex1 = 40 + rand()%20;
        randIndex2 = 40 + rand()%20;
        randIndex3 = 40 + rand()%20;
        randIndex4 = 40 + rand()%20;
        tishiStr = CCString::createWithFormat("你的审美是那个星系的~!不敢恭维.");
        tishiStr->retain();
    }else if (_rating == 2) {
        randIndex1 = 20 + rand()%20;
        randIndex2 = 20 + rand()%20;
        randIndex3 = 20 + rand()%20;
        randIndex4 = 20 + rand()%20;
        tishiStr = CCString::createWithFormat("你这样,还处的去门嘛.");
        tishiStr->retain();
    }else if (_rating == 1) {
        randIndex1 = 2 + rand()%20;
        randIndex2 = 2 + rand()%20;
        randIndex3 = 2 + rand()%20;
        randIndex4 = 2 + rand()%20;
        tishiStr = CCString::createWithFormat("你光着屁股出门都比这一身强.");
        tishiStr->retain();
    }
    this->init(tishiStr->getCString());
    wordCount = 0;
    
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
    
    CCSprite* shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share.png");
    CCSprite* shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share.png");
    shareSpr2->setScale(1.02f);
    shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(TaskSettlementLayer2::shareCallBack));
    shareItem->setAnchorPoint(ccp(0, .5f));
    shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
    CCMenu* shareMenu = CCMenu::create(shareItem, NULL);
    shareMenu->setPosition(CCPointZero);
    this->addChild(shareMenu, 20);
    
    coinSpr->setVisible(false);
    lingquItem->setVisible(false);
    shareItem->setVisible(false);
    
    CCSprite* bgSpr2 = CCSprite::create("res/pic/taskSettlement/ts_bg2.png");
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr2, 20);
}
void TaskSettlementLayer2::updataNumber(){
    SPECIAL->showPetal2At(this, DISPLAY->center(), 15);
    
    taskLabel1->setVisible(true);
    taskLabel2->setVisible(true);
    taskLabel3->setVisible(true);
    taskLabel4->setVisible(true);
    
    CCString* labelStr1 = CCString::createWithFormat("%d", randIndex1);
    taskLabel1->set_new_number(labelStr1->getCString());
    CCString* labelStr2 = CCString::createWithFormat("%d", randIndex2);
    taskLabel2->set_new_number2(labelStr2->getCString());
    CCString* labelStr3 = CCString::createWithFormat("%d", randIndex3);
    taskLabel3->set_new_number3(labelStr3->getCString());
    CCString* labelStr4 = CCString::createWithFormat("%d", randIndex4);
    taskLabel4->set_new_number4(labelStr4->getCString());
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TaskSettlementLayer2::creatXingXing));
    CCMoveTo* moveTo1 = CCMoveTo::create(.3f, ccp(DISPLAY->ScreenWidth()* .19f, DISPLAY->ScreenHeight()* .13f - 10));
    CCMoveTo* moveTo2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .19f, DISPLAY->ScreenHeight()* .13f + 5));
    CCMoveTo* moveTo3 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .19f, DISPLAY->ScreenHeight()* .13f));
    
    renSpr->runAction(CCSequence::create(CCDelayTime::create(.5f), moveTo1, moveTo2, moveTo3, CCDelayTime::create(.5f), callFunc, NULL));
}
void TaskSettlementLayer2::creatXingXing(){
    _rating = 5;
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TaskSettlementLayer2::openLogic));
    
    if (_rating == 5) {
        xingSpr1 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr1->setPosition(ccp(tiaoSpr->getContentSize().width* .2f, tiaoSpr->getContentSize().height* .87f));
        xingSpr1->setRotation(-15.f);
        xingSpr1->setScale(.5f);
        tiaoSpr->addChild(xingSpr1);
        
        xingSpr2 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr2->setPosition(ccp(tiaoSpr->getContentSize().width* .35f, tiaoSpr->getContentSize().height* .94f));
        xingSpr2->setRotation(-8.f);
        xingSpr2->setScale(.5f);
        tiaoSpr->addChild(xingSpr2);
        
        xingSpr3 = CCSprite::create("res/pic/taskSettlement/ts_xing1.png");
        xingSpr3->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .99f));
        xingSpr3->setScale(.5f);
        tiaoSpr->addChild(xingSpr3);
        
        xingSpr4 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr4->setPosition(ccp(tiaoSpr->getContentSize().width* .65f, tiaoSpr->getContentSize().height* .94f));
        xingSpr4->setRotation(8.f);
        xingSpr4->setScale(.5f);
        tiaoSpr->addChild(xingSpr4);
        
        xingSpr5 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr5->setPosition(ccp(tiaoSpr->getContentSize().width* .8f, tiaoSpr->getContentSize().height* .87f));
        xingSpr5->setRotation(15.f);
        xingSpr5->setScale(.5f);
        tiaoSpr->addChild(xingSpr5);
        
        xingSpr1->setVisible(false);
        xingSpr2->setVisible(false);
        xingSpr3->setVisible(false);
        xingSpr4->setVisible(false);
        xingSpr5->setVisible(false);
        
        for (int i = 1; i <= 5; i++) {
            CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.4f);
            CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, .95f);
            CCScaleTo* scaleTo3 = CCScaleTo::create(.1f, 1.f);
            float time = i* .1f;
            if (i == 1) {
                xingSpr3->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 2){
                xingSpr2->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 3){
                xingSpr4->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 4){
                xingSpr1->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 5){
                xingSpr5->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, callFunc, NULL));
            }
        }
    }else if (_rating == 4){
        xingSpr1 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr1->setPosition(ccp(tiaoSpr->getContentSize().width* .275f, tiaoSpr->getContentSize().height* .92f));
        xingSpr1->setRotation(-13.f);
        xingSpr1->setScale(.5f);
        tiaoSpr->addChild(xingSpr1);
        
        xingSpr2 = CCSprite::create("res/pic/taskSettlement/ts_xing1.png");
        xingSpr2->setPosition(ccp(tiaoSpr->getContentSize().width* .425f, tiaoSpr->getContentSize().height* .985f));
        xingSpr2->setRotation(-3.f);
        xingSpr2->setScale(.5f);
        tiaoSpr->addChild(xingSpr2);
        
        xingSpr3 = CCSprite::create("res/pic/taskSettlement/ts_xing1.png");
        xingSpr3->setPosition(ccp(tiaoSpr->getContentSize().width* .575f, tiaoSpr->getContentSize().height* .985f));
        xingSpr3->setRotation(3.f);
        xingSpr3->setScale(.5f);
        tiaoSpr->addChild(xingSpr3);
        
        xingSpr4 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr4->setPosition(ccp(tiaoSpr->getContentSize().width* .725f, tiaoSpr->getContentSize().height* .92f));
        xingSpr4->setRotation(13.f);
        xingSpr4->setScale(.5f);
        tiaoSpr->addChild(xingSpr4);
        
        xingSpr1->setVisible(false);
        xingSpr2->setVisible(false);
        xingSpr3->setVisible(false);
        xingSpr4->setVisible(false);
        
        for (int i = 1; i <= 4; i++) {
            CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.4f);
            CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, .95f);
            CCScaleTo* scaleTo3 = CCScaleTo::create(.1f, 1.f);
            float time = i* .1f;
            if (i == 1) {
                xingSpr2->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 2){
                xingSpr3->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 3){
                xingSpr1->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 4){
                xingSpr4->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, callFunc, NULL));
            }
        }
    }else if (_rating == 3){
        xingSpr1 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr1->setPosition(ccp(tiaoSpr->getContentSize().width* .35f, tiaoSpr->getContentSize().height* .94f));
        xingSpr1->setRotation(-8.f);
        xingSpr1->setScale(.5f);
        tiaoSpr->addChild(xingSpr1);
        
        xingSpr2 = CCSprite::create("res/pic/taskSettlement/ts_xing1.png");
        xingSpr2->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .99f));
        xingSpr2->setScale(.5f);
        tiaoSpr->addChild(xingSpr2);
        
        xingSpr3 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr3->setPosition(ccp(tiaoSpr->getContentSize().width* .65f, tiaoSpr->getContentSize().height* .94f));
        xingSpr3->setRotation(8.f);
        xingSpr3->setScale(.5f);
        tiaoSpr->addChild(xingSpr3);
        
        xingSpr1->setVisible(false);
        xingSpr2->setVisible(false);
        xingSpr3->setVisible(false);
        
        for (int i = 1; i <= 3; i++) {
            CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.4f);
            CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, .95f);
            CCScaleTo* scaleTo3 = CCScaleTo::create(.1f, 1.f);
            float time = i* .1f;
            if (i == 1) {
                xingSpr2->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 2){
                xingSpr1->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 3){
                xingSpr3->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, callFunc, NULL));
            }
        }
    }else if (_rating == 2){
        xingSpr1 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr1->setPosition(ccp(tiaoSpr->getContentSize().width* .425f, tiaoSpr->getContentSize().height* .97f));
        xingSpr1->setRotation(-3.f);
        xingSpr1->setScale(.5f);
        tiaoSpr->addChild(xingSpr1);
        
        xingSpr2 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr2->setPosition(ccp(tiaoSpr->getContentSize().width* .575f, tiaoSpr->getContentSize().height* .97f));
        xingSpr2->setRotation(3.f);
        xingSpr2->setScale(.5f);
        tiaoSpr->addChild(xingSpr2);
        
        xingSpr1->setVisible(false);
        xingSpr2->setVisible(false);
        
        for (int i = 1; i <= 2; i++) {
            CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.4f);
            CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, .95f);
            CCScaleTo* scaleTo3 = CCScaleTo::create(.1f, 1.f);
            float time = i* .1f;
            if (i == 1) {
                xingSpr1->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, NULL));
            }else if (i == 2){
                xingSpr2->runAction(CCSequence::create(CCDelayTime::create(time), CCShow::create(), scaleTo1, scaleTo2, scaleTo3, callFunc, NULL));
            }
        }
    }else if (_rating == 1){
        xingSpr1 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr1->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .975f));
        xingSpr1->setScale(.5f);
        tiaoSpr->addChild(xingSpr1);
        
        xingSpr1->setVisible(false);
        
        CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.4f);
        CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, .95f);
        CCScaleTo* scaleTo3 = CCScaleTo::create(.1f, 1.f);
        xingSpr1->runAction(CCSequence::create(CCShow::create(), scaleTo1, scaleTo2, scaleTo3, callFunc, NULL));
        
    }else{
        _rating = 1;
        xingSpr1 = CCSprite::create("res/pic/taskSettlement/ts_xing2.png");
        xingSpr1->setPosition(ccp(tiaoSpr->getContentSize().width* .5f, tiaoSpr->getContentSize().height* .975f));
        xingSpr1->setScale(.5f);
        tiaoSpr->addChild(xingSpr1);
        
        xingSpr1->setVisible(false);
        
        CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.4f);
        CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, .95f);
        CCScaleTo* scaleTo3 = CCScaleTo::create(.1f, 1.f);
        xingSpr1->runAction(CCSequence::create(CCShow::create(), scaleTo1, scaleTo2, scaleTo3, callFunc, NULL));
    }
    
    SPECIAL->showPetal2At(this, DISPLAY->center(), 1);
}
void TaskSettlementLayer2::openLogic(){
    logic_open_bool = true;
    this->schedule(schedule_selector(TaskSettlementLayer2::logic), .1f);
}
void TaskSettlementLayer2::logic(float dt){
    this->unschedule(schedule_selector(TaskSettlementLayer2::logic));
    
    if (wordCount > contentLength) {
        wordCount++;
        
        logic_end_Bool = true;
        tishiLabel->setString(getContentByLength(wordCount).c_str());
        
        coinSpr->setVisible(true);
        lingquItem->setVisible(true);
//        shareItem->setVisible(true);
        
        return;
    }else{
        wordCount++;
        
        tishiLabel->setString(getContentByLength(wordCount).c_str());
        
        this->schedule(schedule_selector(TaskSettlementLayer2::logic), .1f);
    }
}

void TaskSettlementLayer2::init(std::string str){
    content = str;
    contentLength = 0;
    
    int length = content.length();
    int i = 0;
    while (i < length) {
        char ch = getContent()[i];
        
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        contentLength++;
    }
}

std::string TaskSettlementLayer2::getContentByLength(int length){
    if (length >= contentLength) {
        return getContent();
    }
    
    int i = 0;
    int index = 0;
    while (index < length) {
        char ch = getContent()[i];
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        index++;
    }
    std::string str = content.substr(0, i);
//    MZLog("index ======= %d", index);
    return str;
}
int TaskSettlementLayer2::getContentLength(){
    return contentLength;
}


void TaskSettlementLayer2::lingquCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    item->setEnabled(false);
    
    lingquBool = true;
    SPECIAL->show_coin_reward(this, _coin, ccp(DISPLAY->halfW() + 200, DISPLAY->H() * 0.15), ccp(DISPLAY->halfW() + 150, DISPLAY->H() * 0.25));
}
void TaskSettlementLayer2::shareCallBack(CCObject* pSender){
    
}

void TaskSettlementLayer2::creat_Man(){
    float widthFolt = .65f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setFlipX(flipxBool);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void TaskSettlementLayer2::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = .65f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    
    for (int i = Tag_QJ_TouFa; i <= Tag_QJ_ZhuangRong; i++) {
        if (i == Tag_QJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_QJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_QJ_TouFa2);
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
                            _tfSpr1->setTag(Tag_QJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_QJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_QJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_QJ_WaiTao1);
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
                            _wtSpr1->setTag(Tag_QJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_QJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_QJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_QJ_ShangYi1);
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
                            _sySpr1->setTag(Tag_QJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_QJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_QJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setScale(scaleFloat);
                _kzSpr1->setFlipX(flipxBool);
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _kzSpr1->setTag(Tag_QJ_KuZi1);
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
                            _kzSpr1->setTag(Tag_QJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_QJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_QJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_QJ_WaZi1);
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
                            _wzSpr1->setTag(Tag_QJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_QJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_QJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_QJ_XieZi1);
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
                            _xzSpr1->setTag(Tag_QJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_QJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_QJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShiPin){
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
        else if (i == Tag_QJ_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_QJ_Bao1);
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
                            _bSpr1->setTag(Tag_QJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_QJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_QJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_QJ_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_QJ_ZhuangRong1);
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
                            _zrSpr1->setTag(Tag_QJ_ZhuangRong1);
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

void TaskSettlementLayer2::initAniamtion(){
    
}
void TaskSettlementLayer2::startAnimation(float dt){
    
}






