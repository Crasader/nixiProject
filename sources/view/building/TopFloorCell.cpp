//
//  TopFloorCell.cpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#include "TopFloorCell.h"
#include "DisplayManager.h"
#include "SpecialManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "AppUtil.h"
#include "AudioManager.h"
#include "RewardPanel.h"
#include "Loading2.h"

const float FLOOR_CELL_WIDTH = 558.f;
const float FLOOR_CELL_HEIGHT = 197.f;

const float ROLE_SCALE = 0.25;
const float STAND_HEIGHT = 3;

TopFloorCell::~TopFloorCell() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

TopFloorCell* TopFloorCell::create(int phase, int idx) {
    TopFloorCell* rtn = new TopFloorCell();
    if (rtn && rtn->init(phase, idx)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool TopFloorCell::init(int phase, int idx) {
    if (phase == 1) {
        _sptFloor = CCSprite::create("pic/building/roof_1.png");
        _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
        this->addChild(_sptFloor);
    }
    else if (phase == 2) {
        _sptFloor = CCSprite::create("pic/building/roof_2.png");
        _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
        this->addChild(_sptFloor);
    }
    else {
        _sptFloor = CCSprite::create("pic/building/roof_3.png");
        _sptFloor->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5, FLOOR_CELL_HEIGHT * 0.5));
        this->addChild(_sptFloor);
    }
    
    CCSprite* bottom = CCSprite::create("pic/building/progress/progress_bar_1.png");
    bottom->setPosition(ccp(FLOOR_CELL_WIDTH * 0.5 - 4, 0));
    this->addChild(bottom);
    
    CCSize barSize = bottom->getContentSize();
    
    CCSprite* middle = CCSprite::create("pic/building/progress/progress_bar_2.png");
    _progress = CCProgressTimer::create(middle);
    _progress->setAnchorPoint(ccp(0, 0.5));
    _progress->setPosition(ccp(0, barSize.height * 0.5));
    _progress->setType(kCCProgressTimerTypeBar);
    _progress->setMidpoint(ccp(0, 0.5));
    _progress->setBarChangeRate(ccp(1, 0));
    _progress->setPercentage(100);
    bottom->addChild(_progress);
    
    _topBar = CCSprite::create("pic/building/progress/progress_bar_3.png");
    _topBar->setPosition(ccp(barSize.width * 0.5, barSize.height * 0.5));
    bottom->addChild(_topBar);
    
    this->setTouchEnabled(true);
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    
    this->setBoxes(CCArray::create());
    this->update_boxes();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&TopFloorCell::nc_take_company_reward_205), "HTTP_FINISHED_205", NULL);
    
    return true;
}

bool TopFloorCell::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint pos = this->convertToNodeSpace(pTouch->getLocation());

    int count = _boxes->count();
    for (int i = 0; i < count; i++) {
        CCSprite* spt = (CCSprite*)_boxes->objectAtIndex(i);
        CCRect rect = spt->boundingBox();
        if (rect.containsPoint(pos)) {
            CCLOG("Touched box index = %d", i);
            CCBool* availble = (CCBool*)spt->getUserObject();
            if (availble->getValue()) {
                CCLOG("YES");
                CoffersComp* coffers = DATA->getCoffers();
                int curPhase = DATA->getPlayer()->phase;
                CCArray* items = coffers->phase_goals(curPhase);
                CCDictionary* item = (CCDictionary*)items->objectAtIndex(spt->getTag());
                
                LOADING->show_loading();
                NET->take_company_reward_205(item->valueForKey("id")->getCString());
            }
            else {
                CCLOG("NO");
            }
            break;
        }
    }
    
    return false;
}

#pragma mark - Inner

