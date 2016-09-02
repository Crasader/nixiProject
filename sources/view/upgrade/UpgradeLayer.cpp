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
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void UpgradeLayer::keyBackClicked(){
    
}

bool UpgradeLayer::init(){
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(250, 250, 250, 0), DISPLAY->ScreenWidth(), DISPLAY->ScreenHeight()));
        
        
        bRet = true;
    } while (0);
    
    
    return bRet;
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
    CCSprite* stencil = CCSprite::create();
    stencil->setTextureRect(CCRect(0, 0, diaocheSpr2->getContentSize().width, diaocheSpr2->getContentSize().height));
    CCClippingNode* node = CCClippingNode::create(stencil);
    node->setPosition(ccp(diaocheSpr1->getContentSize().width* .326f, diaocheSpr1->getContentSize().height* .655f));
    node->setInverted(false);
    node->addChild(diaocheSpr2);
    diaocheSpr1->addChild(node);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(1.8f, ccp(0, 255.6));
    CCMoveTo* moveTo2 = CCMoveTo::create(1.8f, ccp(0, 0));
    diaocheSpr2->runAction(CCRepeatForever::create(CCSequence::create(moveTo1, CCDelayTime::create(.4f), moveTo2, NULL)));
    
    
    CCSprite* weilanSpr1 = CCSprite::create("res/pic/taskScene/upgrade/upgrade_weilan1.png");
    weilanSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(weilanSpr1, 10);
    
    
    CCSprite* weilanSpr2 = CCSprite::create("res/pic/taskScene/upgrade/upgrade_weilan2.png");
    weilanSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(weilanSpr2, 2);
    
    
    CCSprite* paiziSpr = CCSprite::create("res/pic/taskScene/upgrade/upgrade_paizi.png");
    paiziSpr->setAnchorPoint(ccp(.5f, 1));
    paiziSpr->setPosition(ccp(weilanSpr1->getContentSize().width* .41f, weilanSpr1->getContentSize().height* .55f));
    weilanSpr1->addChild(paiziSpr);
    
    
    CCRotateTo* rotateTo1 = CCRotateTo::create(.4f, -30);
    CCRotateTo* rotateTo2 = CCRotateTo::create(.4f, 0);
    CCRotateTo* rotateTo3 = CCRotateTo::create(.4f, 30);
    CCRotateTo* rotateTo4 = CCRotateTo::create(.4f, 0);
    CCSequence* seq = CCSequence::create(rotateTo1, rotateTo2, rotateTo3, rotateTo4, NULL);
    paiziSpr->runAction(CCRepeatForever::create(seq));
    
    this->scheduleOnce(SEL_SCHEDULE(&UpgradeLayer::closeLayer), 6.f);
}
void UpgradeLayer::closeLayer(float dt){
    
}

void UpgradeLayer::creat_view2(){
    
}


























