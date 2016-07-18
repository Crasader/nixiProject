//
//  EnergyBuyPanel.cpp
//  tiegao
//
//  Created by mac on 16/6/30.
//
//

#include "EnergyBuyPanel.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "Loading2.h"
#include "AudioManager.h"
#include <math.h>

EnergyBuyPanel::~EnergyBuyPanel() {
}

bool EnergyBuyPanel::init() {
    if (CCLayer::init()) {
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
//        CCSprite* mask = CCSprite::create("res/pic/mask.png");
//        mask->setPosition(DISPLAY->center());
//        this->addChild(mask);
        
        _content = CCLayer::create();
        _content->setPosition(ccp(_content->getPosition().x, _content->getPosition().y - DISPLAY->H() * 0.1));
        this->addChild(_content);
        
        _panel = CCSprite::create("res/pic/panel/energybuy/eb_panel.png");
        _panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.6));
        _content->addChild(_panel);
        
        CCSprite* cancel1 = CCSprite::create("pic/common/btn_cancel.png");
        CCSprite* cancel2 = CCSprite::create("pic/common/btn_cancel.png");
        cancel2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btn_canel = CCMenuItemSprite::create(cancel1, cancel2, this, SEL_MenuHandler(&EnergyBuyPanel::remove));
        
        CCSprite* confirm1 = CCSprite::create("pic/common/btn_confirm.png");
        CCSprite* confirm2 = CCSprite::create("pic/common/btn_confirm.png");
        confirm2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btn_confirm = CCMenuItemSprite::create(confirm1, confirm2, this, SEL_MenuHandler(&EnergyBuyPanel::buy));
        
        CCMenu* menu = CCMenu::create(btn_canel, btn_confirm, NULL);
        menu->setPosition(ccp(menu->getPosition().x, menu->getPosition().y + 40));
        menu->alignItemsHorizontallyWithPadding(60);
        _content->addChild(menu);
        
        
        PlayerComp* playerInfo = DATA->getPlayer();
        if (playerInfo->energy >= 100) {
            btn_confirm->setScale(1);
            btn_confirm->setEnabled(false);
            btn_confirm->setColor(ccGRAY);
        }
        
        int boughtTimes = DATA->getPurchase()->getEnergyBoughtTimes();
        int limit = DATA->getPurchase()->getEnergyBuyLimit();
        if (boughtTimes >= limit) {
            btn_confirm->setScale(1);
            btn_confirm->setEnabled(false);
            btn_confirm->setColor(ccGRAY);
            
            CCLabelTTF* lbl = CCLabelTTF::create("已达今日体力购买次数上限~", DISPLAY->fangzhengFont(), 28.f);
            lbl->setColor(ccRED);
            lbl->setPosition(DISPLAY->center() + ccp(0, 150));
            _content->addChild(lbl);
        }
        else {
            CCString* str = CCString::createWithFormat("使用%d钻石兑换100点体力", 10 * int(pow(2, boughtTimes)));
            CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 28.f);
            lbl->setColor(DISPLAY->dullBlueColor());
            lbl->setPosition(DISPLAY->center() + ccp(0, 150));
            _content->addChild(lbl);
        }
        
        CCString* str = CCString::createWithFormat("今日已购买%d/%d次", boughtTimes, limit);
        CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 28.f);
        lbl->setColor(ccGRAY);
        lbl->setPosition(DISPLAY->center() + ccp(0, 110));
        _content->addChild(lbl);
        
//        CCLabelTTF* lbl_prompt = CCLabelTTF::create("(体力不满时每6分钟自动增长1点哟~)", DISPLAY->fangzhengFont(), 20.f);
//        lbl_prompt->setColor(ccMAGENTA);
//        lbl_prompt->setPosition(DISPLAY->center() + ccp(0, 60));
//        _content->addChild(lbl_prompt);
        
        return true;
    }
    else {
        return false;
    }
}

void EnergyBuyPanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&EnergyBuyPanel::buy_energy_callback_101), "HTTP_FINISHED_101", NULL);
    
//    this->do_enter();
}

void EnergyBuyPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool EnergyBuyPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = _content->convertToNodeSpace(pTouch->getLocation());
    if (! _panel->boundingBox().containsPoint(location)) {
//        this->do_exit();
        remove();
    }
    
    return true;
}

#pragma mark - export

void EnergyBuyPanel::show() {
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

void EnergyBuyPanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void EnergyBuyPanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.8f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    //    _content->runAction(CCEaseBounceOut::create(spawn));
    _content->runAction(CCEaseElasticOut::create(spawn));
}

void EnergyBuyPanel::do_exit() {
    float duration = 0.6f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&EnergyBuyPanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(CCEaseElasticIn::create(spawn));
}

void EnergyBuyPanel::remove() {
    AUDIO->common_effect();
    this->removeFromParentAndCleanup(true);
}

void EnergyBuyPanel::buy() {
    LOADING->show_loading();
    NET->buy_energy_101();
}

void EnergyBuyPanel::buy_energy_callback_101(CCObject *pObj) {
    LOADING->remove();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    this->remove();
}
