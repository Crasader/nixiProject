//
//  DragLayer.cpp
//  tiegao
//
//  Created by stevenLi on 16/7/12.
//
//

#include "DragLayer.h"

DragLayer::~DragLayer(){
    
}

bool DragLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    return true;
}

void DragLayer::onEnter(){
    CCLayer::onEnter();
}

void DragLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCLayer::onExit();
}

bool DragLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
    last_pos = pTouch->getLocation();
    
    return true;
}

void DragLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent){
    cur_pos= pTouch->getLocation();
    float offset_x = cur_pos.x - last_pos.x;
    last_pos = cur_pos;
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("DRAGING", CCFloat::create(offset_x));
}

void DragLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent){
    CCNotificationCenter::sharedNotificationCenter()->postNotification("EFFECTIVE");
}