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
#include "ConfigManager.h"
#include "NetManager.h"
#include "MainScene.h"
#include "Loading2.h"
#include "TaskStoryScene.h"
#include "PhoneLayer2.h"
#include "HomeLayer.h"

#include "ClothesScene.h"
#include "TaskTableView.h"
#include "MZResourceLoader.h"
#include "AudioManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "BuildingLayer.h"

#include "GuideLayer.h"

TaskScene::~TaskScene(){
    
}

TaskScene* TaskScene::create(bool isPhaseUP) {
    TaskScene* rtn = new TaskScene();
    if (rtn && rtn->init(isPhaseUP)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool TaskScene::init(bool isPhaseUP){
    if (!BaseScene::init()) {
        return false;
    }
    
    _isPhaseUP = isPhaseUP; // 要不要显示升级动画 false不显示
    CCLOG("_isPhaseUP = %d", _isPhaseUP);
    _buildingLayer = BuildingLayer::create(DATA->getTaskPhase(), _isPhaseUP);
    _buildingLayer->setTag(0x55555);
    this->addChild(_buildingLayer);
    
    if (_isPhaseUP == true) {
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&TaskScene::nc_phase_up_finished), "Phase_Up_Finished", NULL);
    }
    else {
        this->init_contents();
    }

    return true;
}

void TaskScene::onEnter(){
    BaseScene::onEnter();
    
    AUDIO->play_company_bgm();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::creat_Tishi), "Task_Creat_Tishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::EnterTheTishi), "Task_EnterTheTishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::ExitTishi), "Task_ExitTishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::exitView), "Task_ExitView", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::_startCallBack), "HTTP_FINISHED_400", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::_905CallBack), "HTTP_FINISHED_905", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::_905status), "Guide_905status", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::phoneCallBack), "Guide_phoneCallBack", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&TaskScene::keyBackStatus), .8f);
}

void TaskScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void TaskScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    BaseScene::onExit();
}

void TaskScene::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1) {
        return;
    }
    
    if (DATA->current_guide_step() == 0) {
        this->backCallBack(NULL);
    }
}

void TaskScene::nc_phase_up_finished(CCObject *pObj) {
    this->init_contents();
}

void TaskScene::init_contents() {
    OpenToWhichOne = 0;
    taskPhase = 0;
    taskIndex = 0;
    
    CCDictionary* ratingDic =  DATA->getPlayer()->rating;
    CCString* str = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
    OpenToWhichOne = ((CCInteger* )ratingDic->objectForKey(str->getCString()))->getValue();
    
    taskArr = CONFIG->mission();
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    backItem1 = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(TaskScene::backCallBack));
    backItem1->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    
    CCSprite* huiSpr1 = CCSprite::create("res/pic/taskScene/task_hui.png");
    CCSprite* huiSpr2 = CCSprite::create("res/pic/taskScene/task_hui.png");
    huiSpr2->setScale(1.02f);
    backItem2 = CCMenuItemSprite::create(huiSpr1, huiSpr2, this, menu_selector(TaskScene::backCallBack));
    backItem2->setPosition(ccp(DISPLAY->ScreenWidth()* .1f, DISPLAY->ScreenHeight()* .04f));
    
    CCMenu* menu = CCMenu::create(backItem1, backItem2, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 20);
    
    backItem1->setVisible(true);
    backItem2->setVisible(false);
    
    this->creat_view();
    this->creat_Man();
    this->initClothes();
    
    this->init_phone();
    
    if (DATA->current_guide_step() == 0) {
        
    }else if (DATA->current_guide_step() == 2){
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }else if (DATA->current_guide_step() == 5){
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }else if (DATA->current_guide_step() == 8){
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }else if (DATA->current_guide_step() == 9){
        for (int i = 0; i < 10; i++) {
            DATA->_guideBool9[i] = true;
        }
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }
}

