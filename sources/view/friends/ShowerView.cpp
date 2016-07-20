//
//  ShowerView.cpp
//  tiegao
//
//  Created by mac on 16/7/19.
//
//

#include "ShowerView.h"
#include "DisplayManager.h"
#include "Shower.h"

#pragma mark - Export API

void ShowerView::change_shower(CCDictionary *clothes) {
    Shower* oldShower = (Shower*)_curShower;
    if (oldShower) {
        oldShower->removeFromParentAndCleanup(true);
        _curShower = NULL;
    }
    
    Shower* newShower = Shower::create();
    newShower->ondress(clothes);
    newShower->setPosition(ccp(-500, 0));
    this->addChild(newShower);
    
    CCMoveTo* moveTo = CCMoveTo::create(.3f, CCPointZero);
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&ShowerView::change_finish));
    CCSequence* seq = CCSequence::create(moveTo, callFunc, NULL);
    newShower->runAction(seq);
    
    _curShower = newShower;
}

#pragma mark - Super API

ShowerView::~ShowerView() {

}

bool ShowerView::init() {
    if (CCLayer::init()) {
        _curShower = NULL;
        
        return true;
    }
    else {
        return false;
    }
}

void ShowerView::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
}

void ShowerView::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}


#pragma mark - Inner API

void ShowerView::change_finish() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("CHANGE_SHOWER_FINISHED", NULL);
}

