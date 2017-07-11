//
//  NXQingjingScene.cpp
//  tiegao
//
//  Created by mac on 17-7-11.
//
//

#include "NXQingjingScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NXMainScene.h"
#include "ConfigManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "AudioManager.h"
#include "GuideLayer.h"
#include "WSManager.h"
#include "ChatPanel.h"
#include "CollectLayer.h"
//#include "StoryScene.h"


NXQingjingScene::NXQingjingScene(){
    
}
NXQingjingScene::~NXQingjingScene(){
    
}

bool NXQingjingScene::init(){
    if (!NXBaseScene::init()) {
        return false;
    }
    num_child = 0;
    
    bgSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_bj.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    
    
    
    this->creat_view();
    
    return true;
}

void NXQingjingScene::onEnter(){
    NXBaseScene::onEnter();
    NXBaseScene::openChat(false);
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, menu_selector(NXClothesScene::ChangeClothes), "ChangeClothes", NULL);
    
    
    this->scheduleOnce(SEL_SCHEDULE(&NXQingjingScene::keyBackStatus), .8f);
}
void NXQingjingScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    NXBaseScene::onExit();
}

void NXQingjingScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void NXQingjingScene::keyBackClicked(){
    num_child++;
    CCLog("===== NXClothesScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->backCallBack(NULL);
}

cocos2d::CCScene* NXQingjingScene::scene(){
    CCScene* scene = CCScene::create();
    NXQingjingScene* layer = NXQingjingScene::create();
    scene->addChild(layer);
    
    return scene;
}

void NXQingjingScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    
}

