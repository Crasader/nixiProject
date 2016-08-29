//
//  GuideLayer.cpp
//  tiegao
//
//  Created by mac on 16-8-24.
//
//

#include "GuideLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "MainScene.h"


GuideLayer::GuideLayer(){
    
}
GuideLayer::~GuideLayer(){
    
}

bool GuideLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
//    m_pTarget = CCRenderTexture::create(DISPLAY->ScreenWidth(), DISPLAY->ScreenHeight(), kCCTexture2DPixelFormat_RGBA8888);
//    m_pTarget->setPosition(ccp(DISPLAY->ScreenWidth() * .5f, DISPLAY->ScreenHeight() * .5f));
//    m_pTarget->setOpacity(155);
//    this->addChild(m_pTarget);
//    this->setHighlightRect(CCRectMake(100, 100, 0, 0));// x,y,w,h
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    wordCount = 0;
    guideIndex = 0;
    touchBool = false;
    
    guideSpr = CCSprite::create();
    this->addChild(guideSpr, 10);
    
    this->creatAnimation();
    
    
    return true;
}
CCScene* GuideLayer::scene(){
    CCScene* scene = CCScene::create();
    GuideLayer* layer = GuideLayer::create();
    scene->addChild(layer);
    
    return scene;
}

void GuideLayer::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&GuideLayer::guideQuan), "GuideQuan", NULL);
    nc->addObserver(this, SEL_CallFuncO(&GuideLayer::closeSwallowEnabled), "CloseSwallowEnabled", NULL);
    nc->addObserver(this, SEL_CallFuncO(&GuideLayer::_905CallBack), "HTTP_FINISHED_905", NULL);
    
}
void GuideLayer::_905CallBack(CCObject* pSender){
    LOADING->remove();
    if (DATA->current_guide_step() == 6) {
        CCScene* scene = MainScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else if (DATA->current_guide_step() == 7){
        CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideBackCallBack");
    }else{
        this->removeFromParentAndCleanup(true);
    }
}
void GuideLayer::onExit(){
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void GuideLayer::setHighlightRect(const CCRect& rect)
{
    CCSprite* pMask = CCSprite::create("res/pic/guide/guide_kuang1.png");
    pMask->setPosition(ccp(rect.origin.x, rect.origin.y));
    pMask->setAnchorPoint(ccp(0,0));
    pMask->setScaleX(rect.size.width / pMask->getContentSize().width);
    pMask->setScaleY(rect.size.height / pMask->getContentSize().height);
    
    ccBlendFunc blend;
    blend.src = GL_ZERO;
    blend.dst = GL_ONE_MINUS_SRC_ALPHA;
    pMask->setBlendFunc(blend);
    
    m_pTarget->clear(0, 0, 0, .5f);
    m_pTarget->begin();
    pMask->visit();
    m_pTarget->end();
}


GuideLayer* GuideLayer::create_with_guide(int _index){
    GuideLayer* rtn = GuideLayer::create();
    rtn->init_with_guide(_index);
    
    return rtn;
}
void GuideLayer::init_with_guide(int _index){
    guideIndex = _index;
    
    if (guideIndex == 1) {
        if (!DATA->_guideBool1[0]) {
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]){
            DATA->_guideBool1[2] = true;
            this->creatShou();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
    }else if (guideIndex == 2){
        if (!DATA->_guideBool2[0]) {
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }
    }else if (guideIndex == 4){
        if (!DATA->_guideBool4[0]) {
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }else if (DATA->_guideBool4[6] && !DATA->_guideBool4[7]){
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 2.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 2.f);
        }
    }else if (guideIndex == 5){
        if (!DATA->_guideBool5[0]) {
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }else if (DATA->_guideBool5[0] && !DATA->_guideBool5[1]){
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }
    }else if (guideIndex == 6){
        if (!DATA->_guideBool6[0]) {
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }else if (DATA->_guideBool6[1] && !DATA->_guideBool6[2]){
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }
    }else if (guideIndex == 7){
        if (!DATA->_guideBool7[0]) {
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }else if (DATA->_guideBool7[1] && !DATA->_guideBool7[2]){
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatLabel), 1.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), 1.f);
        }
    }
    
    else if (guideIndex == 100){
        this->creatShou();
        this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
    }
    
    
}

