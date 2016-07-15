//
//  BuildingLayer.cpp
//  tiegao
//
//  Created by mac on 16/7/5.
//
//

#include "BuildingLayer.h"
#include "DisplayManager.h"
#include "SpecialManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "BuildingView.h"

BuildingLayer::~BuildingLayer() {
}

BuildingLayer* BuildingLayer::create(int phase, bool isPhaseUp) {
    BuildingLayer* rtn = new BuildingLayer();
    if (rtn && rtn->init(phase, isPhaseUp)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool BuildingLayer::init(int phase, bool isPhaseUp) {
    if (! CCLayer::init()) {
        return false;
    }
    
    this->setTouchEnabled(true);
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    _isPhaseUp = isPhaseUp;
    _phase = phase;
    
    CCSprite* bg = CCSprite::create("res/pic/taskScene/task_bg.png");
    bg->setPosition(DISPLAY->center());
    this->addChild(bg);
    
    CCString* str = NULL;
    if (_phase < 3) {
        str = CCString::createWithFormat("res/pic/taskScene/task_building_%d.png", _phase);
    }
    else {
        str = CCString::createWithFormat("res/pic/taskScene/task_building_%d.png", 3);
    }
    _building = CCSprite::create(str->getCString());
    _building->setAnchorPoint(CCPointZero);
    _building->setPosition(ccp(0, DISPLAY->H() * 0.22));
    this->addChild(_building);
    
    CCSprite* fountain = CCSprite::create("res/pic/taskScene/task_fountain.png");
    fountain->setPosition(ccp(DISPLAY->halfW() + 175, DISPLAY->H() * 0.287));
    this->addChild(fountain);
    
    return true;
}

void BuildingLayer::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&BuildingLayer::nc_building_disappear), "BUILDING_DISAPPEAR", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&BuildingLayer::nc_coffers_info_200), "HTTP_FINISHED_200", NULL);
    
    
    if (_isPhaseUp) {
        scheduleOnce(SEL_SCHEDULE(&BuildingLayer::show_phase_up), 1.0);
    }
    else {
        schedule(SEL_SCHEDULE(&BuildingLayer::building_shaking), 1.f);
    }
}

void BuildingLayer::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

bool BuildingLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    SPECIAL->showSpotAt(this->getScene(), location, 100);
    CCRect rect = _building->boundingBox();
    CCRect check = CCRectMake(rect.origin.x, rect.origin.y, (DISPLAY->W() - 200), rect.size.height);
    if (check.containsPoint(location)) {
        this->building_touch_callback();
        return true;
    }
    
    return false;
}

#pragma mark - export


#pragma mark - inner

void BuildingLayer::building_shaking() {
    unschedule(SEL_SCHEDULE(&BuildingLayer::building_shaking));
    if (_building) {
        float during = 0.1f;
        CCSequence* seq = CCSequence::create(CCScaleTo::create(during, 1.1, 0.9), CCDelayTime::create(0.04), CCScaleTo::create(during, 1), NULL);
        _building->runAction(seq);
        schedule(SEL_SCHEDULE(&BuildingLayer::building_shaking), 5.f);
    }
}

void BuildingLayer::building_touch_callback() {
    this->setTouchEnabled(false);
    CCCallFunc* call = CCCallFunc::create(this, SEL_CallFunc(&BuildingLayer::show_building));
    CCSequence* seq = CCSequence::create(CCScaleTo::create(0.1, 1.008f), CCScaleTo::create(0.08, 1.f), call, NULL);
    _building->runAction(seq);
}

void BuildingLayer::show_building() {
    LOADING->show_loading();
    NET->coffers_info_200();
}

void BuildingLayer::show_phase_up() {
    CCLOG("BuildingLayer::show_phase_up()");
}

void BuildingLayer::nc_building_disappear(CCObject *pObj) {
    schedule(SEL_SCHEDULE(&BuildingLayer::building_shaking), 1.f);
}

void BuildingLayer::nc_coffers_info_200(CCObject *pObj) {
    LOADING->remove();
    unschedule(SEL_SCHEDULE(&BuildingLayer::building_shaking));
    this->setTouchEnabled(true);
    BuildingView* building = BuildingView::create(_phase);
    this->getScene()->addChild(building);
}
