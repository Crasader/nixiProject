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
        
//        this->setTouchSwallowEnabled(true);
//        this->setTouchMode(kCCTouchesOneByOne);
//        this->setTouchEnabled(true);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

//bool UpgradeLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    
//    return true;
//}

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
    content = CCLayer::create();
    this->addChild(content);
    content->setPosition(ccp(-DISPLAY->halfW() * 0.5, 0));
    content->runAction(CCMoveTo::create(0.5, CCPointZero));
    
    CCSprite* louSpr = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_lou.png");
    louSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    content->addChild(louSpr);
    
    CCSprite* diaocheSpr1 = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_diaoche1.png");
    diaocheSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    content->addChild(diaocheSpr1, 5);
    
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
    content->addChild(weilanSpr1, 10);
    
    CCSprite* weilanSpr2 = CCSprite::create("res/pic/taskScene/upgrade1/upgrade_weilan2.png");
    weilanSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    content->addChild(weilanSpr2, 2);
    
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
    
//    this->scheduleOnce(SEL_SCHEDULE(&UpgradeLayer::closeLayer), 6.f);
}

void UpgradeLayer::closeLayer(){
    CCSequence* seq = CCSequence::create(CCMoveTo::create(0.5, ccp(-DISPLAY->halfW() * 0.5, 0)), CCRemoveSelf::create(), NULL);
    content->runAction(seq);
}

void UpgradeLayer::creat_view2(){
    this->show_roles();
    this->creat_hua1();
    this->show_title();
}

void UpgradeLayer::show_roles() {
    CCSprite* shadow1 = CCSprite::create("res/pic/taskScene/upgrade2/shadow1.png");
    shadow1->setPosition(ccp(DISPLAY->halfW() - 140, DISPLAY->H() * 0.255));
    this->addChild(shadow1, 4);
    
    CCSprite* shadow2 = CCSprite::create("res/pic/taskScene/upgrade2/shadow2.png");
    shadow2->setPosition(ccp(DISPLAY->halfW() + 125, DISPLAY->H() * 0.255));
    this->addChild(shadow2, 4);
    
    manSpr = CCSprite::create("res/pic/taskScene/upgrade2/upgrade_man1.png");
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .4f));
    this->addChild(manSpr, 5);
    
    CCCallFunc* anim = CCCallFunc::create(this, SEL_CallFunc(&UpgradeLayer::role_animation));
    manSpr->runAction(CCSequence::create(CCFadeIn::create(1.5), CCDelayTime::create(0.3), anim, NULL));
}

void UpgradeLayer::role_animation() {
    {
        CCSprite* caidai = CCSprite::create("res/pic/taskScene/upgrade2/caidai.png");
        caidai->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.35));
        this->addChild(caidai);
    }
    
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
}

void UpgradeLayer::show_title() {
    float originScale = 0.2f;
    float duration = 0.6f;
    
    CCSprite* titleSpr = CCSprite::create("res/pic/taskScene/upgrade2/upgrade_title.png");
    titleSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .7f));
    titleSpr->setScale(originScale);
    this->addChild(titleSpr, 100);

    CCCallFunc* done = CCCallFunc::create(this, SEL_CallFunc(&UpgradeLayer::show_stars));
    CCSequence* seq = CCSequence::create(CCEaseExponentialIn::create(CCScaleTo::create(duration, 1)), done, NULL);
    titleSpr->runAction(seq);
}

void UpgradeLayer::show_stars() {
    float originScale = 0.2f;
    float duration = 0.6f;
    
    CCSprite* stars = CCSprite::create("res/pic/taskScene/upgrade2/stars.png");
    stars->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .7f));
    stars->setAnchorPoint(ccp(0.5, 0));
    stars->setScale(originScale);
    this->addChild(stars);
    
    CCCallFunc* done = CCCallFunc::create(this, SEL_CallFunc(&UpgradeLayer::show_stars_done));
    CCSequence* seq = CCSequence::create(CCEaseExponentialIn::create(CCScaleTo::create(duration, 1)), done, NULL);
    stars->runAction(seq);
}

void UpgradeLayer::show_stars_done() {
    CCSprite* balabala = CCSprite::create("res/pic/taskScene/upgrade2/hua0.png");
    balabala->setPosition(ccp(DISPLAY->halfW(), DISPLAY->halfH()));
    this->addChild(balabala);
    
    CCSprite* hoodle = CCSprite::create("res/pic/taskScene/upgrade2/hua0.png");
    hoodle->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.45));
    this->addChild(hoodle);
    
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






















