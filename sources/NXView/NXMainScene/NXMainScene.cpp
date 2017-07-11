//
//  NXMainScene.cpp
//  tiegao
//
//  Created by mac on 17-6-29.
//
//

#include "NXMainScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NoticeManager.h"
#include "AudioManager.h"
#include "WSManager.h"
#include "PromptLayer.h"
#include "Loading2.h"
#include "NetManager.h"
#include "ConfigManager.h"
#include "AppUtil.h"
#include "RewardLayer.h"
#include "AHMessageBox.h"


#include "JNIController.h"
#include "NXScrollView.h"
#include "NXClothesScene.h"
#include "DailySigninRewardPanel.h"
#include "NewSignin7Panel.h"
#include "TotalRechargePanel.h"
#include "NXQingjingScene.h"
#include "NXSocialScene.h"
// --------------- test ----------------


NXMainScene::NXMainScene(){
    
}
NXMainScene::~NXMainScene(){
    
}

bool NXMainScene::init(){
    if (!NXBaseScene::init()) {
        return false;
    }
    num_child = 0;
    leftBool = false;
    rightBool = false;
    isOk = false;
    isrenwuBool = false;
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    tempSpr = CCSprite::create("res/nxpic/NXMainscene/huodong/NXMainscene_dikuang.png");
    tempSpr->setAnchorPoint(ccp(1, 0));
    tempSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 2.f, 4.f));
    tempSpr->setVisible(false);
    this->addChild(tempSpr, 10);
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->creat_view();
    
    
    
    struct timeval star;
    struct tm* time;
    gettimeofday(&star,  NULL);
    time = localtime(&star.tv_sec);
    std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("Tm_MDay", "0");
    int num = atoi(str.c_str());
    if (num < time->tm_mday) {
        
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_MDay", CCString::createWithFormat("%d", time->tm_mday)->getCString());
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_One", CCString::createWithFormat("%d", 0)->getCString());
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_Ten", CCString::createWithFormat("%d", 0)->getCString());
    }else if (num > time->tm_mday) {
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_MDay", CCString::createWithFormat("%d", time->tm_mday)->getCString());
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_One", CCString::createWithFormat("%d", 0)->getCString());
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_Ten", CCString::createWithFormat("%d", 0)->getCString());
    }
    CCUserDefault::sharedUserDefault()->flush();
    
    return true;
}
CCScene* NXMainScene::scene(){
    CCScene* scene = CCScene::create();
    NXMainScene* layer = NXMainScene::create();
    scene->addChild(layer);
    
    return scene;
}

void NXMainScene::onEnter(){
    NXBaseScene::onEnter();
    NXBaseScene::openChat(true);

    this->setAccelerometerEnabled(true); // ?
    
    CCString* strBGM = AUDIO->getCurBGM();
    if (strBGM && strBGM->compare("MAIN") == 0) {
        
    }
    else {
        AUDIO->play_main_bgm();
    }
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&NXMainScene::nc_recharge_info_304), "HTTP_FINISHED_304", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXMainScene::nc_signin_info_312), "HTTP_FINISHED_312", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXMainScene::_500CallBack), "HTTP_FINISHED_500", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXMainScene::_600CallBack), "HTTP_FINISHED_600", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXMainScene::_905CallBack), "HTTP_FINISHED_905", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXMainScene::_800Callback), "HTTP_FINISHED_800", NULL);
    
    if (DATA->current_guide_step() == 0) {
        if (DATA->getNews()->signin7 == 1) {
            isOk = true;
            this->qiandaoCallBack(NULL);
        }
        else {
            this->check_dailysignin();
        }
    }
    
    this->checkVersion();
    
    NET->fetch_tryst_info_620();
}
void NXMainScene::checkVersion() {
    CCDictionary* mainConf = DATA->getLogin()->config();
    CCInteger* versionFlag = (CCInteger*)mainConf->objectForKey("ver");
    if (NULL != versionFlag) {
        int isNewVersion = versionFlag->getValue();
        if (0 != isNewVersion) {
            AHMessageBox* mb = AHMessageBox::create_with_message("检测到您当前不是最新版本!\n请前往应用商店更新~!", NULL, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YES, false);
            mb->setPosition(DISPLAY->center());
            this->addChild(mb, 3900);
        }
    }
}
void NXMainScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    NXBaseScene::onExit();
}

void NXMainScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void NXMainScene::keyBackClicked(){
    CCLog("===== MMChooseLayer::keyBackClicked");
    if (DATA->current_guide_step() == 0 || DATA->current_guide_step() >= 7) {
        num_child++;
        CCLog("===== MainScene  children_num: %d", num_child);
        if (num_child> 1) {
            num_child = 0;
            return;
        }
        
        if (CONFIG->baiOrYijie == 0) {// 白包
            JNIController::setUserId(DATA->getLogin()->obtain_sid());
            JNIController::setPlayerName(DATA->getShow()->nickname());
            CCString* phaseStr = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
            JNIController::setPlayerLevel(phaseStr->getCString());
            CCString* diamStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
            JNIController::setPlayerGold(diamStr->getCString());
            JNIController::setData(4);
            
            num_child = 0;
            JNIController::exitGame(0);
        }else if (CONFIG->baiOrYijie == 1 || CONFIG->baiOrYijie == 2){// 易接
            JNIController::setUserId(DATA->getLogin()->obtain_sid());
            JNIController::setPlayerName(DATA->getShow()->nickname());
            CCString* phaseStr = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
            JNIController::setPlayerLevel(phaseStr->getCString());
            CCString* diamStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
            JNIController::setPlayerGold(diamStr->getCString());
            JNIController::setData(4);
            
            num_child = 0;
            JNIController::exitGame(1);
        }else if (CONFIG->baiOrYijie == 3){
            num_child = 0;
            JNIController::exitGame(1);
        }
    }
}



void NXMainScene::creat_view(){
    
    CCSprite* bgSpr = CCSprite::create("res/nxpic/NXMainscene/nx_bgImage.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    this->creat_Man();
    this->initClothes();
    
    this->creatHuodong();
    this->creatQiu();
}
void NXMainScene::xingdong_inAction(){
    
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.6f, ccp(-7.f + 3.f, 10.f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.4f, ccp(-7.f, 10.f));
    xingdongItem->runAction(CCSequence::create(CCDelayTime::create(.4f), moveTo1_1, moveTo1_2, NULL));
    
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.6f, ccp(DISPLAY->ScreenWidth() - 2.f - 5.f, 4.f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.4f, ccp(DISPLAY->ScreenWidth() - 2.f, 4.f));
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&NXMainScene::openTouch));
    huodongKangSpr->runAction(CCSequence::create(CCDelayTime::create(.4f), moveTo2_1, moveTo2_2, callFunc, NULL));
}
void NXMainScene::xingdong_outAction(CCObject* pSender){
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.4f, ccp(-7.f + 3.f, 10.f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.6f, ccp(-7.f - 400.f, 10.f));
    xingdongItem->runAction(CCSequence::create(CCDelayTime::create(.4f), moveTo1_1, moveTo1_2, NULL));
    
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.4f, ccp(DISPLAY->ScreenWidth() - 2.f - 5.f, 4.f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.6f, ccp(DISPLAY->ScreenWidth() - 2.f + 400, 4.f));
    huodongKangSpr->runAction(CCSequence::create(CCDelayTime::create(.4f), moveTo2_1, moveTo2_2, NULL));
    
    if (pSender != NULL) {
        rightBool = true;
        leftBool = false;
        this->setTouchEnabled(false);
        this->scheduleOnce(SEL_SCHEDULE(&NXMainScene::qiu_inAction), .8f);
    }
}

