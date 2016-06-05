//
//  TaskTableView.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "TaskTableView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"


TaskTableView::TaskTableView(){
    
}
TaskTableView::~TaskTableView(){
    
}

bool TaskTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/taskScene/task_button.plist");
    
    OpenToWhichOne = 30;
    allNumber = CONFIG->mission()->count();
    selectedIndex = -1;
    
    pTableView = CCTableView::create(this, CCSizeMake(170, 114*def_Page_Index));
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
void TaskTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){

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
void TaskTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    if (cell->getIdx() < OpenToWhichOne) {
        if (selectedIndex == -1) {
            selectedIndex = cell->getIdx();
            
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* button = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("task_button2.png");
            button->setDisplayFrame(frame);
            
            DATA->setTaskNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_Creat_Tishi");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_EnterTheTishi");
        }else if (selectedIndex == cell->getIdx()){
            selectedIndex = -1;
            
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* button = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("task_button1.png");
            button->setDisplayFrame(frame);
            
            DATA->setTaskNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_ExitTishi");
        }else{
            CCSprite* button1 = (CCSprite* )sprNode->getChildByTag(selectedIndex);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("task_button1.png");
            button1->setDisplayFrame(frame);
            
            selectedIndex = cell->getIdx();
            
            sprNode = cell->getChildByTag(selectedIndex);
            CCSprite* button2 = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("task_button2.png");
            button2->setDisplayFrame(frame2);
            
            DATA->setTaskNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_Creat_Tishi");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_EnterTheTishi");
        }
    }
}

//每个cell的size
cocos2d::CCSize TaskTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(170, 114);
}

//生成cell
cocos2d::extension::CCTableViewCell* TaskTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    
    if (idx < OpenToWhichOne) {
        CCSprite* button = CCSprite::createWithSpriteFrameName("task_button1.png");
        button->setAnchorPoint(CCPointZero);
        button->setPosition(CCPointZero);
        button->setTag(idx);
        spr->addChild(button);
        this->buttonStatus(idx, button);
        
        CCLabelTTF* label = CCLabelTTF::create(getTaskName(idx)->getCString(), DISPLAY->fangzhengFont(), 25);
        label->setPosition(ccp(button->getContentSize().width* .5f, button->getContentSize().height* .6f));
        label->setColor(ccWHITE);
//        label->enableStroke(ccWHITE, .4f);
        button->addChild(label, 2);
        
        CCLabelTTF* label1 = CCLabelTTF::create(getTaskName(idx)->getCString(), DISPLAY->fangzhengFont(), 25);
        label1->setPosition(ccp(button->getContentSize().width* .5f + 2, button->getContentSize().height* .6f - 2));
        label1->setColor(ccGRAY);
//        label1->enableStroke(ccGRAY, .4f);
        button->addChild(label1);
        
    }else{
        CCSprite* button = CCSprite::create("res/pic/taskScene/task_weijiesuo.png");
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
unsigned int TaskTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}

void TaskTableView::onEnter(){
    CCLayer::onEnter();
}

void TaskTableView::onExit(){
    CCLayer::onExit();
}

void TaskTableView::buttonStatus(int index, CCSprite* button){
    
    int num = 1 + rand()%3;
    
    CCMenuItem* xingItem1,* xingItem2,* xingItem3;
    CCMenu* menu;
    for (int i = 1; i <= num; i++) {
        if (i == 1) {
            CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing.png");
            CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing.png");
            xingItem1 = CCMenuItemSprite::create(xingSpr1, xingSpr2);
        }else if (i == 2){
            CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing.png");
            CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing.png");
            xingItem2 = CCMenuItemSprite::create(xingSpr1, xingSpr2);
        }else if (i == 3){
            CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing.png");
            CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing.png");
            xingItem3 = CCMenuItemSprite::create(xingSpr1, xingSpr2);
        }
    }
    if (num == 1) {
        menu = CCMenu::create(xingItem1, NULL);
    }else if (num == 2){
        menu = CCMenu::create(xingItem1, xingItem2, NULL);
    }else if (num == 3){
        menu = CCMenu::create(xingItem1, xingItem2, xingItem3, NULL);
    }
    menu->setAnchorPoint(ccp(.5f, .5f));
    menu->setPosition(ccp(button->getContentSize().width* .5f, button->getContentSize().height* .3f));
    menu->alignItemsHorizontallyWithPadding(2.f);
    button->addChild(menu);
    
}
CCString* TaskTableView::getTaskName(int index){
    CCArray* arr = CONFIG->mission();
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(index);
    
    return (CCString*)dic->valueForKey("name");
}




