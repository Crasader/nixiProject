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
#include "RewardPanel.h"

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
    _boxNode = NULL;
    
    CCSprite* bg = CCSprite::create("res/pic/taskScene/task_bg.png");
    bg->setPosition(DISPLAY->center());
    this->addChild(bg);
    
    // 云
    this->addAction();
    this->scheduleUpdate();
    
    if (_phase < 4) {
        CCString* str = CCString::createWithFormat("pic/taskScene/task_building_%d.png", _phase);
        _building = CCSprite::create(str->getCString());
        _building->setAnchorPoint(CCPointZero);
        _building->setPosition(ccp(0, DISPLAY->H() * 0.22));
        this->addChild(_building);
    }
    else {
        CCSprite* phaseBg = CCSprite::create("pic/taskScene/task_phase_bg.png");
        phaseBg->setPosition(DISPLAY->center());
        this->addChild(phaseBg);
        
        CCString* str = CCString::createWithFormat("pic/taskScene/task_building_%d.png", 4);
        _building = CCSprite::create(str->getCString());
        _building->setPosition(DISPLAY->center());
        this->addChild(_building);
    }
    
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
    
    nc->addObserver(this, SEL_CallFuncO(&BuildingLayer::createCompanyRewardIcon), "UPDATE_BOX_STATUS", NULL);
    
    
    if (_isPhaseUp) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (CONFIG->baiOrYijie == 0) {// 白包
        }else if (CONFIG->baiOrYijie == 1 || CONFIG->baiOrYijie == 2){// 易接
            JNIController::setUserId(DATA->getLogin()->obtain_sid());
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
        if (DATA->getNeedShowUnlockMystery()) {
            this->scheduleOnce(SEL_SCHEDULE(&BuildingLayer::show_mystery_unlock), 0.6);
        }
        
        
        if (_phase == DATA->getPlayer()->phase) {
            // 公司奖励图标
            this->createCompanyRewardIcon();
            
            schedule(SEL_SCHEDULE(&BuildingLayer::building_shaking), 1.f);
            int pahseTotalRationg = DATA->getPlayer()->ratings(DATA->getPlayer()->phase);
            CoffersComp* coffers = DATA->getCoffers();
            if (coffers->have_untake_reward(_phase, pahseTotalRationg) || coffers->is_coffers_full()) {
                this->show_building();
            }
        }
    }
}

