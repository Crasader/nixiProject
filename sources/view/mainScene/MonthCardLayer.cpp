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
    nc->addObserver(this, menu_selector(MonthCardLayer::_151Callback), "HTTP_FINISHED_151", NULL);
    nc->addObserver(this, menu_selector(MonthCardLayer::_153Callback), "HTTP_FINISHED_153", NULL);
    nc->addObserver(this, menu_selector(MonthCardLayer::_157Callback), "HTTP_FINISHED_157", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&MonthCardLayer::keyBackStatus), .8f);
}
void MonthCardLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
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
    
    PurchaseComp* purchase = DATA->getPurchase();
    goldCardItem = purchase->getMonthlyCard1();
    moneyCardItem = purchase->getMonthlyCard2();
    
    
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
    goldKuangSpr = CCSprite::create("res/pic/panel/month/month_gold.png");
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
    
    this->gold_view();
    
    
    // rmb卡
    moneyKuangSpr = CCSprite::create("res/pic/panel/month/month_money.png");
    moneyKuangSpr->setPosition(ccp(kuangSpr->getContentSize().width* .525f, kuangSpr->getContentSize().height* .32f));
    kuangSpr->addChild(moneyKuangSpr);
    moneyTishiLabel = CCLabelTTF::create("请先领取邮件.", DISPLAY->fangzhengFont(), 20);
    moneyTishiLabel->setAnchorPoint(ccp(1, 1));
    moneyTishiLabel->setPosition(ccp(moneyKuangSpr->getContentSize().width - 6, -1));
    moneyTishiLabel->setColor(ccRED);
    moneyKuangSpr->addChild(moneyTishiLabel);
    moneyTishiLabel->setVisible(false);
    
    CCSprite* moneyButtonSpr1 = CCSprite::create("res/pic/panel/month/month_button.png");
    CCSprite* moneyButtonSpr2 = CCSprite::create("res/pic/panel/month/month_button.png");
    moneyButtonSpr2->setScale(1.02f);
    CCMenuItem* moneyButton = CCMenuItemSprite::create(moneyButtonSpr1, moneyButtonSpr2, this, menu_selector(MonthCardLayer::moneyButtonCallBack));
    moneyButton->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .12f));
    CCSprite* _30Spr = CCSprite::create("res/pic/panel/month/month_30.png");
    _30Spr->setPosition(ccp(moneyButton->getContentSize().width* .5f, moneyButton->getContentSize().height* .48f));
    moneyButton->addChild(_30Spr);
    
    this->money_view();
    
    
    CCMenu* menu = CCMenu::create(goldButton, moneyButton, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu);
    
    
    
    this->creat_lingqu();
    
}
void MonthCardLayer::gold_view(){
    // 剩余天数
    if (goldCardItem->getDaysRest() > 0) {
        
        goldTishiSpr = CCSprite::create("res/pic/panel/month/month_tishi2.png");
        goldTishiSpr->setAnchorPoint(ccp(.5f, 1));
        goldTishiSpr->setPosition(ccp(goldKuangSpr->getContentSize().width* .53f, goldKuangSpr->getContentSize().height - 14));
        goldKuangSpr->addChild(goldTishiSpr);
        
        CCString* goldStr = CCString::createWithFormat("%d", goldCardItem->getDaysRest());
        CCLabelTTF* goldLabel = CCLabelTTF::create(goldStr->getCString(), DISPLAY->fangzhengFont(), 25);
        goldLabel->setPosition(ccp(goldTishiSpr->getContentSize().width* .52f, goldTishiSpr->getContentSize().height* .68f));
        goldLabel->setColor(ccRED);
        goldTishiSpr->addChild(goldLabel);
    }
}
void MonthCardLayer::money_view(){
    // 剩余天数
    if (moneyCardItem->getDaysRest() > 0) {
        
        CCString* moneyStr = CCString::createWithFormat("%d", moneyCardItem->getDaysRest());
        moneyTishiSpr = CCSprite::create("res/pic/panel/month/month_tishi2.png");
        moneyTishiSpr->setAnchorPoint(ccp(.5f, 1));
        moneyTishiSpr->setPosition(ccp(moneyKuangSpr->getContentSize().width* .5f, moneyKuangSpr->getContentSize().height - 8));
        moneyKuangSpr->addChild(moneyTishiSpr);
        
        CCLabelTTF* moneyLabel = CCLabelTTF::create(moneyStr->getCString(), DISPLAY->fangzhengFont(), 25);
        moneyLabel->setPosition(ccp(moneyTishiSpr->getContentSize().width* .52f, moneyTishiSpr->getContentSize().height* .68f));
        moneyLabel->setColor(ccRED);
        moneyTishiSpr->addChild(moneyLabel);
        
        moneyTishiLabel->setVisible(false);
    }
}
void MonthCardLayer::creat_lingqu(){
    // 领取按钮
    CCSprite* goldLingquSpr1 = CCSprite::create("res/pic/building/btn_take.png");
    CCSprite* goldLingquSpr2 = CCSprite::create("res/pic/building/btn_take.png");
    goldLingquSpr2->setScale(1.02f);
    goldLingquItem = CCMenuItemSprite::create(goldLingquSpr1, goldLingquSpr2, this, SEL_MenuHandler(&MonthCardLayer::goldLingquCallback));
    goldLingquItem->setPosition(ccp(kuangSpr->getContentSize().width* .85f, kuangSpr->getContentSize().height* .56f));
    
    if (goldCardItem->getStatus() == 0) {// 0-未获得
        goldLingquItem->setVisible(false);
    }else if (goldCardItem->getStatus() == 1){// 1-可领取
        goldLingquItem->setVisible(true);
        CCScaleTo* goldScaleTo1 = CCScaleTo::create(.5f, 1.1f);
        CCScaleTo* goldScaleTo2 = CCScaleTo::create(.5f, 1.f);
        CCSequence* goldSeq = CCSequence::create(goldScaleTo1, goldScaleTo2, NULL);
        goldLingquItem->runAction(CCRepeatForever::create(goldSeq));
    }else if (goldCardItem->getStatus() == 2){// 2-当日已领取
        goldLingquItem->setVisible(true);
        goldLingquItem->setColor(ccGRAY);
    }
    
    
    
    // 领取按钮
    CCSprite* moneyLingquSpr1 = CCSprite::create("res/pic/building/btn_take.png");
    CCSprite* moneyLingquSpr2 = CCSprite::create("res/pic/building/btn_take.png");
    moneyLingquSpr2->setScale(1.02f);
    moneyLingquItem = CCMenuItemSprite::create(moneyLingquSpr1, moneyLingquSpr2, this, SEL_MenuHandler(&MonthCardLayer::moneyLingquCallback));
    moneyLingquItem->setPosition(ccp(kuangSpr->getContentSize().width* .85f, kuangSpr->getContentSize().height* .12f));
    
    if (moneyCardItem->getStatus() == 0) {// 0-未获得
        moneyLingquItem->setVisible(false);
    }else if (moneyCardItem->getStatus() == 1){// 1-可领取
        moneyLingquItem->setVisible(true);
        CCScaleTo* moneyScaleTo1 = CCScaleTo::create(.5f, 1.1f);
        CCScaleTo* moneyScaleTo2 = CCScaleTo::create(.5f, 1.f);
        CCSequence* moneySeq = CCSequence::create(moneyScaleTo1, moneyScaleTo2, NULL);
        moneyLingquItem->runAction(CCRepeatForever::create(moneySeq));
    }else if (moneyCardItem->getStatus() == 2){// 2-当日已领取
        moneyLingquItem->setVisible(true);
        moneyLingquItem->setColor(ccGRAY);
    }
    
    
    lingquMenu = CCMenu::create(goldLingquItem, moneyLingquItem, NULL);
    lingquMenu->setPosition(CCPointZero);
    lingquMenu->setTag(0x656565);
    kuangSpr->addChild(lingquMenu);
}