void TaskScene::creat_view(){
    if (this->getChildByTag(0x55555) != NULL) {
        this->removeChildByTag(0x55555);
    }
    if (this->getChildByTag(0x44444) != NULL) {
        this->removeChildByTag(0x44444);
    }
    if (this->getChildByTag(0x66666) != NULL) {
        this->removeChildByTag(0x66666);
    }
    
    
    _buildingLayer = BuildingLayer::create(DATA->getTaskPhase(), false);
    _buildingLayer->setTag(0x55555);
    this->addChild(_buildingLayer);
    
    taskPhase = DATA->getTaskPhase();
    CCArray* tempArr = CCArray::create();
    for (int i = 0; i < taskArr->count(); i++) {
        CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(i);
        int phase = dic->valueForKey("phase")->intValue();
        if (phase == taskPhase) {
            tempArr->addObject(dic);
        }
    }
    DATA->setTaskSource(tempArr);
    
    if (taskPhase >= DATA->getPlayer()->phase) {
        //
        CCSprite* historySpr1_1 = CCSprite::create("res/pic/taskScene/task_paopao1.png");
        CCSprite* historySpr1_2 = CCSprite::create("res/pic/taskScene/task_paopao1.png");
        historySpr1_2->setScale(1.02f);
        historyItem1 = CCMenuItemSprite::create(historySpr1_1, historySpr1_2, this, menu_selector(TaskScene::historyCallBack));
        historyItem1->setTag(1);
        historyItem1->setVisible(false);
//        CCSprite* historykuang1 = CCSprite::create("res/pic/taskScene/task_kuang.png");
//        historykuang1->setPosition(ccp(historyItem1->getContentSize().width* .5f, 8));
//        historyItem1->addChild(historykuang1);
//        CCSprite* historyLabelSpr1 = CCSprite::create("res/pic/taskScene/task_label1.png");
//        historyLabelSpr1->setPosition(ccp(historykuang1->getContentSize().width* .12f, historykuang1->getContentSize().height* .7f));
//        historykuang1->addChild(historyLabelSpr1, 2);
//        CCLabelTTF* historyLabel1 = CCLabelTTF::create("弘鼎企业", DISPLAY->fangzhengFont(), 15, CCSizeMake(100, 15), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//        historyLabel1->setPosition(ccp(historykuang1->getContentSize().width* .5f, historykuang1->getContentSize().height* .34f));
//        historyLabel1->setColor(ccc3(141, 169, 235));
//        historykuang1->addChild(historyLabel1);
        
        //
        CCSprite* historySpr2_1 = CCSprite::create("res/pic/taskScene/task_paopao2.png");
        CCSprite* historySpr2_2 = CCSprite::create("res/pic/taskScene/task_paopao2.png");
        historySpr2_2->setScale(1.02f);
        historyItem2 = CCMenuItemSprite::create(historySpr2_1, historySpr2_2, this, menu_selector(TaskScene::historyCallBack));
        historyItem2->setTag(2);
        historyItem2->setVisible(false);
//        CCSprite* historykuang2 = CCSprite::create("res/pic/taskScene/task_kuang.png");
//        historykuang2->setPosition(ccp(historyItem2->getContentSize().width* .5f, 8));
//        historyItem2->addChild(historykuang2);
//        CCSprite* historyLabelSpr2 = CCSprite::create("res/pic/taskScene/task_label2.png");
//        historyLabelSpr2->setPosition(ccp(historykuang2->getContentSize().width* .14f, historykuang2->getContentSize().height* .7f));
//        historykuang2->addChild(historyLabelSpr2, 2);
//        CCLabelTTF* historyLabel2 = CCLabelTTF::create("弘鼎有限公司", DISPLAY->fangzhengFont(), 15, CCSizeMake(100, 15), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//        historyLabel2->setPosition(ccp(historykuang2->getContentSize().width* .5f, historykuang2->getContentSize().height* .34f));
//        historyLabel2->setColor(ccc3(141, 169, 235));
//        historykuang2->addChild(historyLabel2);
        
        //
        CCSprite* historySpr3_1 = CCSprite::create("res/pic/taskScene/task_paopao3.png");
        CCSprite* historySpr3_2 = CCSprite::create("res/pic/taskScene/task_paopao3.png");
        historySpr3_2->setScale(1.02f);
        historyItem3 = CCMenuItemSprite::create(historySpr3_1, historySpr3_2, this, menu_selector(TaskScene::historyCallBack));
        historyItem3->setTag(3);
        historyItem3->setVisible(false);
//        CCSprite* historykuang3 = CCSprite::create("res/pic/taskScene/task_kuang.png");
//        historykuang3->setPosition(ccp(historyItem3->getContentSize().width* .5f, 8));
//        historyItem3->addChild(historykuang3);
//        CCSprite* historyLabelSpr3 = CCSprite::create("res/pic/taskScene/task_label3.png");
//        historyLabelSpr3->setPosition(ccp(historykuang3->getContentSize().width* .12f, historykuang3->getContentSize().height* .7f));
//        historykuang3->addChild(historyLabelSpr3, 2);
//        CCLabelTTF* historyLabel3 = CCLabelTTF::create("弘鼎集团", DISPLAY->fangzhengFont(), 15, CCSizeMake(100, 15), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//        historyLabel3->setPosition(ccp(historykuang3->getContentSize().width* .5f, historykuang3->getContentSize().height* .34f));
//        historyLabel3->setColor(ccc3(141, 169, 235));
//        historykuang3->addChild(historyLabel3);
        
        //
        CCSprite* historySpr4_1 = CCSprite::create("res/pic/taskScene/task_paopao4.png");
        CCSprite* historySpr4_2 = CCSprite::create("res/pic/taskScene/task_paopao4.png");
        historySpr4_2->setScale(1.02f);
        historyItem4 = CCMenuItemSprite::create(historySpr4_1, historySpr4_2, this, menu_selector(TaskScene::historyCallBack));
        historyItem4->setTag(4);
        historyItem4->setVisible(false);
//        CCSprite* historykuang4 = CCSprite::create("res/pic/taskScene/task_kuang.png");
//        historykuang4->setPosition(ccp(historyItem4->getContentSize().width* .5f, 8));
//        historyItem4->addChild(historykuang4);
//        CCSprite* historyLabelSpr4 = CCSprite::create("res/pic/taskScene/task_label4.png");
//        historyLabelSpr4->setPosition(ccp(historykuang4->getContentSize().width* .12f, historykuang4->getContentSize().height* .7f));
//        historykuang4->addChild(historyLabelSpr4, 2);
//        CCLabelTTF* historyLabel4 = CCLabelTTF::create("弘鼎国际", DISPLAY->fangzhengFont(), 15, CCSizeMake(100, 15), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//        historyLabel4->setPosition(ccp(historykuang4->getContentSize().width* .5f, historykuang4->getContentSize().height* .34f));
//        historyLabel4->setColor(ccc3(141, 169, 235));
//        historykuang4->addChild(historyLabel4);
        
        //
        CCSprite* historySpr5_1 = CCSprite::create("res/pic/taskScene/task_paopao5.png");
        CCSprite* historySpr5_2 = CCSprite::create("res/pic/taskScene/task_paopao5.png");
        historySpr5_2->setScale(1.02f);
        historyItem5 = CCMenuItemSprite::create(historySpr5_1, historySpr5_2, this, menu_selector(TaskScene::historyCallBack));
        historyItem5->setTag(5);
        historyItem5->setVisible(false);
//        CCSprite* historykuang5 = CCSprite::create("res/pic/taskScene/task_kuang.png");
//        historykuang5->setPosition(ccp(historyItem5->getContentSize().width* .5f, 8));
//        historyItem5->addChild(historykuang5);
//        CCSprite* historyLabelSpr5 = CCSprite::create("res/pic/taskScene/task_label5.png");
//        historyLabelSpr5->setPosition(ccp(historykuang5->getContentSize().width* .12f, historykuang5->getContentSize().height* .7f));
//        historykuang5->addChild(historyLabelSpr5, 2);
//        CCLabelTTF* historyLabel5 = CCLabelTTF::create("弘鼎全球集团", DISPLAY->fangzhengFont(), 15, CCSizeMake(100, 15), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//        historyLabel5->setPosition(ccp(historykuang5->getContentSize().width* .5f, historykuang5->getContentSize().height* .34f));
//        historyLabel5->setColor(ccc3(141, 169, 235));
//        historykuang5->addChild(historyLabel5);
        
        
        int menuIndex = 0;
        CCMenu* historyMenu;
        if (DATA->getTaskPhase() > 1 && DATA->getTaskPhase() <= 2) {
            menuIndex = 1;
            historyMenu = CCMenu::create(historyItem1, NULL);
            historyMenu->alignItemsHorizontallyWithPadding(10);
            historyMenu->setAnchorPoint(ccp(1, 0));
            historyMenu->setPosition(ccp(DISPLAY->ScreenWidth() - ((historySpr1_1->getContentSize().width* menuIndex + 5* menuIndex)/ 2), DISPLAY->ScreenHeight()* .06f));
            this->addChild(historyMenu, 20);
        }else if (DATA->getTaskPhase() > 2 && DATA->getTaskPhase() <= 3){
            menuIndex = 2;
            historyMenu = CCMenu::create(historyItem1, historyItem2, NULL);
            historyMenu->alignItemsHorizontallyWithPadding(10);
            historyMenu->setAnchorPoint(ccp(1, 0));
            historyMenu->setPosition(ccp(DISPLAY->ScreenWidth() - ((historySpr1_1->getContentSize().width* menuIndex + 5* menuIndex)/ 2), DISPLAY->ScreenHeight()* .06f));
            this->addChild(historyMenu, 20);
        }else if (DATA->getTaskPhase() > 3 && DATA->getTaskPhase() <= 4){
            menuIndex = 3;
            historyMenu = CCMenu::create(historyItem1, historyItem2, historyItem3, NULL);
            historyMenu->alignItemsHorizontallyWithPadding(10);
            historyMenu->setAnchorPoint(ccp(1, 0));
            historyMenu->setPosition(ccp(DISPLAY->ScreenWidth() - ((historySpr1_1->getContentSize().width* menuIndex + 5* menuIndex)/ 2), DISPLAY->ScreenHeight()* .06f));
            this->addChild(historyMenu, 20);
        }else if (DATA->getTaskPhase() > 4 && DATA->getTaskPhase() <= 5){
            menuIndex = 4;
            historyMenu = CCMenu::create(historyItem1, historyItem2, historyItem3, historyItem4, NULL);
            historyMenu->alignItemsHorizontallyWithPadding(10);
            historyMenu->setAnchorPoint(ccp(1, 0));
            historyMenu->setPosition(ccp(DISPLAY->ScreenWidth() - ((historySpr1_1->getContentSize().width* menuIndex + 5* menuIndex)/ 2), DISPLAY->ScreenHeight()* .06f));
            this->addChild(historyMenu, 20);
        }else if (DATA->getTaskPhase() > 5){
            menuIndex = 5;
            historyMenu = CCMenu::create(historyItem1, historyItem2, historyItem3, historyItem4, historyItem5, NULL);
            historyMenu->alignItemsHorizontallyWithPadding(10);
            historyMenu->setAnchorPoint(ccp(1, 0));
            historyMenu->setPosition(ccp(DISPLAY->ScreenWidth() - ((historySpr1_1->getContentSize().width* menuIndex + 5* menuIndex)/ 2), DISPLAY->ScreenHeight()* .06f));
            historyMenu->setTag(0x66666);
            this->addChild(historyMenu, 20);
        }
    }
    
    taskKuang = CCSprite::create("res/pic/taskScene/task_dikuang1.png");
    taskKuang->setAnchorPoint(ccp(1, .5f));
//    taskKuang->setPosition(ccp(DISPLAY->ScreenWidth()+7, DISPLAY->ScreenHeight()* .475f));
    taskKuang->setPosition(ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .31f));
    taskKuang->setTag(0x44444);
    this->addChild(taskKuang, 20);
    CCSprite* shangkuangSpr = CCSprite::create("res/pic/taskScene/task_shangkuang.png");
    shangkuangSpr->setAnchorPoint(ccp(.5f, 0));
    shangkuangSpr->setPosition(ccp(taskKuang->getContentSize().width* .5f, taskKuang->getContentSize().height - 2));
    taskKuang->addChild(shangkuangSpr);

    
    int curPhase = DATA->getPlayer()->phase;
    int ratingsRequire = CONFIG->phase_up_required(curPhase);
    int curRatings = DATA->getPlayer()->ratings(curPhase);
    
    
    CCString* shangStr = CCString::createWithFormat("总星:%d/%d", curRatings, ratingsRequire);
    CCLabelTTF* shangLabel = CCLabelTTF::create(shangStr->getCString(), DISPLAY->fangzhengFont(), 25);
    shangLabel->setAnchorPoint(ccp(0, .5f));
    shangLabel->setPosition(ccp(5.f, shangkuangSpr->getContentSize().height* .5f));
    shangLabel->setColor(ccc3(232,136,174));
    shangkuangSpr->addChild(shangLabel);
    CCSprite* shangXingSpr = CCSprite::create("res/pic/taskScene/task_xing3.png");
    shangXingSpr->setAnchorPoint(ccp(0, .5));
    shangXingSpr->setPosition(ccp(shangLabel->getContentSize().width + 5, shangLabel->getContentSize().height* .53f));
    shangLabel->addChild(shangXingSpr);
    
    
    TaskTableView* tabLayer = TaskTableView::create();
    tabLayer->setPosition(ccp(7, 12));
    tabLayer->setTag(0x77777);
    taskKuang->addChild(tabLayer, 5);
    taskKuang->setScale(.3f);
    
    if (ratingsRequire == 9999) {
        barSpr = CCSprite::create("res/pic/mainScene/txt_bar.png");
        barSpr->setAnchorPoint(ccp(.5f, 0));
        barSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, 1));
        this->addChild(barSpr, 15);//478 37
        CCString* barStr = CCString::createWithFormat("升级公司还差 %d 星级", ratingsRequire - curRatings);
        CCLabelTTF* barLabel = CCLabelTTF::create(barStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(barSpr->getContentSize().width* .8f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        barLabel->setPosition(ccp(barSpr->getContentSize().width* .5f, barSpr->getContentSize().height* .5f));
        barLabel->setColor(ccc3(80, 63, 68));
        barSpr->addChild(barLabel);
    }
    
    
    this->scheduleOnce(SEL_SCHEDULE(&TaskScene::enterTheKuang), .1f);
}

