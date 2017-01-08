//
//  RankListCell.cpp
//  tiegao
//
//  Created by mac on 16/12/24.
//
//

#include "RankListCell.h"

#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"

#include "ShowComp.h"

#include "Loading2.h"

CCSprite* RankListCell::getNumSprite(int num){
    CCSprite* spr;
    switch (num) {
        case 0:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_0.png");
            break;
        case 1:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_1.png");
            break;
        case 2:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_2.png");
            break;
        case 3:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_3.png");
            break;
        case 4:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_4.png");
            break;
        case 5:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_5.png");
            break;
        case 6:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_6.png");
            break;
        case 7:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_7.png");
            break;
        case 8:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_8.png");
            break;
        case 9:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_9.png");
            break;
        default:
            break;
    }
    
    return spr;
}



RankListCell::~RankListCell() {

}

RankListCell* RankListCell::create(const char *pszFileName) {
    RankListCell* pRet = new RankListCell();
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

bool RankListCell::init(const char *pszFileName) {
    if (!CCSprite::initWithFile(pszFileName)) {
        return false;
    }
    
    return true;
}


void RankListCell::config(int idx, ShowComp *show, float cellWidth, float cellHeight, int type) {
    float halfCellWidth = cellWidth * 0.5;
    float halfCellHeight = cellHeight * 0.5;
    
    this->setPosition(ccp(halfCellWidth, halfCellHeight));
    
    const char* nickname = show->nickname();
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(cellWidth * 0.75f, cellHeight * 0.68f));
    name->setColor(ccc3(109, 98, 96));
    name->setTag(0x10500);
    this->addChild(name);
    
    if (type == 1) {
        CCSprite* flag = CCSprite::create("pic/ranklist/rl_text_competition.png");
        flag->setPosition(ccp(cellWidth * 0.8f, cellHeight* .375));
        flag->setAnchorPoint(ccp(1, 0.5));
        flag->setTag(0x10111);
        this->addChild((flag));
        
        CCString* collect_str = CCString::createWithFormat("%d", show->competition());
        CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 20);
        cloth_count->setPosition(flag->getPosition() + ccp(2, -1.5));
        cloth_count->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count->setTag(0x10300);
        this->addChild(cloth_count);
    }
    else {
        CCSprite* flag = CCSprite::create("pic/haoyoupaihang/text_collected_nor.png");
        flag->setPosition(ccp(cellWidth * 0.8f, cellHeight* .375));
        flag->setAnchorPoint(ccp(1, 0.5));
        flag->setTag(0x10111);
        this->addChild((flag));
        
        CCString* collect_str = CCString::createWithFormat("%d", show->collected());
        CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 20);
        cloth_count->setPosition(flag->getPosition() + ccp(2, -1.5));
        cloth_count->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count->setTag(0x10300);
        this->addChild(cloth_count);
    }
    
    int num = idx + 1;
    float numSprPosX = cellWidth * 0.17f;
    if (num < 10) {
        CCSprite* spr = RankListCell::getNumSprite(num);
        spr->setPosition(ccp(numSprPosX, cellHeight* .5f));
        this->addChild(spr);
    }
    else if (num < 100) {
        CCSprite* spr1 = RankListCell::getNumSprite((int)floor(num/10));
        spr1->setPosition(ccp(numSprPosX - 7, halfCellHeight));
        this->addChild(spr1);
        CCSprite* spr2 = RankListCell::getNumSprite((int)floor(num%10));
        spr2->setPosition(ccp(numSprPosX + 7, halfCellHeight));
        this->addChild(spr2);
    }
    else if (num < 1000) {
        int baiwei = (int)floor(num / 100);
        CCSprite* spr1 = RankListCell::getNumSprite(baiwei);
        spr1->setPosition(ccp(numSprPosX - 12, halfCellHeight));
        this->addChild(spr1);
        
        int shiwei = (int)floor((num - baiwei * 100) / 10);
        CCSprite* spr2 = RankListCell::getNumSprite(shiwei);
        spr2->setPosition(ccp(numSprPosX, halfCellHeight));
        this->addChild(spr2);
        
        int gewei = (int)floor(num % 10);
        CCSprite* spr3 = RankListCell::getNumSprite(gewei);
        spr3->setPosition(ccp(numSprPosX + 12, halfCellHeight));
        this->addChild(spr3);
    }

    
    CCSprite* add_spr1 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr2 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
    CCSprite* add_spr5 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCSprite* add_spr6 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    add_spr2->setScale(1.02f);
    CCMenuItemSprite* item_add;
    
    if (show->getShowID().compare(DATA->getLogin()->obtain_sid()) == 0) {
        //这是我自己
        
    }else {
        if (DATA->getSocial()->is_friend(show->getShowID().c_str())) {
            //是好友
            item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
            item_add->setTag(idx);
            item_add->setUserObject(CCInteger::create(idx + 3));
            CCMenu* menu_add = CCMenu::create(item_add, NULL);
            menu_add->setPosition(ccp(cellWidth  - add_spr3->getContentSize().width/2 -10, 8));
            menu_add->setTag(0x10900);
            menu_add->setVisible(false);
            this->addChild(menu_add);
        }else{
            bool hasRequested = DATA->getSocial()->is_requested(show->getShowID().c_str());
            //不是好友
            if (! hasRequested || show->isadd == 0) {
                item_add = CCMenuItemSprite::create(add_spr1, add_spr2, this, menu_selector(RankListCell::toBeFriend));
                item_add->setTag(idx);
                item_add->setUserObject(CCInteger::create(idx + 3));
                CCMenu* menu_add = CCMenu::create(item_add, NULL);
                menu_add->setPosition(ccp(cellWidth  - add_spr1->getContentSize().width/2 -10, 8));
                menu_add->setTag(0x10900);
                menu_add->setVisible(false);
                this->addChild(menu_add);
            }else{
                item_add = CCMenuItemSprite::create(add_spr5, add_spr6, this, NULL);
                item_add->setTag(idx);
                item_add->setUserObject(CCInteger::create(idx + 3));
                CCMenu* menu_add = CCMenu::create(item_add, NULL);
                menu_add->setPosition(ccp(cellWidth  - add_spr3->getContentSize().width/2 -10, 8));
                menu_add->setTag(0x10900);
                menu_add->setVisible(false);
                this->addChild(menu_add);
            }
        }
    }
}

