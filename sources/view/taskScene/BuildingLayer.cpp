//
//  BuildingLayer.cpp
//  tiegao
//
//  Created by mac on 16/7/5.
//
//

#include "DisplayManager.h"
#include "SpecialManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "AudioManager.h"

#include "AppUtil.h"
#include "Loading2.h"

#include "BuildingLayer.h"
#include "BuildingView.h"
#include "UpgradeLayer.h"

#include "ConfigManager.h"
#include "JNIController.h"

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
    
    // 云
    this->addAction();
    this->scheduleUpdate();
    
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
    nc->addObserver(this, SEL_CallFuncO(&BuildingLayer::nc_take_income_203), "HTTP_FINISHED_203", NULL);
    
    if (_isPhaseUp) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (CONFIG->baiOrYijie == 0) {// 白包
        }else if (CONFIG->baiOrYijie == 1){// 易接
            JNIController::setUserId(JNIController::getSessionid().c_str());
            JNIController::setPlayerName(DATA->getShow()->nickname());
            CCString* phaseStr = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
            JNIController::setPlayerLevel(phaseStr->getCString());
            CCString* diamStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
            JNIController::setPlayerGold(diamStr->getCString());
            JNIController::setData(4);
            
            JNIController::exitGame(1);
        }
#endif
        
        this->_isAction = true;
        scheduleOnce(SEL_SCHEDULE(&BuildingLayer::show_phase_up), 1.0);
    }
    else {
        if (_phase == DATA->getPlayer()->phase) {
            schedule(SEL_SCHEDULE(&BuildingLayer::building_shaking), 1.f);
            CoffersComp* coffers = DATA->getCoffers();
            if (coffers->have_untake_reward(_phase) || coffers->is_coffers_full()) {
                this->show_arrow();
            }
        }
    }
}

void BuildingLayer::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void BuildingLayer::addAction() {
    //-----云------
    _cloud_1 = CCSprite::create("res/pic/mainScene/cloud.png");
    _cloud_1->setPosition(ccp(this->getContentSize().width* .5f, this->getContentSize().height - _cloud_1->getContentSize().height* .5f + 50));
    this->addChild(_cloud_1);
    
    _cloud_2 = CCSprite::create("res/pic/mainScene/cloud.png");
    _cloud_2->setPosition(ccp(_cloud_1->getPositionX() + _cloud_2->getContentSize().width, this->getContentSize().height - _cloud_1->getContentSize().height* .5f + 50));
    this->addChild(_cloud_2);
    
    //-----鸟------
    _bird_1 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_1->setPosition(ccp(this->getContentSize().width* .9f, this->getContentSize().height* .77));//0.25f
    _bird_1->setScale(0.3f);
    this->addChild(_bird_1);
    
    CCAnimation* animation = CCAnimation::create();
    for (int i = 0; i < 5; i++) {
        CCString* str = CCString::createWithFormat("res/pic/mainScene/bird_%d.png", i + 1);
        animation->addSpriteFrameWithFileName(str->getCString());
    }
    animation->setDelayPerUnit(0.22f);
    animation->setRestoreOriginalFrame(true);
    
    CCAnimate* ac1 = CCAnimate::create(animation);
    CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(ac1, ac1->reverse(), NULL));
    _bird_1->runAction(rep);
    
    _bird_2 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_2->setPosition(ccp(this->getContentSize().width* .4f, this->getContentSize().height* .71));
    _bird_2->setScale(0.2f);
    this->addChild(_bird_2);
    animation->setDelayPerUnit(0.18f);
    CCAnimate* ac2 = CCAnimate::create(animation);
    CCRepeatForever* rep2 = CCRepeatForever::create(CCSequence::create(ac2, ac2->reverse(), NULL));
    _bird_2->runAction(rep2);
    
    _bird_3 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_3->setPosition(ccp(this->getContentSize().width* .75, this->getContentSize().height* .75));
    _bird_3->setScale(0.4f);
    this->addChild(_bird_3);
    animation->setDelayPerUnit(0.19f);
    CCAnimate* ac3 = CCAnimate::create(animation);
    CCRepeatForever* rep3 = CCRepeatForever::create(CCSequence::create(ac3, ac3->reverse(), NULL));
    _bird_3->runAction(rep3);
    
    _bird_4 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_4->setPosition(ccp(this->getContentSize().width* .85, this->getContentSize().height* .8));
    _bird_4->setScale(0.3f);
    this->addChild(_bird_4);
    animation->setDelayPerUnit(0.21f);
    CCAnimate* ac4 = CCAnimate::create(animation);
    CCRepeatForever* rep4 = CCRepeatForever::create(CCSequence::create(ac4, ac4->reverse(), NULL));
    _bird_4->runAction(rep4);
}