void TaskScene::enterTheKuang(float dt){
    CCMoveTo* moveTo1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth() + 7, DISPLAY->ScreenHeight()* .49f));
    CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 1.f);
    CCSpawn* spawn1 = CCSpawn::create(moveTo1, scaleTo1, NULL);
    taskKuang->runAction(CCSequence::create(spawn1, NULL));
    
    if (taskPhase >= DATA->getPlayer()->phase) {
        CCFadeIn* fadeIn1 = CCFadeIn::create(.3f);
        CCFadeIn* fadeIn2 = CCFadeIn::create(.3f);
        CCFadeIn* fadeIn3 = CCFadeIn::create(.3f);
        CCFadeIn* fadeIn4 = CCFadeIn::create(.3f);
        CCFadeIn* fadeIn5 = CCFadeIn::create(.3f);
        
        if (DATA->getTaskPhase() > 1 && DATA->getTaskPhase() <= 2) {
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeIn1, NULL));
        }else if (DATA->getTaskPhase() > 2 && DATA->getTaskPhase() <= 3){
            historyItem2->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeIn1, NULL));
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeIn2, NULL));
        }else if (DATA->getTaskPhase() > 3 && DATA->getTaskPhase() <= 4){
            historyItem3->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeIn1, NULL));
            historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeIn2, NULL));
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeIn3, NULL));
        }else if (DATA->getTaskPhase() > 4 && DATA->getTaskPhase() <= 5){
            historyItem4->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeIn1, NULL));
            historyItem3->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeIn2, NULL));
            historyItem2->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeIn3, NULL));
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCShow::create(), fadeIn4, NULL));
        }else if (DATA->getTaskPhase() > 5){
            historyItem5->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeIn1, NULL));
            historyItem4->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeIn2, NULL));
            historyItem3->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeIn3, NULL));
            historyItem2->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCShow::create(), fadeIn4, NULL));
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(1.3f), CCShow::create(), fadeIn5, NULL));
        }
    }else{
        historyBool = true;
        taskPhase = DATA->getPlayer()->phase;
        DATA->setTaskPhase(taskPhase);
    }
}

