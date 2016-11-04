//
//  TrystProgress.cpp
//  tiegao
//
//  Created by mac on 16/11/2.
//
//

#include "TrystProgress.h"

#include "DisplayManager.h"
#include "DataManager.h"

#pragma mark - Export

TrystProgress::~TrystProgress() {}

TrystProgress* TrystProgress::create(int max, int left) {
    TrystProgress* rtn = new TrystProgress();
    if (rtn && rtn->init(max, left)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

CCSize TrystProgress::sizeOfContent() const {
    return planeSize;
}


#pragma mark - Inner

bool TrystProgress::init(int max, int left) {
    if (CCNode::create()) {
        this->max = max;
        this->left = left;
        this->savedTs = DATA->cur_timestamp();
        
        float offset = 40;
        plane = CCSprite::create("pic/tryst/tryst_progress.png");
        plane->setPosition(ccp(-offset, 0));
        this->addChild(plane);
        
        planeSize = CCSizeMake(plane->getContentSize().width, plane->getContentSize().height);
        
        //
        CCSprite* bar1 = CCSprite::create("pic/tryst/tryst_progress_bar1.png");
        bar1->setPosition(ccp(planeSize.width * 0.5 - 34, planeSize.height * 0.5 + 1.6));
        plane->addChild(bar1);
        
        barWidth = bar1->getContentSize().width;
        
        CCSprite* bar2 = CCSprite::create("pic/tryst/tryst_progress_bar2.png");
        prog = CCProgressTimer::create(bar2);
        prog->setType(kCCProgressTimerTypeBar);
        prog->setMidpoint(ccp(0, 0.5));
        prog->setBarChangeRate(ccp(1, 0));
        prog->setPosition(bar1->getPosition());
        plane->addChild(prog);
        
        //
        countDown = CCLabelAtlas::create("00:00", "pic/baseScene/base_number3.png", 14, 20, '0');
        countDown->setScale(0.9);
        countDown->setAnchorPoint(ccp(0.5, 0.5));
        countDown->setPosition(ccp(planeSize.width - 44, bar1->getPositionY()));
        plane->addChild(countDown);
        
        //
        heart = CCSprite::create("pic/tryst/tryst_progress_heart.png");
        heart->setAnchorPoint(ccp(0.35, 0.5));
        heart->setPosition(ccp(0, bar1->getPositionY()));
        plane->addChild(heart);
        
        //
        CCSprite* box1 = CCSprite::create("pic/tryst/tryst_reward_box.png");
        CCSprite* box2 = CCSprite::create("pic/tryst/tryst_reward_box.png");
        box2->setScale(1.01);
        CCMenuItem* itemBox = CCMenuItemSprite::create(box1, box2, this, SEL_MenuHandler(&TrystProgress::onOpenRewardBox));
        CCMenu* menuBox = CCMenu::createWithItem(itemBox);
        menuBox->setPosition(260, 30);
        this->addChild(menuBox);
        
        return true;
    }
    else {
        return false;
    }
}

void TrystProgress::onEnter() {
    CCNode::onEnter();
    
    this->updateProgress(0);
    this->schedule(SEL_SCHEDULE(&TrystProgress::updateProgress), 1);
}

void TrystProgress::onExit() {

    CCNode::onExit();
}

void TrystProgress::updateProgress(float dt) {
    time_t nowTS = DATA->cur_timestamp();
    time_t delta = nowTS - savedTs;
    time_t curLeftSencods = left - delta;

    if (curLeftSencods > 0) {
        int minute = int(curLeftSencods) / 60;
        int second = int(curLeftSencods) % 60;
        CCString* timeLeft = CCString::createWithFormat("%02d:%02d", minute, second);
        countDown->setString(timeLeft->getCString());
    }
    else {
        this->unschedule(SEL_SCHEDULE(&TrystProgress::updateProgress));
        
        
        
    }
    
    //
    if (max > 0) {
        float percent = MIN((max - (float)curLeftSencods) / max, 1.0);
        prog->setPercentage(percent * 100);
        heart->setPosition(ccp(barWidth * percent, heart->getPositionY()));
    }
}

void TrystProgress::onOpenRewardBox(CCMenuItem *pSender) {
    
}

