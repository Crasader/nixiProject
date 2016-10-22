//
//  MysteryLayer.cpp
//  tiegao
//
//  Created by mac on 16/8/23.
//
//

#include "MysteryLayer.h"
#include "DisplayManager.h"

#define CELL_WIDTH          187
#define CELL_HEIGHT         50

#pragma mark - Export

CCScene* MysteryLayer::scene() {
    MysteryLayer* layer = MysteryLayer::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

#pragma mark - Inherit

MysteryLayer::~MysteryLayer() {
}

bool MysteryLayer::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        CCSprite* mask = CCSprite::create("pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _panel = CCSprite::create("pic/clothesScene/gj_bgkuang.png");
        _panel->setAnchorPoint(ccp(1, 0.5));
        _panel->setPosition(ccp(DISPLAY->W(), DISPLAY->H() * 0.5));
        this->addChild(_panel);
        
        CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        this->addChild(txt_close);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        return true;
    }
    else {
        return false;
    }
}

void MysteryLayer::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&MysteryLayer::keyBackStatus), .8f);
}
void MysteryLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void MysteryLayer::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

bool MysteryLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = this->convertToNodeSpace(pTouch->getLocation());
    if (! _panel->boundingBox().containsPoint(location)) {
        //        this->do_exit();
        remove();
    }
    
    return false;
}

#pragma mark - Inner

void MysteryLayer::do_enter() {

}

void MysteryLayer::do_exit() {

}

void MysteryLayer::remove() {
    this->removeFromParentAndCleanup(true);
}

void MysteryLayer::keyBackClicked(){
    num_child++;
    CCLog("===== MysteryLayer  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->remove();
}


