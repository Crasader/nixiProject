//
//  TrystScene.cpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#include "TrystScene.h"

#include "DisplayManager.h"
#include "ConfigManager.h"

#include "TrystData.h"
#include "TrystView.h"
#include "TrystDialogState.h"

#include "MainScene.h"
#include "ClothesScene.h"


// Export
TrystScene::~TrystScene() {
    delete _state;
    _state = nullptr;
    
    CC_SAFE_DELETE(_data);
}

TrystScene* TrystScene::create(const char *id) {
    TrystScene* rtn = new TrystScene();
    if (rtn && rtn->init(id)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}



// Inner

bool TrystScene::init(const char *id) {
    if (CCScene::init()) {
        _nextDialogIndex = 0;
        
        _data = TrystData::create(CONFIG->trystDialog(id));
        _data->retain();
        
        _state = new TrystDialogState();
        
        _view = TrystView::create(id);
        this->addChild(_view);
        
        CCMenuItem* itemGoback = CCMenuItemImage::create("pic/tryst/tryst_btn_goback.png", "pic/tryst/tryst_btn_goback.png", this, SEL_MenuHandler(&TrystScene::onBtnGoback));
        CCMenu* menuGoback = CCMenu::createWithItem(itemGoback);
        menuGoback->setPosition(ccp(DISPLAY->halfW() - 285, DISPLAY->H() * 0.95));
        this->addChild(menuGoback);
        
        return  true;
    }
    else {
        return false;
    }
}

void TrystScene::onEnter() {
    CCScene::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&TrystScene::onDialogAppeared), "TRYST_DIALOG_APPEARED", NULL);
    
    _state->transform(TrystDialogState::StateIdle);
    this->schedule(SEL_SCHEDULE(&TrystScene::playing), 0.5);
}

void TrystScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCScene::onExit();
}

void TrystScene::playing(float dt) {
    const std::string state = _state->curState();
    if (state.compare(TrystDialogState::StateEnd) == 0) {
        this->unschedule(SEL_SCHEDULE(&TrystScene::playing));
        this->setCouldStart();
    }
    else if (state.compare(TrystDialogState::StateIdle) == 0) {
        _state->transform(TrystDialogState::StatePlaying);
        CCDictionary* dialog = _data->fetchDialog(_nextDialogIndex);
        const CCString* nameID = dialog->valueForKey("nameID");
        const CCString* said = dialog->valueForKey("said");
        CCLOG("SAID: %s", said->getCString());
        
        if (nameID->compare("101") == 0) {
            _view->insertDialog(false, said->getCString());
        }
        else {
            _view->insertDialog(true, said->getCString());
        }
        
        _nextDialogIndex = dialog->valueForKey("next")->intValue();
    }
}

void TrystScene::onDialogAppeared() {
    if (_nextDialogIndex == -1) {
        _state->transform(TrystDialogState::StateEnd);
    }
    else {
        _state->transform(TrystDialogState::StateIdle);
    }
}

void TrystScene::setCouldStart() {
    CCSprite* spr1 = CCSprite::create("pic/tryst/tryst_btn_start.png");
    CCSprite* spr2 = CCSprite::create("pic/tryst/tryst_btn_start.png");
    spr2->setScale(1.012);
    CCMenuItem* itemStart = CCMenuItemSprite::create(spr1, spr2, this, SEL_MenuHandler(&TrystScene::onBtnStartTryst));
    CCMenu* menuStart = CCMenu::createWithItem(itemStart);
    menuStart->setPosition(DISPLAY->halfW() + 260, 37.5);
    this->addChild(menuStart);
}

void TrystScene::onBtnStartTryst() {
    CCLOG("TrystScene::onBtnStartTryst() -");
    CCDictionary* firstDialog = dynamic_cast<CCDictionary*>(_data->fetchDialog(0));
    const CCString* tishi = firstDialog->valueForKey("tishi");
    CCLayer* layer = ClothesScene::create_with_tryst(tishi->getCString());
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void TrystScene::onBtnGoback() {
    CCLOG("TrystScene::onBtnGoback() -");
    CCLayer* layer = MainScene::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