void TaskScene::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    
    if (DATA->getHomeBool()) {
        DATA->setHomeBool(false);
        
        CCScene* scene = HomeLayer::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else{
        if (historyBool) {
            backItem1->setVisible(true);
            backItem2->setVisible(false);
            historyBool = false;
            DATA->setTaskPhase(DATA->getPlayer()->phase);
            
            // _isPhaseUP 要不要显示升级动画 false不显示
            _buildingLayer = BuildingLayer::create(DATA->getTaskPhase(), false);
            _buildingLayer->setTag(0x55555);
            this->addChild(_buildingLayer);
            
            CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TaskScene::creat_view));
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
            CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
            taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, callFunc, NULL));
        }else{
            CCScene* scene = MainScene::scene();
            CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
            CCDirector::sharedDirector()->replaceScene(trans);
        }
    }
}

void TaskScene::historyCallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    CCMenuItem* item = (CCMenuItem* )pSender;
    CCLog("房子是%d", item->getTag());
    historyIndex = item->getTag();
    historyBool = true;
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TaskScene::creat_historyview));
    
    CCFadeOut* fadeOut1 = CCFadeOut::create(.3f);
    CCFadeOut* fadeOut2 = CCFadeOut::create(.3f);
    CCFadeOut* fadeOut3 = CCFadeOut::create(.3f);
    CCFadeOut* fadeOut4 = CCFadeOut::create(.3f);
    CCFadeOut* fadeOut5 = CCFadeOut::create(.3f);
    if (DATA->getTaskPhase() == 1){
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
        CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
        taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, CCDelayTime::create(.1f), callFunc, NULL));
    }else if (DATA->getTaskPhase() > 1 && DATA->getTaskPhase() <= 2) {
        historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, callFunc, NULL));
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
        CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
        taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
    }else if (DATA->getTaskPhase() > 2 && DATA->getTaskPhase() <= 3){
        historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
        historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, callFunc, NULL));
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
        CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
        taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
    }else if (DATA->getTaskPhase() > 3 && DATA->getTaskPhase() <= 4){
        historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
        historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, NULL));
        historyItem3->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeOut3, callFunc, NULL));
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
        CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
        taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
    }else if (DATA->getTaskPhase() > 4 && DATA->getTaskPhase() <= 5){
        historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
        historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, NULL));
        historyItem3->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeOut3, NULL));
        historyItem4->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCShow::create(), fadeOut4, callFunc, NULL));
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
        CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
        taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
    }else if (DATA->getTaskPhase() > 5){
        historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
        historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, NULL));
        historyItem3->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeOut3, NULL));
        historyItem4->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCShow::create(), fadeOut4, NULL));
        historyItem5->runAction(CCSequence::create(CCDelayTime::create(1.3f), CCShow::create(), fadeOut5, callFunc, NULL));
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
        CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
        taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
    }
}
void TaskScene::creat_historyview(){
    if (this->getChildByTag(0x55555) != NULL) {
        this->removeChildByTag(0x55555);
    }
    if (this->getChildByTag(0x44444) != NULL) {
        this->removeChildByTag(0x44444);
    }
    if (this->getChildByTag(0x66666) != NULL) {
        this->removeChildByTag(0x66666);
    }
    
    backItem1->setVisible(false);
    backItem2->setVisible(true);
    
    taskPhase = historyIndex;
    DATA->setTaskPhase(taskPhase);
    CCArray* tempArr = CCArray::create();
    for (int i = 0; i < taskArr->count(); i++) {
        CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(i);
        int phase = dic->valueForKey("phase")->intValue();
        if (phase == taskPhase) {
            tempArr->addObject(dic);
        }
    }
    DATA->setTaskSource(tempArr);
    
    // _isPhaseUP 要不要显示升级动画 false不显示
    _buildingLayer = BuildingLayer::create(taskPhase, false);
    _buildingLayer->setTag(0x55555);
    this->addChild(_buildingLayer);
    
    taskKuang = CCSprite::create("res/pic/taskScene/task_dikuang1.png");
    taskKuang->setAnchorPoint(ccp(1, .5f));
//    taskKuang->setPosition(ccp(DISPLAY->ScreenWidth()+7, DISPLAY->ScreenHeight()* .475f));
    taskKuang->setPosition(ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .31f));
    taskKuang->setTag(0x44444);
    this->addChild(taskKuang, 20);
    CCSprite* shangkuangSpr = CCSprite::create("res/pic/taskScene/task_shangkuang.png");
    shangkuangSpr->setAnchorPoint(ccp(.5f, 0));
    shangkuangSpr->setPosition(ccp(taskKuang->getContentSize().width* .5f, taskKuang->getContentSize().height - 2));
    taskKuang->addChild(shangkuangSpr);
    CCString* shangStr = CCString::createWithFormat("总星:%d/100", DATA->getPlayer()->ratings(taskPhase));
    CCLabelTTF* shangLabel = CCLabelTTF::create(shangStr->getCString(), DISPLAY->fangzhengFont(), 25);
    shangLabel->setAnchorPoint(ccp(0, .5f));
    shangLabel->setPosition(ccp(5.f, shangkuangSpr->getContentSize().height* .5f));
    shangLabel->setColor(ccc3(232,136,174));
    shangkuangSpr->addChild(shangLabel);
    CCSprite* shangXingSpr = CCSprite::create("res/pic/taskScene/task_xing3.png");
    shangXingSpr->setAnchorPoint(ccp(0, .5));
    shangXingSpr->setPosition(ccp(shangLabel->getContentSize().width + 5, shangLabel->getContentSize().height* .53f));
    shangLabel->addChild(shangXingSpr);
    
    
    TaskTableView* tabLayer = TaskTableView::create();
    tabLayer->setPosition(ccp(7, 12));
    tabLayer->setTag(0x77777);
    taskKuang->addChild(tabLayer, 5);
    taskKuang->setScale(.3f);
    
    this->scheduleOnce(SEL_SCHEDULE(&TaskScene::historyEnterTheKuang), .1f);
}
void TaskScene::historyEnterTheKuang(float dt){
    CCMoveTo* moveTo1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth() + 7, DISPLAY->ScreenHeight()* .49f));
    CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 1.f);
    CCSpawn* spawn1 = CCSpawn::create(moveTo1, scaleTo1, NULL);
    taskKuang->runAction(CCSequence::create(spawn1, NULL));
}