void BuildingLayer::update(float dt) {
    //云
    if (_cloud_1->getPositionX() <= -_cloud_1->getContentSize().width* .5f) {
        _cloud_1->setPositionX(_cloud_2->getPositionX() + _cloud_1->getContentSize().width);
    }else{
        _cloud_1->setPositionX(_cloud_1->getPositionX() - 3 * dt);
    }
    
    if (_cloud_2->getPositionX() <= -_cloud_2->getContentSize().width* .5f) {
        _cloud_2->setPositionX(_cloud_1->getPositionX() + _cloud_1->getContentSize().width);
    }else{
        _cloud_2->setPositionX(_cloud_2->getPositionX() - 3 * dt);
    }
    
    //鸟
    if (_bird_1->getPositionX() <= -100) {
        _bird_1->setPositionX(this->getContentSize().width + 100);
    }else{
        _bird_1->setPositionX(_bird_1->getPositionX() - 35 * dt);
    }
    
    if (_bird_2->getPositionX() <= -100) {
        _bird_2->setPositionX(this->getContentSize().width + 100);
    }else{
        _bird_2->setPositionX(_bird_2->getPositionX() - 35 * dt);
    }
    
    if (_bird_3->getPositionX() <= -100) {
        _bird_3->setPositionX(this->getContentSize().width + 100);
    }else{
        _bird_3->setPositionX(_bird_3->getPositionX() - 35 * dt);
    }
    
    if (_bird_4->getPositionX() <= -100) {
        _bird_4->setPositionX(this->getContentSize().width + 100);
    }else{
        _bird_4->setPositionX(_bird_4->getPositionX() - 35 * dt);
    }
}


bool BuildingLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    SPECIAL->showSpotAt(this->getScene(), location, 100);
    CCRect rect = _building->boundingBox();
    CCRect check = CCRectMake(rect.origin.x, rect.origin.y, (DISPLAY->W() - 200), rect.size.height);
    
    if (true == this->_isAction) {
        return false;
    }
    
    if (check.containsPoint(location)) {
        if (DATA->current_guide_step() == 5) {
            if (!DATA->_guideBool5[0]){
                CCNotificationCenter::sharedNotificationCenter()->postNotification("CloseSwallowEnabled");
            }
        }
        
        AUDIO->comfirm_effect();
        if (_phase == DATA->getPlayer()->phase) {
            this->building_touch_callback();
            return true;
        }
    }
    
    return false;
}

#pragma mark - inner

void BuildingLayer::building_shaking() {
    unschedule(SEL_SCHEDULE(&BuildingLayer::building_shaking));
    if (_building) {
        CCSequence* seq = CCSequence::create(CCScaleTo::create(0.5, 1.03, 0.97), CCDelayTime::create(0.1), CCScaleTo::create(0.3, 1), NULL);
        _building->runAction(seq);
        schedule(SEL_SCHEDULE(&BuildingLayer::building_shaking), 3.f);
    }
}

