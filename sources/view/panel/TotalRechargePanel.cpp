//
//  TotalRechargePanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/8/5.
//
//

#include "TotalRechargePanel.h"
#include "DisplayManager.h"


#pragma mark - Export

void TotalRechargePanel::show() {
    TotalRechargePanel* panel = TotalRechargePanel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel, 1000);
}


#pragma mark - Super

TotalRechargePanel::~TotalRechargePanel() {
}

bool TotalRechargePanel::init(){
    if (!CCLayer::init()) {
        return false;
    }
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    _content = CCLayer::create();
    this->addChild(_content);
    
    _panel = CCSprite::create("pic/panel/signin7/signin7_panel.png");
    _panel->setPosition(DISPLAY->center());
    _content->addChild(_panel);
    
    CCSize panelSize = _panel->boundingBox().size;
    
    
    return true;
}

void TotalRechargePanel::onEnter() {
    CCLayer::onEnter();
    
//    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&Signin7Panel::signin_callback_303), "HTTP_FINISHED_303", NULL);
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->scheduleOnce(SEL_SCHEDULE(&TotalRechargePanel::keyBackStatus), .8f);
}
void TotalRechargePanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void TotalRechargePanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool TotalRechargePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

#pragma mark - Inner

void TotalRechargePanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void TotalRechargePanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}