bool GuideLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    if (touchBool) {
        touchBool = false;
        
        if (guideIndex == 1) {
            if (!DATA->_guideBool1[0]) {
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool1[0] && !DATA->_guideBool1[1]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool1[2] && !DATA->_guideBool1[3]){
                if (isContainTouchPoint1_1(pTouch)) {
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideRichangMethods");
                    this->setTouchSwallowEnabled(true);
                }else {
                    this->openTouch(0);
                    this->setTouchSwallowEnabled(true);
                }
            }
        }else if (guideIndex == 2){
            if (!DATA->_guideBool2[0]) {
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool2[0] && !DATA->_guideBool2[1]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool2[1] && !DATA->_guideBool2[2]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool2[2] && !DATA->_guideBool2[3]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool2[3] && !DATA->_guideBool2[4]){
                if (isContainTouchPoint2_1(pTouch)) {
                    this->setTouchSwallowEnabled(false);
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }else {
                    this->openTouch(0);
                    this->setTouchSwallowEnabled(true);
                }
            }else if (DATA->_guideBool2[4] && !DATA->_guideBool2[5]){
                if (isContainTouchPoint2_1(pTouch)) {
                    if (this->getChildByTag(0x77777) != NULL) {
                        this->removeChildByTag(0x77777);
                    }
                    if (guideSpr) {
                        guideSpr->removeAllChildren();
                    }
                    this->setTouchSwallowEnabled(false);
                }else {
                    this->openTouch(0);
                    this->setTouchSwallowEnabled(true);
                }
            }
        }else if (guideIndex == 4) {
            if (!DATA->_guideBool4[0]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool4[0] && !DATA->_guideBool4[1]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    if (isContainTouchPoint4_1(pTouch)) {
                        this->setTouchSwallowEnabled(true);
                        if (this->getChildByTag(0x77777) != NULL) {
                            this->removeChildByTag(0x77777);
                        }
                        this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                        CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideRenwukuangCallBack");
                        
                    }else {
                        this->openTouch(0);
                        this->setTouchSwallowEnabled(true);
                    }
                }
            }else if (DATA->_guideBool4[1] && !DATA->_guideBool4[2]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    if (isContainTouchPoint4_2(pTouch)) {
                        this->setTouchSwallowEnabled(false);
                        if (this->getChildByTag(0x77777) != NULL) {
                            this->removeChildByTag(0x77777);
                        }
                        this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                    }else {
                        this->openTouch(0);
                        this->setTouchSwallowEnabled(true);
                    }
                }
            }else if (DATA->_guideBool4[2] && !DATA->_guideBool4[3]){
                if (isContainTouchPoint4_3(pTouch)) {
                    this->setTouchSwallowEnabled(true);
                    if (this->getChildByTag(0x77777) != NULL) {
                        this->removeChildByTag(0x77777);
                    }
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideButtonCallBack");
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }else {
                    this->openTouch(0);
                    this->setTouchSwallowEnabled(true);
                }
            }else if (DATA->_guideBool4[3] && !DATA->_guideBool4[4]){
                if (isContainTouchPoint4_2(pTouch)) {
                    this->setTouchSwallowEnabled(false);
                    if (this->getChildByTag(0x77777) != NULL) {
                        this->removeChildByTag(0x77777);
                    }
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }else {
                    this->openTouch(0);
                    this->setTouchSwallowEnabled(true);
                }
            }else if (DATA->_guideBool4[4] && !DATA->_guideBool4[5]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool4[5] && !DATA->_guideBool4[6]){
                if (isContainTouchPoint4_4(pTouch)) {
                    this->setTouchSwallowEnabled(true);
                    DATA->_guideBool4[6] = true;
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideStartCallBack");
                }else {
                    this->openTouch(0);
                    this->setTouchSwallowEnabled(true);
                }
            }else if (DATA->_guideBool4[6] && !DATA->_guideBool4[7]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }
        }else if (guideIndex == 5){
            if (!DATA->_guideBool5[0]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    if (isContainTouchPoint5_1(pTouch)) {
                        this->setTouchSwallowEnabled(false);
                        DATA->_guideBool5[0] = true;
                        this->removeFromParentAndCleanup(true);
                    }else {
                        this->openTouch(0);
                        this->setTouchSwallowEnabled(true);
                    }
                }
            }else if (DATA->_guideBool5[0] && !DATA->_guideBool5[1]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    if (isContainTouchPoint5_2(pTouch)) {
                        if (guideSpr) {
                            guideSpr->removeAllChildren();
                        }
                        
                        CCSprite* _panel = CCSprite::create("pic/building/reward/panel.png");
                        _panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75));
                        _panel->setTag(0x22222);
                        this->addChild(_panel);
                        
                        CCLabelTTF* label = CCLabelTTF::create("点击任意处关闭", DISPLAY->fangzhengFont(), 25);
                        label->setPosition(ccp(_panel->getContentSize().width* .75f, 17));
                        label->setColor(ccWHITE);
                        _panel->addChild(label);
                        
                        CCSize panelSize = _panel->boundingBox().size;
                        
                        CCSprite* light = CCSprite::create("pic/building/reward/light.png");
                        light->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
                        _panel->addChild(light);
                        
                        CCSprite* spots = CCSprite::create("pic/building/reward/spots.png");
                        spots->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
                        _panel->addChild(spots);
                        
                        CCSequence* seq = CCSequence::create(CCFadeIn::create(0.6), CCDelayTime::create(0.3), CCFadeOut::create(0.6), CCDelayTime::create(0.5), NULL);
                        spots->runAction(CCRepeatForever::create(seq));
                        
                        CCSprite* title = CCSprite::create("pic/building/reward/title.png");
                        title->setPosition(ccp(panelSize.width* .5f, panelSize.height* .88f));
                        _panel->addChild(title);
                        
                        CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
                        plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
                        _panel->addChild(plate);
                        
                        CCSprite* coin = CCSprite::create("pic/common/coin2.png");
                        coin->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
                        _panel->addChild(coin);
                        
                        CoffersComp* coffers = DATA->getCoffers();
                        CCString* str = CCString::createWithFormat("%d 金币", coffers->profit);
                        CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24);
                        lbl->setColor(ccc3(107, 143, 190));
                        lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.28));
                        _panel->addChild(lbl);
                        
                        _panel->setScale(0.1);
                        _panel->setVisible(true);
                        
                        float duration1 = 0.2f;
                        CCMoveTo* moveto1 = CCMoveTo::create(duration1, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.45));
                        CCScaleTo* scaleto1 = CCScaleTo::create(duration1, 1.1);
                        CCSpawn* spawn1 = CCSpawn::create(moveto1, scaleto1, NULL);
                        
                        float duration2 = 0.2f;
                        CCMoveTo* moveto2 = CCMoveTo::create(duration2, DISPLAY->center());
                        CCScaleTo* scaleto2 = CCScaleTo::create(duration2, 1);
                        CCSpawn* spawn2 = CCSpawn::create(moveto2, scaleto2, NULL);
                        
                        _panel->runAction(CCEaseSineIn::create(CCSequence::create(spawn1, spawn2, NULL)));
                        
                        this->setTouchSwallowEnabled(false);
                        
                        if (this->getChildByTag(0x77777) != NULL) {
                            this->removeChildByTag(0x77777);
                        }
                        this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                    }else {
                        this->openTouch(0);
                        this->setTouchSwallowEnabled(true);
                    }
                }
            }else if (DATA->_guideBool5[1] && !DATA->_guideBool5[2]){
                LOADING->show_loading();
                DATA->getPlayer()->setGuide(6);
                NET->update_guide_905(DATA->getPlayer()->getGuide());
            }
        }else if (guideIndex == 6){
            if (!DATA->_guideBool6[0]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    if (isContainTouchPoint6_1(pTouch)) {
                        this->setTouchSwallowEnabled(false);
                        if (this->getChildByTag(0x77777) != NULL) {
                            this->removeChildByTag(0x77777);
                        }
                        this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                    }else {
                        this->openTouch(0);
                        this->setTouchSwallowEnabled(true);
                    }
                }
            }else if (DATA->_guideBool6[0] && !DATA->_guideBool6[1]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    if (isContainTouchPoint6_2(pTouch)) {
                        DATA->_guideBool6[1] = true;
                        this->setTouchSwallowEnabled(false);
                    }else {
                        this->openTouch(0);
                        this->setTouchSwallowEnabled(true);
                    }
                }
            }else if (DATA->_guideBool6[1] && !DATA->_guideBool6[2]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    if (isContainTouchPoint4_2(pTouch)) {
                        this->setTouchSwallowEnabled(false);
                        this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                    }else {
                        this->openTouch(0);
                        this->setTouchSwallowEnabled(true);
                    }
                }
            }else if (DATA->_guideBool6[2] && !DATA->_guideBool6[3]){
                if (isContainTouchPoint6_3(pTouch)) {
                    this->setTouchSwallowEnabled(false);
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }else {
                    this->openTouch(0);
                    this->setTouchSwallowEnabled(true);
                }
            }else if (DATA->_guideBool6[3] && !DATA->_guideBool6[4]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool6[4] && !DATA->_guideBool6[5]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->openTouch(0);
                }
            }else if (DATA->_guideBool6[5] && !DATA->_guideBool6[6]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
            }else if (DATA->_guideBool6[6] && !DATA->_guideBool6[7]){
                this->openTouch(0);
                this->setTouchSwallowEnabled(false);
                this->unschedule(SEL_SCHEDULE(&GuideLayer::guideTime));
                this->schedule(SEL_SCHEDULE(&GuideLayer::guideTime), 3.f);
            }
        }else if (guideIndex == 7){
            if (!DATA->_guideBool7[0]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::getIndex), .5f);
                }
            }else if (DATA->_guideBool7[0] && !DATA->_guideBool7[1]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    if (isContainTouchPoint1_1(pTouch)) {
                        DATA->_guideBool7[1] = true;
                        CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideRichangMethods");
                        this->setTouchSwallowEnabled(true);
                    }else {
                        this->openTouch(0);
                        this->setTouchSwallowEnabled(true);
                    }
                }
            }else if (DATA->_guideBool7[1] && !DATA->_guideBool7[2]){
                if (wordCount < contentLength) {
                    wordCount = getContentLength();
                }else {
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("Guide_905status");
                    LOADING->show_loading();
                    DATA->getPlayer()->setGuide(8);
                    NET->update_guide_905(DATA->getPlayer()->getGuide());
                    this->setTouchSwallowEnabled(true);
                }
            }
            
        }
        
        else if (guideIndex == 100){
            if (isContainTouchPoint1_1(pTouch)) {
                CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideRichangMethods");
                this->setTouchSwallowEnabled(true);
            }else {
                this->openTouch(0);
                this->setTouchSwallowEnabled(true);
            }
        }
        
        
    }else{
        this->setTouchSwallowEnabled(true);
    }
    
    return true;
}