void NXQingjingScene::creat_view(){
    
    CCSprite* titleSpr = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_title.png");
    titleSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .84f));
    this->addChild(titleSpr);
    
    // 返回
    CCSprite* backSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_fanhui.png");
    CCSprite* backSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_fanhui.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(NXQingjingScene::backCallBack));
    backItem->setAnchorPoint(CCPointZero);
    backItem->setPosition(ccp(-7.f, 8.f));
    
    CCSprite* qjSpr1 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_kuang.png");
    CCSprite* qjSpr2 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_kuang.png");
    CCMenuItem* qjItem = CCMenuItemSprite::create(qjSpr1, qjSpr2);
    qjItem->setAnchorPoint(ccp(.5f, .5f));
    qjItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .7f));
    
    // 开启
    CCSprite* startSpr1 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_start.png");
    CCSprite* startSpr2 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_start.png");
    startSpr2->setScale(1.02f);
    CCMenuItem* startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(NXQingjingScene::qjCallback));
    startItem->setAnchorPoint(ccp(1.f, 0.f));
    startItem->setPosition(ccp(qjItem->getContentSize().width - 15.f, 20.f));
    CCMenu* startMenu = CCMenu::create(startItem, NULL);
    startMenu->setPosition(CCPointZero);
    qjItem->addChild(startMenu);
    
    // 1
    CCSprite* qjSpr1_1 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_kuang1.png");
    CCSprite* qjSpr1_2 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_kuang1.png");
    qjSpr1_2->setScale(1.02f);
    CCMenuItem* qjItem1 = CCMenuItemSprite::create(qjSpr1_1, qjSpr1_2, this, menu_selector(NXQingjingScene::qjCallback1));
    qjItem1->setAnchorPoint(ccp(.5f, .5f));
    qjItem1->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .345f));
    CCSprite* manSpr1 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_man1.png");
    manSpr1->setPosition(ccp(qjItem1->getContentSize().width* .5f, qjItem1->getContentSize().height* .5f + 3.f));
    qjItem1->addChild(manSpr1);
    CCSprite* nameSpr1 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_name1.png");
    nameSpr1->setAnchorPoint(ccp(1.f, 0.f));
    nameSpr1->setPosition(ccp(qjItem1->getContentSize().width - 16.f, 19.f));
    qjItem1->addChild(nameSpr1);
    
    // 2
    CCSprite* qjSpr2_1 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_kuang2.png");
    CCSprite* qjSpr2_2 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_kuang2.png");
    qjSpr2_2->setScale(1.02f);
    CCMenuItem* qjItem2 = CCMenuItemSprite::create(qjSpr2_1, qjSpr2_2, this, menu_selector(NXQingjingScene::qjCallback2));
    qjItem2->setAnchorPoint(ccp(.5f, .5f));
    qjItem2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .345f));
    CCSprite* manSpr2 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_man2.png");
    manSpr2->setPosition(ccp(qjItem2->getContentSize().width* .5f, qjItem2->getContentSize().height* .5f));
    qjItem2->addChild(manSpr2);
    CCSprite* nameSpr2 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_name2.png");
    nameSpr2->setAnchorPoint(ccp(1.f, 0.f));
    nameSpr2->setPosition(ccp(qjItem2->getContentSize().width - 16.f, 19.f));
    qjItem2->addChild(nameSpr2);
    
    // 3
    CCSprite* qjSpr3_1 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_kuang3.png");
    CCSprite* qjSpr3_2 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_kuang3.png");
    qjSpr3_2->setScale(1.02f);
    CCMenuItem* qjItem3 = CCMenuItemSprite::create(qjSpr3_1, qjSpr3_2, this, menu_selector(NXQingjingScene::qjCallback3));
    qjItem3->setAnchorPoint(ccp(.5f, .5f));
    qjItem3->setPosition(ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* .345f));
    CCSprite* manSpr3 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_man3.png");
    manSpr3->setPosition(ccp(qjItem3->getContentSize().width* .5f, qjItem3->getContentSize().height* .5f));
    qjItem3->addChild(manSpr3);
    CCSprite* nameSpr3 = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_name3.png");
    nameSpr3->setAnchorPoint(ccp(1.f, 0.f));
    nameSpr3->setPosition(ccp(qjItem3->getContentSize().width - 18.f, 19.f));
    qjItem3->addChild(nameSpr3);
    
    
    CCMenu* menu = CCMenu::create(backItem, qjItem, qjItem1, qjItem2, qjItem3, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
}
void NXQingjingScene::qjCallback(CCObject* pSender){
    
}
void NXQingjingScene::qjCallback1(CCObject* pSender){
    this->creat_tishi();
}
void NXQingjingScene::qjCallback2(CCObject* pSender){
    this->creat_tishi();
}
void NXQingjingScene::qjCallback3(CCObject* pSender){
    this->creat_tishi();
}
void NXQingjingScene::creat_tishi(){
    if (this->getChildByTag(0x100022) != NULL) {
        CCNode* node = this->getChildByTag(0x100022);
        node->removeAllChildren();
        this->removeChildByTag(0x100022);
    }
    
    CCSprite* tishiSpr = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_dikuang.png");
    tishiSpr->setScale(0.f);
    tishiSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .45f));
    tishiSpr->setTag(0x100022);
    this->addChild(tishiSpr, 2000);
    
    CCString* saidStr = CCString::createWithFormat("请先通关星途闪耀");
    CCLabelTTF* message = CCLabelTTF::create(saidStr->getCString(), DISPLAY->fangzhengFont(), 25);
    message->setColor(ccc3(80, 80, 80));
    message->setAnchorPoint(CCPoint(.5f, .5f));
    message->setPosition(ccp(tishiSpr->getContentSize().width* .5f, tishiSpr->getContentSize().height* .5f));
    tishiSpr->addChild(message);
    CCSprite* fuhaoSpr = CCSprite::create("res/nxpic/NXQingjingScene/NXQingjingScene_fuhao.png");
    fuhaoSpr->setAnchorPoint(ccp(1.f, .5f));
    fuhaoSpr->setPosition(ccp(-8.f, message->getContentSize().height* .5f));
    message->addChild(fuhaoSpr);
    
    CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    CCScaleTo* scaleTo = CCScaleTo::create(.5f, 1.f);
    CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
    CCFadeOut* fadeOut1 = CCFadeOut::create(1.5f);
    tishiSpr->runAction(CCSequence::create(CCDelayTime::create(.3f), spawn,CCDelayTime::create(1.f), fadeOut1, NULL));
    
    CCFadeOut* fadeOut2 = CCFadeOut::create(1.5f);
    message->runAction(CCSequence::create(CCDelayTime::create(1.8f), fadeOut2, NULL));
    
    CCFadeOut* fadeOut3 = CCFadeOut::create(1.5f);
    fuhaoSpr->runAction(CCSequence::create(CCDelayTime::create(1.8f), fadeOut3, NULL));
}


void NXQingjingScene::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    
    if (DATA->getOpenGuideBool9()) {
        DATA->setOpenGuideBool9(false);
    }
    num_child = 0;
    CCScene* scene = NXMainScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}













