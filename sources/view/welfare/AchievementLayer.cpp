//
//  AchievementLayer.cpp
//  tiegao
//
//  Created by mac on 16/12/27.
//
//

#include "AchievementLayer.h"

#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "AudioManager.h"

#include "AchievementComp.h"
#include "AchievementCell.h"

#include "Loading2.h"

const float CELL_WIDTH = 470;
const float CELL_HEIGHT = 110;


#pragma mark - Export

void AchievementLayer::show() {
    AchievementLayer* panel = AchievementLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}

CCRect AchievementLayer::getBoundary() {
    return _panel->boundingBox();
}

#pragma mark - Supper

AchievementLayer::~AchievementLayer() {
}

bool AchievementLayer::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        _dataSource = DATA->getAchievement();
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("res/pic/welfare/white_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);

        // tableview
        float panelW = CELL_WIDTH;
        float panelH = 640;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->ignoreAnchorPointForPosition(false);
        _tv->setAnchorPoint(ccp(0.5, 1));
        _tv->setPosition(ccp(DISPLAY->halfW(), DISPLAY->halfH() + 315));
        _tv->setDelegate(this);
        this->addChild(_tv);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(false);
        
        return true;
    }
    else {
        return false;
    }
}

void AchievementLayer::onEnter() {
    CCLayer::onEnter();
    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
}

void AchievementLayer::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool AchievementLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCLOG("AchievementLayer::ccTouchBegan");
    
    return true;
}

#pragma mark - inner

void AchievementLayer::remove() {
    //    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_REMOVE");
}

void AchievementLayer::disappear_cell(AchievementCell* node) {
    CCCallFunc* finished = CCCallFunc::create(this, SEL_CallFunc(&AchievementLayer::disappear_finished));
    
    CCSequence* seq = CCSequence::create(CCEaseBackIn::create(CCScaleTo::create(0.7, 0.1)), finished, NULL);
    node->runAction(seq);
}

void AchievementLayer::disappear_finished() {
    _tv->reloadData();
}

#pragma mark - CCTableViewDataSource

CCSize AchievementLayer::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return this->cellSizeForTable(table);
}

CCSize AchievementLayer::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCTableViewCell* AchievementLayer::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell* cell = new CCTableViewCell();
    AchievementItem* item = _dataSource->fetchItem(idx);
    
    AchievementCell* itemCell = AchievementCell::create("pic/welfare/welfare_plane.png");
    itemCell->configWithAchievementItem((int)idx, item, CELL_WIDTH, CELL_HEIGHT);
    itemCell->setAnchorPoint(ccp(0.5f, 0.5f));
    itemCell->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    itemCell->setTag(1);
    cell->addChild(itemCell);
    
    return cell;
}

unsigned int AchievementLayer::numberOfCellsInTableView(CCTableView *table) {
    return _dataSource->itemCount();
}

#pragma mark - CCTableViewDelegate

void AchievementLayer::tableCellTouched(CCTableView *table, CCTableViewCell *cell) {
    //    CCNode* node = cell->getChildByTag(123);
    //    int idx = cell->getIdx();
    //    node->runAction(CCSequence::create(CCScaleTo::create(0.08, 0.9), CCScaleTo::create(0.06, 1.1), CCScaleTo::create(0.08, 0.95), CCScaleTo::create(0.06, 1.0), NULL));
    
    //    WelfareCell* itemCell = (WelfareCell* )cell->getChildByTag(1);
    //    this->disappear_cell(itemCell);
}

void AchievementLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {
    CCPoint contOffsetPos = view->getContentOffset();
    if (this->numberOfCellsInTableView(_tv) > 5) {
        //        _tv->setBounceable(true);
        if (contOffsetPos.y < view->minContainerOffset().y) {
            view->setContentOffset(CCPoint(contOffsetPos.x, view->minContainerOffset().y));
        }else if (contOffsetPos.y > view->maxContainerOffset().y){
            view->setContentOffset(CCPoint(contOffsetPos.x, view->maxContainerOffset().y));
        }
    }
}

void AchievementLayer::scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {
    
}

