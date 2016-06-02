//
//  QingjingTableView.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "QingjingTableView.h"
#include "DataManager.h"
#include "DisplayManager.h"


QingjingTableView::QingjingTableView(){
    
}
QingjingTableView::~QingjingTableView(){
    
}

bool QingjingTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/qingjingScene/qj_button.plist");
    
    OpenToWhichOne = 30;
    allNumber = 50;
    selectedIndex = -1;
    
    pTableView = CCTableView::create(this, CCSizeMake(156, 84*def_Page_Index + 15));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setAnchorPoint(CCPointZero);
    pTableView->setPosition(CCPointZero);
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    this->addChild(pTableView, 1);
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    
    return true;
}


//CCTableViewDelegate继承自CCScrollViewDelegate
void QingjingTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){

    CCPoint contOffsetPos = pTableView->getContentOffset();
    if (allNumber > 6) {
        if (contOffsetPos.y < pTableView->minContainerOffset().y) {
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->minContainerOffset().y));
        }else if (contOffsetPos.y > pTableView->maxContainerOffset().y){
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->maxContainerOffset().y));
        }
    }
}

//点击哪个cell
void QingjingTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    if (cell->getIdx() < OpenToWhichOne) {
        if (selectedIndex == -1) {
            selectedIndex = cell->getIdx();
            
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* button = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qj_button2.png");
            button->setDisplayFrame(frame);
            
            DATA->setChapterNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Qingjing_Creat_Tishi");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Qingjing_EnterTheTishi");
        }else if (selectedIndex == cell->getIdx()){
            selectedIndex = -1;
            
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* button = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qj_button1.png");
            button->setDisplayFrame(frame);
            
            DATA->setChapterNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Qingjing_ExitTishi");
        }else{
            CCSprite* button1 = (CCSprite* )sprNode->getChildByTag(selectedIndex);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qj_button1.png");
            button1->setDisplayFrame(frame);
            
            selectedIndex = cell->getIdx();
            
            sprNode = cell->getChildByTag(selectedIndex);
            CCSprite* button2 = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("qj_button2.png");
            button2->setDisplayFrame(frame2);
            
            DATA->setChapterNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Qingjing_Creat_Tishi");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Qingjing_EnterTheTishi");
        }
    }
}

//每个cell的size
cocos2d::CCSize QingjingTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(156, 86);
}

//生成cell
cocos2d::extension::CCTableViewCell* QingjingTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    
    if (idx < OpenToWhichOne) {
        CCSprite* button = CCSprite::createWithSpriteFrameName("qj_button1.png");
        button->setAnchorPoint(CCPointZero);
        button->setPosition(CCPointZero);
        button->setTag(idx);
        spr->addChild(button);
        
        CCLabelTTF* label = CCLabelTTF::create(DISPLAY->GetOffTheNumber(idx + 1)->getCString(), "Arial", 25);
        label->setPosition(ccp(button->getContentSize().width* .5f, button->getContentSize().height* .5f));
        label->setColor(ccWHITE);
        label->enableStroke(ccWHITE, 1.1f);
        button->addChild(label, 2);
        
        CCLabelTTF* label1 = CCLabelTTF::create(DISPLAY->GetOffTheNumber(idx + 1)->getCString(), "Arial", 25);
        label1->setPosition(ccp(button->getContentSize().width* .5f + 2, button->getContentSize().height* .5f - 2));
        label1->setColor(ccGRAY);
        label1->enableStroke(ccGRAY, 1.1f);
        button->addChild(label1);
        
    }else{
        CCSprite* button = CCSprite::create("res/pic/qingjingScene/qj_weijiesuo.png");
        button->setAnchorPoint(CCPointZero);
        button->setPosition(CCPointZero);
        button->setTag(idx);
        spr->addChild(button);
    }
    
    spr->setTag(idx);
    pCell->addChild(spr);
    
    return pCell;
}

//cell的数量
unsigned int QingjingTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}

void QingjingTableView::onEnter(){
    CCLayer::onEnter();
}

void QingjingTableView::onExit(){
    CCLayer::onExit();
}







