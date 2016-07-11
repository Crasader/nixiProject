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
    
    taskMission = DATA->getTaskSource();
    ratingDic =  DATA->getPlayer()->rating;
    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/taskScene/task_button.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/taskScene/task_newButton.plist");
    
    allNumber = taskMission->count();
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
    
    OpenToWhichOne = cell->getIdx();
    int unlockCondition = DATA->getPlayer()->mission;
    if (OpenToWhichOne <= unlockCondition) {
        if (selectedIndex == -1) {
            selectedIndex = cell->getIdx();
            
            CCString* buttonStr;
            CCSprite* renSpr;
            if (getTaskIcon(cell->getIdx()) == 1) {
                buttonStr = CCString::createWithFormat("task_button_1_2.png");
                renSpr = CCSprite::create("res/pic/taskScene/task_ren1.png");
            }else if (getTaskIcon(cell->getIdx()) == 2){
                buttonStr = CCString::createWithFormat("task_button_2_2.png");
                renSpr = CCSprite::create("res/pic/taskScene/task_ren2.png");
            }else if (getTaskIcon(cell->getIdx()) == 3){
                buttonStr = CCString::createWithFormat("task_button_3_2.png");
                renSpr = CCSprite::create("res/pic/taskScene/task_ren3.png");
            }else if (getTaskIcon(cell->getIdx()) == 4){
                buttonStr = CCString::createWithFormat("task_button_4_2.png");
                renSpr = CCSprite::create("res/pic/taskScene/task_ren4.png");
            }else if (getTaskIcon(cell->getIdx()) == 5){
                buttonStr = CCString::createWithFormat("task_button_5_2.png");
                renSpr = CCSprite::create("res/pic/taskScene/task_ren5.png");
            }
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* button = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buttonStr->getCString());
            button->setDisplayFrame(frame);
            
            
            CCSprite* tiliSpr = (CCSprite* )button->getChildByTag(cell->getIdx() + 30000);
            tiliSpr->setVisible(true);
            CCSprite* startSpr = (CCSprite* )button->getChildByTag(cell->getIdx() + 40000);
            startSpr->setVisible(true);
            
            if (button->getChildByTag(cell->getIdx() + 10000) != NULL) {
                button->removeChildByTag(cell->getIdx() + 10000);
            }
            if (button->getChildByTag(cell->getIdx() + 20000) != NULL) {
                button->removeChildByTag(cell->getIdx() + 20000);
            }
            renSpr->setTag(cell->getIdx() + 10000);
            renSpr->setScale(1.1f);
            renSpr->setPosition(ccp(button->getContentSize().width* .19f, button->getContentSize().height* .62f));
            button->addChild(renSpr, 5);
            
            CCDictionary* dic2 = (CCDictionary* )taskMission->objectAtIndex(cell->getIdx());
            CCLabelTTF* titleLabel = CCLabelTTF::create(((CCString*)dic2->valueForKey("name"))->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(button->getContentSize().width* .9f, button->getContentSize().height* .8f), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            titleLabel->setPosition(ccp(button->getContentSize().width* .64f, button->getContentSize().height* .63f));
            titleLabel->setScale(1.1f);
            titleLabel->setColor(ccWHITE);
            titleLabel->setTag(cell->getIdx() + 20000);
            button->addChild(titleLabel, 10);
            
            CCNode* xingNode1 = button->getChildByTag(Tag_Task_Xing1);
            xingNode1->setVisible(false);
            CCNode* xingNode2 = button->getChildByTag(Tag_Task_Xing2);
            xingNode2->setVisible(false);
            CCNode* xingNode3 = button->getChildByTag(Tag_Task_Xing3);
            xingNode3->setVisible(false);
            CCNode* xingNode4 = button->getChildByTag(Tag_Task_Xing4);
            xingNode4->setVisible(false);
            CCNode* xingNode5 = button->getChildByTag(Tag_Task_Xing5);
            xingNode5->setVisible(false);
//            CCNode* xingNode1 = button->getChildByTag(Tag_Task_Xing1);
//            xingNode1->setScale(.8f);
//            CCNode* xingNode2 = button->getChildByTag(Tag_Task_Xing2);
//            xingNode2->setScale(.8f);
//            CCNode* xingNode3 = button->getChildByTag(Tag_Task_Xing3);
//            xingNode3->setScale(.8f);
//            CCNode* xingNode4 = button->getChildByTag(Tag_Task_Xing4);
//            xingNode4->setScale(.8f);
//            CCNode* xingNode5 = button->getChildByTag(Tag_Task_Xing5);
//            xingNode5->setScale(.8f);
//            
//            xingNode1->setPosition(ccp(button->getContentSize().width* .28f, button->getContentSize().height* .29f));
//            xingNode2->setPosition(ccp(button->getContentSize().width* .41f, button->getContentSize().height* .29f));
//            xingNode3->setPosition(ccp(button->getContentSize().width* .54f, button->getContentSize().height* .29f));
//            xingNode4->setPosition(ccp(button->getContentSize().width* .67f, button->getContentSize().height* .29f));
//            xingNode5->setPosition(ccp(button->getContentSize().width* .8f, button->getContentSize().height* .29f));
            
            
            DATA->setTaskNumber(selectedIndex);
//            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_Creat_Tishi");
//            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_EnterTheTishi");
        }else if (selectedIndex == cell->getIdx()){
//            selectedIndex = -1;
//            
//            sprNode = cell->getChildByTag(cell->getIdx());
//            CCString* buttonStr;
//            CCSprite* renSpr;
//            if (getTaskIcon(cell->getIdx()) == 1) {
//                buttonStr = CCString::createWithFormat("task_button_1_1.png");
//                renSpr = CCSprite::create("res/pic/taskScene/task_ren1.png");
//            }else if (getTaskIcon(cell->getIdx()) == 2){
//                buttonStr = CCString::createWithFormat("task_button_2_1.png");
//                renSpr = CCSprite::create("res/pic/taskScene/task_ren2.png");
//            }else if (getTaskIcon(cell->getIdx()) == 3){
//                buttonStr = CCString::createWithFormat("task_button_3_1.png");
//                renSpr = CCSprite::create("res/pic/taskScene/task_ren3.png");
//            }else if (getTaskIcon(cell->getIdx()) == 4){
//                buttonStr = CCString::createWithFormat("task_button_4_1.png");
//                renSpr = CCSprite::create("res/pic/taskScene/task_ren4.png");
//            }else if (getTaskIcon(cell->getIdx()) == 5){
//                buttonStr = CCString::createWithFormat("task_button_5_1.png");
//                renSpr = CCSprite::create("res/pic/taskScene/task_ren5.png");
//            }
//            CCSprite* button = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
//            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buttonStr->getCString());
//            button->setDisplayFrame(frame);
//            
//            
//            CCSprite* tiliSpr = (CCSprite* )button->getChildByTag(cell->getIdx() + 30000);
//            tiliSpr->setVisible(false);
//            CCSprite* startSpr = (CCSprite* )button->getChildByTag(cell->getIdx() + 40000);
//            startSpr->setVisible(false);
//            
//            if (button->getChildByTag(cell->getIdx() + 10000) != NULL) {
//                button->removeChildByTag(cell->getIdx() + 10000);
//            }
//            if (button->getChildByTag(cell->getIdx() + 20000) != NULL) {
//                button->removeChildByTag(cell->getIdx() + 20000);
//            }
//            renSpr->setTag(cell->getIdx() + 10000);
//            renSpr->setScale(1.f);
//            renSpr->setPosition(ccp(button->getContentSize().width* .22f, button->getContentSize().height* .62f));
//            button->addChild(renSpr, 5);
//            
//            CCArray* taskArr = CONFIG->mission();
//            CCDictionary* dic2 = (CCDictionary* )taskArr->objectAtIndex(cell->getIdx());
//            CCLabelTTF* titleLabel = CCLabelTTF::create(((CCString*)dic2->valueForKey("name"))->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(button->getContentSize().width* .9f, button->getContentSize().height* .8f), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//            titleLabel->setPosition(ccp(button->getContentSize().width* .63f, button->getContentSize().height* .63f));
//            titleLabel->setScale(1.f);
//            titleLabel->setColor(ccWHITE);
//            titleLabel->setTag(cell->getIdx() + 20000);
//            button->addChild(titleLabel, 10);
//            
//            CCNode* xingNode1 = button->getChildByTag(Tag_Task_Xing1);
//            xingNode1->setScale(.8f);
//            CCNode* xingNode2 = button->getChildByTag(Tag_Task_Xing2);
//            xingNode2->setScale(.8f);
//            CCNode* xingNode3 = button->getChildByTag(Tag_Task_Xing3);
//            xingNode3->setScale(.8f);
//            CCNode* xingNode4 = button->getChildByTag(Tag_Task_Xing4);
//            xingNode4->setScale(.8f);
//            CCNode* xingNode5 = button->getChildByTag(Tag_Task_Xing5);
//            xingNode5->setScale(.8f);
//            
//            xingNode1->setPosition(ccp(button->getContentSize().width* .28f, button->getContentSize().height* .34f));
//            xingNode2->setPosition(ccp(button->getContentSize().width* .41f, button->getContentSize().height* .34f));
//            xingNode3->setPosition(ccp(button->getContentSize().width* .54f, button->getContentSize().height* .34f));
//            xingNode4->setPosition(ccp(button->getContentSize().width* .67f, button->getContentSize().height* .34f));
//            xingNode5->setPosition(ccp(button->getContentSize().width* .8f, button->getContentSize().height* .34f));
//            
//            DATA->setTaskNumber(selectedIndex);
//            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_ExitTishi");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_ExitView");
            pTableView->setTouchEnabled(false);
        }else{
            CCSprite* button1 = (CCSprite* )sprNode->getChildByTag(selectedIndex);
            CCString* buttonStr1;
            CCSprite* renSpr1;
            if (getTaskIcon(selectedIndex) == 1) {
                buttonStr1 = CCString::createWithFormat("task_button_1_1.png");
                renSpr1 = CCSprite::create("res/pic/taskScene/task_ren1.png");
            }else if (getTaskIcon(selectedIndex) == 2){
                buttonStr1 = CCString::createWithFormat("task_button_2_1.png");
                renSpr1 = CCSprite::create("res/pic/taskScene/task_ren2.png");
            }else if (getTaskIcon(selectedIndex) == 3){
                buttonStr1 = CCString::createWithFormat("task_button_3_1.png");
                renSpr1 = CCSprite::create("res/pic/taskScene/task_ren3.png");
            }else if (getTaskIcon(selectedIndex) == 4){
                buttonStr1 = CCString::createWithFormat("task_button_4_1.png");
                renSpr1 = CCSprite::create("res/pic/taskScene/task_ren4.png");
            }else if (getTaskIcon(selectedIndex) == 5){
                buttonStr1 = CCString::createWithFormat("task_button_5_1.png");
                renSpr1 = CCSprite::create("res/pic/taskScene/task_ren5.png");
            }
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buttonStr1->getCString());
            button1->setDisplayFrame(frame);
            
            CCSprite* tiliSpr1 = (CCSprite* )button1->getChildByTag(selectedIndex + 30000);
            tiliSpr1->setVisible(false);
            CCSprite* startSpr1 = (CCSprite* )button1->getChildByTag(selectedIndex + 40000);
            startSpr1->setVisible(false);
            
            if (button1->getChildByTag(selectedIndex + 10000) != NULL) {
                button1->removeChildByTag(selectedIndex + 10000);
            }
            if (button1->getChildByTag(selectedIndex + 20000) != NULL) {
                button1->removeChildByTag(selectedIndex + 20000);
            }
            renSpr1->setTag(selectedIndex + 10000);
            renSpr1->setScale(1.f);
            renSpr1->setPosition(ccp(button1->getContentSize().width* .22f, button1->getContentSize().height* .62f));
            button1->addChild(renSpr1, 5);
            
            CCDictionary* titleDic1 = (CCDictionary* )taskMission->objectAtIndex(selectedIndex);
            CCLabelTTF* titleLabel1 = CCLabelTTF::create(((CCString*)titleDic1->valueForKey("name"))->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(button1->getContentSize().width* .9f, button1->getContentSize().height* .8f), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            titleLabel1->setPosition(ccp(button1->getContentSize().width* .63f, button1->getContentSize().height* .63f));
            titleLabel1->setScale(1.f);
            titleLabel1->setColor(ccWHITE);
            titleLabel1->setTag(selectedIndex + 20000);
            button1->addChild(titleLabel1, 10);
            
            CCNode* xingNode1 = button1->getChildByTag(Tag_Task_Xing1);
            xingNode1->setScale(.8f);
            CCNode* xingNode2 = button1->getChildByTag(Tag_Task_Xing2);
            xingNode2->setScale(.8f);
            CCNode* xingNode3 = button1->getChildByTag(Tag_Task_Xing3);
            xingNode3->setScale(.8f);
            CCNode* xingNode4 = button1->getChildByTag(Tag_Task_Xing4);
            xingNode4->setScale(.8f);
            CCNode* xingNode5 = button1->getChildByTag(Tag_Task_Xing5);
            xingNode5->setScale(.8f);
            
            xingNode1->setPosition(ccp(button1->getContentSize().width* .28f, button1->getContentSize().height* .34f));
            xingNode2->setPosition(ccp(button1->getContentSize().width* .41f, button1->getContentSize().height* .34f));
            xingNode3->setPosition(ccp(button1->getContentSize().width* .54f, button1->getContentSize().height* .34f));
            xingNode4->setPosition(ccp(button1->getContentSize().width* .67f, button1->getContentSize().height* .34f));
            xingNode5->setPosition(ccp(button1->getContentSize().width* .8f, button1->getContentSize().height* .34f));
            
            xingNode1->setVisible(true);
            xingNode2->setVisible(true);
            xingNode3->setVisible(true);
            xingNode4->setVisible(true);
            xingNode5->setVisible(true);
            
            
            selectedIndex = cell->getIdx();
            
            sprNode = cell->getChildByTag(selectedIndex);
            CCString* buttonStr2;
            CCSprite* renSpr2;
            if (getTaskIcon(selectedIndex) == 1) {
                buttonStr2 = CCString::createWithFormat("task_button_1_2.png");
                renSpr2 = CCSprite::create("res/pic/taskScene/task_ren1.png");
            }else if (getTaskIcon(selectedIndex) == 2){
                buttonStr2 = CCString::createWithFormat("task_button_2_2.png");
                renSpr2 = CCSprite::create("res/pic/taskScene/task_ren2.png");
            }else if (getTaskIcon(selectedIndex) == 3){
                buttonStr2 = CCString::createWithFormat("task_button_3_2.png");
                renSpr2 = CCSprite::create("res/pic/taskScene/task_ren3.png");
            }else if (getTaskIcon(selectedIndex) == 4){
                buttonStr2 = CCString::createWithFormat("task_button_4_2.png");
                renSpr2 = CCSprite::create("res/pic/taskScene/task_ren4.png");
            }else if (getTaskIcon(selectedIndex) == 5){
                buttonStr2 = CCString::createWithFormat("task_button_5_2.png");
                renSpr2 = CCSprite::create("res/pic/taskScene/task_ren5.png");
            }
            CCSprite* button2 = (CCSprite* )sprNode->getChildByTag(selectedIndex);
            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buttonStr2->getCString());
            button2->setDisplayFrame(frame2);
            
            CCSprite* tiliSpr2 = (CCSprite* )button2->getChildByTag(selectedIndex + 30000);
            tiliSpr2->setVisible(true);
            CCSprite* startSpr2 = (CCSprite* )button2->getChildByTag(selectedIndex + 40000);
            startSpr2->setVisible(true);
            
            if (button2->getChildByTag(selectedIndex + 10000) != NULL) {
                button2->removeChildByTag(selectedIndex + 10000);
            }
            if (button2->getChildByTag(selectedIndex + 20000) != NULL) {
                button2->removeChildByTag(selectedIndex + 20000);
            }
            renSpr2->setTag(selectedIndex + 10000);
            renSpr2->setScale(1.1f);
            renSpr2->setPosition(ccp(button1->getContentSize().width* .19f, button1->getContentSize().height* .62f));
            button2->addChild(renSpr2, 5);
            
            CCDictionary* titleDic2 = (CCDictionary* )taskMission->objectAtIndex(selectedIndex);
            CCLabelTTF* titleLabel2 = CCLabelTTF::create(((CCString*)titleDic2->valueForKey("name"))->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(button1->getContentSize().width* .9f, button1->getContentSize().height* .8f), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            titleLabel2->setPosition(ccp(button1->getContentSize().width* .64f, button1->getContentSize().height* .63f));
            titleLabel2->setScale(1.1f);
            titleLabel2->setColor(ccWHITE);
            titleLabel2->setTag(selectedIndex + 20000);
            button2->addChild(titleLabel2, 10);
            
            
            CCNode* xingNode11 = button2->getChildByTag(Tag_Task_Xing1);
            xingNode11->setVisible(false);
            CCNode* xingNode22 = button2->getChildByTag(Tag_Task_Xing2);
            xingNode22->setVisible(false);
            CCNode* xingNode33 = button2->getChildByTag(Tag_Task_Xing3);
            xingNode33->setVisible(false);
            CCNode* xingNode44 = button2->getChildByTag(Tag_Task_Xing4);
            xingNode44->setVisible(false);
            CCNode* xingNode55 = button2->getChildByTag(Tag_Task_Xing5);
            xingNode55->setVisible(false);
//
//            xingNode11->setPosition(ccp(button2->getContentSize().width* .28f, button2->getContentSize().height* .29f));
//            xingNode22->setPosition(ccp(button2->getContentSize().width* .41f, button2->getContentSize().height* .29f));
//            xingNode33->setPosition(ccp(button2->getContentSize().width* .54f, button2->getContentSize().height* .29f));
//            xingNode44->setPosition(ccp(button2->getContentSize().width* .67f, button2->getContentSize().height* .29f));
//            xingNode55->setPosition(ccp(button2->getContentSize().width* .8f, button2->getContentSize().height* .29f));
            
            
            DATA->setTaskNumber(selectedIndex);
//            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_Creat_Tishi");
//            CCNotificationCenter::sharedNotificationCenter()->postNotification("Task_EnterTheTishi");
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
    
    OpenToWhichOne = idx;
    
    int unlockCondition = DATA->getPlayer()->mission;
    if (OpenToWhichOne <= unlockCondition) {
        
        CCString* buttonStr;
        if (selectedIndex == idx) {
            buttonStr = CCString::createWithFormat("task_button_%d_2.png", getTaskIcon(idx));
        }else{
            buttonStr = CCString::createWithFormat("task_button_%d_1.png", getTaskIcon(idx));
        }
        CCSprite* button = CCSprite::createWithSpriteFrameName(buttonStr->getCString());
        button->setAnchorPoint(ccp(.5f, .5f));
        button->setPosition(ccp(button->getContentSize().width* .5f, button->getContentSize().height* .5f));
        button->setTag(idx);
        spr->addChild(button);
        
        CCString* renStr = CCString::createWithFormat("res/pic/taskScene/task_ren%d.png", getTaskIcon(idx));
        CCSprite* renSpr = CCSprite::create(renStr->getCString());
        if (selectedIndex == idx) {
            renSpr->setPosition(ccp(button->getContentSize().width* .19f, button->getContentSize().height* .62f));
            renSpr->setScale(1.1f);
        }else{
            renSpr->setPosition(ccp(button->getContentSize().width* .22f, button->getContentSize().height* .62f));
            renSpr->setScale(1.f);
        }
        renSpr->setTag(idx + 10000);
        button->addChild(renSpr, 5);
    
        CCDictionary* dic = (CCDictionary* )taskMission->objectAtIndex(idx);
        int taskId = dic->valueForKey("id")->intValue();
        
        this->buttonStatus(taskId, button);
        
        CCDictionary* dic2 = (CCDictionary* )taskMission->objectAtIndex(idx);
        CCLabelTTF* titleLabel = CCLabelTTF::create(((CCString*)dic2->valueForKey("name"))->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(button->getContentSize().width* .9f, button->getContentSize().height* .8f), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        if (selectedIndex == idx) {
            titleLabel->setPosition(ccp(button->getContentSize().width* .63f, button->getContentSize().height* .63f));
            titleLabel->setScale(1.1f);
        }else{
            titleLabel->setPosition(ccp(button->getContentSize().width* .63f, button->getContentSize().height* .63f));
            titleLabel->setScale(1.f);
        }
        titleLabel->setColor(ccWHITE);
        titleLabel->setTag(idx + 20000);
        button->addChild(titleLabel, 10);
        
        
        // 消耗体力
        CCSprite* tiliSpr = CCSprite::create("res/pic/taskScene/task_tili_bar.png");
        tiliSpr->setPosition(ccp(button->getContentSize().width* .33f, button->getContentSize().height* .32f));
        tiliSpr->setTag(idx + 30000);
        button->addChild(tiliSpr);
        // 体力数
        int tiliIndex = 12;
        CCString* labStr4 = CCString::createWithFormat("%d", tiliIndex);
        CCLabelTTF* tiliLabel = CCLabelTTF::create(labStr4->getCString(), DISPLAY->fangzhengFont(), 15);
        tiliLabel->setPosition(ccp(tiliSpr->getContentSize().width* .685f, tiliSpr->getContentSize().height* .45f));
        tiliLabel->setColor(ccWHITE);
        tiliSpr->addChild(tiliLabel);
        // 心
        CCSprite* xinSpr = CCSprite::create("res/pic/qingjingScene/qj_tili.png");
        xinSpr->setScale(.9f);
        xinSpr->setPosition(ccp(tiliSpr->getContentSize().width* .86f, tiliSpr->getContentSize().height* .49f));
        tiliSpr->addChild(xinSpr);
        
        CCSprite* startSpr = CCSprite::create("res/pic/taskScene/task_start2.png");
        startSpr->setPosition(ccp(button->getContentSize().width* .77f, button->getContentSize().height* .39f));
        startSpr->setTag(idx + 40000);
        button->addChild(startSpr);
        
        tiliSpr->setVisible(false);
        startSpr->setVisible(false);
    }else{
        CCString* buttonStr = CCString::createWithFormat("task_button_%d_1.png", getTaskIcon(idx));
        CCString* renStr = CCString::createWithFormat("res/pic/taskScene/task_ren%d.png", getTaskIcon(idx));
        CCSprite* button1 = CCSprite::createWithSpriteFrameName(buttonStr->getCString());
        button1->setAnchorPoint(CCPointZero);
        button1->setPosition(CCPointZero);
        button1->setTag(idx);
        spr->addChild(button1);
        
        CCSprite* button2 = CCSprite::create("res/pic/taskScene/task_weijiesuo.png");
        button2->setAnchorPoint(CCPointZero);
        button2->setPosition(ccp(29.5f, button1->getContentSize().height* .3f));
        button1->addChild(button2, 10);
        
        CCSprite* renSpr = CCSprite::create(renStr->getCString());
        renSpr->setPosition(ccp(button1->getContentSize().width* .22f, button1->getContentSize().height* .7f));
        renSpr->setScale(1.f);
        renSpr->setTag(idx + 10000);
        button1->addChild(renSpr, 5);
        
        
        CCDictionary* dic2 = (CCDictionary* )taskMission->objectAtIndex(idx);
        CCLabelTTF* titleLabel = CCLabelTTF::create(((CCString*)dic2->valueForKey("name"))->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(button1->getContentSize().width* .9f, button1->getContentSize().height* .8f), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        titleLabel->setPosition(ccp(button1->getContentSize().width* .63f, button1->getContentSize().height* .71f));
        titleLabel->setScale(1.f);
        titleLabel->setColor(ccWHITE);
        titleLabel->setTag(idx + 20000);
        button1->addChild(titleLabel, 12);
        
#warning "测试用，需替换"
//        CCString* condition = CCString::createWithFormat("开启要求：%d", unlockCondition);
//        CCLabelTTF* lblCondition = CCLabelTTF::create(condition->getCString(), DISPLAY->fangzhengFont(), 22.f);
//        lblCondition->setColor(ccBLUE);
//        lblCondition->setPosition(ccp(177 * 0.5, 110 * 0.16));
//        pCell->addChild(lblCondition);
    }

    spr->setTag(idx);
    if (selectedIndex == idx) {
        sprNode = (CCNode* )spr;
    }
    pCell->addChild(spr);
    
    return pCell;
}

//cell的数量
unsigned int TaskTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void TaskTableView::tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){

    OpenToWhichOne = cell->getIdx();
    int unlockCondition = DATA->getPlayer()->mission;
    if (OpenToWhichOne <= unlockCondition) {
        if (selectedIndex == -1) {
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* kuang = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCScaleTo* scaleTo = CCScaleTo::create(.2f, .93f);
            kuang->runAction(scaleTo);
            
            CCSprite* renSpr = (CCSprite* )kuang->getChildByTag(cell->getIdx() + 10000);
            CCScaleTo* renScaleTo = CCScaleTo::create(.2f, .93f);
            renSpr->runAction(renScaleTo);
            
            CCLabelTTF* label = (CCLabelTTF* )kuang->getChildByTag(cell->getIdx() + 20000);
            CCScaleTo* labelScaleTo = CCScaleTo::create(.2f, .93f);
            label->runAction(labelScaleTo);
        }else if (selectedIndex == cell->getIdx()){
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* kuang = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCScaleTo* scaleTo = CCScaleTo::create(.2f, 1.07f);
            kuang->runAction(scaleTo);
            
            CCSprite* renSpr = (CCSprite* )kuang->getChildByTag(cell->getIdx() + 10000);
            CCScaleTo* renScaleTo = CCScaleTo::create(.2f, 1.07f);
            renSpr->runAction(renScaleTo);
            
            CCLabelTTF* label = (CCLabelTTF* )kuang->getChildByTag(cell->getIdx() + 20000);
            CCScaleTo* labelScaleTo = CCScaleTo::create(.2f, 1.07f);
            label->runAction(labelScaleTo);
        }else{
            CCSprite* kuang1 = (CCSprite* )sprNode->getChildByTag(selectedIndex);
            CCScaleTo* scaleTo1 = CCScaleTo::create(.1f, 1.f);
            kuang1->runAction(CCSequence::create(scaleTo1, NULL));
            CCSprite* renSpr1 = (CCSprite* )kuang1->getChildByTag(selectedIndex + 10000);
            CCScaleTo* renScaleTo1 = CCScaleTo::create(.1f, 1.f);
            renSpr1->runAction(renScaleTo1);
            CCLabelTTF* label1 = (CCLabelTTF* )kuang1->getChildByTag(selectedIndex + 20000);
            CCScaleTo* labelScaleTo1 = CCScaleTo::create(.1f, 1.f);
            label1->runAction(labelScaleTo1);
            
            CCNode* node = cell->getChildByTag(cell->getIdx());
            CCSprite* kuang2 = (CCSprite* )node->getChildByTag(cell->getIdx());
            CCScaleTo* scaleTo2 = CCScaleTo::create(.2f, .93f);
            kuang2->runAction(scaleTo2);
            CCSprite* renSpr2 = (CCSprite* )kuang2->getChildByTag(cell->getIdx() + 10000);
            CCScaleTo* renScaleTo2 = CCScaleTo::create(.2f, .93f);
            renSpr2->runAction(renScaleTo2);
            
            CCLabelTTF* label2 = (CCLabelTTF* )kuang2->getChildByTag(cell->getIdx() + 20000);
            CCScaleTo* labelScaleTo2 = CCScaleTo::create(.2f, .93f);
            label2->runAction(labelScaleTo2);
        }
    }
}

//松开的时候，取消高亮状态
void TaskTableView::tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){

    OpenToWhichOne = cell->getIdx();
    int unlockCondition = DATA->getPlayer()->mission;
    if (OpenToWhichOne <= unlockCondition) {
        if (selectedIndex == -1) {
            sprNode = cell->getChildByTag(cell->getIdx());
            CCSprite* kuang = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            CCScaleTo* kuangScaleTo1 = CCScaleTo::create(.1f, 1.05f);
            CCScaleTo* kuangScaleTo2 = CCScaleTo::create(.1f, 1.f);
            kuang->runAction(CCSequence::create(kuangScaleTo1, kuangScaleTo2, NULL));
            
            CCSprite* renSpr = (CCSprite* )kuang->getChildByTag(cell->getIdx() + 10000);
            CCScaleTo* renScaleTo1 = CCScaleTo::create(.1f, 1.05f);
            CCScaleTo* renScaleTo2 = CCScaleTo::create(.1f, 1.f);
            renSpr->runAction(CCSequence::create(renScaleTo1, renScaleTo2, NULL));
            
            CCLabelTTF* label = (CCLabelTTF* )kuang->getChildByTag(cell->getIdx() + 20000);
            CCScaleTo* labelScaleTo1 = CCScaleTo::create(.1f, 1.05f);
            CCScaleTo* labelScaleTo2 = CCScaleTo::create(.1f, 1.f);
            label->runAction(CCSequence::create(labelScaleTo1, labelScaleTo2, NULL));
        }else if (selectedIndex == cell->getIdx()){
            sprNode = cell->getChildByTag(cell->getIdx());
            CCScaleTo* scaleTo1 = CCScaleTo::create(.1f, .95f);
            CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, 1.f);
            CCSprite* kuang = (CCSprite* )sprNode->getChildByTag(cell->getIdx());
            kuang->runAction(CCSequence::create(scaleTo1, scaleTo2, NULL));
            
            CCSprite* renSpr = (CCSprite* )kuang->getChildByTag(cell->getIdx() + 10000);
            CCScaleTo* renScaleTo1 = CCScaleTo::create(.1f, .95f);
            CCScaleTo* renScaleTo2 = CCScaleTo::create(.1f, 1.f);
            renSpr->runAction(CCSequence::create(renScaleTo1, renScaleTo2, NULL));
            
            CCLabelTTF* label = (CCLabelTTF* )kuang->getChildByTag(cell->getIdx() + 20000);
            CCScaleTo* labelScaleTo1 = CCScaleTo::create(.1f, .95f);
            CCScaleTo* labelScaleTo2 = CCScaleTo::create(.1f, 1.f);
            label->runAction(CCSequence::create(labelScaleTo1, labelScaleTo2, NULL));
        }else{
            CCNode* node = cell->getChildByTag(cell->getIdx());
            CCScaleTo* scaleTo1 = CCScaleTo::create(.1f, 1.05f);
            CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, 1.f);
            CCSprite* kuang = (CCSprite* )node->getChildByTag(cell->getIdx());
            kuang->runAction(CCSequence::create(scaleTo1, scaleTo2, NULL));
            
            CCSprite* renSpr = (CCSprite* )kuang->getChildByTag(cell->getIdx() + 10000);
            CCScaleTo* renScaleTo1 = CCScaleTo::create(.1f, 1.05f);
            CCScaleTo* renScaleTo2 = CCScaleTo::create(.1f, 1.f);
            renSpr->runAction(CCSequence::create(renScaleTo1, renScaleTo2, NULL));
            
            CCLabelTTF* label = (CCLabelTTF* )kuang->getChildByTag(cell->getIdx() + 20000);
            CCScaleTo* labelScaleTo1 = CCScaleTo::create(.1f, 1.05f);
            CCScaleTo* labelScaleTo2 = CCScaleTo::create(.1f, 1.f);
            label->runAction(CCSequence::create(labelScaleTo1, labelScaleTo2, NULL));
        }
    }
}

