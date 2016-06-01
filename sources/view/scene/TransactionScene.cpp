//
//  TransactionScene.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "TransactionScene.h"

TransactionScene::~TransactionScene() {
}

bool TransactionScene::init() {
    if (CCLayer::init()) {
        
        return true;
    }
    else {
        return false;
    }
}

void TransactionScene::onEnter() {
    CCLayer::onEnter();
}

void TransactionScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}