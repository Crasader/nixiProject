//
//  TaskScene.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "TaskScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "MainScene.h"
#include "TaskTableView.h"
#include "MZResourceLoader.h"
#include "StoryScene.h"
#include "ConfigManager.h"


TaskScene::TaskScene(){
    
}
TaskScene::~TaskScene(){
    
}

bool TaskScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    taskArr = CONFIG->mission();
    
    this->creat_view();
    
    return true;
}
CCScene* TaskScene::scene(){
    CCScene* scene = CCScene::create();
    TaskScene* layer = TaskScene::create();
    scene->addChild(layer);
    
    return scene;
}

void TaskScene::onEnter(){
    BaseScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::creat_Tishi), "Task_Creat_Tishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::EnterTheTishi), "Task_EnterTheTishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::ExitTishi), "Task_ExitTishi", NULL);
}
void TaskScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    BaseScene::onExit();
}

void TaskScene::keyBackClicked(){
    
}

void TaskScene::creat_view(){
    
    roomSpr = CCSprite::create("res/pic/taskScene/task_bg.png");
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(roomSpr);
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/taskScene/task_back.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/taskScene/task_back.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(TaskScene::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    
    taskKuang = CCSprite::create("res/pic/taskScene/task_dikuang1.png");
    taskKuang->setAnchorPoint(ccp(1, .5f));
    taskKuang->setPosition(ccp(DISPLAY->ScreenWidth()+7, DISPLAY->ScreenHeight()* .585f));
    this->addChild(taskKuang, 5);
    
    TaskTableView* tabLayer = TaskTableView::create();
    tabLayer->setPosition(ccp(16, 38));
    tabLayer->setTag(0x77777);
    taskKuang->addChild(tabLayer, 5);
    
}

void TaskScene::backCallBack(CCObject* pSender){
    CCScene* scene = MainScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}


void TaskScene::creat_Tishi(){
    
    if (this->getChildByTag(0x88888) != NULL) {
        this->removeChildByTag(0x88888);
        kuangSpr = NULL;
    }
    
    int index = DATA->getTaskNumber();
    int tiliIndex = 9;
    
    kuangSpr = CCSprite::create("res/pic/taskScene/task_dikuang2.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .12f));
    kuangSpr->setTag(0x88888);
    this->addChild(kuangSpr, 10);
    
    CCSprite* xianSpr = CCSprite::create("res/pic/taskScene/task_xian.png");
    xianSpr->setPosition(ccp(kuangSpr->getContentSize().width* .34f, kuangSpr->getContentSize().height* .21f));
    kuangSpr->addChild(xianSpr);
    
    // 标题
    CCLabelTTF* nameLabel = CCLabelTTF::create(getTaskName(index)->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(310, 25), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .86f));
    nameLabel->setColor(ccc3(80, 63, 68));
//    nameLabel->enableStroke(ccc3(80, 63, 68), .4f);
    kuangSpr->addChild(nameLabel, 2);
    
    CCLabelTTF* nameLabel2 = CCLabelTTF::create(getTaskName(index)->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(310, 25), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .5f + 2, kuangSpr->getContentSize().height* .86f - 2));
    nameLabel2->setColor(ccGRAY);
//    nameLabel2->enableStroke(ccGRAY, .4f);
    kuangSpr->addChild(nameLabel2);
    
    // 提示
    CCLabelTTF* tishiLabel = CCLabelTTF::create("提示: ", DISPLAY->fangzhengFont(), 20, CCSizeMake(310, 20), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    tishiLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .75f));
    tishiLabel->setColor(ccc3(103, 81, 95));
//    tishiLabel->enableStroke(ccc3(103, 81, 95), .4f);
    kuangSpr->addChild(tishiLabel, 2);
    
    // 提示内容
    CCLabelTTF* descriptionLabel = CCLabelTTF::create(getTaskDescription(index)->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .6f), kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    descriptionLabel->setPosition(ccp(kuangSpr->getContentSize().width* .34f, kuangSpr->getContentSize().height* .39f));
    descriptionLabel->setColor(ccc3(103, 81, 95));