void TaskTableView::onEnter(){
    CCLayer::onEnter();
}

void TaskTableView::onExit(){
    CCLayer::onExit();
}

void TaskTableView::buttonStatus(int index, CCSprite* button){
    CCString* indexStr = CCString::createWithFormat("%d", index);
    int num = DATA->getMission()->mission_rating(indexStr->getCString());
    
    CCSprite* xingDiSpr1,* xingDiSpr2,* xingDiSpr3,* xingDiSpr4,* xingDiSpr5;
    xingDiSpr1 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr1->setPosition(ccp(button->getContentSize().width* .28f, button->getContentSize().height* .34f));
    xingDiSpr1->setScale(.8f);
    xingDiSpr1->setTag(Tag_Task_Xing1);
    button->addChild(xingDiSpr1);
    
    xingDiSpr2 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr2->setPosition(ccp(button->getContentSize().width* .41f, button->getContentSize().height* .34f));
    xingDiSpr2->setScale(.8f);
    xingDiSpr2->setTag(Tag_Task_Xing2);
    button->addChild(xingDiSpr2);
    
    xingDiSpr3 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr3->setPosition(ccp(button->getContentSize().width* .54f, button->getContentSize().height* .34f));
    xingDiSpr3->setScale(.8f);
    xingDiSpr3->setTag(Tag_Task_Xing3);
    button->addChild(xingDiSpr3);
    
    xingDiSpr4 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr4->setPosition(ccp(button->getContentSize().width* .67f, button->getContentSize().height* .34f));
    xingDiSpr4->setScale(.8f);
    xingDiSpr4->setTag(Tag_Task_Xing4);
    button->addChild(xingDiSpr4);
    
    xingDiSpr5 = CCSprite::create("res/pic/taskScene/task_xing1.png");
    xingDiSpr5->setPosition(ccp(button->getContentSize().width* .8f, button->getContentSize().height* .34f));
    xingDiSpr5->setScale(.8f);
    xingDiSpr5->setTag(Tag_Task_Xing5);
    button->addChild(xingDiSpr5);
    
    //
    CCSprite* xingSpr1,* xingSpr2,* xingSpr3,* xingSpr4,* xingSpr5;
    if (num >= 1) {
        xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr1->setScale(.8f);
        xingSpr1->setAnchorPoint(ccp(.5f, .5f));
        xingSpr1->setPosition(ccp(xingDiSpr1->getContentSize().width* .5f, xingDiSpr1->getContentSize().height* .5f));
        xingDiSpr1->addChild(xingSpr1);
    }
    if (num >= 2) {
        xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr2->setScale(.8f);
        xingSpr2->setAnchorPoint(ccp(.5f, .5f));
        xingSpr2->setPosition(ccp(xingDiSpr1->getContentSize().width* .5f, xingDiSpr1->getContentSize().height* .5f));
        xingDiSpr2->addChild(xingSpr2);
    }
    if (num >= 3) {
        xingSpr3 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr3->setScale(.8f);
        xingSpr3->setAnchorPoint(ccp(.5f, .5f));
        xingSpr3->setPosition(ccp(xingDiSpr1->getContentSize().width* .5f, xingDiSpr1->getContentSize().height* .5f));
        xingDiSpr3->addChild(xingSpr3);
    }
    if (num >= 4) {
        xingSpr4 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr4->setScale(.8f);
        xingSpr4->setAnchorPoint(ccp(.5f, .5f));
        xingSpr4->setPosition(ccp(xingDiSpr1->getContentSize().width* .5f, xingDiSpr1->getContentSize().height* .5f));
        xingDiSpr4->addChild(xingSpr4);
    }
    if (num >= 5) {
        xingSpr5 = CCSprite::create("res/pic/taskScene/task_xing2.png");
        xingSpr5->setScale(.8f);
        xingSpr5->setAnchorPoint(ccp(.5f, .5f));
        xingSpr5->setPosition(ccp(xingDiSpr1->getContentSize().width* .5f, xingDiSpr1->getContentSize().height* .5f));
        xingDiSpr5->addChild(xingSpr5);
    }
}

CCString* TaskTableView::getTaskName(int index){
    CCDictionary* dic = (CCDictionary* )taskMission->objectAtIndex(index);
    
    return (CCString*)dic->valueForKey("name");
}

int TaskTableView::getTaskIcon(int index){
    CCDictionary* dic = (CCDictionary* )taskMission->objectAtIndex(index);
    
    return dic->valueForKey("icon")->intValue();
}

int TaskTableView::getTaskPhase(int index){
    CCDictionary* dic = (CCDictionary* )taskMission->objectAtIndex(index);
    
    return dic->valueForKey("phase")->intValue();
}