void NXMainScene::creatHuodong(){
    CCSprite* xingdongSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_xingdong.png");
    CCSprite* xingdongSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_xingdong.png");
    xingdongItem = CCMenuItemSprite::create(xingdongSpr1, xingdongSpr2, this, menu_selector(NXMainScene::xingdong_outAction));
    xingdongItem->setAnchorPoint(CCPointZero);
    xingdongItem->setPosition(ccp(-7.f - 400.f, 10.f));
    CCMenu* xingdongMenu = CCMenu::create(xingdongItem, NULL);
    xingdongMenu->setPosition(CCPointZero);
    this->addChild(xingdongMenu, 50);
    
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.6f, ccp(-7.f + 3.f, 10.f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.4f, ccp(-7.f, 10.f));
    xingdongItem->runAction(CCSequence::create(CCDelayTime::create(.8f), moveTo1_1, moveTo1_2, NULL));
    
    //
    huodongKangSpr = CCSprite::create("res/nxpic/NXMainscene/huodong/NXMainscene_dikuang.png");
    huodongKangSpr->setAnchorPoint(ccp(1, 0));
    huodongKangSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 2.f + 400, 4.f));
    this->addChild(huodongKangSpr, 10);
    
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.6f, ccp(DISPLAY->ScreenWidth() - 2.f - 5.f, 4.f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.4f, ccp(DISPLAY->ScreenWidth() - 2.f, 4.f));
    huodongKangSpr->runAction(CCSequence::create(CCDelayTime::create(.8f), moveTo2_1, moveTo2_2, NULL) );
    
    CCLayer *scrollLayer = NXScrollView::create();
    scrollLayer->setPosition(ccp(9, 0));
