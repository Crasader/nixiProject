//
//  MonthCardLayer.cpp
//  tiegao
//
//  Created by mac on 17-3-16.
//
//

#include "MonthCardLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "NetManager.h"
#include "MainScene.h"
#include "Loading2.h"

#include "MZResourceLoader.h"
#include "AudioManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "BuildingLayer.h"
#include "TDCCAccount.h"
#include "TDCCTalkingDataGA.h"
#include "JNIController.h"



MonthCardLayer::MonthCardLayer(){
    
}
MonthCardLayer::~MonthCardLayer(){
    
}

void MonthCardLayer::onEnter(){
    CCLayer::onEnter();
    
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&GashaponScene::backCallBack), "GashaponBack", NULL);
//    nc->addObserver(this, SEL_CallFuncO(&GashaponScene::creat_Exchange), "Creat_Exchange", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&MonthCardLayer::keyBackStatus), .8f);
}
void MonthCardLayer::onExit(){
    
    
    CCLayer::onExit();
}
void MonthCardLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void MonthCardLayer::keyBackClicked(){
    num_child++;
//    CCLog("===== GashaponScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    if (DATA->current_guide_step() == 0) {
        this->removeFromParentAndCleanup(true);
    }
}

bool MonthCardLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    
    CCSprite* diSpr = CCSprite::create("res/pic/panel/newSignin7/newSignin_di.png");
    diSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(diSpr);
    
    kuangSpr = CCSprite::create("res/pic/panel/month/month_dikuang.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(kuangSpr);
    
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->creat_view();
    
    return true;
}

bool MonthCardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    if (! kuangSpr->boundingBox().containsPoint(location)) {
        AUDIO->goback_effect();
        
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}


void MonthCardLayer::creat_view(){
    
    // 钻石卡
    CCSprite* goldKuangSpr = CCSprite::create("res/pic/panel/month/month_gold.png");
    goldKuangSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .76f));
    kuangSpr->addChild(goldKuangSpr);
    CCSprite* goldButtonSpr1 = CCSprite::create("res/pic/panel/month/month_button.png");
    CCSprite* goldButtonSpr2 = CCSprite::create("res/pic/panel/month/month_button.png");
    goldButtonSpr2->setScale(1.02f);
    CCMenuItem* goldButton = CCMenuItemSprite::create(goldButtonSpr1, goldButtonSpr2, this, menu_selector(MonthCardLayer::goldButtonCallBack));
    goldButton->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .56f));
    CCSprite* _888Spr = CCSprite::create("res/pic/panel/month/month_888.png");
    _888Spr->setPosition(ccp(goldButton->getContentSize().width* .5f, goldButton->getContentSize().height* .48f));
    goldButton->addChild(_888Spr);
    
    goldTishiSpr = CCSprite::create("res/pic/panel/month/month_tishi2.png");
    goldTishiSpr->setAnchorPoint(ccp(.5f, 1));
    goldTishiSpr->setPosition(ccp(goldKuangSpr->getContentSize().width* .53f, goldKuangSpr->getContentSize().height - 14));
    goldKuangSpr->addChild(goldTishiSpr);
    CCLabelTTF* goldLabel = CCLabelTTF::create("9", DISPLAY->fangzhengFont(), 25);
    goldLabel->setPosition(ccp(goldTishiSpr->getContentSize().width* .52f, goldTishiSpr->getContentSize().height* .68f));
    goldLabel->setColor(ccRED);
    goldTishiSpr->addChild(goldLabel);
    
    
    // rmb卡
    CCSprite* moneyKuangSpr = CCSprite::create("res/pic/panel/month/month_money.png");
    moneyKuangSpr->setPosition(ccp(kuangSpr->getContentSize().width* .525f, kuangSpr->getContentSize().height* .32f));
    kuangSpr->addChild(moneyKuangSpr);
    CCSprite* moneyButtonSpr1 = CCSprite::create("res/pic/panel/month/month_button.png");
    CCSprite* moneyButtonSpr2 = CCSprite::create("res/pic/panel/month/month_button.png");
    moneyButtonSpr2->setScale(1.02f);
    CCMenuItem* moneyButton = CCMenuItemSprite::create(moneyButtonSpr1, moneyButtonSpr2, this, menu_selector(MonthCardLayer::moneyButtonCallBack));
    moneyButton->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .12f));
    CCSprite* _30Spr = CCSprite::create("res/pic/panel/month/month_30.png");
    _30Spr->setPosition(ccp(moneyButton->getContentSize().width* .5f, moneyButton->getContentSize().height* .48f));
    moneyButton->addChild(_30Spr);
    
    moneyTishiSpr = CCSprite::create("res/pic/panel/month/month_tishi2.png");
    moneyTishiSpr->setAnchorPoint(ccp(.5f, 1));
    moneyTishiSpr->setPosition(ccp(moneyKuangSpr->getContentSize().width* .5f, moneyKuangSpr->getContentSize().height - 8));
    moneyKuangSpr->addChild(moneyTishiSpr);
    CCLabelTTF* moneyLabel = CCLabelTTF::create("1", DISPLAY->fangzhengFont(), 25);
    moneyLabel->setPosition(ccp(moneyTishiSpr->getContentSize().width* .52f, moneyTishiSpr->getContentSize().height* .68f));
    moneyLabel->setColor(ccRED);
    moneyTishiSpr->addChild(moneyLabel);
    
    
    CCMenu* menu = CCMenu::create(goldButton, moneyButton, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu);
    
    
    
    
    
}
void MonthCardLayer::goldButtonCallBack(CCObject* pSender){
    
}
void MonthCardLayer::moneyButtonCallBack(CCObject* pSender){
    
}