void TaskScene::creat_Tishi(){
    
    if (this->getChildByTag(0x88888) != NULL) {
        this->removeChildByTag(0x88888);
        kuangSpr = NULL;
    }
    
    int index = DATA->getTaskNumber();
    CCArray* arr = DATA->getTaskSource();
    CCDictionary* taskDic = (CCDictionary* )arr->objectAtIndex(index);
    int taskId = taskDic->valueForKey("id")->intValue();
    CCDictionary* dic;
    int id;
    for (int i = 0; i < taskArr->count(); i++) {
        dic = (CCDictionary* )taskArr->objectAtIndex(i);
        id = dic->valueForKey("id")->intValue();
        if (taskId == id) {
            break;
        }
    }
    int unlockCondition = dic->valueForKey("require")->intValue();
    int tiliIndex = 12;
    
    kuangSpr = CCSprite::create("res/pic/taskScene/task_dikuang2.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .12f));
    kuangSpr->setTag(0x88888);
    this->addChild(kuangSpr, 20);
    
    // 标题
    CCLabelTTF* nameLabel = CCLabelTTF::create(getTaskName(id)->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(370, 25), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .92f));
    nameLabel->setColor(ccWHITE);
//    nameLabel->setColor(ccc3(80, 63, 68));
    kuangSpr->addChild(nameLabel, 2);
    
    CCLabelTTF* nameLabel2 = CCLabelTTF::create(getTaskName(id)->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(370, 25), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .5f + 2, kuangSpr->getContentSize().height* .92f - 2));
    nameLabel2->setColor(ccGRAY);
    kuangSpr->addChild(nameLabel2);
    
    // 提示
    CCLabelTTF* tishiLabel = CCLabelTTF::create("提示: ", DISPLAY->fangzhengFont(), 22, CCSizeMake(370, 22), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    tishiLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .78f));
    tishiLabel->setColor(ccc3(103, 81, 95));