//    scrollLayer->setPosition(CCPointZero);
    huodongKangSpr->addChild(scrollLayer);
}
void NXMainScene::creatQiu(){
    CCSprite* xingdongSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_fanhui.png");
    CCSprite* xingdongSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_fanhui.png");
    fanhuiItem = CCMenuItemSprite::create(xingdongSpr1, xingdongSpr2, this, menu_selector(NXMainScene::qiu_outAction));
    fanhuiItem->setAnchorPoint(CCPointZero);
    fanhuiItem->setPosition(ccp(-7.f - 400.f, 10.f));
    CCMenu* fanhuiMenu = CCMenu::create(fanhuiItem, NULL);
    fanhuiMenu->setPosition(CCPointZero);
    this->addChild(fanhuiMenu, 50);
    
    // 好友
    CCSprite* hySpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_haoyou1.png");
    CCSprite* hySpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_haoyou1.png");
    hySpr2->setScale(1.02f);
    haoyouItem = CCMenuItemSprite::create(hySpr1, hySpr2, this, menu_selector(NXMainScene::haoyouCallback));
    haoyouItem->setPosition(ccp(DISPLAY->ScreenWidth()* .13f, DISPLAY->ScreenHeight()* .37f - 500.f));
    CCSprite* haoyouSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_haoyou.png");
    haoyouSpr2->setAnchorPoint(ccp(.5f, 1.f));
    haoyouSpr2->setPosition(ccp(haoyouItem->getContentSize().width* .5f, -5.f));
    haoyouItem->addChild(haoyouSpr2);
    haoyouSpr = CCSprite::create("res/nxpic/NXMainscene/nx_haoyou2.png");
    haoyouSpr->setPosition(ccp(haoyouItem->getContentSize().width* .5f, haoyouItem->getContentSize().height* .5f));
    haoyouItem->addChild(haoyouSpr);
    
    // 任务
    CCSprite* tSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_task1.png");
    CCSprite* tSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_task1.png");
    tSpr2->setScale(1.02f);
    taskItem = CCMenuItemSprite::create(tSpr1, tSpr2, this, menu_selector(NXMainScene::taskCallback));
    taskItem->setPosition(ccp(DISPLAY->ScreenWidth()* .27f, DISPLAY->ScreenHeight()* .24f - 500.f));
    CCSprite* taskSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_task.png");
    taskSpr2->setAnchorPoint(ccp(.5f, 1.f));
    taskSpr2->setPosition(ccp(taskItem->getContentSize().width* .5f, -5.f));
    taskItem->addChild(taskSpr2);
    taskSpr = CCSprite::create("res/nxpic/NXMainscene/nx_task2.png");
    taskSpr->setPosition(ccp(taskItem->getContentSize().width* .5f, taskItem->getContentSize().height* .5f));
    taskItem->addChild(taskSpr);
    
    // 情景
    CCSprite* qjSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_qingjing1.png");
    CCSprite* qjSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_qingjing1.png");
    qjSpr2->setScale(1.02f);
    qingjingItem = CCMenuItemSprite::create(qjSpr1, qjSpr2, this, menu_selector(NXMainScene::qingjingCallback));
    qingjingItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .18f - 500.f));
    CCSprite* qingjingSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_qingjing.png");
    qingjingSpr2->setAnchorPoint(ccp(.5f, 1.f));
    qingjingSpr2->setPosition(ccp(qingjingItem->getContentSize().width* .5f, -5.f));
    qingjingItem->addChild(qingjingSpr2);
    qingjingSpr = CCSprite::create("res/nxpic/NXMainscene/nx_qingjing2.png");
    qingjingSpr->setPosition(ccp(qingjingItem->getContentSize().width* .5f, qingjingItem->getContentSize().height* .5f));
    qingjingItem->addChild(qingjingSpr);
    
    // 商店
    CCSprite* sSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_shop1.png");
    CCSprite* sSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_shop1.png");
    sSpr2->setScale(1.02f);
    shopItem = CCMenuItemSprite::create(sSpr1, sSpr2, this, menu_selector(NXMainScene::shopCallback));
    shopItem->setPosition(ccp(DISPLAY->ScreenWidth()* .73f, DISPLAY->ScreenHeight()* .24f - 500.f));
    CCSprite* shopSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_shop.png");
    shopSpr2->setAnchorPoint(ccp(.5f, 1.f));
    shopSpr2->setPosition(ccp(shopItem->getContentSize().width* .5f, -5.f));
    shopItem->addChild(shopSpr2);
    shopSpr = CCSprite::create("res/nxpic/NXMainscene/nx_shop2.png");
    shopSpr->setPosition(ccp(shopItem->getContentSize().width* .5f, shopItem->getContentSize().height* .5f));
    shopItem->addChild(shopSpr);
    
    // pk
    CCSprite* pSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_pk1.png");
    CCSprite* pSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_pk1.png");
    pSpr2->setScale(1.02f);
    pkItem = CCMenuItemSprite::create(pSpr1, pSpr2, this, menu_selector(NXMainScene::pkCallback));
    pkItem->setPosition(ccp(DISPLAY->ScreenWidth()* .87f, DISPLAY->ScreenHeight()* .37f - 500.f));
    CCSprite* pkSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_pk.png");
    pkSpr2->setAnchorPoint(ccp(.5f, 1.f));
    pkSpr2->setPosition(ccp(pkItem->getContentSize().width* .5f, -5.f));
    pkItem->addChild(pkSpr2);
    pkSpr = CCSprite::create("res/nxpic/NXMainscene/nx_pk2.png");
    pkSpr->setPosition(ccp(pkItem->getContentSize().width* .5f, pkItem->getContentSize().height* .5f));
    pkItem->addChild(pkSpr);
    
    CCMenu* menu = CCMenu::create(haoyouItem, taskItem, qingjingItem, shopItem, pkItem, NULL);
    menu->setAnchorPoint(ccp(.5f, .5f));
    menu->setPosition(CCPointZero);
    this->addChild(menu, 30);
    
    this->scheduleOnce(SEL_SCHEDULE(&NXMainScene::randAnimation), 1.f);
}
void NXMainScene::randAnimation(float dt){
    int index = 1 + rand()%5;
    if (index == 1) {
        this->openAnimation(haoyouSpr, "haoyou");
    }else if (index == 2){
        this->openAnimation(taskSpr, "task");
    }else if (index == 3){
        this->openAnimation(qingjingSpr, "qingjing");
    }else if (index == 4){
        this->openAnimation(shopSpr, "shop");
    }else if (index == 5){
        this->openAnimation(pkSpr, "pk");
    }
}
void NXMainScene::openAnimation(CCSprite* spr, std::string str){
    float timeFloat = .7f;
    CCArray* qiuAnimations = CCArray::createWithCapacity(8);
    char qiuStr[100] = {};
    for (int i = 4; i <= 3; i--) {
        
        sprintf(qiuStr, "res/nxpic/NXMainscene/nx_%s%d.png", str.c_str(), i);
        CCSpriteFrame* qiuFrame = CCSpriteFrame::create(qiuStr, CCRectMake(0, 0, 106, 106));
        qiuAnimations->addObject(qiuFrame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(qiuAnimations, timeFloat);
    
    CCString* qiuStr2 = CCString::createWithFormat("res/nxpic/NXMainscene/nx_%s3.png", str.c_str());
    CCSprite* qiuSpr2 = CCSprite::create(qiuStr2->getCString());
    qiuSpr2->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    qiuSpr2->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .5f));
    spr->addChild(qiuSpr2);
    
    CCPoint pos = spr->getPosition();
    CCMoveTo* moveTo1 = CCMoveTo::create(timeFloat, ccp(pos.x, pos.y + 8.f));
    CCMoveTo* moveTo2 = CCMoveTo::create(timeFloat, ccp(pos.x, pos.y));
    CCScaleTo* scaleTo1 = CCScaleTo::create(timeFloat, 1.1f);
    CCScaleTo* scaleTo2 = CCScaleTo::create(timeFloat, 1.f);
    CCSequence* seq1 = CCSequence::create(moveTo1, moveTo2, NULL);
    CCSequence* seq2 = CCSequence::create(scaleTo1, scaleTo2, NULL);
    CCSpawn* spawn = CCSpawn::create(seq1, seq2, NULL);
    spr->runAction(CCRepeatForever::create(spawn));
    
    this->scheduleOnce(SEL_SCHEDULE(&NXMainScene::closeAnimation), 3.f);
}
void NXMainScene::closeAnimation(){
    haoyouSpr->removeAllChildren();
    haoyouSpr->stopAllActions();
    haoyouSpr->setScale(1.f);
    haoyouSpr->setPosition(ccp(haoyouItem->getContentSize().width* .5f, haoyouItem->getContentSize().height* .5f));

    taskSpr->removeAllChildren();
    taskSpr->stopAllActions();
    taskSpr->setScale(1.f);
    taskSpr->setPosition(ccp(taskItem->getContentSize().width* .5f, taskItem->getContentSize().height* .5f));

    qingjingSpr->removeAllChildren();
    qingjingSpr->stopAllActions();
    qingjingSpr->setScale(1.f);
    qingjingSpr->setPosition(ccp(qingjingItem->getContentSize().width* .5f, qingjingItem->getContentSize().height* .5f));

    shopSpr->removeAllChildren();
    shopSpr->stopAllActions();
    shopSpr->setScale(1.f);
    shopSpr->setPosition(ccp(shopItem->getContentSize().width* .5f, shopItem->getContentSize().height* .5f));

    pkSpr->removeAllChildren();
    pkSpr->stopAllActions();
    pkSpr->setScale(1.f);
    pkSpr->setPosition(ccp(pkItem->getContentSize().width* .5f, pkItem->getContentSize().height* .5f));

    this->scheduleOnce(SEL_SCHEDULE(&NXMainScene::randAnimation), 1.f);
}
void NXMainScene::qiu_inAction(){
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.6f, ccp(-7.f + 3.f, 10.f));
    CCLog("fanhuiItem1 == %f", fanhuiItem->getPosition().x);
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.4f, ccp(-7.f, 10.f));
    CCLog("fanhuiItem2 == %f", fanhuiItem->getPosition().x);
    fanhuiItem->runAction(CCSequence::create(CCDelayTime::create(.4f), moveTo1_1, moveTo1_2, NULL));
    
    CCFiniteTimeAction* haoyouMoveTo = CCMoveBy::create(.8f, ccp(0, + 500.f));
    CCFiniteTimeAction* pkMoveTo = CCMoveBy::create(.8f, ccp(0, + 500.f));
    CCFiniteTimeAction* taskMoveTo = CCMoveBy::create(.8f, ccp(0, + 500.f));
    CCFiniteTimeAction* shopMoveTo = CCMoveBy::create(.8f, ccp(0, + 500.f));
    CCFiniteTimeAction* qingjingMoveTo = CCMoveBy::create(.8f, ccp(0, + 500.f));
    
    haoyouItem->runAction(haoyouMoveTo);
    pkItem->runAction(pkMoveTo);
    
    taskItem->runAction(CCSequence::create(CCDelayTime::create(.3f), taskMoveTo, NULL));
    shopItem->runAction(CCSequence::create(CCDelayTime::create(.3f), shopMoveTo, NULL));
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&NXMainScene::openTouch));
    qingjingItem->runAction(CCSequence::create(CCDelayTime::create(.6f), qingjingMoveTo, callFunc, NULL));
}
void NXMainScene::openTouch(float dt){
    this->setTouchEnabled(true);
}
void NXMainScene::qiu_outAction(CCObject* pSender){
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.4f, ccp(-7.f + 3.f, 10.f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.6f, ccp(-7.f - 400.f, 10.f));
    fanhuiItem->runAction(CCSequence::create(CCDelayTime::create(.4f), moveTo1_1, moveTo1_2, NULL));
    
    CCFiniteTimeAction* haoyouMoveTo = CCMoveBy::create(.8f, ccp(0, - 500.f));
    CCFiniteTimeAction* pkMoveTo = CCMoveBy::create(.8f, ccp(0, - 500.f));
    CCFiniteTimeAction* taskMoveTo = CCMoveBy::create(.8f, ccp(0, - 500.f));
    CCFiniteTimeAction* shopMoveTo = CCMoveBy::create(.8f, ccp(0, - 500.f));
    CCFiniteTimeAction* qingjingMoveTo = CCMoveBy::create(.8f, ccp(0, - 500.f));
    
    qingjingItem->runAction(qingjingMoveTo);
    
    taskItem->runAction(CCSequence::create(CCDelayTime::create(.3f), taskMoveTo, NULL));
    shopItem->runAction(CCSequence::create(CCDelayTime::create(.3f), shopMoveTo, NULL));
    
    haoyouItem->runAction(CCSequence::create(CCDelayTime::create(.6f), haoyouMoveTo, NULL));
    pkItem->runAction(CCSequence::create(CCDelayTime::create(.6f), pkMoveTo, NULL));
    
    if (pSender != NULL) {
        leftBool = true;
        rightBool = false;
        this->setTouchEnabled(false);
        this->scheduleOnce(SEL_SCHEDULE(&NXMainScene::xingdong_inAction), .8f);
    }
}