void MonthCardLayer::goldButtonCallBack(CCObject* pSender){
    
    LOADING->show_loading();
    NET->buy_monthly_card1_151();
}
void MonthCardLayer::_151Callback(CCObject* pObj){
    LOADING->remove();
    
    CCLog("_151Callback_________1");
    PurchaseComp* purchase = DATA->getPurchase();
    goldCardItem = purchase->getMonthlyCard1();
    moneyCardItem = purchase->getMonthlyCard2();
    CCLog("_151Callback_________2");
    if (goldCardItem->getStatus() == 0) {// 0-未获得
        goldLingquItem->setVisible(false);
        CCLog("_151Callback_________2_1");
    }else if (goldCardItem->getStatus() == 1){// 1-可领取
        goldLingquItem->setVisible(true);
        CCScaleTo* goldScaleTo1 = CCScaleTo::create(.5f, 1.1f);
        CCScaleTo* goldScaleTo2 = CCScaleTo::create(.5f, 1.f);
        CCSequence* goldSeq = CCSequence::create(goldScaleTo1, goldScaleTo2, NULL);
        goldLingquItem->runAction(CCRepeatForever::create(goldSeq));
        CCLog("_151Callback_________2_2");
    }else if (goldCardItem->getStatus() == 2){// 2-当日已领取
        goldLingquItem->setVisible(true);
        goldLingquItem->setColor(ccGRAY);
        CCLog("_151Callback_________2_3");
    }
    CCLog("_151Callback_________3");
    
    goldKuangSpr->removeAllChildren();
    this->gold_view();
    CCLog("_151Callback_________4");
    moneyKuangSpr->removeAllChildren();
    this->money_view();
    CCLog("_151Callback_________5");
    
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney", NULL);
    PromptLayer* layer = PromptLayer::create();
    layer->show_prompt(this->getScene(), "购买成功");
}
void MonthCardLayer::moneyButtonCallBack(CCObject* pSender){
    if (CONFIG->baiOrYijie == 0) {// 白包
        if (CONFIG->openPay == 0) {
            // talkingData
            DATA->onEvent("支付意向", "支付界面", "点击购买钻石");
        }else if (CONFIG->openPay == 1){
            LOADING->show_loading();
            
        }
    }else if (CONFIG->baiOrYijie == 1 || CONFIG->baiOrYijie == 2 || CONFIG->baiOrYijie == 3){// 易接
        if (CONFIG->openPay == 0) {
            // talkingData
            DATA->onEvent("支付意向", "支付界面", "点击购买钻石");
        }else if (CONFIG->openPay == 1){
            LOADING->show_loading();
            
            JNIController::setChannelId(CONFIG->channelId);
            JNIController::setMoneyStatus(30 * 100);
            JNIController::setGoldStatus(0);
            JNIController::setPlayerName(DATA->getShow()->nickname());
            JNIController::setSidId(DATA->getLogin()->obtain_sid());
            JNIController::setProductId("tiegao_9");
            JNIController::isGamePay(9);
            
            this->schedule(schedule_selector(MonthCardLayer::updatePay), 1.f);
        }
    }
}
void MonthCardLayer::updatePay(float dt){
    LOADING->remove();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (JNIController::getSmsStatus() == 1) {
        JNIController::setSmsStatus(0);
        CCLog("<><><><><><> updatePay");
        this->unschedule(SEL_SCHEDULE(&MonthCardLayer::updatePay));
        
        string orderId = JNIController::getCpOrderId();
        string productId = JNIController::getProductId();
        CCString* iapId = CCString::createWithFormat("包月");
        
        DATA->onChargeRequest(orderId, iapId->getCString(), JNIController::getMoneyStatus()/100, 0);
        
        LOADING->show_loading();
        this->scheduleOnce(SEL_SCHEDULE(&MonthCardLayer::sendPay), 2.f);
    }else if (JNIController::getSmsStatus() == 2) {
        LOADING->remove();
        
        CCUserDefault::sharedUserDefault()->setStringForKey("CpOrderId", "");
        CCUserDefault::sharedUserDefault()->setIntegerForKey("Product_Index", 100);
        CCUserDefault::sharedUserDefault()->setBoolForKey("PayBool", false);
        JNIController::setSmsStatus(0);
        this->unschedule(SEL_SCHEDULE(&MonthCardLayer::updatePay));
        
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this->getParent(), "支付失败");
    }
