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
    }
    
    return _instance;
}

void Loading::onEnter() {
    CCLayer::onEnter();
    
    _loading = CCSprite::create("pic/loading.png");
    _loading->setPosition(DISPLAY->center());
    this->addChild(_loading);
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
    _loading->removeFromParentAndCleanup(true);
    _loading = NULL;
}

void Loading::remove() {
    this->removeFromParent();
}

#pragma mark - inner

void Loading::loading() {
    if (_loading) {
        _loading->setRotation(_loading->getRotation() - 30);
    }
}