//    tishiLabel->enableStroke(ccc3(103, 81, 95), .4f);
    kuangSpr->addChild(tishiLabel, 2);
    
    CCSprite* xianSpr = CCSprite::create("res/pic/taskScene/task_xian.png");
    xianSpr->setAnchorPoint(ccp(0, .5f));
    xianSpr->setPosition(ccp(2, kuangSpr->getContentSize().height* .74f));
    kuangSpr->addChild(xianSpr);
    
    // 开始故事
    CCSprite* startSpr1;
    CCSprite* startSpr2;
    CCMenuItem* startItem;
    if (unlockCondition <= OpenToWhichOne) {
        startSpr1 = CCSprite::create("res/pic/common/btn_startmission.png");
        startSpr2 = CCSprite::create("res/pic/common/btn_startmission.png");
        startSpr2->setScale(1.02f);
        startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(TaskScene::startCallBack));
        startItem->setPosition(ccp(kuangSpr->getContentSize().width* .845f, kuangSpr->getContentSize().height* .225f));
        startItem->setTag(index);
    }else{
        startSpr1 = CCSprite::create("res/pic/common/btn_startmission.png");
        startSpr1->setColor(ccGRAY);
        startSpr2 = CCSprite::create("res/pic/common/btn_startmission.png");
        startSpr2->setColor(ccGRAY);
        startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, NULL);
        startItem->setPosition(ccp(kuangSpr->getContentSize().width* .845f, kuangSpr->getContentSize().height* .225f));
        startItem->setColor(ccGRAY);
        startItem->setTag(index);
    }
    CCMenu* menu = CCMenu::create(startItem, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu);
    
    // 小人
//    CCSprite* xiaorenSpr = CCSprite::create("res/pic/taskScene/task_xiaoren.png");
//    xiaorenSpr->setPosition(ccp(kuangSpr->getContentSize().width* .84f, kuangSpr->getContentSize().height* .69f));
//    kuangSpr->addChild(xiaorenSpr, 100);
    
    // 消耗体力
    CCSprite* tiliSpr = CCSprite::create("res/pic/taskScene/task_tili_bar.png");
    tiliSpr->setPosition(ccp(kuangSpr->getContentSize().width* .845f, kuangSpr->getContentSize().height* .066f));
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
    xinSpr->setPosition(ccp(tiliSpr->getContentSize().width* .86f, tiliSpr->getContentSize().height* .49f));
    tiliSpr->addChild(xinSpr);
    
    int tag1 = dic->valueForKey("tag1")->intValue();
    int tag2 = dic->valueForKey("tag2")->intValue();
    int tag3 = dic->valueForKey("tag3")->intValue();
    if (tag1 > 0) {
        CCString* tagStr1 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag1);
        CCSprite* tagSpr1 = CCSprite::create(tagStr1->getCString());
        tagSpr1->setPosition(ccp(kuangSpr->getContentSize().width* .1f, kuangSpr->getContentSize().height* .13f));
        kuangSpr->addChild(tagSpr1);
    }
    if (tag2 > 0) {
        CCString* tagStr2 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag2);
        CCSprite* tagSpr2 = CCSprite::create(tagStr2->getCString());
        tagSpr2->setPosition(ccp(kuangSpr->getContentSize().width* .26f, kuangSpr->getContentSize().height* .13f));
        kuangSpr->addChild(tagSpr2);
    }
    if (tag3 > 0) {
        CCString* tagStr3 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag3);
        CCSprite* tagSpr3 = CCSprite::create(tagStr3->getCString());
        tagSpr3->setPosition(ccp(kuangSpr->getContentSize().width* .42f, kuangSpr->getContentSize().height* .13f));
        kuangSpr->addChild(tagSpr3);
    }
}

void TaskScene::startCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    taskIndex = item->getTag();
    
    if (DATA->getClothes()->has_init_clothes == true) {
        this->_startCallBack(NULL);
    }
    else {
        LOADING->show_loading();
        NET->owned_clothes_400();
    }
}

void TaskScene::_startCallBack(CCObject* pSender){
    LOADING->remove();
    CCLayer* layer = ClothesScene::create_with_type(1, taskIndex, taskPhase);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
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
    CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(index - 1);
    
    return (CCString*)dic->valueForKey("name");
}
CCString* TaskScene::getTaskDescription(int index){
    CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(index - 1);
    
    return (CCString*)dic->valueForKey("desc");
}