void TopFloorCell::update_boxes() {
    if (_boxes->count() > 0) {
        _boxes->removeAllObjects();
        _topBar->removeAllChildrenWithCleanup(true);
    }
    
    CCSize barSize = _topBar->getContentSize();
    
//    CoffersComp* coffers = DATA->getCoffers();
//    int curPhase = DATA->getPlayer()->phase;
//    int totalRatings = DATA->getPlayer()->total_ratings();
//    int start = coffers->phase_start(curPhase);
//    
//    CCArray* items = coffers->phase_goals(curPhase);
//    CCDictionary* lastItem = (CCDictionary*)items->lastObject();
//    int end = ((CCInteger*)lastItem->objectForKey("goal"))->getValue();
//    float curPrecent = (totalRatings - start) / (float)(end - start) * 100;
//    CCLOG("curPrecent = %f", curPrecent);
//    _progress->setPercentage(curPrecent);
    
    CoffersComp* coffers = DATA->getCoffers();
    int curPhase = DATA->getPlayer()->phase;
    int totalRatings = DATA->getPlayer()->ratings(curPhase);
    
    CCArray* items = coffers->phase_goals(curPhase);
    CCDictionary* lastItem = (CCDictionary*)items->lastObject();
    int end = ((CCInteger*)lastItem->objectForKey("goal"))->getValue();
    float curPrecent = totalRatings / (float)end * 100;
    CCLOG("curPrecent = %f", curPrecent);
    _progress->setPercentage(curPrecent);
    
    CCSprite* star = CCSprite::create("pic/building/progress/star.png");
    star->setPosition(ccp(barSize.width * curPrecent / 100, barSize.height * 0.5));
    _progress->addChild(star);
    
    int count = items->count();
    for (int i = 0; i < count; i++) {
        CCDictionary* item = (CCDictionary*)items->objectAtIndex(i);
        int itemGoal = ((CCInteger*)item->objectForKey("goal"))->getValue();
        CCLOG("itemGoal = %d", itemGoal);
//        float precent = (itemGoal - start) / (float)(end - start);
        float precent = itemGoal / (float)end;

        CCSprite* box = NULL;
        float baseHeight = barSize.height * 0.5 + 5;
        if (totalRatings >= itemGoal) {
            CCString* itemId = (CCString*)item->objectForKey("id");
            if (coffers->has_taken_reward(itemId)) {
                CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d_taken.png", i);
                box = CCSprite::create(boxName->getCString());
                box->setPosition(ccp(barSize.width * precent, baseHeight));
                box->setUserObject(CCBool::create(false));
                _topBar->addChild(box);
            }
            else {
                CCString* lightName = CCString::createWithFormat("pic/building/progress/available_light_%d.png", i);
                CCSprite* light = CCSprite::create(lightName->getCString());
                light->setPosition(ccp(barSize.width * precent, baseHeight));
                _topBar->addChild(light);
                
                CCSequence* seq = CCSequence::create(CCFadeOut::create(0.6), CCDelayTime::create(0.5), CCFadeIn::create(0.6), CCDelayTime::create(0.3), NULL);
                light->runAction(CCRepeatForever::create(seq));
                
                
                CCString* spotName = CCString::createWithFormat("pic/building/progress/available_spot_%d.png", i);
                CCSprite* spot = CCSprite::create(spotName->getCString());
                spot->setPosition(ccp(light->getContentSize().width * 0.5, light->getContentSize().height * 0.7));
                light->addChild(spot);
                
                CCSequence* seq2 = CCSequence::create(CCFadeOut::create(0.6), CCDelayTime::create(0.5), CCFadeIn::create(0.6), CCDelayTime::create(0.3), NULL);
                spot->runAction(CCRepeatForever::create(seq2));
                
                
                CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
                box = CCSprite::create(boxName->getCString());
                box->setPosition(light->getPosition());
                box->setUserObject(CCBool::create(true));
                _topBar->addChild(box);
            }
        }
        else {
            CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
            box = CCSprite::create(boxName->getCString());
            box->setPosition(ccp(barSize.width * precent, baseHeight));
            box->setUserObject(CCBool::create(false));
            _topBar->addChild(box);
        }
        
        box->setTag(i);
        _boxes->addObject(box);
        
        // 奖励内容
        int rewardValue = ((CCInteger*)item->objectForKey("reward_value"))->getValue();
        CCString* strReward = CCString::createWithFormat("%d", rewardValue);
        CCLabelTTF* lblReward = CCLabelTTF::create(strReward->getCString(), DISPLAY->fangzhengFont(), 18.0f);
        lblReward->setAnchorPoint(ccp(0.8, 0.5));
        lblReward->setPosition(ccp(28, 22));
        lblReward->enableShadow(CCSizeMake(2, 1), 1, 2);
        box->addChild(lblReward);
        
        const CCString* rewardType = item->valueForKey("reward_type");
        if (rewardType->compare("coin") == 0) {
            CCSprite* icon = CCSprite::create("pic/common/coin2.png");
            icon->setPosition(ccp(lblReward->getPositionX() + 11, lblReward->getPositionY()));
            icon->setScale(0.3);
            box->addChild(icon);
        }
        else if (rewardType->compare("diam") == 0) {
            CCSprite* icon = CCSprite::create("pic/common/diam2.png");
            icon->setPosition(ccp(lblReward->getPositionX() + 11, lblReward->getPositionY()));
            icon->setScale(0.3);
            box->addChild(icon);
        }
    }
}

void TopFloorCell::nc_take_company_reward_205(CCObject *pObj) {
    LOADING->remove();
    CCDictionary* dic = (CCDictionary*)pObj;
    const CCString* type = dic->valueForKey("type");
    CCInteger* num = (CCInteger*)dic->objectForKey("num");
    RewardPanel::show(this->getScene(), type->getCString(), num->getValue());
    this->update_boxes();
}
