//
//  MailPanel.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "MailPanel.h"

MailCell::~MailCell() {
}

bool MailCell::init() {
    if (CCSprite::init()) {
        
        return true;
    }
    else {
        return false;
    }
}

void MailCell::onEnter() {
    CCSprite::onEnter();
}

void MailCell::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCSprite::onExit();
}

void MailCell::config_with_module() {
    
}


// --------------------------------------------------------

MailPanel::~MailPanel() {
}

bool MailPanel::init() {
    if (CCLayer::init()) {
        
        return true;
    }
    else {
        return false;
    }
}

void MailPanel::onEnter() {
    CCLayer::onEnter();
}

void MailPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void MailPanel::do_enter() {

}

void MailPanel::do_exit() {

}


