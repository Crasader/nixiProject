//
//  TrystRewardPanel.cpp
//  tiegao
//

#include "TrystRewardPanel.h"

#include "DisplayManager.h"
#include "AudioManager.h"

#include "Loading2.h"
#include "PromptLayer.h"


#pragma mark - Export

void TrystRewardPanel::show(CCNode* parent, string type, int num) {
    TrystRewardPanel* panel = TrystRewardPanel::createWithReward(type, num);
    parent->addChild(panel);
}


#pragma mark - Super

TrystRewardPanel::~TrystRewardPanel() {
}

TrystRewardPanel* TrystRewardPanel::createWithReward(string type, int num) {
    TrystRewardPanel* rtn = new TrystRewardPanel();
    if (rtn && rtn->initWithReward(type, num)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool TrystRewardPanel::initWithReward(string type, int num) {
    if (! CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    
    couldClose = false;
    
    rewardDisplay = NULL;
    this->type = string(type.c_str());
    this->num = num;
    
//    CCSprite* mask = CCSprite::create("res/pic/mask.png");
//    mask->setPosition(DISPLAY->center());
//    this->addChild(mask);
    
//    panel = CCSprite::create("pic/building/reward/panel.png");
    panel = CCSprite::create("pic/tryst/tryst_box_open.png");
    
    panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75));
    panel->setVisible(false);
    this->addChild(panel);
    
    panelSize = panel->boundingBox().size;
    
    CCSprite* light = CCSprite::create("pic/building/reward/light.png");
    light->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
    panel->addChild(light);
    
//    CCSprite* spots = CCSprite::create("pic/building/reward/spots.png");
//    spots->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
//    panel->addChild(spots);
//    
//    CCSequence* seq = CCSequence::create(CCFadeIn::create(0.6), CCDelayTime::create(0.3), CCFadeOut::create(0.6), CCDelayTime::create(0.5), NULL);
//    spots->runAction(CCRepeatForever::create(seq));
    
//    CCSprite* title = CCSprite::create("pic/building/reward/title2.png");
//    title->setPosition(ccp(panelSize.width* .5f, panelSize.height* .92f));
//    panel->addChild(title);
    
    
//    if (_type.compare("coin") == 0) {
//        CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
//        plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
//        panel->addChild(plate);
//        
//        CCSprite* coin = CCSprite::create("pic/common/coin2.png");
//        coin->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
//        panel->addChild(coin);
//        
//        CCString* str = CCString::createWithFormat("%d 金币", _value);
//        CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24);
//        lbl->setColor(ccc3(107, 143, 190));
//        lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.28));
//        panel->addChild(lbl);
//    }
//    else if (_type.compare("diam") == 0) {
//        CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
//        plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
//        panel->addChild(plate);
//        
//        CCSprite* diam = CCSprite::create("pic/common/diam2.png");
//        diam->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.5));
//        panel->addChild(diam);
//        
//        CCString* str = CCString::createWithFormat("%d 钻石", _value);
//        CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24);
//        lbl->setColor(ccc3(107, 143, 190));
//        lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * 0.28));
//        panel->addChild(lbl);
//    }
    
//    moveNode = CCNode::create();
//    moveNode->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.5));
//    panel->addChild(moveNode);
//    
//    attachedNode1 = this->createAttachedNode();
//    attachedNode2 = this->createAttachedNode();
//    
//    moveNode->addChild(attachedNode1);
//    attachedNode2->setPosition(ccp(0, GRID_HEIGHT * GRID_COUNT));
//    moveNode->addChild(attachedNode2);
    
    
    return true;
}

void TrystRewardPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&TrystRewardPanel::keyBackStatus), .8f);
}

void TrystRewardPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void TrystRewardPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool TrystRewardPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (couldClose && ! panel->boundingBox().containsPoint(location)) {
        this->remove();
    }
    
    return true;
}

#pragma mark - Inner

