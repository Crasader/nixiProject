//
//  GashaponPanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "GashaponPanel.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "TransactionScene.h"
#include "PromptLayer.h"
#include "Loading2.h"

#pragma mark - Export

void GashaponPanel::show() {
    GashaponPanel* panel = GashaponPanel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}

#pragma mark - Supper

GashaponPanel::~GashaponPanel() {
    
}

bool GashaponPanel::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("res/pic/panel/gashapon/gashapon_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        _content->addChild(txt_close);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        return true;
    }
    else {
        return false;
    }
}

void GashaponPanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&GashaponPanel::nc_take_energy_301), "HTTP_FINISHED_301", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&GashaponPanel::keyBackStatus), .8f);
}

void GashaponPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void GashaponPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

bool GashaponPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        remove();
    }
    
    return true;
}

#pragma mark - inner

void GashaponPanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void GashaponPanel::on_purchase() {
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_PURCHASEPANEL");
}

void GashaponPanel::on_purchase_achievement() {

}

void GashaponPanel::on_signin7() {
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_SIGNIN7");
}

void GashaponPanel::on_take_energy(CCMenuItem *btn) {
    LOADING->show_loading();
    NET->take_energy_reward_301();
}

void GashaponPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1) {
        return;
    }
    
    this->remove();
}

void GashaponPanel::nc_take_energy_301(CCObject *pObj) {

}

