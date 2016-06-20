//
//  Loading.cpp
//  tiegao
//
//  Created by mac on 16/5/31.
//
//

#include "Loading.h"
#include "DisplayManager.h"

static Loading* _instance = nullptr;

Loading::~Loading() {
}

Loading* Loading::Inst() {
    if (_instance == nullptr) {
        _instance = new Loading();
        _instance->addChild(CCLayerColor::create(ccc4(255, 255, 255, 0)));
        
        _instance->_loading = CCSprite::create("res/pic/loading.png");
        _instance->_loading->setPosition(DISPLAY->center());
        _instance->addChild(_instance->_loading);
    }
    
    return _instance;
}

void Loading::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    _loading->setVisible(true);
    this->schedule(SEL_SCHEDULE(&Loading::loading), 0.1f);
}

void Loading::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool Loading::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    return true;
}

#pragma mark - export

void Loading::show_loading() {
    this->show_loading(CCDirector::sharedDirector()->getRunningScene());
}

void Loading::show_loading(CCNode* node) {
    if (this->getParent()) {
        return;
    }
    node->addChild(this, 3003);
}

void Loading::stop_loading() {
    this->unschedule(SEL_SCHEDULE(&Loading::loading));
//    _loading->removeFromParentAndCleanup(true);
//    _loading = NULL;
    _loading->setVisible(false);
}

void Loading::remove() {
    this->stop_loading();
    this->removeFromParentAndCleanup(true);
}

#pragma mark - inner

void Loading::loading() {
    if (_loading) {
        _loading->setRotation(_loading->getRotation() - 30);
    }
}


