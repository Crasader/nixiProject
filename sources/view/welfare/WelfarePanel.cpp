//
//  WelfarePanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "WelfarePanel.h"

#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "AudioManager.h"

#include "WelfareComp.h"
#include "WelfareCell.h"

#include "Loading2.h"

const float CELL_WIDTH = 482;
const float CELL_HEIGHT = 110;


#pragma mark - Export

void WelfarePanel::show() {
    WelfarePanel* panel = WelfarePanel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}

#pragma mark - Supper

WelfarePanel::~WelfarePanel() {
}

bool WelfarePanel::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        _dataSource = DATA->getWelfare();
        
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("res/pic/welfare/welfare_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        _content->addChild(txt_close);
        
        //
        this->init_header();
        
        // tableview
        float panelW = CELL_WIDTH + 3;
        float panelH = 500;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->ignoreAnchorPointForPosition(false);
        _tv->setAnchorPoint(ccp(0.5, 0.5));
        _tv->setPosition(ccp(DISPLAY->halfW(), DISPLAY->halfH() - 100));
        _tv->setDelegate(this);
        this->addChild(_tv);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        return true;
    }
    else {
        return false;
    }
}

void WelfarePanel::onEnter() {
    CCLayer::onEnter();
    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    this->scheduleOnce(SEL_SCHEDULE(&WelfarePanel::keyBackStatus), .8f);
}

void WelfarePanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void WelfarePanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

bool WelfarePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    _touchLocation = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(_touchLocation)) {
        remove();
    }
    
    return true;
}

#pragma mark - inner

