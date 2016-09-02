//
//  UpgradeLayer.cpp
//  tiegao
//
//  Created by mac on 16-9-2.
//
//

#include "UpgradeLayer.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "TaskScene.h"
#include "DisplayManager.h"



UpgradeLayer::UpgradeLayer(){
    
}
UpgradeLayer::~UpgradeLayer(){
    
}

void UpgradeLayer::onEnter(){
    CCLayer::onEnter();
    
    this->scheduleOnce(SEL_SCHEDULE(&UpgradeLayer::keyBackStatus), .8f);
}
void UpgradeLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void UpgradeLayer::onExit(){
    
    CCLayer::onExit();
}

void UpgradeLayer::keyBackClicked(){
    
}

bool UpgradeLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    
    
    return true;
}

bool UpgradeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    
    
    return true;
}

UpgradeLayer* UpgradeLayer::create_with_index(int index){
    UpgradeLayer* rtn = UpgradeLayer::create();
    rtn->init_with_index(index);
    
    return rtn;
}
void UpgradeLayer::init_with_index(int index){
    
    if (index == 1) {
        this->creat_view1();
    }else if (index == 2){
        this->creat_view2();
    }
}

void UpgradeLayer::creat_view1(){
    
    CCSprite* louSpr = CCSprite::create("res/pic/taskScene/upgrade/upgrade_lou.png");
    louSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(louSpr);
    
    CCSprite* diaocheSpr1 = CCSprite::create("res/pic/taskScene/upgrade/upgrade_diaoche1.png");
    diaocheSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(diaocheSpr1, 5);
    
    
    diaocheSpr2 = CCSprite::create("res/pic/taskScene/upgrade/upgrade_diaoche2.png");
    diaocheSpr2->setPosition(ccp(diaocheSpr1->getContentSize().width* .35f, diaocheSpr1->getContentSize().height* .7f));
    diaocheSpr1->addChild(diaocheSpr2);
    
    
    CCSprite* weilanSpr1 = CCSprite::create("res/pic/taskScene/upgrade/upgrade_weilan1.png");
    weilanSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(weilanSpr1, 10);
    
    
    CCSprite* weilanSpr2 = CCSprite::create("res/pic/taskScene/upgrade/upgrade_weilan2.png");
    weilanSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(weilanSpr2, 2);
    
    
    CCSprite* paiziSpr = CCSprite::create("res/pic/taskScene/upgrade/upgrade_paizi.png");
    paiziSpr->setAnchorPoint(ccp(.5f, 1));
    paiziSpr->setPosition(ccp(weilanSpr1->getContentSize().width* .42f, weilanSpr1->getContentSize().height* .44f));
    weilanSpr1->addChild(paiziSpr);
    
    
    CCRotateTo* rotateTo1 = CCRotateTo::create(.4f, -30);
    CCRotateTo* rotateTo2 = CCRotateTo::create(.4f, 0);
    CCRotateTo* rotateTo3 = CCRotateTo::create(.4f, 30);
    CCRotateTo* rotateTo4 = CCRotateTo::create(.4f, 0);
    CCSequence* seq = CCSequence::create(rotateTo1, rotateTo2, rotateTo3, rotateTo4, NULL);
    paiziSpr->runAction(CCRepeatForever::create(seq));
    
}
void UpgradeLayer::updateDiaoche(float dt){
    
}



void UpgradeLayer::creat_view2(){
    
}


