void TaskScene::exitView(){
    BaseScene::hideBaseScene();
    
    int curPhase = DATA->getPlayer()->phase;
    int ratingsRequire = CONFIG->phase_up_required(curPhase);
    if (ratingsRequire == 9999) {
        barSpr->setVisible(false);
    }
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TaskScene::openTaskStoryScene));
    
    if (historyBool) {
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
        CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
        taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, callFunc, NULL));
    }else{
        CCFadeOut* fadeOut1 = CCFadeOut::create(.3f);
        CCFadeOut* fadeOut2 = CCFadeOut::create(.3f);
        CCFadeOut* fadeOut3 = CCFadeOut::create(.3f);
        CCFadeOut* fadeOut4 = CCFadeOut::create(.3f);
        CCFadeOut* fadeOut5 = CCFadeOut::create(.3f);
        if (DATA->getTaskPhase() == 1){
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
            CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
            taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, CCDelayTime::create(.1f), callFunc, NULL));
        }else if (DATA->getTaskPhase() > 1 && DATA->getTaskPhase() <= 2) {
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, callFunc, NULL));
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
            CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
            taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
        }else if (DATA->getTaskPhase() > 2 && DATA->getTaskPhase() <= 3){
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
            historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, callFunc, NULL));
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
            CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
            taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
        }else if (DATA->getTaskPhase() > 3 && DATA->getTaskPhase() <= 4){
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
            historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, NULL));
            historyItem3->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeOut3, callFunc, NULL));
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
            CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
            taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
        }else if (DATA->getTaskPhase() > 4 && DATA->getTaskPhase() <= 5){
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
            historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, NULL));
            historyItem3->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeOut3, NULL));
            historyItem4->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCShow::create(), fadeOut4, callFunc, NULL));
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
            CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
            taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
        }else if (DATA->getTaskPhase() > 5){
            historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
            historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, NULL));
            historyItem3->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeOut3, NULL));
            historyItem4->runAction(CCSequence::create(CCDelayTime::create(1.1f), CCShow::create(), fadeOut4, NULL));
            historyItem5->runAction(CCSequence::create(CCDelayTime::create(1.3f), CCShow::create(), fadeOut5, callFunc, NULL));
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
            CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
            taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
        }
    }
}
void TaskScene::openTaskStoryScene(){
    
    if (DATA->current_guide_step() == 0 || DATA->current_guide_step() == 9) {
        CCScene* scene = TaskStoryScene::scene();
        CCDirector::sharedDirector()->replaceScene(scene);
    }else{
        PlayerComp* _player = DATA->getPlayer();
        
        if (_player->getGuide() == 2) {
            _player->setGuide(3);
        }
        
        LOADING->show_loading();
        NET->update_guide_905(_player->getGuide());
    }
}
void TaskScene::_905status(){
    _905Bool = true;
}
void TaskScene::_905CallBack(CCObject* pSender){
    if (!_905Bool) {
        CCScene* scene = TaskStoryScene::scene();
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}


void TaskScene::creat_Man(){
    float widthFolt = DISPLAY->ScreenWidth()* .5f + 95;
    float heightFloat = .3f;
    float scaleFloat = 1.5f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void TaskScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = DISPLAY->ScreenWidth()* .5f + 95;
    float heightFloat = .3f;
    float scaleFloat = 1.5f;
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_CL_TouFa; i <= Tag_CL_ZhuangRong; i++) {
        if (i == Tag_CL_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_CL_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_CL_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_CL_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_CL_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_CL_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_CL_WaiTao1);
                _ManSpr->addChild(_wtSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_CL_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_CL_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_CL_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_CL_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
                
//                CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
//                xingSpr1->setFlipX(true);
//                xingSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight()* .488f));
//                _ManSpr->addChild(xingSpr1, 420);
//                
//                CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
//                xingSpr2->setFlipX(true);
//                xingSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .385f, DISPLAY->ScreenHeight()* .473f));
//                _ManSpr->addChild(xingSpr2, 420);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        sub_part = clothDic->valueForKey("sub_part")->intValue();
                        
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_CL_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_CL_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_CL_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_CL_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_CL_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }
                
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_CL_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_CL_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_CL_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_CL_WaZi1);
                _ManSpr->addChild(_wzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr1->setTag(Tag_CL_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_CL_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_CL_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_CL_XieZi1);
                _ManSpr->addChild(_xzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_CL_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_CL_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_CL_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr->addChild(_spSpr1, 50);
                }else{
                    CCDictionary* dic = CONFIG->clothes();// 所有衣服
                    CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                    for (int k = 0; k < clothesArr->count(); k++) {
                        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(k);
                        int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                        if (now_clothes_Id == cloth_id->getValue()) {
                            const CCString* layer1 =  clothDic->valueForKey("layer1");
                            const CCString* layer2 =  clothDic->valueForKey("layer2");
                            const CCString* layer3 =  clothDic->valueForKey("layer3");
                            if (layer1->compare("") != 0) {
                                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_CL_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_CL_Bao1);
                _ManSpr->addChild(_bSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_CL_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_CL_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_CL_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_CL_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_CL_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _zrSpr1->setTag(Tag_CL_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}


void TaskScene::init_phone(){
    int renwuIndex = 0;
    int phaseIndex = 0;
    int allNumber = 0;
    // 显示的任务的结局
    CSJson::Value taskConditionsData = AppUtil::read_json_file("res/story/taskConditions");
    CCDictionary* taskConditionsDic = AppUtil::dictionary_with_json(taskConditionsData);
    allNumber = taskConditionsDic->count();
    
    for (int i = 0; i < allNumber; i++) {
        CCString* taskConditionsKeyStr = CCString::createWithFormat("101_80100_%d", i);
        CCArray* taskConditionsAchievemArr = (CCArray* )taskConditionsDic->objectForKey(taskConditionsKeyStr->getCString());
        std::string renwuIndexStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(2))->getCString();
        renwuIndex = atoi(renwuIndexStr.c_str());
        std::string phaseIndexStr = ((CCString* )taskConditionsAchievemArr->objectAtIndex(5))->getCString();
        phaseIndex = atoi(phaseIndexStr.c_str());
        if (renwuIndex <= DATA->getPlayer()->ratings(phaseIndex)) {// 解锁
            CCString* story_index = CCString::createWithFormat("%d", i);
            CCArray* storyArr = DATA->getStory()->story_achievments(story_index->getCString());
            if (storyArr == NULL) {
                now_task_index = i;
                
                if (DATA->current_guide_step() == 9) {
                    this->creat_phone2();
                }else{
                    if (DATA->current_guide_step() == 0) {
                        this->creat_phone();
                    }
                }
                
                break;
            }
        }
    }
}
void TaskScene::creat_phone(){
    CCSprite* phoneDiSpr = CCSprite::create("res/pic/taskScene/animation/task_phoneDi.png");
    phoneDiSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .22f));
    phoneDiSpr->setTag(0x7788);
    _ManSpr->addChild(phoneDiSpr, 900);
    
    CCSprite* phoneSpr1 = CCSprite::create("res/pic/taskScene/animation/task_phone.png");
    CCSprite* phoneSpr2 = CCSprite::create("res/pic/taskScene/animation/task_phone.png");
    CCMenuItem* phoneItem = CCMenuItemSprite::create(phoneSpr1, phoneSpr2, this, menu_selector(TaskScene::phoneCallBack));
    phoneItem->setPosition(ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .26f));
    phoneItem->setTag(now_task_index);
    CCMenu* menu = CCMenu::create(phoneItem, NULL);
    menu->setPosition(CCPointZero);
    _ManSpr->addChild(menu, 1000);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .26f + 5));
    CCMoveTo* moveTo2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .26f));
    phoneItem->runAction(CCRepeatForever::create(CCSequence::create(moveTo1, moveTo2, CCDelayTime::create(.1f), NULL)));
    
    if (DATA->current_guide_step() == 5) {
        phoneItem->setEnabled(false);
    }
    
    
    CCArray* phoneAnimations = CCArray::createWithCapacity(5);
    char strPei[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(strPei, "res/pic/taskScene/animation/task_phone%d.png", i);
        CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 154, 51));
        phoneAnimations->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(phoneAnimations, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "phoneStr");
    CCSprite* phoneSpr = CCSprite::create("res/pic/taskScene/animation/task_phone1.png");
    phoneSpr->setPosition(ccp(phoneItem->getContentSize().width* .5f, phoneItem->getContentSize().height* .5f));
    phoneSpr->setTag(0x88999);
    phoneItem->addChild(phoneSpr, 100);
    CCAnimation* phoneAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("phoneStr");
    CCAnimate* animate = CCAnimate::create(phoneAnimation);
    CCRepeatForever* rep = CCRepeatForever::create(animate);
    CCDirector::sharedDirector()->getActionManager()->addAction(rep, phoneSpr, false);
}
void TaskScene::creat_phone2(){
    CCSprite* phoneDiSpr = CCSprite::create("res/pic/taskScene/animation/task_phoneDi.png");
    phoneDiSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .22f));
    phoneDiSpr->setTag(0x7788);
    phoneDiSpr->setVisible(false);
    _ManSpr->addChild(phoneDiSpr, 900);
    
    CCSprite* phoneSpr1 = CCSprite::create("res/pic/taskScene/animation/task_phone.png");
    CCSprite* phoneSpr2 = CCSprite::create("res/pic/taskScene/animation/task_phone.png");
    CCMenuItem* phoneItem = CCMenuItemSprite::create(phoneSpr1, phoneSpr2, this, menu_selector(TaskScene::phoneCallBack));
