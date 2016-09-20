//
//  EmoticonPanel.cpp
//  tiegao
//
//  Created by mac on 16/8/23.
//
//

#include "EmoticonPanel.h"
#include "ConfigManager.h"
#include "DisplayManager.h"

#define CELL_WIDTH          187
#define CELL_HEIGHT         50

#pragma mark - Export

void EmoticonPanel::show(CCNode *parent) {
    EmoticonPanel* panel = EmoticonPanel::create();
    parent->addChild(panel);
}

#pragma mark - Inherit

EmoticonPanel::~EmoticonPanel() {
}

bool EmoticonPanel::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
//        CCSprite* mask = CCSprite::create("res/pic/mask.png");
//        mask->setPosition(DISPLAY->center());
//        this->addChild(mask);
        
        _panel = CCSprite::create("pic/clothesScene/gj_bgkuang.png");
        _panel->setAnchorPoint(ccp(1, 0.5));
        _panel->setPosition(ccp(DISPLAY->W(), DISPLAY->H() * 0.5));
        this->addChild(_panel);
        
        // pic size 542, 788
        float panelW = CELL_WIDTH;
        float panelH = 900;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->setDelegate(this);
        _panel->addChild(_tv);
        
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

void EmoticonPanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&EmoticonPanel::keyBackStatus), .8f);
}
void EmoticonPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void EmoticonPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

bool EmoticonPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = this->convertToNodeSpace(pTouch->getLocation());
    if (! _panel->boundingBox().containsPoint(location)) {
        //        this->do_exit();
        remove();
    }
    
    return false;
}

#pragma mark - Inner

void EmoticonPanel::do_enter() {

}

void EmoticonPanel::do_exit() {

}

void EmoticonPanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void EmoticonPanel::config_cell(CCTableViewCell* cell, int idx) {
    vector<string> emoticon = CONFIG->emoticon();
    CCLabelTTF* lbl = CCLabelTTF::create(emoticon[idx].c_str(), DISPLAY->fangzhengFont(), 24.f);
    lbl->setColor(ccBLACK);
    lbl->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    cell->addChild(lbl);
}

void EmoticonPanel::keyBackClicked(){
    num_child++;
    CCLog("===== EmoticonPanel  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->remove();
}

#pragma mark - CCTableViewDataSource

CCSize EmoticonPanel::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return this->cellSizeForTable(table);
}

CCSize EmoticonPanel::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCTableViewCell* EmoticonPanel::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell* cell = table->dequeueCell();
    if (cell) {
        cell->removeAllChildren();
    }
    else {
        cell = new CCTableViewCell();
    }

    this->config_cell(cell, idx);
    return cell;
}

unsigned int EmoticonPanel::numberOfCellsInTableView(CCTableView *table) {
    return CONFIG->emoticon().size();
}

#pragma mark - CCTableViewDelegate

void EmoticonPanel::scrollViewDidScroll(CCScrollView* view) {

}

void EmoticonPanel::scrollViewDidZoom(CCScrollView* view) {

}

void EmoticonPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    unsigned int idx = cell->getIdx();
    vector<string> emoticon = CONFIG->emoticon();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("ON_EMOTICON", ccs(emoticon[idx].c_str()));
}

void EmoticonPanel::tableCellHighlight(CCTableView* table, CCTableViewCell* cell) {

}

void EmoticonPanel::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell){

}

void EmoticonPanel::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell) {

}