#endif
}
void MonthCardLayer::sendPay(float dt){
    LOADING->remove();
    
    string orderId = JNIController::getCpOrderId();
    DATA->onChargeSuccess(orderId);
    
    moneyTishiLabel->setVisible(true);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "购买结果请稍后去邮件确认.");
    
    LOADING->show_loading();
    this->scheduleOnce(SEL_SCHEDULE(&MonthCardLayer::closeMonthCardLayer), 1.f);
}
void MonthCardLayer::closeMonthCardLayer(float dt){
    LOADING->remove();
    this->removeFromParentAndCleanup(true);
}


void MonthCardLayer::goldLingquCallback(CCObject* pSender){
    if (goldCardItem->getStatus() == 1){
        LOADING->show_loading();
        NET->take_monthly_card1_daily_reward_153();
    }else if (goldCardItem->getStatus() == 2){
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(this->getScene(), "已领取");
    }
}
void MonthCardLayer::_153Callback(CCObject* pObj){
    LOADING->remove();
    
    PurchaseComp* purchase = DATA->getPurchase();
    goldCardItem = purchase->getMonthlyCard1();
    moneyCardItem = purchase->getMonthlyCard2();
    
    if (goldCardItem->getStatus() == 0) {// 0-未获得
        goldLingquItem->setVisible(false);
    }else if (goldCardItem->getStatus() == 1){// 1-可领取
        goldLingquItem->setVisible(true);
    }else if (goldCardItem->getStatus() == 2){// 2-当日已领取
        goldLingquItem->setVisible(true);
        goldLingquItem->setColor(ccGRAY);
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney", NULL);
    PromptLayer* layer = PromptLayer::create();
    layer->show_prompt(this->getScene(), "领取成功");
}


void MonthCardLayer::moneyLingquCallback(CCObject* pSender){
    if (moneyCardItem->getStatus() == 1){
        LOADING->show_loading();
        NET->take_monthly_card2_daily_reward_157();
    }else if (moneyCardItem->getStatus() == 2){
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(this->getScene(), "已领取");
    }
}
void MonthCardLayer::_157Callback(CCObject* pObj){
    LOADING->remove();
    
    PurchaseComp* purchase = DATA->getPurchase();
    goldCardItem = purchase->getMonthlyCard1();
    moneyCardItem = purchase->getMonthlyCard2();
    
    if (moneyCardItem->getStatus() == 0) {// 0-未获得
        moneyLingquItem->setVisible(false);
    }else if (moneyCardItem->getStatus() == 1){// 1-可领取
        moneyLingquItem->setVisible(true);
    }else if (moneyCardItem->getStatus() == 2){// 2-当日已领取
        moneyLingquItem->setVisible(true);
        moneyLingquItem->setColor(ccGRAY);
    }
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney", NULL);
    PromptLayer* layer = PromptLayer::create();
    layer->show_prompt(this->getScene(), "领取成功");
}