void BuildingLayer::show_mystery_unlock() {
    RewardPanel* pannel = RewardPanel::createWithMystery();
    this->getScene()->addChild(pannel);
    DATA->setNeedShowUnlockMystery(false);
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

void BuildingLayer::createCompanyRewardIcon() {
    if (_boxNode == NULL) {
        _boxNode = CCNode::create();
        _boxNode->setPosition(50, DISPLAY->H() * 0.85);
        _boxNode->setScale(1.3f);
        this->addChild(_boxNode);
    }
    else {
        _boxNode->removeAllChildrenWithCleanup(true);
    }
    
    bool is_show = false;
    
    CoffersComp* coffers = DATA->getCoffers();
    int curPhase = _phase; //DATA->getPlayer()->phase;
    int totalRatings = DATA->getPlayer()->ratings(curPhase);
    
    CCArray* items = coffers->phase_goals(curPhase);
    int count = items->count();
    
    ccColor3B textColor = ccc3(127, 76, 76);
    
    int i = 0;
    for (; i < count; i++) {
        CCDictionary* item = (CCDictionary*)items->objectAtIndex(i);
        int itemGoal = ((CCInteger*)item->objectForKey("goal"))->getValue();
        CCLOG("itemGoal = %d", itemGoal);
        // 优先显示当前阶段第一没有领取的
        if (totalRatings >= itemGoal) {
            CCString* itemId = (CCString*)item->objectForKey("id");
            if (! coffers->has_taken_reward(itemId)) { // 没有领取
                is_show = true;
                
                CCString* lightName = CCString::createWithFormat("pic/building/progress/available_light_%d.png", i);
                CCSprite* light = CCSprite::create(lightName->getCString());
                _boxNode->addChild(light);
                
                CCSequence* seq = CCSequence::create(CCFadeOut::create(0.6), CCDelayTime::create(0.5), CCFadeIn::create(0.6), CCDelayTime::create(0.3), NULL);
                light->runAction(CCRepeatForever::create(seq));
                
                
                CCString* spotName = CCString::createWithFormat("pic/building/progress/available_spot_%d.png", i);
                CCSprite* spot = CCSprite::create(spotName->getCString());
                spot->setPosition(ccp(light->getContentSize().width * 0.5, light->getContentSize().height * 0.7));
                light->addChild(spot);
                
                CCSequence* seq2 = CCSequence::create(CCFadeOut::create(0.6), CCDelayTime::create(0.5), CCFadeIn::create(0.6), CCDelayTime::create(0.3), NULL);
                spot->runAction(CCRepeatForever::create(seq2));
                
                
                CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
//                CCSprite* box = CCSprite::create(boxName->getCString());
//                box->setPosition(light->getPosition());
//                node->addChild(box);
                CCSprite* box1 = CCSprite::create(boxName->getCString());
                CCSprite* box2 = CCSprite::create(boxName->getCString());
                box2->setScale(1.04);
                CCMenuItem* btn = CCMenuItemSprite::create(box1, box2, this, SEL_MenuHandler(&BuildingLayer::show_building));
                CCMenu* menu = CCMenu::createWithItem(btn);
                menu->ignoreAnchorPointForPosition(false);
                
                
                // 显示奖励数量
                int rewardValue = ((CCInteger*)item->objectForKey("reward_value"))->getValue();
                CCString* strReward = CCString::createWithFormat("%d", rewardValue);
                CCLabelTTF* lblReward = CCLabelTTF::create(strReward->getCString(), DISPLAY->fangzhengFont(), 18.0f);
                
                const CCString* rewardType = item->valueForKey("reward_type");
                CCSprite* icon;
                if (rewardType->compare("coin") == 0) {
                    icon = CCSprite::create("pic/building/reward_coin.png");
                    
                }
                else if (rewardType->compare("diam") == 0) {
                    icon = CCSprite::create("pic/building/reward_diam.png");
                }
                
                
                CCSize plateSize = CCSizeMake((lblReward->getContentSize().width + icon->getContentSize().width + 18), 28);
                CCScale9Sprite* plate = CCScale9Sprite::create("pic/building/progress/plate_99.png");
                plate->setContentSize(plateSize);
                plate->setPosition( ccp(0, -(box1->getContentSize().height * 0.5 + plateSize.height * 0.3)) );
                
                
                lblReward->setAnchorPoint(ccp(0, 0.5));
                lblReward->setPosition(ccp(10, plateSize.height * 0.5));
                lblReward->setColor(textColor);
                plate->addChild(lblReward);
                

                float iconX = lblReward->getPositionX() + lblReward->getContentSize().width + icon->getContentSize().width * 0.5 + 1;
                icon->setPosition(ccp(iconX, plateSize.height * 0.5 + 2));
                plate->addChild(icon);
                
                
                _boxNode->addChild(plate);
                _boxNode->addChild(menu);
                
                break;
            }
        }
        else { // 若无没有领取的，显示下一个目标
            is_show = true;
            //
            CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
            CCSprite* box1 = CCSprite::create(boxName->getCString());
            CCSprite* box2 = CCSprite::create(boxName->getCString());
            box2->setScale(1.04);
            CCMenuItem* btn = CCMenuItemSprite::create(box1, box2, this, SEL_MenuHandler(&BuildingLayer::show_building));
            CCMenu* menu = CCMenu::createWithItem(btn);
            menu->ignoreAnchorPointForPosition(false);
            
            // 显示条件
            CCString* strCondition = CCString::createWithFormat("%d", itemGoal);
            CCLabelTTF* lblCondition = CCLabelTTF::create(strCondition->getCString(), DISPLAY->fangzhengFont(), 18.0f);
            CCSprite* star = CCSprite::create("pic/building/progress/star2.png");
            
            CCSize plateSize = CCSizeMake((lblCondition->getContentSize().width + star->getContentSize().width + 18), 28);
            
            CCScale9Sprite* plate = CCScale9Sprite::create("pic/building/progress/plate_99.png");
            plate->setContentSize(plateSize);
            plate->setPosition( ccp(0, -(box1->getContentSize().height * 0.5 + plateSize.height * 0.3)) );
            
            
            lblCondition->setAnchorPoint(ccp(0, 0.5));
            lblCondition->setPosition(ccp(10, plateSize.height * 0.5));
            lblCondition->setColor(textColor);
            plate->addChild(lblCondition);
            
            
            float starX = lblCondition->getPositionX() + lblCondition->getContentSize().width + star->getContentSize().width * 0.5 + 1;
            star->setPosition(ccp(starX, plateSize.height * 0.5 + 2));
            plate->addChild(star);
            
            //
            _boxNode->addChild(plate);
            _boxNode->addChild(menu);
            
            break;
        }
        
        
        // 若无目标（全达成），显示最后一个的已领取状态
        if (is_show == false) {
            CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d_taken.png", 2);
            CCSprite* box1 = CCSprite::create(boxName->getCString());
            CCSprite* box2 = CCSprite::create(boxName->getCString());
            box2->setScale(1.04);
            CCMenuItem* btn = CCMenuItemSprite::create(box1, box2, this, SEL_MenuHandler(&BuildingLayer::show_building));
            CCMenu* menu = CCMenu::createWithItem(btn);
            menu->ignoreAnchorPointForPosition(false);
            _boxNode->addChild(menu);
        }
    }
}


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
    
    int pahseTotalRationg = DATA->getPlayer()->ratings(DATA->getPlayer()->phase);
    CoffersComp* coffers = DATA->getCoffers();
    if (coffers->have_untake_reward(_phase, pahseTotalRationg) || coffers->is_coffers_full()) {
//        this->show_arrow();
        this->show_building();
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
