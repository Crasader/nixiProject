//
//  OperationPanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "OperationPanel.h"

#include "DataManager.h"
#include "DisplayManager.h"
#include "NetManager.h"

#include "Reward.h"
#include "Loading.h"

#define CELL_WIDTH          500
#define CELL_HEIGHT         196


OperationPanel::~OperationPanel() {
}

bool OperationPanel::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        _content->setScale(0.1);
        this->addChild(_content);
        _content->setVisible(false);
        
        _bg = CCSprite::create("pic/panel/mail/operation_bg.png");
        _bg->setPosition(DISPLAY->center());
        _content->addChild(_bg);
        
        CCSprite* txt_close = CCSprite::create("pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        _content->addChild(txt_close);
        
        return true;
    }
    else {
        return false;
    }
}

void OperationPanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&OperationPanel::hanle_mail_oper), "HTTP_FINISHED_701", NULL);
    
    this->do_enter();
}

void OperationPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool OperationPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _bg->boundingBox().containsPoint(location)) {
        this->do_exit();
    }
    
    return true;
}

#pragma mark - export

void OperationPanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void OperationPanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.8f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    //    _content->runAction(CCEaseBounceOut::create(spawn));
    _content->runAction(CCEaseElasticOut::create(spawn));
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
}

void OperationPanel::do_exit() {
    float duration = 0.6f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&OperationPanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(CCEaseElasticIn::create(spawn));
}

void OperationPanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void OperationPanel::on_purchase(cocos2d::CCMenuItem *btn) {
    LOADING->show_loading();
    int* id = (int*)btn->getUserData();
}

void OperationPanel::on_monthlycard(cocos2d::CCMenuItem *btn) {
    LOADING->show_loading();
    int* id = (int*)btn->getUserData();
}