void TrystRewardPanel::remove() {
    this->do_exit();
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void TrystRewardPanel::keyBackClicked(){
    num_child++;
    CCLog("===== TrystRewardPanel  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}

void TrystRewardPanel::do_enter() {
    panel->setScale(0.1);
    panel->setVisible(true);
    
    float duration1 = 0.2f;
    CCMoveTo* moveto1 = CCMoveTo::create(duration1, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.45));
    CCScaleTo* scaleto1 = CCScaleTo::create(duration1, 1.1);
    CCSpawn* spawn1 = CCSpawn::create(moveto1, scaleto1, NULL);
    
    float duration2 = 0.2f;
    CCMoveTo* moveto2 = CCMoveTo::create(duration2, DISPLAY->center());
    CCScaleTo* scaleto2 = CCScaleTo::create(duration2, 1);
    CCCallFunc* done = CCCallFunc::create(this, SEL_CallFunc(&TrystRewardPanel::showReward));
    CCSpawn* spawn2 = CCSpawn::create(moveto2, scaleto2, done, NULL);
    
    panel->runAction(CCEaseSineIn::create(CCSequence::create(spawn1, spawn2, NULL)));
}

void TrystRewardPanel::do_exit() {
    AUDIO->goback_effect();
    
    CCPoint posFrom = DISPLAY->center();
    CCDictionary* postData = CCDictionary::create();
    postData->setObject(CCInteger::create(num), "num");
    CCString* from = CCString::createWithFormat("{%f,%f}", posFrom.x, posFrom.y);
    CCLOG("from -- %s", from->getCString());
    postData->setObject(from, "from");
    
    if (type.compare("coin") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_COIN_FLY", postData);
    }
    else if (type.compare("diam") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_GOLD_FLY", postData);
    }
    else if (type.compare("energy") == 0) {
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_ENERGY_FLY", postData);
    }
}

//CCNode* TrystRewardPanel::createAttachedNode() {
//    CCNode* rtn = CCNode::create();
//    CCSprite* grid = NULL;
//    for (int i = 0; i < GRID_COUNT; i++) {
//        grid = CCSprite::create("pic/tryst/tryst_reward_plate.png");
//        grid->setAnchorPoint(ccp(0.5, 0));
//        grid->setPosition(ccp(0, GRID_HEIGHT * i));
//        rtn->addChild(grid);
//    }
//    
//    return rtn;
//}

//void TrystRewardPanel::showReward() {
//    float spawnDuration = 0.5f;
//    int count = 5;
//    CCSprite* grid = NULL;
//    CCPoint basePos = ccp(panelSize.width * 0.5, panelSize.height * 0.5);
//    for (int i = 0; i < count; i++) {
//        grid = CCSprite::create("pic/tryst/tryst_reward_plate.png");
//        grid->setPosition(basePos + ccp(CCRANDOM_MINUS1_1() * 5, CCRANDOM_MINUS1_1() * 5));
//        grid->setScale(0.2);
//        grid->setVisible(false);
//        panel->addChild(grid);
//        
//        CCSpawn* spawn = CCSpawn::create(CCScaleTo::create(spawnDuration, 1), CCMoveTo::create(spawnDuration, basePos), NULL);
//        CCSequence* seq = CCSequence::create(CCShow::create(), CCDelayTime::create(0.5 * i), CCScaleTo::create(0.3, 1.1), CCDelayTime::create(0.2), spawn, NULL);
//        grid->runAction(seq);
//    }
//}

void TrystRewardPanel::showReward() {
    CCCallFunc* call = CCCallFunc::create(this, SEL_CallFunc(&TrystRewardPanel::changeRewardDisplay));
    CCSequence* seq1 = CCSequence::create(call, CCDelayTime::create(0.1), NULL);
    CCCallFunc* done = CCCallFunc::create(this, SEL_CallFunc(&TrystRewardPanel::chooseRewardDisplay));
    CCSequence* seq2 = CCSequence::create(CCRepeat::create(seq1, 20), done, NULL);
    this->runAction(seq2);
}

void TrystRewardPanel::changeRewardDisplay() {
    float ran = CCRANDOM_0_1();
    
    if (rewardDisplay) {
        rewardDisplay->removeFromParent();
        rewardDisplay = NULL;
    }
    
    if (ran < 0.25) {
        if (lastType == 0) {
            rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_diam.png");
        }
        else {
            rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_coin.png");
        }
        lastType = 0;
    }
    else if (ran < 0.5) {
        if (lastType == 1) {
            rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_piece.png");
        }
        else {
            rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_energy.png");
        }
        lastType = 1;
    }
    else if (ran < 0.75) {
        if (lastType == 2) {
            rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_coin.png");
        }
        else {
            rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_piece.png");
        }
        lastType = 2;
    }
    else {
        if (lastType == 3) {
            rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_energy.png");
        }
        else {
            rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_diam.png");
        }
        lastType = 3;
    }
    
    if (rewardDisplay) {
        rewardDisplay->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.5));
        panel->addChild(rewardDisplay);
    }
}

void TrystRewardPanel::chooseRewardDisplay() {
    if (rewardDisplay) {
        rewardDisplay->removeFromParent();
        rewardDisplay = NULL;
    }
    
    if (type.compare("coin") == 0) {
        rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_coin.png");
    }
    else if (type.compare("energy") == 0) {
        rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_energy.png");
    }
    else if (type.compare("piece") == 0) {
        rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_piece.png");
    }
    else if (type.compare("diam") == 0) {
        rewardDisplay = CCSprite::create("pic/tryst/tryst_reward_diam.png");
    }
    
    if (rewardDisplay) {
        rewardDisplay->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.5));
        panel->addChild(rewardDisplay, 10);
        CCCallFunc* showLight = CCCallFunc::create(this, SEL_CallFunc(&TrystRewardPanel::showLightAndRewardNum));
        rewardDisplay->runAction(CCSequence::create(CCDelayTime::create(0.3), CCScaleTo::create(0.3, 1.2f), showLight, NULL));
    }
}

void TrystRewardPanel::showLightAndRewardNum() {
    CCSprite* light = CCSprite::create("pic/tryst/tryst_light.png");
    light->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.5));
    light->runAction(CCRepeatForever::create(CCRotateBy::create(3, -360)));
    panel->addChild(light, rewardDisplay->getZOrder() - 1);
    
    CCLabelTTF* lblNum = CCLabelTTF::create(CCString::createWithFormat("%d", this->num)->getCString(), DISPLAY->fangzhengFont(), 22);
    lblNum->setColor(ccc3(179, 85, 123));
    lblNum->setPosition(ccp(rewardDisplay->getContentSize().width * 0.5, rewardDisplay->getContentSize().height * 0.2));
    rewardDisplay->addChild(lblNum);
    
    couldClose = true;
}

