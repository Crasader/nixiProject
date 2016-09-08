//
//  CoinExchangePanel.cpp
//  tiegao
//
//  Created by mac on 16/6/30.
//
//

#include "CoinExchangePanel.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "AudioManager.h"

#include "PromptLayer.h"
#include "Loading2.h"
#include <math.h>

CoinExchangePanel::~CoinExchangePanel() {
}

bool CoinExchangePanel::init() {
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
        
        CCSprite* cancel1 = CCSprite::create("res/pic/common/btn_cancel.png");
        CCSprite* cancel2 = CCSprite::create("res/pic/common/btn_cancel.png");
        cancel2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btn_canel = CCMenuItemSprite::create(cancel1, cancel2, this, SEL_MenuHandler(&CoinExchangePanel::remove));
        
        CCSprite* confirm1 = CCSprite::create("res/pic/common/btn_purchase.png");
        CCSprite* confirm2 = CCSprite::create("res/pic/common/btn_purchase.png");
        confirm2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btn_confirm = CCMenuItemSprite::create(confirm1, confirm2, this, SEL_MenuHandler(&CoinExchangePanel::buy));
        
        CCMenu* menu = CCMenu::create(btn_canel, btn_confirm, NULL);
        menu->setPosition(ccp(menu->getPosition().x, menu->getPosition().y + 40));
        menu->alignItemsHorizontallyWithPadding(60);
        _content->addChild(menu);

        int exchangeTimes = DATA->getPurchase()->getCoinExchangedTimes();
        int limit = DATA->getPurchase()->getCoinExchangeLimit();
        if (exchangeTimes >= limit) {
            btn_confirm->setEnabled(false);
            btn_confirm->setColor(ccGRAY);
            
            CCLabelTTF* lbl = CCLabelTTF::create("已达今日金币兑换次数上限~", DISPLAY->fangzhengFont(), 28.f);
            lbl->setColor(ccRED);
            lbl->setPosition(DISPLAY->center() + ccp(0, 150));
            _content->addChild(lbl);
        }
        else {
            CCString* str = CCString::createWithFormat("使用%d钻石兑换%d金币", DATA->getPurchase()->getCoinExchangeCost(), DATA->getPurchase()->getCoinExchangeGain());
            CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 28.f);
            lbl->setColor(DISPLAY->dullBlueColor());
            lbl->setPosition(DISPLAY->center() + ccp(0, 150));
            _content->addChild(lbl);
        }
        
        CCString* str = CCString::createWithFormat("今日已兑换%d/%d次", exchangeTimes, limit);
        CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 28.f);
        lbl->setColor(ccGRAY);
        lbl->setPosition(DISPLAY->center() + ccp(0, 110));
        _content->addChild(lbl);
        
        return true;
    }
    else {
        return false;
    }
}

void CoinExchangePanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&CoinExchangePanel::nc_exchange_coin_103), "HTTP_FINISHED_103", NULL);
    
//    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&CoinExchangePanel::keyBackStatus), .8f);
}

void CoinExchangePanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void CoinExchangePanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool CoinExchangePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = _content->convertToNodeSpace(pTouch->getLocation());
    if (! _panel->boundingBox().containsPoint(location)) {
//        this->do_exit();
        remove();
    }
    
    return true;
}

#pragma mark - export

void CoinExchangePanel::show() {
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

void CoinExchangePanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void CoinExchangePanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.8f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    //    _content->runAction(CCEaseBounceOut::create(spawn));
    _content->runAction(CCEaseElasticOut::create(spawn));
}

void CoinExchangePanel::do_exit() {
    float duration = 0.6f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&CoinExchangePanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(CCEaseElasticIn::create(spawn));
}

void CoinExchangePanel::remove() {
    AUDIO->goback_effect();
    
    this->removeFromParentAndCleanup(true);
}

void CoinExchangePanel::buy() {
    LOADING->show_loading();
    NET->exchange_coin_103();
}

void CoinExchangePanel::nc_exchange_coin_103(CCObject *pObj) {
    AUDIO->comfirm_effect();
    LOADING->remove();
    
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(this->getScene(), "成功兑换金币~!");
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    this->remove();
}


void CoinExchangePanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}
