//
//  WelfareCell.cpp
//  tiegao
//
//  Created by mac on 16/12/24.
//
//

#include "WelfareCell.h"

#include "DisplayManager.h"

#include "WelfareComp.h"

WelfareCell::~WelfareCell() {

}

void WelfareCell::configWithWelfareItem(int idx, WelfareItem *item, float cellWidth, float cellHeight) {
    float halfCellWidth = cellWidth * 0.5;
    float halfCellHeight = cellHeight * 0.5;
    
    CCSprite* spt = CCSprite::create("pic/welfare/welfare_plane.png");;
    spt->setPosition(ccp(cellWidth * 0.5, cellHeight * 0.5));
    this->addChild(spt);
    
    CCString* name = CCString::createWithFormat("%d.", idx + 1);
    CCLabelTTF* lblName = CCLabelTTF::create(name->getCString(), DISPLAY->fangzhengFont(), 20);
    lblName->setAnchorPoint(ccp(0, 0.5));
    lblName->setPosition(ccp(cellWidth * 0.03, halfCellHeight));
    this->addChild(lblName);
    
    
    CCSprite* progressBottom = CCSprite::create("pic/welfare/welfare_total_bar_1.png");
    progressBottom->setPosition(ccp(halfCellWidth, cellHeight * 0.2));
    this->addChild(progressBottom);
    
    CCSize barSize = progressBottom->getContentSize();
    
    _progress = CCProgressTimer::create(CCSprite::create("pic/welfare/welfare_item_bar_2.png"));
    _progress->setPosition(progressBottom->getPosition());
    _progress->setType(kCCProgressTimerTypeBar);
    _progress->setMidpoint(ccp(0, 0.5));
    _progress->setBarChangeRate(ccp(1, 0));
    _progress->setPercentage(100);
    this->addChild(_progress);
}