void GuideLayer::openTouch(float dt){
    touchBool = true;
}

void GuideLayer::creatView(){
    saidLabel = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 22,CCSizeMake(dkSpr->getContentSize().width* .85f, dkSpr->getContentSize().height* .75f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    saidLabel->setPosition(ccp(dkSpr->getContentSize().width* .5f, dkSpr->getContentSize().height* .41f));
    saidLabel->setColor(ccc3(135, 117, 114));
    saidLabel->setTag(0x88888);
    dkSpr->addChild(saidLabel, 2);
    
    this->schedule(schedule_selector(GuideLayer::logic), .5f);
}
void GuideLayer::creatAnimation(){
    CCArray* arrowAnimations = CCArray::createWithCapacity(8);
    char arrowStr[100] = {};
    for (int i = 9; i <= 13; i++) {
        
        sprintf(arrowStr, "res/pic/guide/quan/guide_quan%d.png", i);
        CCSpriteFrame* arrowFrame = CCSpriteFrame::create(arrowStr, CCRectMake(0, 0, 129, 129));
        arrowAnimations->addObject(arrowFrame);
    }
    CCAnimation* arrowAnimation = CCAnimation::createWithSpriteFrames(arrowAnimations, .2f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation, "quanStr");
}

void GuideLayer::openAnimation(){
    
}
void GuideLayer::closeAnimation(){
    
}

void GuideLayer::getIndex(float dt){
    if (guideIndex == 1) {
        if (!DATA->_guideBool1[0]) {
            DATA->_guideBool1[0] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool1[0] && !DATA->_guideBool1[1]){
            DATA->_guideBool1[1] = true;
            if (guideSpr) {
                guideSpr->removeAllChildren();
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]){
            DATA->_guideBool1[2] = true;
            if (guideSpr) {
                guideSpr->removeAllChildren();
            }
            
            this->creatShou();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
    }else if (guideIndex == 2){
        if (!DATA->_guideBool2[0]) {
            DATA->_guideBool2[0] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool2[0] && !DATA->_guideBool2[1]){
            DATA->_guideBool2[1] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool2[1] && !DATA->_guideBool2[2]){
            DATA->_guideBool2[2] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool2[2] && !DATA->_guideBool2[3]){
            DATA->_guideBool2[3] = true;
            this->creatShou();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool2[3] && !DATA->_guideBool2[4]){
            DATA->_guideBool2[4] = true;
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
    }else if (guideIndex == 4){
        if (!DATA->_guideBool4[0]){
            DATA->_guideBool4[0] = true;
            if (guideSpr) {
                guideSpr->removeAllChildren();
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool4[0] && !DATA->_guideBool4[1]){
            DATA->_guideBool4[1] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool4[1] && !DATA->_guideBool4[2]){
            DATA->_guideBool4[2] = true;
            if (guideSpr) {
                guideSpr->removeAllChildren();
            }
            if (this->getChildByTag(0x77777) != NULL) {
                this->removeChildByTag(0x77777);
            }
            this->creatShou();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool4[2] && !DATA->_guideBool4[3]){
            DATA->_guideBool4[3] = true;
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatClothes4_2_1), 1.f);
//            this->creatShou();
//            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool4[3] && !DATA->_guideBool4[4]){
            DATA->_guideBool4[4] = true;
            if (this->getChildByTag(0x77777) != NULL) {
                this->removeChildByTag(0x77777);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool4[4] && !DATA->_guideBool4[5]){
            DATA->_guideBool4[5] = true;
            this->creatShou();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool4[6] && !DATA->_guideBool4[7]){
            if (DATA->getPlayer()->getGuide() == 4) {
                DATA->getPlayer()->setGuide(5);
            }
            
            LOADING->show_loading();
            NET->update_guide_905(DATA->getPlayer()->getGuide());
        }
    }else if (guideIndex == 5){
        if (DATA->_guideBool5[0] && !DATA->_guideBool5[1]) {
            DATA->_guideBool5[1] = true;
            
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
    }else if (guideIndex == 6){
        if (!DATA->_guideBool6[0]) {
            DATA->_guideBool6[0] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool6[1] && !DATA->_guideBool6[2]){
            if (this->getChildByTag(0x77777) != NULL) {
                this->removeChildByTag(0x77777);
            }
            DATA->_guideBool6[2] = true;
            this->creatShou();
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool6[2] && !DATA->_guideBool6[3]){
            if (this->getChildByTag(0x77777) != NULL) {
                this->removeChildByTag(0x77777);
            }
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            DATA->_guideBool6[3] = true;
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool6[3] && !DATA->_guideBool6[4]){
            DATA->_guideBool6[4] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }else if (DATA->_guideBool6[5] && !DATA->_guideBool6[6]){
            if (this->getChildByTag(0x22222) != NULL) {
                this->removeChildByTag(0x22222);
            }
            if (guideSpr) {
                guideSpr->removeAllChildren();
            }
            DATA->_guideBool6[6] = true;
            this->schedule(SEL_SCHEDULE(&GuideLayer::guideTime), 3.f);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
    }else if (guideIndex == 7){
        if (!DATA->_guideBool7[0]){
            DATA->_guideBool7[0] = true;
            if (dkSpr) {
                dkSpr->removeChildByTag(0x88888);
            }
            this->creatLabel(1);
            this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
        }
        
    }
    
    
    
}
void GuideLayer::creatShou(){
    CCSprite* quanSpr = CCSprite::create("res/pic/guide/quan/guide_quan1.png");
    if (guideIndex == 1) {
        if (DATA->_guideBool1[2] && !DATA->_guideBool1[3]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .15f, DISPLAY->ScreenHeight()* .65f));
        }
    }else if (guideIndex == 2){
        if (DATA->_guideBool2[3] && !DATA->_guideBool2[4]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth() - quanSpr->getContentSize().width* .7f, DISPLAY->ScreenHeight()* .79f));
        }
    }else if (guideIndex == 4){
        if (DATA->_guideBool4[0] && !DATA->_guideBool4[1]){
            quanSpr->setPosition(ccp(160, DISPLAY->ScreenHeight()* .9f));
        }else if (DATA->_guideBool4[1] && !DATA->_guideBool4[2]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth() - quanSpr->getContentSize().width* .7f, DISPLAY->ScreenHeight()* .79f));
        }else if (DATA->_guideBool4[2] && !DATA->_guideBool4[3]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 192, DISPLAY->ScreenHeight()* .45f));
        }else if (DATA->_guideBool4[3] && !DATA->_guideBool4[4]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth() - quanSpr->getContentSize().width* .7f, DISPLAY->ScreenHeight()* .79f));
        }else if (DATA->_guideBool4[5] && !DATA->_guideBool4[6]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 195, 50));
        }
    }else if (guideIndex == 5){
        if (!DATA->_guideBool5[0]){
            quanSpr->setPosition(ccp(100, DISPLAY->ScreenHeight()* .7f));
        }else if (DATA->_guideBool5[0] && !DATA->_guideBool5[1]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 235, DISPLAY->ScreenHeight()* .11f));
        }
    }else if (guideIndex == 6){
        if (!DATA->_guideBool6[0]) {
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth()- 30, DISPLAY->ScreenHeight()* .88f));
        }else if (DATA->_guideBool6[0] && !DATA->_guideBool6[1]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth()- 20, DISPLAY->ScreenHeight()* .79f));
        }else if (DATA->_guideBool6[1] && !DATA->_guideBool6[2]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth() - quanSpr->getContentSize().width* .7f, DISPLAY->ScreenHeight()* .79f));
        }else if (DATA->_guideBool6[2] && !DATA->_guideBool6[3]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth() - quanSpr->getContentSize().width* .3f, DISPLAY->ScreenHeight()* .03f));
        }
    }else if (guideIndex == 7){
        if (DATA->_guideBool7[0] && !DATA->_guideBool7[1]){
            quanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .15f, DISPLAY->ScreenHeight()* .65f));
        }
    }
    
    
    else if (guideIndex == 100){
        quanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .15f, DISPLAY->ScreenHeight()* .65f));
    }
    
    
    
    quanSpr->setTag(0x77777);
    this->addChild(quanSpr);
    CCAnimation* quanAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("quanStr");
    CCAnimate* quanAnimate = CCAnimate::create(quanAnimation);
    quanSpr->runAction(CCRepeatForever::create(quanAnimate));
    
    CCSprite* arrowSpr = CCSprite::create("res/pic/guide/guide_jiantou1.png");
    arrowSpr->setAnchorPoint(ccp(0, 1));
    arrowSpr->setPosition(ccp(quanSpr->getContentSize().width* .3f, quanSpr->getContentSize().height* .62f));
    if (guideIndex == 4){
        if (DATA->_guideBool4[5] && !DATA->_guideBool4[6]){
            quanSpr->setRotation(-90);
        }
    }else if (guideIndex == 5){
        if (!DATA->_guideBool5[0]){
            quanSpr->setRotation(-90);
        }else if (DATA->_guideBool5[0] && !DATA->_guideBool5[1]){
            quanSpr->setRotation(-90);
        }
        
        
    }
    quanSpr->addChild(arrowSpr);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(.2f, ccp(quanSpr->getContentSize().width* .45f, quanSpr->getContentSize().height* .48f));
    CCMoveTo* moveTo2 = CCMoveTo::create(.2f, ccp(quanSpr->getContentSize().width* .3f, quanSpr->getContentSize().height* .62f));
    arrowSpr->runAction(CCRepeatForever::create(CCSequence::create(moveTo1, moveTo2, CCDelayTime::create(.6f), NULL)));
}