void BuildingLayer::show_arrow() {
    CCSprite* sptArrow = CCSprite::create("pic/building/arrow.png");
    sptArrow->setPosition(ccp(DISPLAY->W() * 0.22, DISPLAY->H() * 0.81));
    sptArrow->setTag(7451);
    this->addChild(sptArrow);
    sptArrow->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5, ccp(0, -60)), CCMoveBy::create(0.5, ccp(0, 60)), NULL)));
    
    CCSprite* sptFlow = CCSprite::create("pic/building/arrow_flow.png");
    sptFlow->setPosition(ccp(50, 30));
    sptFlow->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1.0f), CCFadeIn::create(1.0f), CCDelayTime::create(0.5), NULL)));
    sptArrow->addChild(sptFlow);
}

void BuildingLayer::building_touch_callback() {
    this->setTouchEnabled(false);
    CCCallFunc* call = CCCallFunc::create(this, SEL_CallFunc(&BuildingLayer::show_building));
    CCSequence* seq = CCSequence::create(CCScaleTo::create(0.1, 1.008f), CCScaleTo::create(0.08, 1.f), call, NULL);
    _building->runAction(seq);
}

void BuildingLayer::show_building() {
    LOADING->show_loading();
    if (DATA->getCoffers()->getHasInitTemplate()) {
        NET->coffers_info_200(false);
    }
    else {
        NET->coffers_info_200(true);
    }
}

void BuildingLayer::show_phase_up() {
    CCLOG("BuildingLayer::show_phase_up()");
    CCSequence* seqBuilding = CCSequence::create(CCMoveTo::create(0.5, ccp(-DISPLAY->halfW() * 0.5, this->_building->getPositionY())), CCFadeOut::create(0.1), NULL);
    this->_building->runAction(seqBuilding);
    
    UpgradeLayer* layer = UpgradeLayer::create_with_index(1);
    this->addChild(layer);
    
    CCCallFuncN* done = CCCallFuncN::create(this, SEL_CallFuncN(&BuildingLayer::on_construction_finish));
    CCSequence* seq = CCSequence::create(CCDelayTime::create(6), done, NULL);
    layer->runAction(seq);
}

void BuildingLayer::on_construction_finish(CCNode* node) {
//    node->removeFromParentAndCleanup(true);
    ((UpgradeLayer*)node)->closeLayer();
    
    CCString* str = NULL;
    if (_phase < 3) {
        str = CCString::createWithFormat("res/pic/taskScene/task_building_%d.png", _phase);
    }
    else {
        str = CCString::createWithFormat("res/pic/taskScene/task_building_%d.png", 3);
    }
    
    this->_building->setVisible(true);
    CCSprite* newBuilding = CCSprite::create(str->getCString());
    newBuilding->setAnchorPoint(CCPointZero);
    _building->addChild(newBuilding);
    
    UpgradeLayer* layer = UpgradeLayer::create_with_index(2);
    this->addChild(layer);
    CCLOG("layer = %p", layer);

    CCCallFuncN* done = CCCallFuncN::create(this, SEL_CallFuncN(&BuildingLayer::on_phaseup_finish));
    CCSequence* seq = CCSequence::create(CCDelayTime::create(5), done, NULL);
    layer->runAction(seq);
}

void BuildingLayer::on_phaseup_finish(CCNode* node) {
    CCLOG("layer = %p", node);
    node->removeFromParentAndCleanup(true);
    
    this->_isAction = false;
    
    CoffersComp* coffers = DATA->getCoffers();
    if (coffers->have_untake_reward(_phase) || coffers->is_coffers_full()) {
        this->show_arrow();
    }

    schedule(SEL_SCHEDULE(&BuildingLayer::building_shaking), 1.f);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("Phase_Up_Finished");
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

void BuildingLayer::nc_take_income_203(CCObject *pObj) {
    CCNode* node = this->getChildByTag(7451);
    if (node && node->getParent()) {
        node->removeFromParentAndCleanup(true);
    }
    
//    if (_phase == DATA->getPlayer()->phase) {
//        schedule(SEL_SCHEDULE(&BuildingLayer::building_shaking), 1.f);
//        CoffersComp* coffers = DATA->getCoffers();
//        if (coffers->have_untake_reward(_phase) || coffers->is_coffers_full()) {
//            this->show_arrow();
//        }
//    }
}