void RankListCell::goStar(int idx, int progress, int goal, int status) {
    float per = MIN(progress * 0.1 / goal * 10, 1.0);
    float duration = 1.f * per;

    _progress->runAction(CCProgressFromTo::create(duration, 0, per * 100));
    
    _star = CCSprite::create("pic/welfare/welfare_star1.png");
    _progress->addChild(_star);
    CCSize progressSize = _progress->getContentSize();
    float toX = progressSize.width * per;
    
    CCCallFuncO* finish = CCCallFuncO::create(this, SEL_CallFuncO(&RankListCell::moveFinish), CCInteger::create(status));
    CCSequence* starSeq = CCSequence::create(CCPlace::create(ccp(0, progressSize.height * 0.5)), CCMoveTo::create(duration, ccp(toX, progressSize.height * 0.5)), finish, NULL);
    _star->runAction(starSeq);
}

void RankListCell::moveFinish(CCObject* pObj) {
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

void RankListCell::toBeFriend(CCMenuItem* btn){
    CCMenuItem* item = (CCMenuItem* )btn;
    int idx = ((CCInteger*)item->getUserObject())->getValue();
    
    //    CCNode* bg = (CCNode* )item->getParent()->getParent();
    //    bg->removeChildByTag(0x10900, true);
    //    CCArray* rankers = DATA->getRanking()->ranking();
    //    ShowComp* other = (ShowComp*)rankers->objectAtidx(idx);
    //    other->isadd = 1;
    //
    //    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    //    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    //    CCMenuItemSprite* item_add;
    //    item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
    //    CCMenu* menu_add = CCMenu::create(item_add, NULL);
    //    menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
    //    menu_add->setTag(0X10900);
    //    bg->addChild(menu_add);
    //
    //    NET->send_message_803(other->getShowID().c_str(), 1);
}

//void RankListCell::showRewardAction(const char* type, int num) {
//    if (! _rewardIcon) {
//        return;
//    }
//    
//    CCDictionary* postData = CCDictionary::create();
//    postData->setObject(CCInteger::create(num), "num");
//    CCPoint pos = _rewardIcon->getPosition();
//    CCPoint worldPos = this->convertToWorldSpace(pos);
//    CCString* from = CCString::createWithFormat("{%f,%f}", worldPos.x, worldPos.y);
//    CCLOG("from -- %s", from->getCString());
//    postData->setObject(from, "from");
//    
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_GOLD_FLY", postData);
//    if (strcmp(type, "coin") == 0) {
//        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_COIN_FLY", postData);
//    }
//    else if (strcmp(type, "diam") == 0) {
//        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_GOLD_FLY", postData);
//    }
//    else if (strcmp(type, "energy") == 0) {
//        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_ENERGY_FLY", postData);
//    }
//    else if (strcmp(type, "piece") == 0) {
//        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_PIECE_FLY", postData);
//    }
//}




