//
//  BuildingView.cpp
//  tiegao
//
//  Created by mac on 16/7/6.
//
//

#include "BuildingView.h"
#include "FloorCell.h"
#include "UnreusedTableView.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"

BuildingView::~BuildingView() {
    CC_SAFE_RELEASE_NULL(_floors);
}

BuildingView* BuildingView::create(int phase) {
    BuildingView* rtn = new BuildingView();
    if (rtn && rtn->init(phase)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool BuildingView::init(int phase) {
    if (! CCLayer::init()) {
        return false;
    }
    
    _phase = phase;
    
    _floors = CCArray::create();
    _floors->retain();
    
    this->setTouchEnabled(true);
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    float viewHeight = MIN(DISPLAY->H() * 0.85, FLOOR_CELL_HEIGHT * (_phase + 3));
    _tbView = UnreusedTableView::create(this, CCSizeMake(FLOOR_CELL_WIDTH, viewHeight));
    _tbView->setDirection(kCCScrollViewDirectionVertical);
    _tbView->setVerticalFillOrder(kCCTableViewFillBottomUp);
    _tbView->setPosition(ccp((DISPLAY->W() - FLOOR_CELL_WIDTH) * 0.5, DISPLAY->H() * (1 - 0.85) * 0.5));
    _tbView->setEageBounched(false);
    _tbView->setDelegate(this);
    this->addChild(_tbView);
    
    CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
    txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.06));
    this->addChild(txt_close);
    
    return true;
}

void BuildingView::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&BuildingView::nc_collect_coin), "COLLECT_COIN", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BuildingView::nc_collect_coin_201), "HTTP_FINISHED_201", NULL);
    
    _tbView->setContentOffset(CCPointZero);
    schedule(SEL_SCHEDULE(&BuildingView::update_produce), CCRANDOM_0_1() * 10);
}

void BuildingView::onExit() {
    unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void BuildingView::update_produce(float dt) {
    if (DATA->could_prduce()) {
        int count = numberOfCellsInTableView(_tbView) - 1;
        if (count > 0) {
            DATA->getCoffers()->produce();
            int randFloor = floor(CCRANDOM_0_1() * count);
            FloorCell* floorCell = (FloorCell*)_tbView->cellAtIndex(randFloor);
            floorCell->show_coin();
        }
    }
}

bool BuildingView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint pos = pTouch->getLocation();
    CCRect rect = _tbView->boundingBox();
    if (! rect.containsPoint(pos)) {
        this->go_back();
        return true;
    }
    
    return false;
}

void BuildingView::scrollViewDidScroll(CCScrollView *view) {

}

void BuildingView::scrollViewDidZoom(CCScrollView *view) {

}

#pragma mark - Inner

void BuildingView::start() {
    
}

void BuildingView::go_back() {
    this->removeFromParentAndCleanup(true);
}

void BuildingView::nc_collect_coin(CCObject *pObj) {
    NET->collect_coin_201();
}

void BuildingView::nc_collect_coin_201(CCObject *pObj) {
    if (_tbView && numberOfCellsInTableView(_tbView) > 0) {
        FloorCell* floor = (FloorCell*)_tbView->cellAtIndex(0);
        floor->update_coffers();
    }
}

#pragma mark - CCTableViewDataSource

CCSize BuildingView::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(FLOOR_CELL_WIDTH, FLOOR_CELL_HEIGHT);
}

CCSize BuildingView::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return cellSizeForTable(table);
};

CCTableViewCell* BuildingView::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCLOG("cell index = %d", idx);
    FloorCell* cell = (FloorCell*)table->dequeueCell();
    int floorCount = numberOfCellsInTableView(table);
    if (! cell) {
        CCLOG("NEW !!!");
        if (idx == 0) { // 前台
            cell = FloorCell::create(FloorCellType_Reception, _phase, idx);
        }
        else if (idx == floorCount - 1) { // 屋顶
            cell = FloorCell::create(FloorCellType_Roof, _phase, idx);
        }
        else if (idx == floorCount - 2) { // 总裁办
            cell = FloorCell::create(FloorCellType_Manager_Office, _phase, idx);
        }
        else {
            if (_phase > 3 && idx == floorCount - 3) { // Bar
                cell = FloorCell::create(FloorCellType_Leisure, _phase, idx);
            }
            else {
                cell = FloorCell::create(FloorCellType_Office, _phase, idx);
            }
        }
    }
    
    return cell;
}

unsigned int BuildingView::numberOfCellsInTableView(CCTableView *table) {
    return _phase + 3;
}

#pragma mark - CCTableViewDelegate

void BuildingView::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    FloorCell* floor = (FloorCell*)cell;
    floor->collected_coin();
}
