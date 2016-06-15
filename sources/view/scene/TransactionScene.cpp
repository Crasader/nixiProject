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
    if (BaseScene::init()) {
        
        return true;
    }
    else {
        return false;
    }
}

void TransactionScene::onEnter() {
    BaseScene::onEnter();
}

void TransactionScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    BaseScene::onExit();
}