void NXMainScene::haoyouCallback(CCObject* pSender){
    DATA->onEvent("点击事件", "主界面", "点击好友");
    
    AUDIO->comfirm_effect();
    LOADING->show_loading();
    NET->social_info_800();
    
}
void NXMainScene::pkCallback(CCObject* pSender){
    
}
void NXMainScene::qingjingCallback(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击剧情");
    
    if (isOk) {
        if (DATA->getStory()->has_init_story()) {
            this->_500CallBack(NULL);
        }else{
            LOADING->show_loading();
            NET->completed_story_500();
        }
    }
}
void NXMainScene::_500CallBack(CCObject* pSender){
    if (isrenwuBool) {
        LOADING->show_loading();
        NET->completed_mission_600();
    }else {
        AUDIO->comfirm_effect();
        CCScene* scene = NXQingjingScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void NXMainScene::_800Callback(CCObject* pObj) {
    LOADING->remove();
    CCScene* scene = NXSocialScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void NXMainScene::shopCallback(CCObject* pSender){
    AUDIO->comfirm_effect();
    
    DATA->setClothesBool(true);
    
    CCLayer* layer = NXClothesScene::create_with_type(2, 0, 0);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void NXMainScene::taskCallback(CCObject* pSender){
    PlayerComp* _player = DATA->getPlayer();
    if (_player->getGuide() == 1) {
        _player->setGuide(2);
        // talkingData
        DATA->onEvent("引导事件", "引导界面", "完成引导第1步");
    }else if (_player->getGuide() == 8){
        _player->setGuide(8);
    }else if (_player->getGuide() == 100){
        _player->setGuide(5);
    }
    
    LOADING->show_loading();
    NET->update_guide_905(_player->getGuide());
}
void NXMainScene::_905CallBack(CCObject *pObj){
    
    if (DATA->getStory()->has_init_story()) {
        NET->completed_mission_600();
    }else {
        isrenwuBool = true;
        NET->completed_story_500();
    }
}
void NXMainScene::_600CallBack(CCObject* pSender){
    LOADING->remove();
    AUDIO->comfirm_effect();
//    DATA->setTaskPhase(DATA->getPlayer()->phase);
//    CCLayer* layer = TaskScene::create(false);
//    CCScene* scene = CCScene::create();
//    scene->addChild(layer);
//    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
//    CCDirector::sharedDirector()->replaceScene(trans);
}

bool NXMainScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    if (!tempSpr->boundingBox().containsPoint(location)) {
        //获取触摸的X轴和Y轴
        CCPoint touchPoint = pTouch->getLocation(); //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）
        pTouch->getLocationInView();
        firstX = touchPoint.x;
        firstY = touchPoint.y;
        return true;
    }
    
	return false;
}
void NXMainScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    //获取X轴和Y轴的移动范围
    CCPoint touchPoint = pTouch->getLocation(); //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）
    int endX = firstX - touchPoint.x;
    int endY = firstY - touchPoint.y;
    int touchx = touchPoint.x;
    int touchy = touchPoint.y;
    
    CCLog("firstX %d   firstY %d     touchx  %d  touchy  %d", firstX, firstY, touchx, touchy);
    CCLog(" firstX - touchPoint.x %d  firstY - touchPoint.y %d", endX, endY);
    //判断X轴和Y轴的移动距离，如果X轴的绝对值大，则向左右滑动，如果Y轴的绝对值大，则向上下滑动
    if (abs(endX) > abs(endY))
    {
        //手势向左右
        //判断向左还是向右
        if (endX  > 0)
        {
            //向左函数
            if (endX > 150) {
                if (!leftBool) {
                    leftBool = true;
                    rightBool = false;
                    this->setTouchEnabled(false);
                    this->qiu_outAction(NULL);
                    this->scheduleOnce(SEL_SCHEDULE(&NXMainScene::xingdong_inAction), .8f);
                    CCLog("left");
                }
                
            }
        }
        else
        {
            //向右函数
            if (abs(endX) > 150) {
                if (!rightBool) {
                    rightBool = true;
                    leftBool = false;
                    this->setTouchEnabled(false);
                    this->xingdong_outAction(NULL);
                    this->scheduleOnce(SEL_SCHEDULE(&NXMainScene::qiu_inAction), .8f);
                    CCLog("right");
                }
            }
            
        }
    }
}

void NXMainScene::creat_Man(){
    float widthFolt = .5f;
    float heightFloat = .2f;
    float scaleFloat = 1.f;
    
    CCSprite* manSpr = CCSprite::create("res/nxpic/NXClothes/man/nx_man.png");
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    manSpr->setScale(scaleFloat);
    _ManSpr->addChild(manSpr, 200);
    
    CCSprite* geboSpr = CCSprite::create("res/nxpic/NXClothes/man/nx_gebo.png");
    geboSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    geboSpr->setScale(scaleFloat);
    _ManSpr->addChild(geboSpr, 415);
    
    _touSpr = CCSprite::create("res/nxpic/NXClothes/man/nx_lian.png");
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _touSpr->setScale(scaleFloat);
    _ManSpr->addChild(_touSpr, 210);
}
void NXMainScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = DISPLAY->ScreenWidth()* .5f;
    float heightFloat = .2f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_MS_TouFa; i <= Tag_MS_ZhuangRong; i++) {
        if (i == Tag_MS_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_MS_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_MS_TouFa2);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_MS_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_MS_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_MS_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_MS_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_MS_WaiTao1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_MS_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_MS_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_MS_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_MS_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_MS_ShangYi1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_MS_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_MS_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_MS_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_MS_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_MS_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_MS_KuZi1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_MS_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_MS_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_MS_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_MS_TeXiao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/5texiao/%d.png", 50000);
                _txSpr1 = CCSprite::create(str->getCString());
                _txSpr1->setScale(scaleFloat);
                _txSpr1->setFlipX(flipxBool);
                _txSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _txSpr1->setTag(Tag_MS_TeXiao1);
                _ManSpr->addChild(_txSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/5texiao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _txSpr1 = CCSprite::create(str1->getCString());
                            _txSpr1->setScale(scaleFloat);
                            _txSpr1->setFlipX(flipxBool);
                            _txSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _txSpr1->setTag(Tag_MS_TeXiao1);
                            _ManSpr->addChild(_txSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        break;
                    }
                }
            }
        }
        else if (i == Tag_MS_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_MS_XieZi1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_MS_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_MS_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_MS_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_MS_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/7shipin/%d.png", 70000);
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
                                CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
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
        else if (i == Tag_MS_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_MS_Bao1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_MS_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_MS_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_MS_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_MS_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_MS_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
                //
                DISPLAY->setCurZRId(90000);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _zrSpr1->setTag(Tag_MS_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                            //
                            DISPLAY->setCurZRId(layer1->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}


void NXMainScene::qiandaoCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击签到");
    
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        // 1.8.1前的版本
//        if (DATA->getSignin()->has_init_signin7_template()) {
//            NET->signin7_info_302(false);
//        }
//        else {
//            NET->signin7_info_302(true);
//        }
        // 1.8.1后的版本
        if (DATA->getSignin()->has_init_signin7_template()) {
            NET->signin7_info_312(false);
        }
        else {
            NET->signin7_info_312(true);
        }
        
        CCMenuItem* btn = (CCMenuItem* )pSender;
        if (btn) {
            btn->setEnabled(false);
            CCCallFuncN* afterCalled = CCCallFuncN::create(this, SEL_CallFuncN(&NXMainScene::afterMenuItemCalled));
            btn->runAction(CCSequence::create(CCDelayTime::create(1.f), afterCalled, NULL));
        }
    }
}
void NXMainScene::afterMenuItemCalled(CCMenuItem* btn) {
    CCLOG("MainScene::afterMenuItemCalled()");
    btn->setEnabled(true);
}

// 各种检查 [[
void NXMainScene::update_news_status(){
    
}
void NXMainScene::check_dailysignin(){
    this->scheduleOnce(SEL_SCHEDULE(&NXMainScene::update_dailysignin), 0.1);
}
void NXMainScene::update_dailysignin() {
    if (DATA->getNews()->dailySignin == 1) { // 可签
        DailySigninRewardPanel::show(this->getScene());
    }
}
void NXMainScene::check_free_gashapon(){
    
}
void NXMainScene::nc_recharge_info_304(CCObject *pObj){
    LOADING->remove();
    TotalRechargePanel::show();
}
void NXMainScene::nc_signin_info_312(CCObject* pSender){
    LOADING->remove();
    // 需要还原读取服务器数据的方法
//    Signin7Panel::show(this->getScene());
    // 当前
//    NewSignin7Panel::show(this->getScene());
}








void NXMainScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    
}