void GuideLayer::creatLabel(float dt){
    CCString* str;
    wordCount = 0;
    if (guideIndex == 1) {
        if (!DATA->_guideBool1[0]) {
            this->creatDiKuang(1);
            str = CCString::createWithFormat("欢迎(%s)来到女总的世界.\n我是剧里的后期编辑小妮.", DATA->getShow()->nickname());
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool1[0] && !DATA->_guideBool1[1]){
            str = CCString::createWithFormat("剧里各种飘字就是我干的.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]){
            this->creatDiKuang(4);
            str = CCString::createWithFormat("开始游戏的第一步,让我们建立一家公司吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }
    }else if (guideIndex == 2){
        if (!DATA->_guideBool2[0]) {
            this->creatDiKuang(6);
            str = CCString::createWithFormat("右侧的列表,就是云总的日常事务了.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool2[0] && !DATA->_guideBool2[1]){
            str = CCString::createWithFormat("每个任务最高可获得5颗星星.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool2[1] && !DATA->_guideBool2[2]){
            str = CCString::createWithFormat("第一个公司阶段累计达到88颗星星,诗彤的公司就可以升级.\n可以去更广阔的空间发展.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool2[2] && !DATA->_guideBool2[3]){
            str = CCString::createWithFormat("那么先让我们去注册第一间公司吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }
    }else if (guideIndex == 4){
        if (!DATA->_guideBool4[0]) {
            this->creatDiKuang(3);
            str = CCString::createWithFormat("好多衣服~!让我们女总穿什么去才好呢?");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool4[0] && !DATA->_guideBool4[1]){
            this->creatDiKuang(4);
            str = CCString::createWithFormat("这里可以筛选出任务相关的衣服哦!");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool4[1] && !DATA->_guideBool4[2]){
            str = CCString::createWithFormat("选择符合的衣服,不至于让我们挑花眼呦~!");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool4[4] && !DATA->_guideBool4[5]){
            this->creatDiKuang(6);
            str = CCString::createWithFormat("嗯嗯,不错.这两件衣服都是适合职场的衣服.\n日常任务一定会得高分的呢~!");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool4[6] && !DATA->_guideBool4[7]){
            dkSpr = CCSprite::create("res/pic/guide/guide_kuang3.png");
            dkSpr->setAnchorPoint(CCPointZero);
            dkSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .19f + 100, DISPLAY->ScreenHeight()* .23f));
            dkSpr->setVisible(false);
            guideSpr->addChild(dkSpr, 4);
            
            CCLabelTTF* dkLabel = CCLabelTTF::create("新手引导", DISPLAY->fangzhengFont(), 28);
            dkLabel->setPosition(ccp(dkSpr->getContentSize().width* .5f, dkSpr->getContentSize().height* .89f));
            dkLabel->setColor(ccc3(168, 102, 103));
            dkLabel->setVisible(false);
            dkSpr->addChild(dkLabel);
            
            dkSpr->runAction(CCSequence::create(CCDelayTime::create(.2f), CCShow::create(), NULL));
            dkLabel->runAction(CCSequence::create(CCDelayTime::create(.6f), CCShow::create(), NULL));
            
            str = CCString::createWithFormat("不错不错~!公司的第一步完成的不错.\n每次日常任务,会按照穿着的评级获得金币.\n固定返还体力点呦~!");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }
    }else if (guideIndex == 5){
        if (!DATA->_guideBool5[0]) {
            this->creatDiKuang(3);
            str = CCString::createWithFormat("公司正式成立了.让我们进入公司内部去参观一下吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool5[0] && !DATA->_guideBool5[1]){
            this->creatDiKuang(3);
            str = CCString::createWithFormat("公司的正常运转,会定期有收入啊.需要您进来领取.会获得金币呦~!");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }
    }else if (guideIndex == 6){
        if (!DATA->_guideBool6[0]){
            this->creatDiKuang(1);
            str = CCString::createWithFormat("哈哈!辣么多衣服,你说你都想穿出来看~!\n那我们就去服装店,看看吧~!");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool6[0] && !DATA->_guideBool6[1]){
            str = CCString::createWithFormat("这里也可以进入服装店哦~!");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool6[1] && !DATA->_guideBool6[2]){
            this->creatDiKuang(1);
            str = CCString::createWithFormat("妮妮觉得,这件粉红记忆不错.我们买下来吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool6[3] && !DATA->_guideBool6[4]){
            str = CCString::createWithFormat("钻石累的衣服,让女总处理日常事务,更有自信呦.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool6[4] && !DATA->_guideBool6[5]){
            str = CCString::createWithFormat("小妮偷偷的送给你一些金币.愉快的去购买几件衣服吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }
    }else if (guideIndex == 7){
        if (!DATA->_guideBool7[0]){
            this->creatDiKuang(1);
            str = CCString::createWithFormat("还有辣么多好玩的地方.如果想去,就让我们继续日常,获得更多的星星吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool7[0] && !DATA->_guideBool7[1]){
            CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideIsTxt_Bar");
            str = CCString::createWithFormat("在下方,有个提示框框.\n会随时提醒您,要做什么事情请多留意.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }else if (DATA->_guideBool7[1] && !DATA->_guideBool7[2]){
            this->creatDiKuang(1);
            str = CCString::createWithFormat("初步的知道就到这里.先帮助女总继续公司日常吧.");
            this->creat_ContentLength(str->getCString());
            this->creatView();
        }
        
        
    }
    
    
    
}
void GuideLayer::logic(float dt){
    this->unschedule(schedule_selector(GuideLayer::logic));
    
    if (wordCount > contentLength) {
        wordCount += 2;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        if (guideIndex == 1) {
            if (!DATA->_guideBool1[0]) {
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool1[0] && !DATA->_guideBool1[1]) {
                this->flyLabel();
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool1[1] && !DATA->_guideBool1[2]) {
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }
        }else if (guideIndex == 2){
            if (!DATA->_guideBool2[0]) {
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool2[0] && !DATA->_guideBool2[1]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool2[1] && !DATA->_guideBool2[2]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool2[2] && !DATA->_guideBool2[3]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }
        }else if (guideIndex == 4){
            if (!DATA->_guideBool4[0]) {
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool4[0] && !DATA->_guideBool4[1]){
                this->creatShou();
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool4[1] && !DATA->_guideBool4[2]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatClothes4_1_1), 1.f);
//                this->creatShou();
//                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool4[4] && !DATA->_guideBool4[5]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool4[6] && !DATA->_guideBool4[7]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }
        }else if (guideIndex == 5){
            if (!DATA->_guideBool5[0]){
                this->creatShou();
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool5[0] && !DATA->_guideBool5[1]){
                this->creatShou();
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }
        }else if (guideIndex == 6){
            if (!DATA->_guideBool6[0]){
                this->creatShou();
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool6[0] && !DATA->_guideBool6[1]){
                this->creatShou();
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool6[1] && !DATA->_guideBool6[2]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatClothes6_1_1), 1.f);
//                this->creatShou();
//                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool6[3] && !DATA->_guideBool6[4]){
                CCNotificationCenter::sharedNotificationCenter()->postNotification("GuideRemoveAnimation");
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool6[4] && !DATA->_guideBool6[5]){
                DATA->_guideBool6[5] = true;
                
                CCSprite* _panel = CCSprite::create("pic/building/reward/panel.png");
                _panel->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _panel->setTag(0x22222);
                this->addChild(_panel);
                
                CCLabelTTF* label = CCLabelTTF::create("点击任意处关闭", DISPLAY->fangzhengFont(), 25);
                label->setPosition(ccp(_panel->getContentSize().width* .75f, 17));
                label->setColor(ccWHITE);
                _panel->addChild(label);
                
                CCSize panelSize = _panel->boundingBox().size;
                
                CCSprite* light = CCSprite::create("pic/building/reward/light.png");
                light->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
                _panel->addChild(light);
                
                CCSprite* spots = CCSprite::create("pic/building/reward/spots.png");
                spots->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
                _panel->addChild(spots);
                
                CCSequence* seq = CCSequence::create(CCFadeIn::create(0.6), CCDelayTime::create(0.3), CCFadeOut::create(0.6), CCDelayTime::create(0.5), NULL);
                spots->runAction(CCRepeatForever::create(seq));
                
                CCSprite* title = CCSprite::create("pic/building/reward/title.png");
                title->setPosition(ccp(panelSize.width* .5f, panelSize.height* .88f));
                _panel->addChild(title);
                
                CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
                plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
                _panel->addChild(plate);
                
                CCSprite* coin = CCSprite::create("pic/common/coin2.png");
                coin->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
                _panel->addChild(coin);
                
                CCString* str = CCString::createWithFormat("%d 金币", 100);
                CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24);
                lbl->setColor(ccc3(107, 143, 190));
                lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.28));
                _panel->addChild(lbl);
                
                _panel->setScale(0.1);
                _panel->setVisible(true);
                
                float duration1 = 0.2f;
                CCMoveTo* moveto1 = CCMoveTo::create(duration1, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .67f));
                CCScaleTo* scaleto1 = CCScaleTo::create(duration1, 1.1);
                CCSpawn* spawn1 = CCSpawn::create(moveto1, scaleto1, NULL);
                
                float duration2 = 0.2f;
                CCMoveTo* moveto2 = CCMoveTo::create(duration2, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
                CCScaleTo* scaleto2 = CCScaleTo::create(duration2, 1);
                CCSpawn* spawn2 = CCSpawn::create(moveto2, scaleto2, NULL);
                
                _panel->runAction(CCEaseSineIn::create(CCSequence::create(spawn1, spawn2, NULL)));
                
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }
        }else if (guideIndex == 7){
            if (!DATA->_guideBool7[0]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool7[0] && !DATA->_guideBool7[1]){
                this->creatShou();
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }else if (DATA->_guideBool7[1] && !DATA->_guideBool7[2]){
                this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .5f);
            }
            
        }
        
        
    }else{
        wordCount += 2;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        this->schedule(schedule_selector(GuideLayer::logic), .001f);
    }
}
void GuideLayer::creatDiKuang(int index){
    
    CCString* manStr = CCString::createWithFormat("res/pic/guide/mishu/guide_mishu%d.png", index);
    manSpr = CCSprite::create(manStr->getCString());
    manSpr->setScale(.5f);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .15f, DISPLAY->ScreenHeight()* .15f));
    guideSpr->addChild(manSpr, 10);
    
    dkSpr = CCSprite::create("res/pic/guide/guide_kuang3.png");
    dkSpr->setAnchorPoint(CCPointZero);
    dkSpr->setPosition(ccp(manSpr->getPosition().x + 121, manSpr->getPosition().y + 120));
    dkSpr->setVisible(false);
    guideSpr->addChild(dkSpr, 4);
    
    CCLabelTTF* dkLabel = CCLabelTTF::create("新手引导", DISPLAY->fangzhengFont(), 28);
    dkLabel->setPosition(ccp(dkSpr->getContentSize().width* .5f, dkSpr->getContentSize().height* .89f));
    dkLabel->setColor(ccc3(168, 102, 103));
    dkLabel->setVisible(false);
    dkSpr->addChild(dkLabel);
    
    
    dkSpr->runAction(CCSequence::create(CCDelayTime::create(.2f), CCShow::create(), NULL));
    dkLabel->runAction(CCSequence::create(CCDelayTime::create(.6f), CCShow::create(), NULL));
}
void GuideLayer::creat_ContentLength(std::string str){
    content = str;
    contentLength = 0;
    
    long length = content.length();
    int i = 0;
    while (i < length) {
        char ch = getContent()[i];
        
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        contentLength++;
    }
    
//    CCLog("content=%s, length=%d", content.c_str(), contentLength);
}
std::string GuideLayer::getContentByLength(int length){
    if (length >= contentLength) {
        return getContent();
    }
    
    int i = 0;
    int index = 0;
    while (index < length) {
        char ch = getContent()[i];
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        index++;
    }
    std::string str = content.substr(0, i);
//    CCLog("index ======= %d", index);
    return str;
}
int GuideLayer::getContentLength(){
    return contentLength;
}



void GuideLayer::flyLabel(){
    CCSprite* spr1 = CCSprite::create("res/pic/guide/guide_label1.png");
    spr1->setPosition(ccp(-500, DISPLAY->ScreenHeight()* .92f));
    this->addChild(spr1);
    CCSprite* spr2 = CCSprite::create("res/pic/guide/guide_label2.png");
    spr2->setPosition(ccp(-500, DISPLAY->ScreenHeight()* .82f));
    this->addChild(spr2);
    CCSprite* spr3 = CCSprite::create("res/pic/guide/guide_label3.png");
    spr3->setPosition(ccp(-500, DISPLAY->ScreenHeight()* .72f));
    this->addChild(spr3);
    CCSprite* spr4 = CCSprite::create("res/pic/guide/guide_label4.png");
    spr4->setPosition(ccp(-500, DISPLAY->ScreenHeight()* .62f));
    this->addChild(spr4);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(12.f, ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .92f));
    CCMoveTo* moveTo2 = CCMoveTo::create(8.f, ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .82f));
    CCMoveTo* moveTo3 = CCMoveTo::create(6.f, ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .72f));
    CCMoveTo* moveTo4 = CCMoveTo::create(9.f, ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .62f));
    spr1->runAction(moveTo1);
    spr2->runAction(moveTo2);
    spr3->runAction(moveTo3);
    spr4->runAction(moveTo4);
}

