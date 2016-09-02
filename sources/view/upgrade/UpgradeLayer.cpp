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
        
        this->setTouchSwallowEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchEnabled(true);
        
        
        bRet = true;
    } while (0);
    
    
    return bRet;
}

bool UpgradeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    this->removeFromParentAndCleanup(true);
    
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
    
    CCSprite* louSpr = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_lou.png");
    louSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(louSpr);
    
    CCSprite* diaocheSpr1 = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_diaoche1.png");
    diaocheSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(diaocheSpr1, 5);
    
    
    diaocheSpr2 = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_diaoche2.png");
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
    
    
    CCSprite* weilanSpr1 = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_weilan1.png");
    weilanSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(weilanSpr1, 10);
    
    
    CCSprite* weilanSpr2 = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_weilan2.png");
    weilanSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(weilanSpr2, 2);
    
    
    CCSprite* paiziSpr = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_paizi.png");
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
    CCSprite* bgSpr = CCSprite::create("res/pic/mask.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    CCSprite* titleSpr = CCSprite::create("res/pic/taskScene/upgrade2/upgrade_title.png");
    titleSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .7f));
    this->addChild(titleSpr);
    
    // 人
    manSpr = CCSprite::create("res/pic/taskScene/upgrade2/upgrade_man1.png");
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .4f));
    this->addChild(manSpr, 5);
    CCArray* manAnimations = CCArray::createWithCapacity(3);
    char manStr[100] = {};
    for (int i = 1; i <= 2; i++) {
        
        sprintf(manStr, "res/pic/taskScene/upgrade2/upgrade_man%d.png", i);
        CCSpriteFrame* manFrame = CCSpriteFrame::create(manStr, CCRectMake(0, 0, 466, 352));
        manAnimations->addObject(manFrame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(manAnimations, .5f);
    CCAnimate* animate = CCAnimate::create(animation);
    CCRepeatForever* rep = CCRepeatForever::create(animate);
    CCDirector::sharedDirector()->getActionManager()->addAction(rep, manSpr, false);
    
    this->creat_hua1();
}

void UpgradeLayer::creat_hua1(){
    if (manSpr->getChildByTag(0x99999) != NULL) {
        manSpr->removeChildByTag(0x99999);
    }
    // 花
    huaSpr = CCSprite::create("res/pic/taskScene/upgrade2/upgrade_hua1.png");
    huaSpr->setPosition(ccp(manSpr->getContentSize().width* .35f, manSpr->getContentSize().height* .49f));
    huaSpr->setTag(0x99999);
    manSpr->addChild(huaSpr);
    CCMoveTo* moveTo = CCMoveTo::create(.3f, ccp(manSpr->getContentSize().width* .45f, manSpr->getContentSize().height* .9f));
    CCCallFuncN* callFunc = CCCallFuncN::create(this, callfuncN_selector(UpgradeLayer::creat_hua2));
    huaSpr->runAction(CCSequence::create(moveTo, callFunc, NULL));
}
void UpgradeLayer::creat_hua2(){
    if (manSpr->getChildByTag(0x99999) != NULL) {
        manSpr->removeChildByTag(0x99999);
    }
    huaSpr = CCSprite::create("res/pic/taskScene/upgrade2/upgrade_hua2.png");
    huaSpr->setPosition(ccp(manSpr->getContentSize().width* .45f, manSpr->getContentSize().height* .91f));
    huaSpr->setTag(0x99999);
    manSpr->addChild(huaSpr);
    CCMoveTo* moveTo = CCMoveTo::create(.2f, ccp(manSpr->getContentSize().width* .5f, manSpr->getContentSize().height* 1.55f));
    CCCallFuncN* callFunc = CCCallFuncN::create(this, callfuncN_selector(UpgradeLayer::creat_hua3));
    huaSpr->runAction(CCSequence::create(moveTo, callFunc, NULL));
}
void UpgradeLayer::creat_hua3(){
    if (manSpr->getChildByTag(0x99999) != NULL) {
        manSpr->removeChildByTag(0x99999);
    }
    huaSpr = CCSprite::create("res/pic/taskScene/upgrade2/upgrade_hua3.png");
    huaSpr->setPosition(ccp(manSpr->getContentSize().width* .5f, manSpr->getContentSize().height* 1.56f));
    huaSpr->setTag(0x99999);
    manSpr->addChild(huaSpr);
    CCCallFuncN* callFunc = CCCallFuncN::create(this, callfuncN_selector(UpgradeLayer::creat_hua4));
    huaSpr->runAction(CCSequence::create(CCDelayTime::create(.2f), callFunc, NULL));
}
void UpgradeLayer::creat_hua4(){
    if (manSpr->getChildByTag(0x99999) != NULL) {
        manSpr->removeChildByTag(0x99999);
    }
    
    this->scheduleOnce(SEL_SCHEDULE(&UpgradeLayer::creat_hua1), .3f);
}






