//    phoneItem->setPosition(ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .26f));
    phoneItem->setPosition(ccp(0, DISPLAY->ScreenHeight()* .26f));
    phoneItem->setTag(now_task_index);
    CCMenu* menu = CCMenu::create(phoneItem, NULL);
    menu->setPosition(CCPointZero);
    menu->setTag(0x66555);
    _ManSpr->addChild(menu, 1000);
    
    CCRotateTo* rotateTo1 = CCRotateTo::create(.8f, 55.f);
    CCRotateTo* rotateTo2 = CCRotateTo::create(.8f, 0.f);
    CCJumpTo* jumpTo1 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .05f, DISPLAY->ScreenHeight()* .26f), -5, 2);
    CCJumpTo* jumpTo2 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .1f, DISPLAY->ScreenHeight()* .26f), 20, 2);
    CCJumpTo* jumpTo3 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .15f, DISPLAY->ScreenHeight()* .26f), 0, 2);
    CCJumpTo* jumpTo4 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .26f), 25, 2);
    CCJumpTo* jumpTo5 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .26f), 0, 2);
    CCJumpTo* jumpTo6 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* .26f), 30, 2);
    CCJumpTo* jumpTo7 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .35f, DISPLAY->ScreenHeight()* .26f), 0, 2);
    CCJumpTo* jumpTo8 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* .26f), 20, 2);
    CCJumpTo* jumpTo9 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .45f, DISPLAY->ScreenHeight()* .26f), 0, 2);
    CCJumpTo* jumpTo10 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .26f), 15, 2);
    CCJumpTo* jumpTo11 = CCJumpTo::create(.15f, ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .26f), 0, 2);
    CCSequence* seq = CCSequence::create(jumpTo1, jumpTo2, jumpTo3, jumpTo4, jumpTo5, jumpTo6, jumpTo7, jumpTo8, jumpTo9, jumpTo10, jumpTo11, NULL);
    CCSpawn* spawn = CCSpawn::create(rotateTo2, seq, NULL);
    phoneItem->runAction(CCSequence::create(rotateTo1, CCDelayTime::create(.5f), spawn, NULL));
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TaskScene::creat_phone3));
    phoneDiSpr->runAction(CCSequence::create(CCDelayTime::create(3.f), CCShow::create(), callFunc, NULL));
}
void TaskScene::creat_phone3(){
    CCMenu* menu = (CCMenu* )_ManSpr->getChildByTag(0x66555);
    CCMenuItem* phoneItem = (CCMenuItem* )menu->getChildByTag(now_task_index);
    
    CCArray* phoneAnimations = CCArray::createWithCapacity(5);
    char strPei[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(strPei, "res/pic/taskScene/animation/task_phone%d.png", i);
        CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 154, 51));
        phoneAnimations->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(phoneAnimations, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "phoneStr");
    CCSprite* phoneSpr = CCSprite::create("res/pic/taskScene/animation/task_phone1.png");
    phoneSpr->setPosition(ccp(phoneItem->getContentSize().width* .5f, phoneItem->getContentSize().height* .5f));
    phoneSpr->setTag(0x88999);
    phoneItem->addChild(phoneSpr, 100);
    CCAnimation* phoneAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("phoneStr");
    CCAnimate* animate = CCAnimate::create(phoneAnimation);
    CCRepeatForever* rep = CCRepeatForever::create(animate);
    CCDirector::sharedDirector()->getActionManager()->addAction(rep, phoneSpr, false);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .26f + 5));
    CCMoveTo* moveTo2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .542f, DISPLAY->ScreenHeight()* .26f));
    phoneItem->runAction(CCRepeatForever::create(CCSequence::create(moveTo1, moveTo2, CCDelayTime::create(.1f), NULL)));
    
    for (int i = 0; i < 10; i++) {
        DATA->_guideBool9[i] = false;
    }
    if (this->getChildByTag(0x445566) != NULL) {
        this->removeChildByTag(0x445566);
    }
    GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
    layer->setTag(0x445566);
    this->addChild(layer, 500);
}

void TaskScene::phoneCallBack(CCObject* pSender){
    AUDIO->shop_effect();
    CCLog("phoneCallBack");
    
    CCLayer* layer = PhoneLayer2::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}




