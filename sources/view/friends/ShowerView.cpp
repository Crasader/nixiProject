//
//  ShowerView.cpp
//  tiegao
//
//  Created by mac on 16/7/19.
//
//

#include "ShowerView.h"
#include "DisplayManager.h"


#pragma mark - Export API



#pragma mark - Super API

ShowerView::~ShowerView() {
}

bool ShowerView::init() {
    if (CCLayer::init()) {
        
        
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

