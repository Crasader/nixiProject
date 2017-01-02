//
//  UnderlineLabel.cpp
//  tiegao
//
//  Created by mac on 17/1/2.
//
//

#include "UnderlineLabel.h"

void UnderlineLabel::setUnderlineColor(const ccColor4B &color) {
    _underlineColor = color;
}

void UnderlineLabel::setUnderlineEnabled(bool isEnabled) {
    _isUnderlineEnabled = isEnabled;
}

void UnderlineLabel::setUnderlineSize(float size) {
    _lineSize = size;
}

void UnderlineLabel::setFrontInterval(float interval) {
    _frontInterval = interval;
}

void UnderlineLabel::setBehindInterval(float interval) {
    _behindInterval = interval;
}

void UnderlineLabel::setUnderlineHeight(float height) {
    _lineHeight = height;
}


UnderlineLabel::~UnderlineLabel() {
    
}

UnderlineLabel* UnderlineLabel::create(const char *string, const char *fontName, float fontSize)
{
    UnderlineLabel * pRet = new UnderlineLabel();
    if (pRet && pRet->initWithString(string, fontName, fontSize)) {
        pRet->autorelease();
    }
    else {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool UnderlineLabel::initWithString(const char *label, const char *fontName, float fontSize) {
    if (CCLabelTTF::initWithString(label, fontName, fontSize)) {
        _isUnderlineEnabled = true;
        _underlineColor = ccc4(0, 0, 0, 255);
        _lineSize = 2.f;
        _frontInterval = 0;
        _behindInterval = 0;
        _lineHeight = 0;
        
        return true;
    }
    
    return false;
}

void UnderlineLabel::draw() {
    CCLabelTTF::draw();
    if (_isUnderlineEnabled) {
        ccDrawColor4B(_underlineColor.r, _underlineColor.g, _underlineColor.b, _underlineColor.a);
        ccPointSize(_lineSize);
        
        ccDrawLine(ccp(0 + _frontInterval, _lineHeight), ccp(this->getContentSize().width + _behindInterval, _lineHeight));
    }
}
