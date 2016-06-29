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
    
    OpenToWhichOne = DATA->getPlayer()->next_mission;
    allNumber = CONFIG->mission()->count();
    selectedIndex = -1;
    
    pTableView = CCTableView::create(this, CCSizeMake(177, 110*def_Page_Index));
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
            
            CCString* buttonStr;
            if (getTaskIcon(cell->getIdx()) == 1) {
                buttonStr = CCString::createWithFormat("task_button_1_2.png");
            }else if (getTaskIcon(cell->getIdx()) == 2){
                buttonStr = CCString::createWithFormat("task_button_2_2.png");
            }else if (getTaskIcon(cell->getIdx()) == 3){
                buttonStr = CCString::createWithFormat("task_button_3_2.png");
            }else if (getTaskIcon(cell->getIdx()) == 4){
                buttonStr = CCString::createWithFormat("task_button_4_2.png");
            }else if (getTaskIcon(cell->getIdx()) == 5){
                buttonStr = CCString::createWithFormat("task_button_5_2.png");
            }
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* button = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buttonStr->getCString());
            button->setDisplayFrame(frame);
            
            CCNode* xingNode1 = button->getChildByTag(Tag_Task_Xing1);
            xingNode1->setScale(1.1f);
            CCNode* xingNode2 = button->getChildByTag(Tag_Task_Xing2);
            xingNode2->setScale(1.1f);
            CCNode* xingNode3 = button->getChildByTag(Tag_Task_Xing3);
            xingNode3->setScale(1.1f);
            
            xingNode1->setPosition(ccp(button->getContentSize().width* .5f - 1, button->getContentSize().height* .33f));
            xingNode2->setPosition(ccp(button->getContentSize().width* .65f, button->getContentSize().height* .33f));
            xingNode3->setPosition(ccp(button->getContentSize().width* .8f + 1, button->getContentSize().height* .33f));
            
            
            DATA->setTaskNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_Creat_Tishi");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_EnterTheTishi");
        }else if (selectedIndex == cell->getIdx()){
            selectedIndex = -1;
            
            sprNode = cell->getChildByTag(cell->getIdx());
            CCString* buttonStr;
            if (getTaskIcon(cell->getIdx()) == 1) {
                buttonStr = CCString::createWithFormat("task_button_1_1.png");
            }else if (getTaskIcon(cell->getIdx()) == 2){
                buttonStr = CCString::createWithFormat("task_button_2_1.png");
            }else if (getTaskIcon(cell->getIdx()) == 3){
                buttonStr = CCString::createWithFormat("task_button_3_1.png");
            }else if (getTaskIcon(cell->getIdx()) == 4){
                buttonStr = CCString::createWithFormat("task_button_4_1.png");
            }else if (getTaskIcon(cell->getIdx()) == 5){
                buttonStr = CCString::createWithFormat("task_button_5_1.png");
            }
            CCSprite* button = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buttonStr->getCString());
            button->setDisplayFrame(frame);
            
            CCNode* xingNode1 = button->getChildByTag(Tag_Task_Xing1);
            xingNode1->setScale(1.f);
            CCNode* xingNode2 = button->getChildByTag(Tag_Task_Xing2);
            xingNode2->setScale(1.f);
            CCNode* xingNode3 = button->getChildByTag(Tag_Task_Xing3);
            xingNode3->setScale(1.f);
            
            xingNode1->setPosition(ccp(button->getContentSize().width* .5f, button->getContentSize().height* .38f));
            xingNode2->setPosition(ccp(button->getContentSize().width* .65f, button->getContentSize().height* .38f));
            xingNode3->setPosition(ccp(button->getContentSize().width* .8f, button->getContentSize().height* .38f));
            
            DATA->setTaskNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_ExitTishi");
        }else{
            CCSprite* button1 = (CCSprite* )sprNode->getChildByTag(selectedIndex);
            CCString* buttonStr1;
            if (getTaskIcon(selectedIndex) == 1) {
                buttonStr1 = CCString::createWithFormat("task_button_1_1.png");
            }else if (getTaskIcon(selectedIndex) == 2){
                buttonStr1 = CCString::createWithFormat("task_button_2_1.png");
            }else if (getTaskIcon(selectedIndex) == 3){
                buttonStr1 = CCString::createWithFormat("task_button_3_1.png");
            }else if (getTaskIcon(selectedIndex) == 4){
                buttonStr1 = CCString::createWithFormat("task_button_4_1.png");
            }else if (getTaskIcon(selectedIndex) == 5){
                buttonStr1 = CCString::createWithFormat("task_button_5_1.png");
            }
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buttonStr1->getCString());
            button1->setDisplayFrame(frame);
            
            CCNode* xingNode1 = button1->getChildByTag(Tag_Task_Xing1);
            xingNode1->setScale(1.f);
            CCNode* xingNode2 = button1->getChildByTag(Tag_Task_Xing2);
            xingNode2->setScale(1.f);
            CCNode* xingNode3 = button1->getChildByTag(Tag_Task_Xing3);
            xingNode3->setScale(1.f);
            
            xingNode1->setPosition(ccp(button1->getContentSize().width* .5f, button1->getContentSize().height* .38f));
            xingNode2->setPosition(ccp(button1->getContentSize().width* .65f, button1->getContentSize().height* .38f));
            xingNode3->setPosition(ccp(button1->getContentSize().width* .8f, button1->getContentSize().height* .38f));
            
            
            selectedIndex = cell->getIdx();
            
            sprNode = cell->getChildByTag(selectedIndex);
            CCString* buttonStr2;
            if (getTaskIcon(selectedIndex) == 1) {
                buttonStr2 = CCString::createWithFormat("task_button_1_2.png");
            }else if (getTaskIcon(selectedIndex) == 2){
                buttonStr2 = CCString::createWithFormat("task_button_2_2.png");
            }else if (getTaskIcon(selectedIndex) == 3){
                buttonStr2 = CCString::createWithFormat("task_button_3_2.png");
            }else if (getTaskIcon(selectedIndex) == 4){
                buttonStr2 = CCString::createWithFormat("task_button_4_2.png");
            }else if (getTaskIcon(selectedIndex) == 5){
                buttonStr2 = CCString::createWithFormat("task_button_5_2.png");
            }
            CCSprite* button2 = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buttonStr2->getCString());
            button2->setDisplayFrame(frame2);
            
            CCNode* xingNode11 = button2->getChildByTag(Tag_Task_Xing1);
            xingNode11->setScale(1.1f);
            CCNode* xingNode22 = button2->getChildByTag(Tag_Task_Xing2);
            xingNode22->setScale(1.1f);
            CCNode* xingNode33 = button2->getChildByTag(Tag_Task_Xing3);
            xingNode33->setScale(1.1f);
            
            xingNode11->setPosition(ccp(button2->getContentSize().width* .5f - 1, button2->getContentSize().height* .33f));
            xingNode22->setPosition(ccp(button2->getContentSize().width* .65f, button2->getContentSize().height* .33f));
            xingNode33->setPosition(ccp(button2->getContentSize().width* .8f + 1, button2->getContentSize().height* .33f));
            
            
            DATA->setTaskNumber(selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_Creat_Tishi");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_EnterTheTishi");
        }
    }
}

