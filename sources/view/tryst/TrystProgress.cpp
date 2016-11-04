//
//  TrystProgress.cpp
//  tiegao
//
//  Created by mac on 16/11/2.
//
//

#include "TrystProgress.h"

#include "DisplayManager.h"


TrystProgress::~TrystProgress() {}

TrystProgress* TrystProgress::create(CCObject *target, SEL_CallFunc callback) {
    TrystProgress* rtn = new TrystProgress();
    if (rtn && rtn->init(target, callback)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

void TrystProgress::startInput(const char *text) {
    _inputCount = 0;
    _origText = text;
    _lbl->setString("");
    
    this->schedule(SEL_SCHEDULE(&TrystProgress::gogogo), 0.3);
}


bool TrystProgress::init(CCObject *target, SEL_CallFunc callback) {
    if (CCNode::create()) {
        _target = target;
        _callback = callback;
        
        CCSprite* inputBar = CCSprite::create("pic/tryst/tryst_input.png");
        this->addChild(inputBar);
        
        CCSprite* mask = CCSprite::create("pic/tryst/tryst_input_mask.png");
        mask->setAnchorPoint(CCPointZero);
        CCClippingNode* node = CCClippingNode::create(mask);
        node->setPosition(74.5, 10);
        inputBar->addChild(node);
        
        _savedInputboxWidth = mask->getContentSize().width;
        
        CCLayerColor* test = CCLayerColor::create(ccc4(200, 200, 200, 200), 800, 200);
        test->setAnchorPoint(ccp(0.5, 0.5));
        node->addChild(test);
        
        _lbl = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 28);
        _lbl->setAnchorPoint(ccp(0, 0.5));
        _lbl->setPosition(ccp(0, mask->getContentSize().height * 0.5));
        _lbl->setColor(ccBLACK);
        node->addChild(_lbl);
        
        return true;
    }
    else {
        return false;
    }
}


void TrystProgress::gogogo() {
    _inputCount += 3;
    if (_inputCount > _origText.length()) {
        _lbl->setString("");
        _lbl->setAnchorPoint(ccp(0, 0.5));
        _lbl->setPosition(ccp(0, _lbl->getPositionY()));
        
        this->unscheduleAllSelectors();
        if (_target && _callback) {
            (_target->*_callback)();
        }
    }
    else {
        std::string strTemp = _origText.substr(0, _inputCount);
        //        CCLOG("INPUT: %s", strTemp.c_str());
        _lbl->setString(strTemp.c_str());
        
        if (_lbl->getContentSize().width >= _savedInputboxWidth) {
            _lbl->setAnchorPoint(ccp(1, 0.5));
            _lbl->setPosition(ccp(_savedInputboxWidth, _lbl->getPositionY()));
        }
    }
}


