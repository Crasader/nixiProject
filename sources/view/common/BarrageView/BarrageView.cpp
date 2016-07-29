//
//  BarrageView.cpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#include "BarrageView.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "WSManager.h"
#include "ChatComp.h"

#pragma mark - Export API

void BarrageView::show() {
    CCDirector::sharedDirector()->getRunningScene()->addChild(BarrageView::create(), 1100);
}

void BarrageView::addNewChat(CCObject *pObj) {
    
    ChatItem* chat = (ChatItem*)pObj;
    CCLOG("-- %s", chat->chat.c_str());
    float fontSize = 20 + CCRANDOM_0_1() * 6;
    CCLabelTTF* lbl = CCLabelTTF::create(chat->chat.c_str(), DISPLAY->fangzhengFont(), fontSize);
    lbl->setAnchorPoint(ccp(0, 0.5));
    lbl->setPosition(ccp(DISPLAY->W(), (0.5 + CCRANDOM_0_1() * 0.5) * DISPLAY->H()));
    CCColor3bObject* color = DATA->getChat()->randColor();
    lbl->setColor(color->value);
    lbl->runAction(CCSequence::create(CCMoveBy::create(5.f, ccp(-1500, 0)), CCRemoveSelf::create(), NULL));
    this->addChild(lbl);
}

#pragma mark - Super API

BarrageView::~BarrageView() {
    
}

bool BarrageView::init() {
    if (CCLayer::init()) {

        return true;
    }
    else {
        return false;
    }
}

void BarrageView::onEnter() {
    CCLayer::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&BarrageView::addNewChat), "NEW_CHAT", NULL);
}

void BarrageView::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}


#pragma mark - Inner API