void WelfarePanel::remove() {
    AUDIO->goback_effect();
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void WelfarePanel::init_header() {
    CCSprite* totalBarBottom = CCSprite::create("pic/welfare/welfare_total_bar_1.png");
    totalBarBottom->setPosition(ccp(_panel->getContentSize().width * 0.5, _panel->getContentSize().height * 0.82));
    _panel->addChild(totalBarBottom);
    
    CCSize barSize = totalBarBottom->getContentSize();
    
    _totalProgress = CCProgressTimer::create(CCSprite::create("pic/welfare/welfare_total_bar_2.png"));
    _totalProgress->setPosition(totalBarBottom->getPosition());
    _totalProgress->setType(kCCProgressTimerTypeBar);
    _totalProgress->setMidpoint(ccp(0, 0.5));
    _totalProgress->setBarChangeRate(ccp(1, 0));
    _totalProgress->setPercentage(100);
    _panel->addChild(_totalProgress);
    

//    _boxes = CCArray::create();
//    _boxes->retain();
//    
//    CoffersComp* coffers = DATA->getCoffers();
//    int curPhase = DATA->getPlayer()->phase;
//    int totalRatings = DATA->getPlayer()->ratings(curPhase);
//    
//    CCArray* items = coffers->phase_goals(curPhase);
//    CCDictionary* lastItem = (CCDictionary*)items->lastObject();
//    int end = ((CCInteger*)lastItem->objectForKey("goal"))->getValue();
//    float curPrecent = totalRatings / (float)end * 100;
//    CCLOG("curPrecent = %f", curPrecent);
//    _totalProgress->setPercentage(curPrecent);
//    
//    CCSprite* star = CCSprite::create("pic/building/progress/star.png");
//    star->setPosition(ccp(barSize.width * curPrecent / 100, barSize.height * 0.5));
//    _totalProgress->addChild(star);
//    
//    int count = items->count();
//    for (int i = 0; i < count; i++) {
//        CCDictionary* item = (CCDictionary*)items->objectAtIndex(i);
//        int itemGoal = ((CCInteger*)item->objectForKey("goal"))->getValue();
//        CCLOG("itemGoal = %d", itemGoal);
//        //        float precent = (itemGoal - start) / (float)(end - start);
//        float precent = itemGoal / (float)end;
//        
//        CCSprite* box = NULL;
//        float baseHeight = barSize.height * 0.5 + 5;
//        if (totalRatings >= itemGoal) {
//            CCString* itemId = (CCString*)item->objectForKey("id");
//            if (coffers->has_taken_reward(itemId)) {
//                CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d_taken.png", i);
//                box = CCSprite::create(boxName->getCString());
//                box->setPosition(ccp(barSize.width * precent, baseHeight));
//                box->setUserObject(CCBool::create(false));
//                totalBarBottom->addChild(box);
//            }
//            else {
//                CCString* lightName = CCString::createWithFormat("pic/building/progress/available_light_%d.png", i);
//                CCSprite* light = CCSprite::create(lightName->getCString());
//                light->setPosition(ccp(barSize.width * precent, baseHeight));
//                totalBarBottom->addChild(light);
//                
//                CCSequence* seq = CCSequence::create(CCFadeOut::create(0.6), CCDelayTime::create(0.5), CCFadeIn::create(0.6), CCDelayTime::create(0.3), NULL);
//                light->runAction(CCRepeatForever::create(seq));
//                
//                
//                CCString* spotName = CCString::createWithFormat("pic/building/progress/available_spot_%d.png", i);
//                CCSprite* spot = CCSprite::create(spotName->getCString());
//                spot->setPosition(ccp(light->getContentSize().width * 0.5, light->getContentSize().height * 0.7));
//                light->addChild(spot);
//                
//                CCSequence* seq2 = CCSequence::create(CCFadeOut::create(0.6), CCDelayTime::create(0.5), CCFadeIn::create(0.6), CCDelayTime::create(0.3), NULL);
//                spot->runAction(CCRepeatForever::create(seq2));
//                
//                
//                CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
//                box = CCSprite::create(boxName->getCString());
//                box->setPosition(light->getPosition());
//                box->setUserObject(CCBool::create(true));
//                totalBarBottom->addChild(box);
//            }
//        }
//        else {
//            CCString* boxName = CCString::createWithFormat("pic/building/progress/pack_%d.png", i);
//            box = CCSprite::create(boxName->getCString());
//            box->setPosition(ccp(barSize.width * precent, baseHeight));
//            box->setUserObject(CCBool::create(false));
//            totalBarBottom->addChild(box);
//        }
//        
//        box->setTag(i);
//        _boxes->addObject(box);
//        
//        // 奖励内容
//        CCSprite* plate = CCSprite::create("pic/building/reward_plate.png");
//        CCSize plateSize = plate->getContentSize();
//        plate->setPosition(ccp(plateSize.width * 0.5 - 10, 22));
//        box->addChild(plate);
//        
//        int rewardValue = ((CCInteger*)item->objectForKey("reward_value"))->getValue();
//        CCString* strReward = CCString::createWithFormat("%d", rewardValue);
//        CCLabelTTF* lblReward = CCLabelTTF::create(strReward->getCString(), DISPLAY->fangzhengFont(), 18.0f);
//        lblReward->setAnchorPoint(ccp(1, 0.5));
//        lblReward->setPosition(ccp(plateSize.width * 0.6, plateSize.height * 0.5 - 1));
//        lblReward->setColor(ccc3(127, 76, 76));
//        plate->addChild(lblReward);
//        
//        const CCString* rewardType = item->valueForKey("reward_type");
//        if (rewardType->compare("coin") == 0) {
//            CCSprite* icon = CCSprite::create("pic/building/reward_coin.png");
//            icon->setPosition(ccp(lblReward->getPositionX() + 10, lblReward->getPositionY()));
//            plate->addChild(icon);
//        }
//        else if (rewardType->compare("diam") == 0) {
//            CCSprite* icon = CCSprite::create("pic/building/reward_diam.png");
//            icon->setPosition(ccp(lblReward->getPositionX() + 10, lblReward->getPositionY()));
//            plate->addChild(icon);
//        }
//    }
}

void WelfarePanel::keyBackClicked(){
    num_child++;
    CCLog("===== WelfarePanel  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}

#pragma mark - CCTableViewDataSource

CCSize WelfarePanel::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return this->cellSizeForTable(table);
}

CCSize WelfarePanel::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCTableViewCell* WelfarePanel::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    WelfareCell* cell = new WelfareCell();
    WelfareItem* item = _dataSource->fetchWelfareItem(idx);
    cell->configWithWelfareItem((int)idx, item, CELL_WIDTH, CELL_HEIGHT);
    
    return cell;
}

unsigned int WelfarePanel::numberOfCellsInTableView(CCTableView *table) {
    return _dataSource->itemCount();
}

#pragma mark - CCTableViewDelegate

void WelfarePanel::tableCellTouched(CCTableView *table, CCTableViewCell *cell) {
//    CCNode* node = cell->getChildByTag(123);
//    int idx = cell->getIdx();
//    node->runAction(CCSequence::create(CCScaleTo::create(0.08, 0.9), CCScaleTo::create(0.06, 1.1), CCScaleTo::create(0.08, 0.95), CCScaleTo::create(0.06, 1.0), NULL));
    
}

void WelfarePanel::scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {
//    CCPoint contOffsetPos = view->getContentOffset();
//    if (this->numberOfCellsInTableView(_tv) > 3) {
//        //        _tv->setBounceable(true);
//        if (contOffsetPos.y < view->minContainerOffset().y) {
//            view->setContentOffset(CCPoint(contOffsetPos.x, view->minContainerOffset().y));
//        }else if (contOffsetPos.y > view->maxContainerOffset().y){
//            view->setContentOffset(CCPoint(contOffsetPos.x, view->maxContainerOffset().y));
//        }
//    }
}

void WelfarePanel::scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {
    
}


