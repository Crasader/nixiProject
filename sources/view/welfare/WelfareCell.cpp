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
    
    
    CCLabelTTF* lblName = CCLabelTTF::create(item->name.c_str(), DISPLAY->fangzhengFont(), 26);
    lblName->setAnchorPoint(ccp(0, 0.5));
    lblName->setColor(ccc3(159, 55, 98));
    lblName->setPosition(ccp(cellWidth * 0.03, halfCellHeight));
    this->addChild(lblName);
    
    
    CCString* strIcon = CCString::createWithFormat("pic/welfare/welfare_reward_plate_%s.png", item->rewardType.c_str());
    if (strIcon) {
        CCSprite* rewardIcon = CCSprite::create(strIcon->getCString());
        rewardIcon->setPosition(ccp(cellWidth * 0.65, halfCellHeight));
        this->addChild(rewardIcon);
        
        CCString* strNum = CCString::createWithFormat("%d", item->rewardNum);
        CCLabelTTF* lblRewardNum = CCLabelTTF::create(strNum->getCString(), DISPLAY->fangzhengFont(), 20);
        lblRewardNum->setColor(ccc3(227, 112, 86));
        lblRewardNum->setPosition(rewardIcon->getPosition() + ccp(17, -2));
        this->addChild(lblRewardNum);
    }
    
    
    _sptButton1 = CCSprite::create("pic/welfare/welfare_btn_take.png");
    _sptButton2 = CCSprite::create("pic/welfare/welfare_btn_take.png");
    _sptButton2->setScale(1.02);
    CCMenuItem* btn = CCMenuItemSprite::create(_sptButton1, _sptButton2);
    _menuBtn = CCMenu::createWithItem(btn);
    _menuBtn->setPosition(ccp(cellWidth * 0.88, halfCellHeight));
    _menuBtn->setEnabled(false);
    this->addChild(_menuBtn);
    
    
    float progressFontSize = 30.f;
    
    CCString* strProgressHead = CCString::createWithFormat("%d", item->progress);
    CCLabelTTF* lblProgressHead = CCLabelTTF::create(strProgressHead->getCString(), DISPLAY->fangzhengFont(), progressFontSize);
    lblProgressHead->setColor(ccc3(246, 255, 4));
    lblProgressHead->setAnchorPoint(ccp(1, 0.5));
    lblProgressHead->setPosition(ccp(_sptButton1->getContentSize().width * 0.45, _sptButton1->getContentSize().height * 0.5));
    _sptButton1->addChild(lblProgressHead);
    
    CCString* strProgressTail = CCString::createWithFormat("/%d", item->goal);
    CCLabelTTF* lblProgressTail = CCLabelTTF::create(strProgressTail->getCString(), DISPLAY->fangzhengFont(), progressFontSize);
    lblProgressTail->setAnchorPoint(ccp(0, 0.5));
    lblProgressTail->setPosition(lblProgressHead->getPosition());
    _sptButton1->addChild(lblProgressTail);
    
    CCSprite* progressBottom = CCSprite::create("pic/welfare/welfare_total_bar_1.png");
    progressBottom->setPosition(ccp(halfCellWidth, cellHeight * 0.15));
    this->addChild(progressBottom);
    
    CCSize barSize = progressBottom->getContentSize();
    
    _progress = CCProgressTimer::create(CCSprite::create("pic/welfare/welfare_item_bar_2.png"));
    _progress->setPosition(progressBottom->getPosition());
    _progress->setType(kCCProgressTimerTypeBar);
    _progress->setMidpoint(ccp(0, 0.5));
    _progress->setBarChangeRate(ccp(1, 0));
    _progress->setPercentage(100);
    this->addChild(_progress);
    
    
    this->goStar(idx, item->progress, item->goal, item->status);
}

//void WelfareCell::onEnter() {
//    this->goStar();
//}


void WelfareCell::goStar(int idx, int progress, int goal, int status) {
    float per = MIN(progress * 0.1 / goal * 10, 1.0);
    float duration = 1.f * per;

    _progress->runAction(CCProgressFromTo::create(duration, 0, per * 100));
    
    _star = CCSprite::create("pic/welfare/welfare_star1.png");
    _progress->addChild(_star);
    CCSize progressSize = _progress->getContentSize();
    float toX = progressSize.width * per;
    
    CCCallFuncO* finish = CCCallFuncO::create(this, SEL_CallFuncO(&WelfareCell::moveFinish), CCInteger::create(status));
    CCSequence* starSeq = CCSequence::create(CCPlace::create(ccp(0, progressSize.height * 0.5)), CCMoveTo::create(duration, ccp(toX, progressSize.height * 0.5)), finish, NULL);
    _star->runAction(starSeq);
}

void WelfareCell::moveFinish(CCObject* pObj) {
    if (pObj) {
        int status = ((CCInteger* )pObj)->getValue();
        if (status == 0) {
            
        }
        else if (status == 1 && _star) {
            _star->runAction(CCRepeatForever::create( CCRotateBy::create(1.2, 360)) );
        }
        else if (status == 2) {
            
        }
    }
}


