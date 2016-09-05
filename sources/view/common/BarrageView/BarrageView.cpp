//
//  BarrageView.cpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#include "BarrageView.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "WSManager.h"
#include "ChatComp.h"

#pragma mark - Export API

void BarrageView::show() {
    CCDirector::sharedDirector()->getRunningScene()->addChild(BarrageView::create(), 1100);
}

void BarrageView::addNewChat(CCObject *pObj) {
    ChatItem* chat = (ChatItem*)pObj;
//    CCLOG("-- %s", chat->chat.c_str());
    float fontSize = 24 + CCRANDOM_MINUS1_1() * 6;
    CCLabelTTF* lbl = CCLabelTTF::create(chat->chat.c_str(), DISPLAY->fangzhengFont(), fontSize);
    lbl->setAnchorPoint(ccp(0, 0.5));
    float randY = (0.5 + CCRANDOM_MINUS1_1() * 0.4) * DISPLAY->H();
    lbl->setPosition(ccp(DISPLAY->W(), randY));
    CCColor3bObject* color = DATA->getChat()->randColor();
    lbl->setColor(color->value);
    lbl->runAction(CCSequence::create(CCMoveBy::create(5.f, ccp(-1500, 0)), CCRemoveSelf::create(), NULL));
    this->addChild(lbl);
}

#pragma mark - Super API

BarrageView::~BarrageView() {
    
}

bool BarrageView::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("res/pic/white_mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.03));
        this->addChild(txt_close);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);

        return true;
    }
    else {
        return false;
    }
}

void BarrageView::onEnter() {
    CCLayer::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&BarrageView::addNewChat), "NEW_CHAT", NULL);
}

void BarrageView::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void BarrageView::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void BarrageView::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}

bool BarrageView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    remove();
    return false;
}

#pragma mark - Inner API

void BarrageView::remove() {
//    AUDIO->common_effect();
    WS->disconnect();
    this->removeFromParentAndCleanup(true);
}
