//
//  RewardPanel.cpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#include "RewardPanel.h"
//
//  RewardPanel.cpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#include "RewardPanel.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "Loading2.h"
#include "PromptLayer.h"

#pragma mark - Export

void RewardPanel::show(CCNode* parent, string type, int value) {
    RewardPanel* panel = RewardPanel::createWithReward(type, value);
    parent->addChild(panel);
}


#pragma mark - Super

RewardPanel::~RewardPanel() {
}

RewardPanel* RewardPanel::createWithMystery() {
    RewardPanel* rtn = new RewardPanel();
    if (rtn && rtn->initWithMystery()) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool RewardPanel::initWithMystery() {
    if (! CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    _couldExit = false;
    
    _type = "";
    _value = 0;
    
    _panel = CCSprite::create("pic/gashapon/gashapon_kuang.png");
    _panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75));
    _panel->setVisible(false);
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
    
//    CCSprite* title = CCSprite::create("pic/building/reward/title2.png");
//    title->setPosition(ccp(panelSize.width* .5f, panelSize.height* .92f));
//    _panel->addChild(title);
    
    CCSprite* titleSpr = CCSprite::create("res/pic/gashapon/gashapon_title2.png");
    titleSpr->setPosition(ccp(_panel->getContentSize().width* .5f, _panel->getContentSize().height* 1.02f));
    _panel->addChild(titleSpr);
    
    CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
    plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
    _panel->addChild(plate);
    
    CCSprite* coin = CCSprite::create("pic/mainScene/main_mystery.png");
    coin->setPosition(ccp(panelSize.width* .5f - 3, panelSize.height * 0.5 + 2));
    _panel->addChild(coin);
    
    CCLabelTTF* lbl = CCLabelTTF::create("解锁 －“事件”", DISPLAY->fangzhengFont(), 22);
//    lbl->setColor(ccc3(107, 143, 190));
    lbl->setAnchorPoint(ccp(0.5, 0.5));
    lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.05));
    _panel->addChild(lbl);
    
    return true;
}

RewardPanel* RewardPanel::createWithReward(string type, int value) {
    RewardPanel* rtn = new RewardPanel();
    if (rtn && rtn->initWithReward(type, value)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool RewardPanel::initWithReward(string type, int value) {
    if (! CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    _couldExit = false;
    
    _type = string(type.c_str());
    _value = value;

//    CCSprite* mask = CCSprite::create("res/pic/mask.png");
//    mask->setPosition(DISPLAY->center());
//    this->addChild(mask);
    
    _panel = CCSprite::create("pic/building/reward/panel.png");
    _panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75));
    _panel->setVisible(false);
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
    
//    CCSprite* title = CCSprite::create("pic/building/reward/title.png");
//    title->setPosition(ccp(panelSize.width* .5f, panelSize.height* .88f));
//    _panel->addChild(title);
    CCSprite* title = CCSprite::create("pic/building/reward/title2.png");
    title->setPosition(ccp(panelSize.width* .5f, panelSize.height* .92f));
    _panel->addChild(title);
    
    if (_type.compare("coin") == 0) {
        CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
        plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
        _panel->addChild(plate);
        
        CCSprite* coin = CCSprite::create("pic/common/coin2.png");
        coin->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
        _panel->addChild(coin);
        
        CCString* str = CCString::createWithFormat("%d 金币", _value);
        CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24);
        lbl->setColor(ccc3(107, 143, 190));
        lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.28));
        _panel->addChild(lbl);
    }
    else if (_type.compare("diam") == 0) {
        CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
        plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
        _panel->addChild(plate);
        
        CCSprite* diam = CCSprite::create("pic/common/diam2.png");
        diam->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
        _panel->addChild(diam);
        
        CCString* str = CCString::createWithFormat("%d 钻石", _value);
        CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24);
        lbl->setColor(ccc3(107, 143, 190));
        lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.28));
        _panel->addChild(lbl);
    }
    else if (_type.compare("clothes") == 0) {
        CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
        plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
        _panel->addChild(plate);
        
        CCSprite* icon = CCSprite::create(DATA->clothes_icon_path_with_id(_value)->getCString());
        icon->setScale(0.86);
        icon->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
        _panel->addChild(icon);
        
        CCDictionary* dic = CONFIG->clothes();// 所有衣服
        int clothesType = _value / 10000;
        CCArray* partArr = (CCArray* )dic->objectForKey(clothesType);
        for (int i = 0; i < partArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )partArr->objectAtIndex(i);
            int id = clothDic->valueForKey("id")->intValue();
            if (id == _value) {
                CCLabelTTF* lbl = CCLabelTTF::create(clothDic->valueForKey("name")->getCString(), DISPLAY->fangzhengFont(), 24);
                lbl->setColor(ccc3(107, 143, 190));
                lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.28));
                _panel->addChild(lbl);
                break;
            }
        }
    }
    
    return true;
}

void RewardPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&RewardPanel::keyBackStatus), .8f);
}
void RewardPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
    _couldExit = true;
}

void RewardPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool RewardPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (_couldExit && ! _panel->boundingBox().containsPoint(location)) {
        this->remove();
    }
    
    return true;
}

#pragma mark - Inner

void RewardPanel::remove() {
    this->do_exit();
    this->removeFromParentAndCleanup(true);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("REWARDPANEL_REMOVED");
}

void RewardPanel::keyBackClicked(){
    num_child++;
    CCLog("===== RewardPanel  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->remove();
}

void RewardPanel::do_enter() {
    _panel->setScale(0.1);
    _panel->setVisible(true);
    
    float duration1 = 0.2f;
    CCMoveTo* moveto1 = CCMoveTo::create(duration1, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.45));
    CCScaleTo* scaleto1 = CCScaleTo::create(duration1, 1.1);
    CCSpawn* spawn1 = CCSpawn::create(moveto1, scaleto1, NULL);
    
    float duration2 = 0.2f;
    CCMoveTo* moveto2 = CCMoveTo::create(duration2, DISPLAY->center());
    CCScaleTo* scaleto2 = CCScaleTo::create(duration2, 1);
    CCSpawn* spawn2 = CCSpawn::create(moveto2, scaleto2, NULL);
    
    _panel->runAction(CCEaseSineIn::create(CCSequence::create(spawn1, spawn2, NULL)));
}

void RewardPanel::do_exit() {
    AUDIO->goback_effect();
    
    CCPoint posFrom = DISPLAY->center();
    CCDictionary* postData = CCDictionary::create();
    postData->setObject(CCInteger::create(_value), "num");
    CCString* from = CCString::createWithFormat("{%f,%f}", posFrom.x, posFrom.y);
    CCLOG("from -- %s", from->getCString());
    postData->setObject(from, "from");
    
    if (_type.compare("coin") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_COIN_FLY", postData);
    }
    else if (_type.compare("diam") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_GOLD_FLY", postData);
    }
}