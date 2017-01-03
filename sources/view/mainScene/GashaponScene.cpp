//
//  GashaponScene.cpp
//  tiegao
//
//  Created by mac on 17-1-3.
//
//

#include "GashaponScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "NetManager.h"
#include "MainScene.h"
#include "Loading2.h"
#include "GashaponLayer.h"
#include "ExchangeLayer.h"

#include "MZResourceLoader.h"
#include "AudioManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "BuildingLayer.h"
#include "TDCCAccount.h"
#include "TDCCTalkingDataGA.h"
#include "JNIController.h"


GashaponScene::GashaponScene(){
    
}
GashaponScene::~GashaponScene(){
    
}
GashaponScene* GashaponScene::create(){
    GashaponScene* rtn = new GashaponScene();
    if (rtn && rtn->init()) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}
bool GashaponScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    num_child = 0;
    
    CCSprite* bgSpr = CCSprite::create("res/pic/gashapon/gashapon_bg2.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    this->creat_view();
    
    return true;
}

void GashaponScene::onEnter(){
    BaseScene::onEnter();
    BaseScene::openChat();
    
    AUDIO->play_company_bgm();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&GashaponScene::backCallBack), "GashaponBack", NULL);
    nc->addObserver(this, SEL_CallFuncO(&GashaponScene::creat_Exchange), "Creat_Exchange", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&GashaponScene::keyBackStatus), .8f);
}

void GashaponScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void GashaponScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    BaseScene::onExit();
}

void GashaponScene::keyBackClicked(){
    num_child++;
//    CCLog("===== GashaponScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    if (DATA->current_guide_step() == 0) {
        this->backCallBack(NULL);
    }
}
void GashaponScene::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    num_child = 0;
    CCScene* scene = MainScene::scene();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void GashaponScene::creat_view(){
    
    GashaponLayer* layer = GashaponLayer::create();
    this->addChild(layer);
}
void GashaponScene::creat_Exchange(){
    ExchangeLayer* layer = ExchangeLayer::create();
    this->addChild(layer);
}