void GuideLayer::guideTime(float dt){
    this->unschedule(SEL_SCHEDULE(&GuideLayer::guideTime));
    LOADING->show_loading();
    DATA->getPlayer()->setGuide(7);
    NET->update_guide_905(DATA->getPlayer()->getGuide());
}
void GuideLayer::guideQuan(){
    
}
void GuideLayer::creatClothes4_1_1(){
    CCSprite* clothesSpr = CCSprite::create("res/pic/clothesScene/icon/3shangyi/icon31071.png");
    clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 90, DISPLAY->ScreenHeight()* .8f));
    clothesSpr->setTag(0x222333);
    this->addChild(clothesSpr);
    clothesSpr->setVisible(false);
    
    CCScaleTo* scaleTo1_1 = CCScaleTo::create(.2f, 1.4f);
    CCScaleTo* scaleTo1_2 = CCScaleTo::create(.1f, 1.2f);
    CCScaleTo* scaleTo1_3 = CCScaleTo::create(.2f, 1.4f);
    CCScaleTo* scaleTo1_4 = CCScaleTo::create(.1f, 1.2f);
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&GuideLayer::creatClothes4_1_2));
    clothesSpr->runAction(CCSequence::create(CCShow::create(), scaleTo1_1, scaleTo1_2, scaleTo1_3, scaleTo1_4, CCDelayTime::create(.5f), callFunc, NULL));
}
void GuideLayer::creatClothes4_1_2(){
    CCSprite* clothesSpr = (CCSprite* )this->getChildByTag(0x222333);
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&GuideLayer::creatClothes4_1_3));
    CCScaleTo* scaleTo = CCScaleTo::create(.2f, 1.f);
    CCFadeOut* fadeOut = CCFadeOut::create(.2f);
    CCSpawn* spawn = CCSpawn::create(scaleTo, fadeOut, NULL);
    clothesSpr->runAction(CCSequence::create(spawn, callFunc, NULL));
}
void GuideLayer::creatClothes4_1_3(){
    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatShou), .3f);
    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .3f);
}
void GuideLayer::creatClothes4_2_1(){
    if (this->getChildByTag(0x222333) != NULL) {
        this->removeChildByTag(0x222333);
    }
    CCSprite* clothesSpr = CCSprite::create("res/pic/clothesScene/icon/6xiezi/icon61071.png");
    clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 90, DISPLAY->ScreenHeight()* .8f));
    clothesSpr->setTag(0x222333);
    this->addChild(clothesSpr);
    clothesSpr->setVisible(false);
    
    CCScaleTo* scaleTo1_1 = CCScaleTo::create(.2f, 1.4f);
    CCScaleTo* scaleTo1_2 = CCScaleTo::create(.1f, 1.2f);
    CCScaleTo* scaleTo1_3 = CCScaleTo::create(.2f, 1.4f);
    CCScaleTo* scaleTo1_4 = CCScaleTo::create(.1f, 1.2f);
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&GuideLayer::creatClothes4_2_2));
    clothesSpr->runAction(CCSequence::create(CCShow::create(), scaleTo1_1, scaleTo1_2, scaleTo1_3, scaleTo1_4, CCDelayTime::create(.5f), callFunc, NULL));
}
void GuideLayer::creatClothes4_2_2(){
    CCSprite* clothesSpr = (CCSprite* )this->getChildByTag(0x222333);
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&GuideLayer::creatClothes4_2_3));
    CCScaleTo* scaleTo = CCScaleTo::create(.2f, 1.f);
    CCFadeOut* fadeOut = CCFadeOut::create(.2f);
    CCSpawn* spawn = CCSpawn::create(scaleTo, fadeOut, NULL);
    clothesSpr->runAction(CCSequence::create(spawn, callFunc, NULL));
}
void GuideLayer::creatClothes4_2_3(){
    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatShou), .3f);
    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .3f);
}
void GuideLayer::creatClothes6_1_1(){
    if (this->getChildByTag(0x222333) != NULL) {
        this->removeChildByTag(0x222333);
    }
    CCSprite* clothesSpr = CCSprite::create("res/pic/clothesScene/icon/3shangyi/icon31011.png");
    clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth() - 90, DISPLAY->ScreenHeight()* .8f));
    clothesSpr->setTag(0x222333);
    this->addChild(clothesSpr);
    clothesSpr->setVisible(false);
    
    CCScaleTo* scaleTo1_1 = CCScaleTo::create(.2f, 1.4f);
    CCScaleTo* scaleTo1_2 = CCScaleTo::create(.1f, 1.2f);
    CCScaleTo* scaleTo1_3 = CCScaleTo::create(.2f, 1.4f);
    CCScaleTo* scaleTo1_4 = CCScaleTo::create(.1f, 1.2f);
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&GuideLayer::creatClothes6_1_2));
    clothesSpr->runAction(CCSequence::create(CCShow::create(), scaleTo1_1, scaleTo1_2, scaleTo1_3, scaleTo1_4, CCDelayTime::create(.5f), callFunc, NULL));
}
void GuideLayer::creatClothes6_1_2(){
    CCSprite* clothesSpr = (CCSprite* )this->getChildByTag(0x222333);
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&GuideLayer::creatClothes6_1_3));
    CCScaleTo* scaleTo = CCScaleTo::create(.2f, 1.f);
    CCFadeOut* fadeOut = CCFadeOut::create(.2f);
    CCSpawn* spawn = CCSpawn::create(scaleTo, fadeOut, NULL);
    clothesSpr->runAction(CCSequence::create(spawn, callFunc, NULL));
}
void GuideLayer::creatClothes6_1_3(){
    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::creatShou), .3f);
    this->scheduleOnce(SEL_SCHEDULE(&GuideLayer::openTouch), .3f);
}

