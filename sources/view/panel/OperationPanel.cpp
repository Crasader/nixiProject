//
//  OperationPanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "OperationPanel.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "Loading2.h"
#include "AudioManager.h"

const float CELL_WIDTH = 500;
const float CELL_HEIGHT = 200;


#pragma mark - Export

void OperationPanel::show() {
    OperationPanel* panel = OperationPanel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}

#pragma mark - Supper

OperationPanel::~OperationPanel() {
}

bool OperationPanel::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("res/pic/panel/operation/operation_bg.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        _content->addChild(txt_close);
        
        float panelW = CELL_WIDTH;
        float panelH = CELL_HEIGHT * 3.2;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->ignoreAnchorPointForPosition(false);
        _tv->setAnchorPoint(ccp(0.5, 0.5));
        _tv->setPosition(ccp(DISPLAY->halfW(), DISPLAY->halfH() - 10));
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

void OperationPanel::onEnter() {
    CCLayer::onEnter();
    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    this->scheduleOnce(SEL_SCHEDULE(&OperationPanel::keyBackStatus), .8f);
}

void OperationPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void OperationPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

bool OperationPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    _touchLocation = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(_touchLocation)) {
        remove();
    }
    
    return true;
}

#pragma mark - inner

void OperationPanel::remove() {
    AUDIO->goback_effect();
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void OperationPanel::on_purchase() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_PURCHASEPANEL");
}

void OperationPanel::on_purchase_achievement() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_RECHARTE");
}

void OperationPanel::on_energy_largess() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_ENERGY_GARLESS");
}

void OperationPanel::on_signin7() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_SIGNIN7");
}

void OperationPanel::on_gashapon() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_GASHAPON");
}

void OperationPanel::keyBackClicked(){
    num_child++;
    CCLog("===== OperationPanel  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}

#pragma mark - CCTableViewDataSource

CCSize OperationPanel::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return this->cellSizeForTable(table);
}

CCSize OperationPanel::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCTableViewCell* OperationPanel::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell* cell = new CCTableViewCell();
    
    CCSprite* spt = NULL;
    switch (idx) {
        case 0: {
            spt = CCSprite::create("pic/panel/operation/operation_purchase.png");
        } break;
            
        case 1: {
            spt = CCSprite::create("pic/panel/operation/operation_purchase_achievement.png");
        } break;
            
        case 2: {
            spt = CCSprite::create("pic/panel/operation/operation_energy_largess.png");
        } break;
            
        case 3: {
            spt = CCSprite::create("pic/panel/operation/operation_signin7.png");
        } break;
            
        case 4: {
            spt = CCSprite::create("pic/panel/operation/operation_gashapon.png");
        } break;
            
        default:
            break;
    }
    
    if (spt) {
        spt->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
        cell->addChild(spt);
        spt->setTag(123);
    }
    
    return cell;
}

unsigned int OperationPanel::numberOfCellsInTableView(CCTableView *table) {
    return 5;
}

#pragma mark - CCTableViewDelegate

void OperationPanel::tableCellTouched(CCTableView *table, CCTableViewCell *cell) {
    CCNode* node = cell->getChildByTag(123);
//    node->stopAllActions();
    int idx = cell->getIdx();
    node->runAction(CCSequence::create(CCScaleTo::create(0.08, 0.9), CCScaleTo::create(0.06, 1.1), CCScaleTo::create(0.08, 0.95), CCScaleTo::create(0.06, 1.0), NULL));
    switch (idx) {
        case 0: {
            this->on_purchase();
        } break;
            
        case 1: {
            this->on_purchase_achievement();
        } break;
            
        case 2: {
            this->on_energy_largess();
        } break;
            
        case 3: {
            this->on_signin7();
        } break;
            
        case 4: {
            this->on_gashapon();
        } break;
            
        default:
            break;
    }
}

void OperationPanel::scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {
    CCPoint contOffsetPos = view->getContentOffset();
    if (this->numberOfCellsInTableView(_tv) > 3) {
        //        _tv->setBounceable(true);
        if (contOffsetPos.y < view->minContainerOffset().y) {
            view->setContentOffset(CCPoint(contOffsetPos.x, view->minContainerOffset().y));
        }else if (contOffsetPos.y > view->maxContainerOffset().y){
            view->setContentOffset(CCPoint(contOffsetPos.x, view->maxContainerOffset().y));
        }
    }
}

void OperationPanel::scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {
    
}


