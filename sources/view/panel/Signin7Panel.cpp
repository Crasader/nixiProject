//
//  Signin7Panel.cpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#include "Signin7Panel.h"
//
//  Signin7Panel.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "Signin7Panel.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "DataManager.h"

#pragma mark - Export

void Signin7Panel::show() {
    Signin7Panel* panel = Signin7Panel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel, 1000);
}


#pragma mark - Super

Signin7Panel::~Signin7Panel() {
}

bool Signin7Panel::init() {
    if (CCLayer::init()) {
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("pic/panel/signin7/signin7_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        CCSize panelSize = _panel->boundingBox().size;
        
        CCSprite* text_spr = CCSprite::create("pic/panel/signin7/text_reward.png");
        text_spr->setPosition(ccp(panelSize.width* .5f, panelSize.height* .9f));
        _panel->addChild(text_spr);
        
        CCSprite* model = CCSprite::create("pic/panel/signin7/si_model.png");
        model->setPosition(ccp(panelSize.width, panelSize.height * 0.5));
        _panel->addChild(model);
        
        
        
        
        return true;
    }
    else {
        return false;
    }
}

void Signin7Panel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->scheduleOnce(SEL_SCHEDULE(&Signin7Panel::keyBackStatus), .8f);
}
void Signin7Panel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void Signin7Panel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool Signin7Panel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

#pragma mark - Inner

void Signin7Panel::remove() {
    this->removeFromParentAndCleanup(true);
}

void Signin7Panel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}
