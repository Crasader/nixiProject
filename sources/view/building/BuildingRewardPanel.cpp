//
//  BuildingRewardPanel.cpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#include "BuildingRewardPanel.h"
//
//  BuildingRewardPanel.cpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#include "BuildingRewardPanel.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "Loading2.h"
#include "PromptLayer.h"

#pragma mark - Export

void BuildingRewardPanel::show(CCNode* parent, string type, int value) {
    BuildingRewardPanel* panel = BuildingRewardPanel::createWithReward(type, value);
    parent->addChild(panel);
}


#pragma mark - Super

BuildingRewardPanel::~BuildingRewardPanel() {
}

BuildingRewardPanel* BuildingRewardPanel::createWithReward(string type, int value) {
    BuildingRewardPanel* rtn = new BuildingRewardPanel();
    if (rtn && rtn->initWithReward(type, value)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool BuildingRewardPanel::initWithReward(string type, int value) {
    if (! CCLayer::init()) {
        return false;
    }

    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    _panel = CCSprite::create("pic/building/reward/panel.png");
    _panel->setPosition(DISPLAY->center());
    this->addChild(_panel);
    
    CCSize panelSize = _panel->boundingBox().size;
    
    CCSprite* light = CCSprite::create("pic/building/reward/light.png");
    light->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
    _panel->addChild(light);
    
    CCSprite* spots = CCSprite::create("pic/building/reward/spots.png");
    spots->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
    _panel->addChild(spots);
    
    CCSequence* seq = CCSequence::create(CCFadeIn::create(0.6), CCDelayTime::create(0.3), CCFadeOut::create(0.6), CCDelayTime::create(0.5), NULL);
    spots->runAction(CCRepeatForever::create(seq));
    
    CCSprite* title = CCSprite::create("pic/building/reward/title.png");
    title->setPosition(ccp(panelSize.width* .5f, panelSize.height* .88f));
    _panel->addChild(title);
    
    if (type.compare("coin") == 0) {
        CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
        plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
        _panel->addChild(plate);
        
        CCSprite* coin = CCSprite::create("pic/common/coin2.png");
        coin->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
        _panel->addChild(coin);
        
        CCString* str = CCString::createWithFormat("%d金币", value);
        CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24);
        lbl->setColor(ccc3(107, 143, 190));
        lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.26));
        _panel->addChild(lbl);
    }
    else if (type.compare("diam") == 0) {
        
    }
    
    return true;
}

void BuildingRewardPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->scheduleOnce(SEL_SCHEDULE(&BuildingRewardPanel::keyBackStatus), .8f);
}
void BuildingRewardPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void BuildingRewardPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool BuildingRewardPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

#pragma mark - Inner

void BuildingRewardPanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void BuildingRewardPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}
