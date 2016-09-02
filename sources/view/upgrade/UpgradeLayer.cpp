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
    this->addChild(diaocheSpr1, 1);
    
    diaocheSpr2 = CCSprite::create("res/pic/taskScene/upgrade/upgrade_diaoche2.png");
    diaocheSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(diaocheSpr2, 2);
    diaocheFolat = 1.f;
    diaochePositionX = diaocheSpr2->getPosition().x;
    diaochePositionY = diaocheSpr2->getPosition().y;
    
    diaocheSpr3 = CCSprite::create("res/pic/taskScene/upgrade/upgrade_diaoche3.png");
    diaocheSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(diaocheSpr3);
    
    CCSprite* weilanSpr = CCSprite::create("res/pic/taskScene/upgrade/upgrade_weilan.png");
    weilanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(weilanSpr, 2);
    
    CCSprite* paiziSpr = CCSprite::create("res/pic/taskScene/upgrade/upgrade_paizi.png");
    paiziSpr->setAnchorPoint(ccp(.5f, 1));
    paiziSpr->setPosition(ccp(weilanSpr->getContentSize().width* .42f, weilanSpr->getContentSize().height* .44f));
    weilanSpr->addChild(paiziSpr, 3);
    
    CCRotateTo* rotateTo1 = CCRotateTo::create(.4f, -30);
    CCRotateTo* rotateTo2 = CCRotateTo::create(.4f, 0);
    CCRotateTo* rotateTo3 = CCRotateTo::create(.4f, 30);
    CCRotateTo* rotateTo4 = CCRotateTo::create(.4f, 0);
    CCSequence* seq = CCSequence::create(rotateTo1, rotateTo2, rotateTo3, rotateTo4, NULL);
    paiziSpr->runAction(CCRepeatForever::create(seq));
    
    this->schedule(SEL_SCHEDULE(&UpgradeLayer::updateDiaoche), .2f);
}
void UpgradeLayer::updateDiaoche(float dt){
    if (diaocheFolat == 2.5f) {
        diaocheBool = true;
    }else if (diaocheFolat == 1.f) {
        diaocheBool = false;
    }
    
    if (diaocheBool) {
        diaocheFolat -= .1f;
        diaocheSpr2->setScaleY(diaocheFolat);
        diaochePositionY += diaocheSpr2->getContentSize().height* .1f;
        diaocheSpr2->runAction(CCMoveTo::create(.1f, ccp(diaochePositionX, diaochePositionY)));
    }else{
        diaocheFolat += .1f;
        diaocheSpr2->setScaleY(diaocheFolat);
        diaochePositionY -= diaocheSpr2->getContentSize().height* .1f;
        diaocheSpr2->runAction(CCMoveTo::create(.1f, ccp(diaochePositionX, diaochePositionY)));
    }
    
    
    
}



void UpgradeLayer::creat_view2(){
    
}


