void GuideLayer::closeSwallowEnabled(){
    this->setTouchSwallowEnabled(true);
}



CCRect GuideLayer::isRect1_1(){
    return CCRectMake(0, DISPLAY->ScreenHeight()* .48f, 250, 400);
}
bool GuideLayer::isContainTouchPoint1_1(CCTouch* touch){
    return isRect1_1().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect2_1(){
    return CCRectMake(DISPLAY->ScreenWidth() - 180.f, DISPLAY->ScreenHeight()* .77f, 165, 85);
}
bool GuideLayer::isContainTouchPoint2_1(CCTouch* touch){
    return isRect2_1().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect4_1(){
    return CCRectMake(8, DISPLAY->ScreenHeight()* .867f, 246, 80);
}
bool GuideLayer::isContainTouchPoint4_1(CCTouch* touch){
    return isRect4_1().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect4_2(){
    return CCRectMake(DISPLAY->ScreenWidth() - 180.f, DISPLAY->ScreenHeight()* .71f, 184, 190);
}
bool GuideLayer::isContainTouchPoint4_2(CCTouch* touch){
    return isRect4_2().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect4_3(){
    return CCRectMake(DISPLAY->ScreenWidth() - 267.f, DISPLAY->ScreenHeight()* .43f, 90, 75);
}
bool GuideLayer::isContainTouchPoint4_3(CCTouch* touch){
    return isRect4_3().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect4_4(){
    return CCRectMake(DISPLAY->ScreenWidth() - 288, 11, 100, 55);
}
bool GuideLayer::isContainTouchPoint4_4(CCTouch* touch){
    return isRect4_4().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect5_1(){
    return CCRectMake(0, DISPLAY->ScreenHeight()* .3f, 500, DISPLAY->ScreenHeight()* .55f);
}
bool GuideLayer::isContainTouchPoint5_1(CCTouch* touch){
    return isRect5_1().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect5_2(){
    return CCRectMake(DISPLAY->ScreenWidth()* .5f + 160, DISPLAY->ScreenHeight()* .1f, 84, 25);
}
bool GuideLayer::isContainTouchPoint5_2(CCTouch* touch){
    return isRect5_2().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect6_1(){
    return CCRectMake(DISPLAY->ScreenWidth()- 100, DISPLAY->ScreenHeight()* .86f, 96, 96);
}
bool GuideLayer::isContainTouchPoint6_1(CCTouch* touch){
    return isRect6_1().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect6_2(){
    return CCRectMake(DISPLAY->ScreenWidth()- 80, DISPLAY->ScreenHeight()* .755f, 80, 96);
}
bool GuideLayer::isContainTouchPoint6_2(CCTouch* touch){
    return isRect6_2().containsPoint(convertTouchToNodeSpace(touch));
}
CCRect GuideLayer::isRect6_3(){
    return CCRectMake(DISPLAY->ScreenWidth() - 160, 10, 145, 65);
}
bool GuideLayer::isContainTouchPoint6_3(CCTouch* touch){
    return isRect6_3().containsPoint(convertTouchToNodeSpace(touch));
}





#pragma mark - ------------------ Draw ------------------

void GuideLayer::beforeDraw()
{
    
}

void GuideLayer::creat_testRect(){
    _testRect = CCRectMake(DISPLAY->ScreenWidth() - 160, 10, 145, 65);
}

void GuideLayer::afterDraw()
{
    CHECK_GL_ERROR_DEBUG();
    ccDrawColor4B(200, 10, 200, 200);
    glLineWidth(4.f);
    
    ccDrawRect(_testRect.origin, _testRect.origin + ccp(_testRect.size.width, _testRect.size.height));
}

void GuideLayer::visit()
{
    m_drawOrder = ++g_drawOrder;
    
    // quick return if not visible
    if (!isVisible())
    {
        return;
    }
    
    kmGLPushMatrix();
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }
    
    this->transform();
    
//#if (DEBUG_GUIDE)
    this->beforeDraw();
//#endif
    
    if(m_pChildren)
    {
        ccArray *arrayData = m_pChildren->data;
        unsigned int i=0;
        
        // draw children zOrder < 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode *child =  (CCNode*)arrayData->arr[i];
            if ( child->getZOrder() < 0 )
            {
                child->visit();
            }
            else
            {
                break;
            }
        }
        
        // this draw
        this->draw();
        
        // draw children zOrder >= 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode* child = (CCNode*)arrayData->arr[i];
            child->visit();
        }
        
    }
    else
    {
        this->draw();
    }
    
//#if (DEBUG_GUIDE)
    this->afterDraw();
//#endif
    
    if ( m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }
    
    kmGLPopMatrix();
}