//每个cell的size
cocos2d::CCSize TaskTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(177, 110);
}

//生成cell
cocos2d::extension::CCTableViewCell* TaskTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    
    if (idx < OpenToWhichOne) {
        CCSprite* button;
        if (getTaskIcon(idx) == 1) {
            button = CCSprite::createWithSpriteFrameName("task_button_1_1.png");
        }else if (getTaskIcon(idx) == 2){
            button = CCSprite::createWithSpriteFrameName("task_button_2_1.png");
        }else if (getTaskIcon(idx) == 3){
            button = CCSprite::createWithSpriteFrameName("task_button_3_1.png");
        }else if (getTaskIcon(idx) == 4){
            button = CCSprite::createWithSpriteFrameName("task_button_4_1.png");
        }else if (getTaskIcon(idx) == 5){
            button = CCSprite::createWithSpriteFrameName("task_button_5_1.png");
        }
        button->setAnchorPoint(CCPointZero);
        button->setPosition(CCPointZero);
        button->setTag(idx);
        spr->addChild(button);
        this->buttonStatus(idx, button);
        
    }else{
        CCSprite* button1;
        CCSprite* button2;
        if (getTaskIcon(idx) == 1) {
            button1 = CCSprite::createWithSpriteFrameName("task_button_1_1.png");
            button2 = CCSprite::createWithSpriteFrameName("task_button_wei_1.png");
        }else if (getTaskIcon(idx) == 2){
            button1 = CCSprite::createWithSpriteFrameName("task_button_2_1.png");
            button2 = CCSprite::createWithSpriteFrameName("task_button_wei_2.png");
        }else if (getTaskIcon(idx) == 3){
            button1 = CCSprite::createWithSpriteFrameName("task_button_3_1.png");
            button2 = CCSprite::createWithSpriteFrameName("task_button_wei_3.png");
        }else if (getTaskIcon(idx) == 4){
            button1 = CCSprite::createWithSpriteFrameName("task_button_4_1.png");
            button2 = CCSprite::createWithSpriteFrameName("task_button_wei_4.png");
        }else if (getTaskIcon(idx) == 5){
            button1 = CCSprite::createWithSpriteFrameName("task_button_5_1.png");
            button2 = CCSprite::createWithSpriteFrameName("task_button_wei_5.png");
        }
        button1->setAnchorPoint(CCPointZero);
        button1->setPosition(CCPointZero);
        button1->setTag(idx);
        spr->addChild(button1);
        
        button2->setAnchorPoint(CCPointZero);
        button2->setPosition(ccp(5.5f, 0));
        button1->addChild(button2);
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
    CCString* indexStr = CCString::createWithFormat("%d", index + 1);
    int num = DATA->getMission()->mission_rating(indexStr->getCString());
    
    CCSprite* xingDiSpr1,* xingDiSpr2,* xingDiSpr3,* xingDiSpr4,* xingDiSpr5;
    xingDiSpr1 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr1->setPosition(ccp(button->getContentSize().width* .5f, button->getContentSize().height* .38f));
    xingDiSpr1->setTag(Tag_Task_Xing1);
    button->addChild(xingDiSpr1);
    
    xingDiSpr2 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr2->setPosition(ccp(button->getContentSize().width* .65f, button->getContentSize().height* .38f));
    xingDiSpr2->setTag(Tag_Task_Xing2);
    button->addChild(xingDiSpr2);
    
    xingDiSpr3 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr3->setPosition(ccp(button->getContentSize().width* .8f, button->getContentSize().height* .38f));
    xingDiSpr3->setTag(Tag_Task_Xing3);
    button->addChild(xingDiSpr3);
    
    xingDiSpr4 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr4->setPosition(ccp(button->getContentSize().width* .91f, button->getContentSize().height* .54f));
    xingDiSpr4->setTag(Tag_Task_Xing4);
    button->addChild(xingDiSpr4);
    
    xingDiSpr5 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr5->setPosition(ccp(button->getContentSize().width* .92f, button->getContentSize().height* .77f));
    xingDiSpr5->setTag(Tag_Task_Xing5);
    button->addChild(xingDiSpr5);
    
    //
    CCSprite* xingSpr1,* xingSpr2,* xingSpr3,* xingSpr4,* xingSpr5;
    if (num >= 1) {
        xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr1->setAnchorPoint(CCPointZero);
        xingSpr1->setPosition(CCPointZero);
        xingDiSpr1->addChild(xingSpr1);
    }
    if (num >= 2) {
        xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr2->setAnchorPoint(CCPointZero);
        xingSpr2->setPosition(CCPointZero);
        xingDiSpr2->addChild(xingSpr2);
    }
    if (num >= 3) {
        xingSpr3 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr3->setAnchorPoint(CCPointZero);
        xingSpr3->setPosition(CCPointZero);
        xingDiSpr3->addChild(xingSpr3);
    }
    if (num >= 4) {
        xingSpr4 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr4->setAnchorPoint(CCPointZero);
        xingSpr4->setPosition(CCPointZero);
        xingDiSpr4->addChild(xingSpr4);
    }
    if (num >= 5) {
        xingSpr5 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr5->setAnchorPoint(CCPointZero);
        xingSpr5->setPosition(CCPointZero);
        xingDiSpr5->addChild(xingSpr5);
    }
}
CCString* TaskTableView::getTaskName(int index){
    CCArray* arr = CONFIG->mission();
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(index);
    
    return (CCString*)dic->valueForKey("name");
}
int TaskTableView::getTaskIcon(int index){
    CCArray* arr = CONFIG->mission();
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(index);
    
    return dic->valueForKey("icon")->intValue();
}