//    descriptionLabel->enableStroke(ccc3(103, 81, 95), .4f);
    kuangSpr->addChild(descriptionLabel, 2);
    
    CCLabelTTF* descriptionLabel2 = CCLabelTTF::create(getTaskDescription(index)->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .6f), kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    descriptionLabel->setPosition(ccp(kuangSpr->getContentSize().width* .34f, kuangSpr->getContentSize().height* .39f));
    descriptionLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .34f + 2, kuangSpr->getContentSize().height* .39f - 2));
    descriptionLabel2->setColor(ccGRAY);
//    descriptionLabel2->enableStroke(ccGRAY, .4f);
    kuangSpr->addChild(descriptionLabel2);
    
    
    // 开始故事
    CCSprite* startSpr1 = CCSprite::create("res/pic/taskScene/task_start.png");
    CCSprite* startSpr2 = CCSprite::create("res/pic/taskScene/task_start.png");
    startSpr2->setScale(1.02f);
    CCMenuItem* startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(TaskScene::startCallBack));
    startItem->setPosition(ccp(kuangSpr->getContentSize().width* .78f, kuangSpr->getContentSize().height* .32f));
    startItem->setTag(index);
    CCMenu* menu = CCMenu::create(startItem, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu);
    
    // 小人
    CCSprite* xiaorenSpr = CCSprite::create("res/pic/taskScene/task_xiaoren.png");
    xiaorenSpr->setPosition(ccp(kuangSpr->getContentSize().width* .73f, kuangSpr->getContentSize().height* .74f));
    kuangSpr->addChild(xiaorenSpr, 100);
    
    // 消耗体力
    CCSprite* tiliSpr = CCSprite::create("res/pic/qingjingScene/qj_tili_bar.png");
    tiliSpr->setPosition(ccp(kuangSpr->getContentSize().width* .78f, kuangSpr->getContentSize().height* .115f));
    kuangSpr->addChild(tiliSpr);
    // 体力数
    CCString* labStr4 = CCString::createWithFormat("%d", tiliIndex);
    CCLabelTTF* tiliLabel = CCLabelTTF::create(labStr4->getCString(), DISPLAY->fangzhengFont(), 15);
    tiliLabel->setPosition(ccp(tiliSpr->getContentSize().width* .685f, tiliSpr->getContentSize().height* .45f));
    tiliLabel->setColor(ccWHITE);
//    tiliLabel->enableStroke(ccWHITE, .4f);
    tiliSpr->addChild(tiliLabel);
    // 心
    CCSprite* xinSpr = CCSprite::create("res/pic/qingjingScene/qj_tili.png");
    xinSpr->setScale(.9f);
    xinSpr->setPosition(ccp(tiliSpr->getContentSize().width* .85f, tiliSpr->getContentSize().height* .49f));
    tiliSpr->addChild(xinSpr);
    
    
    
}

void TaskScene::startCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    int index = item->getTag();
    
//    CCScene* pScene = CCScene::create();
//    StoryScene* layer = StoryScene::create_with_story_id(index);
//    pScene->addChild(layer);
//    CCTransitionScene* trans = CCTransitionFade::create(.3f, pScene);
//    CCDirector::sharedDirector()->replaceScene(trans);
}

void TaskScene::EnterTheTishi(){    
    CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth() - kuangSpr->getContentSize().width* .5f, DISPLAY->ScreenHeight()* .12f));
    kuangSpr->runAction(moveTo);
}
void TaskScene::ExitTishi(){
    if (this->getChildByTag(0x88888) != NULL) {
        kuangSpr->removeAllChildren();
        this->removeChildByTag(0x88888);
        kuangSpr = NULL;
    }
}

CCString* TaskScene::getTaskName(int index){
    CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(index);
    
    return (CCString*)dic->valueForKey("name");
}
CCString* TaskScene::getTaskDescription(int index){
    CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(index);
    
    return (CCString*)dic->valueForKey("description");
}




