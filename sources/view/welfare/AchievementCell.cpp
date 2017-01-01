//
//  AchievementCell.cpp
//  tiegao
//
//  Created by mac on 16/12/24.
//
//

#include "AchievementCell.h"

#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"

#include "AchievementComp.h"
#include "Loading2.h"

AchievementCell::~AchievementCell() {

}

AchievementCell* AchievementCell::create(const char *pszFileName) {
    AchievementCell* pRet = new AchievementCell();
    if (pRet && pRet->init(pszFileName))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool AchievementCell::init(const char *pszFileName) {
    if (!CCSprite::initWithFile(pszFileName)) {
        return false;
    }
    
    return true;
}


void AchievementCell::configWithAchievementItem(int idx, AchievementItem *item, float cellWidth, float cellHeight) {
    float halfCellWidth = cellWidth * 0.5;
    float halfCellHeight = cellHeight * 0.5;
    
    CCLabelTTF* lblName = CCLabelTTF::create(item->getDesc().c_str(), DISPLAY->fangzhengFont(), 26);
    lblName->setAnchorPoint(ccp(0, 0.5));
    lblName->setColor(ccc3(159, 55, 98));
    lblName->setPosition(ccp(cellWidth * 0.03, halfCellHeight));
    this->addChild(lblName);
    
    _rewardIcon = CCSprite::create("pic/welfare/welfare_reward_plate_diam.png");
    _rewardIcon->setPosition(ccp(cellWidth * 0.65, halfCellHeight));
    this->addChild(_rewardIcon);
    
    CCString* strNum = CCString::createWithFormat("%d", item->getNum());
    CCLabelTTF* lblRewardNum = CCLabelTTF::create(strNum->getCString(), DISPLAY->fangzhengFont(), 20);
    lblRewardNum->setColor(ccc3(227, 112, 86));
    lblRewardNum->setPosition(_rewardIcon->getPosition() + ccp(17, -2));
    this->addChild(lblRewardNum);
    
    _sptButton1 = CCSprite::create("pic/welfare/welfare_btn_take.png");
    _sptButton2 = CCSprite::create("pic/welfare/welfare_btn_take.png");
    _sptButton2->setScale(1.005);
    CCMenuItem* btn = CCMenuItemSprite::create(_sptButton1, _sptButton2, this, SEL_MenuHandler(&AchievementCell::on_btn_take));
    btn->setUserObject(CCInteger::create(item->getId()));
    _menuBtn = CCMenu::createWithItem(btn);
    _menuBtn->setPosition(ccp(cellWidth * 0.88, halfCellHeight));
    _menuBtn->setEnabled(false);
    this->addChild(_menuBtn);
    
    
    float progressFontSize = 30.f;
    
    CCSprite* progressBottom = CCSprite::create("pic/welfare/welfare_item_bar_1.png");
    progressBottom->setPosition(ccp(halfCellWidth, cellHeight * 0.15));
    this->addChild(progressBottom);
    
    CCSize barSize = progressBottom->getContentSize();
    
    _progress = CCProgressTimer::create(CCSprite::create("pic/welfare/welfare_item_bar_2.png"));
    _progress->setPosition(progressBottom->getPosition());
    _progress->setType(kCCProgressTimerTypeBar);
    _progress->setMidpoint(ccp(0, 0.5));
    _progress->setBarChangeRate(ccp(1, 0));
    this->addChild(_progress);
    
    _star = CCSprite::create("pic/welfare/welfare_star1.png");
    _progress->addChild(_star);
    
    int status = item->getStatus();
    if (status == 1) {
        CCLabelTTF* lblTake1 = CCLabelTTF::create("领取", DISPLAY->fangzhengFont(), 30);
        lblTake1->setPosition(ccp(_sptButton1->getContentSize().width * 0.5, _sptButton1->getContentSize().height * 0.5));
        _sptButton1->addChild(lblTake1);
        
        CCLabelTTF* lblTake2 = CCLabelTTF::create("领取", DISPLAY->fangzhengFont(), 30);
        lblTake2->setPosition(lblTake1->getPosition());
        _sptButton2->addChild(lblTake2);
        
        _star->setPosition(ccp(_progress->getContentSize().width, barSize.height * 0.5));
        _star->runAction(CCRepeatForever::create( CCRotateBy::create(1.2, 360)) );
        _progress->setPercentage(100);
        _menuBtn->setEnabled(true);
    }
    else if (status == -1) {
        CCLabelTTF* lblTake1 = CCLabelTTF::create("已领", DISPLAY->fangzhengFont(), 30);
        lblTake1->setPosition(ccp(_sptButton1->getContentSize().width * 0.5, _sptButton1->getContentSize().height * 0.5));
        _sptButton1->addChild(lblTake1);
        
        _star->setPosition(ccp(_progress->getContentSize().width, barSize.height * 0.5));
        _progress->setPercentage(100);
        _menuBtn->setEnabled(false);
        
        CCSprite* sptMask = CCSprite::create("pic/welfare/welfare_mask.png");
        sptMask->setPosition(ccp(cellWidth * 0.5, cellHeight * 0.5));
        this->addChild(sptMask);
    }
    else {
        int accumulate = DATA->getAchievement()->fetchItemAccumulate(item->getId());
        CCString* strProgressHead = CCString::createWithFormat("%d", accumulate);
        CCLabelTTF* lblProgressHead = CCLabelTTF::create(strProgressHead->getCString(), DISPLAY->fangzhengFont(), progressFontSize);
        lblProgressHead->setColor(ccc3(246, 255, 4));
        lblProgressHead->setAnchorPoint(ccp(1, 0.5));
        lblProgressHead->setPosition(ccp(_sptButton1->getContentSize().width * 0.45, _sptButton1->getContentSize().height * 0.5));
        _sptButton1->addChild(lblProgressHead);
        
        CCString* strProgressTail = CCString::createWithFormat("/%d", item->getGoal());
        CCLabelTTF* lblProgressTail = CCLabelTTF::create(strProgressTail->getCString(), DISPLAY->fangzhengFont(), progressFontSize);
        lblProgressTail->setAnchorPoint(ccp(0, 0.5));
        lblProgressTail->setPosition(lblProgressHead->getPosition());
        _sptButton1->addChild(lblProgressTail);
        
        float per = accumulate * 0.1 / item->getGoal() * 10;
        _progress->setPercentage(per * 100) ;
        _star->setPosition(ccp(_progress->getContentSize().width * per, barSize.height * 0.5));
        _menuBtn->setEnabled(false);
    }
}

void AchievementCell::goStar(int idx, int progress, int goal, int status) {
    float per = MIN(progress * 0.1 / goal * 10, 1.0);
    float duration = 1.f * per;

    _progress->runAction(CCProgressFromTo::create(duration, 0, per * 100));
    
    _star = CCSprite::create("pic/welfare/welfare_star1.png");
    _progress->addChild(_star);
    CCSize progressSize = _progress->getContentSize();
    float toX = progressSize.width * per;
    
    CCCallFuncO* finish = CCCallFuncO::create(this, SEL_CallFuncO(&AchievementCell::moveFinish), CCInteger::create(status));
    CCSequence* starSeq = CCSequence::create(CCPlace::create(ccp(0, progressSize.height * 0.5)), CCMoveTo::create(duration, ccp(toX, progressSize.height * 0.5)), finish, NULL);
    _star->runAction(starSeq);
}

void AchievementCell::moveFinish(CCObject* pObj) {
    if (pObj) {
        int status = ((CCInteger* )pObj)->getValue();
        if (status == 0) {
            _menuBtn->setEnabled(false);
        }
        else if (status == 1 && _star) {
            _star->runAction(CCRepeatForever::create( CCRotateBy::create(1.2, 360)) );
            
            _sptButton1->removeAllChildren();
            CCLabelTTF* lblTake1 = CCLabelTTF::create("领取", DISPLAY->fangzhengFont(), 30);
            lblTake1->setPosition(ccp(_sptButton1->getContentSize().width * 0.5, _sptButton1->getContentSize().height * 0.5));
            _sptButton1->addChild(lblTake1);
            
            CCLabelTTF* lblTake2 = CCLabelTTF::create("领取", DISPLAY->fangzhengFont(), 30);
            lblTake2->setPosition(lblTake1->getPosition());
            _sptButton2->addChild(lblTake2);
            
            _menuBtn->setEnabled(true);
        }
        else if (status == -1) {
            _sptButton1->removeAllChildren();
            CCLabelTTF* lblTake1 = CCLabelTTF::create("已领", DISPLAY->fangzhengFont(), 30);
            lblTake1->setPosition(ccp(_sptButton1->getContentSize().width * 0.5, _sptButton1->getContentSize().height * 0.5));
            _sptButton1->addChild(lblTake1);
            
            _menuBtn->setEnabled(false);
//            _sptButton1->removeAllChildren();
//            CCSprite* gougou = CCSprite::create("pic/welfare/welfare_done.png");
//            gougou->setPosition(ccp(_sptButton1->getContentSize().width * 0.5, _sptButton1->getContentSize().height * 0.5));
//            _sptButton1->addChild(gougou);
//            
//            _menuBtn->setEnabled(false);
        }
    }
}

void AchievementCell::on_btn_take(CCMenuItem *btn) {
    LOADING->show_loading();
    CCInteger* id = (CCInteger* )btn->getUserObject();
    NET->take_achievement_reward_641(id->getValue());
}

void AchievementCell::showRewardAction(const char* type, int num) {
    if (! _rewardIcon) {
        return;
    }
    
    CCDictionary* postData = CCDictionary::create();
    postData->setObject(CCInteger::create(num), "num");
    CCPoint pos = _rewardIcon->getPosition();
    CCPoint worldPos = this->convertToWorldSpace(pos);
    CCString* from = CCString::createWithFormat("{%f,%f}", worldPos.x, worldPos.y);
    CCLOG("from -- %s", from->getCString());
    postData->setObject(from, "from");
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_GOLD_FLY", postData);
    if (strcmp(type, "coin") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_COIN_FLY", postData);
    }
    else if (strcmp(type, "diam") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_GOLD_FLY", postData);
    }
    else if (strcmp(type, "energy") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_ENERGY_FLY", postData);
    }
    else if (strcmp(type, "piece") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_PIECE_FLY", postData);
    }
}




