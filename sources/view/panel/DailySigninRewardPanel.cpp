//
//  DailySigninRewardPanel.cpp
//  tiegao
//
//

#include "DailySigninRewardPanel.h"

#include "DisplayManager.h"
#include "NetManager.h"
#include "AudioManager.h"

#include "Loading2.h"
#include "PromptLayer.h"

#pragma mark - Export

void DailySigninRewardPanel::show(CCNode* parent) {
    DailySigninRewardPanel* panel = DailySigninRewardPanel::create();
    parent->addChild(panel);
}


#pragma mark - Super

DailySigninRewardPanel::~DailySigninRewardPanel() {
}

bool DailySigninRewardPanel::init() {
    if (! CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    
    couldClose = false;
    
    rewardDisplay = NULL;
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);

    CCSprite* mask = CCSprite::create("pic/panel/dailysignin/ds_mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    CCSprite* title = CCSprite::create("pic/panel/dailysignin/ds_title.png");
    title->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75));
    this->addChild(title);
    
    CCSprite* promptText = CCSprite::create("pic/panel/dailysignin/ds_text_tishi.png");
    promptText->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.68));
    this->addChild(promptText);
    
    this->create_best_show();
    
    panel = CCNode::create();
    panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.55));
    this->addChild(panel);
    panel->setScale(0.1);
    panel->setVisible(true);
    
    
    boxItem = CCMenuItemImage::create("pic/panel/dailysignin/ds_box_0.png", "pic/panel/dailysignin/ds_box_1.png", this, SEL_MenuHandler(&DailySigninRewardPanel::showReward));
    CCMenu* boxMenu = CCMenu::createWithItem(boxItem);
    boxMenu->setPosition(CCPointZero);
    panel->addChild(boxMenu);

    return true;
}

void DailySigninRewardPanel::onEnter() {
    CCLayer::onEnter();

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&DailySigninRewardPanel::after_commit_daily_signin_323), "HTTP_FINISHED_323", NULL);
    
    this->do_enter();

    this->scheduleOnce(SEL_SCHEDULE(&DailySigninRewardPanel::keyBackStatus), .8f);
}

void DailySigninRewardPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void DailySigninRewardPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool DailySigninRewardPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (couldClose && ! panel->boundingBox().containsPoint(location)) {
        this->remove();
    }
    
    return true;
}

#pragma mark - Inner

void DailySigninRewardPanel::create_best_show() {
    CCSprite* best = CCSprite::create("pic/tryst/tryst_reward_diam.png");
    best->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.26));
    this->addChild(best);
    
    CCSize baseSize = best->getContentSize();
    
    CCSprite* baseText = CCSprite::create("pic/panel/dailysignin/ds_text_zuijia.png");
    baseText->setPosition(ccp(baseSize.width * 0.5, baseSize.height + 14));
    best->addChild(baseText);
    
    CCLabelTTF* lblNum = CCLabelTTF::create("100", DISPLAY->fangzhengFont(), 22);
    lblNum->setColor(ccc3(179, 85, 123));
    lblNum->setPosition(ccp(baseSize.width * 0.5, baseSize.height * 0.2));
    best->addChild(lblNum);
}

void DailySigninRewardPanel::remove() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("DAILYSIGNIN_PANEL_EXIT");
    this->do_exit();
    this->removeFromParentAndCleanup(true);
}

void DailySigninRewardPanel::keyBackClicked(){
    num_child++;
    CCLog("===== DailySigninRewardPanel  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->remove();
}

void DailySigninRewardPanel::do_enter() {
    float duration1 = 0.2f;
    CCMoveTo* moveto1 = CCMoveTo::create(duration1, DISPLAY->center());
    CCScaleTo* scaleto1 = CCScaleTo::create(duration1, 1.1);
    CCSpawn* spawn1 = CCSpawn::create(moveto1, scaleto1, NULL);
    
    float duration2 = 0.2f;
    CCMoveTo* moveto2 = CCMoveTo::create(duration2, DISPLAY->center());
    CCScaleTo* scaleto2 = CCScaleTo::create(duration2, 1);
    CCCallFunc* done = CCCallFunc::create(this, SEL_CallFunc(&DailySigninRewardPanel::startBoxAnimation));
    CCSpawn* spawn2 = CCSpawn::create(moveto2, scaleto2, done, NULL);
    
    panel->runAction(CCEaseSineIn::create(CCSequence::create(spawn1, spawn2, NULL)));
}

void DailySigninRewardPanel::do_exit() {
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

void DailySigninRewardPanel::startBoxAnimation() {
    this->schedule(SEL_SCHEDULE(&DailySigninRewardPanel::showBoxAnimation), 0.4);
}

void DailySigninRewardPanel::showBoxAnimation() {
    if (boxItem->isSelected()) {
        boxItem->unselected();
    }
    else {
        boxItem->selected();
    }
}

void DailySigninRewardPanel::showReward() {
    this->unschedule(SEL_SCHEDULE(&DailySigninRewardPanel::showBoxAnimation));
    
    LOADING->show_loading();
    NET->commit_daily_signin_323();
}

void DailySigninRewardPanel::after_commit_daily_signin_323(CCObject *pObj) {
    LOADING->removeFromParent();
    
    boxItem->getParent()->removeFromParent();
    
    CCDictionary* reward = dynamic_cast<CCDictionary*>(pObj);
    this->type = string(reward->valueForKey("type")->getCString());
    this->num = ((CCInteger*)reward->objectForKey("num"))->getValue();
    
    box = CCSprite::create("pic/panel/dailysignin/ds_box_2.png");
    panel->addChild(box);
    
    boxlSize = box->boundingBox().size;
    
    CCSprite* light = CCSprite::create("pic/building/reward/light.png");
    light->setPosition(ccp(boxlSize.width* .5f, boxlSize.height * 1.15));
    box->addChild(light);
    
    
    CCCallFunc* call = CCCallFunc::create(this, SEL_CallFunc(&DailySigninRewardPanel::changeRewardDisplay));
    CCSequence* seq1 = CCSequence::create(call, CCDelayTime::create(0.1), NULL);
    CCCallFunc* done = CCCallFunc::create(this, SEL_CallFunc(&DailySigninRewardPanel::chooseRewardDisplay));
    CCSequence* seq2 = CCSequence::create(CCRepeat::create(seq1, 20), done, NULL);
    this->runAction(seq2);
}

void DailySigninRewardPanel::changeRewardDisplay() {
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
        rewardDisplay->setPosition(ccp(boxlSize.width * 0.5, boxlSize.height * 0.5));
        box->addChild(rewardDisplay);
    }
}

void DailySigninRewardPanel::chooseRewardDisplay() {
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
        rewardDisplay->setPosition(ccp(boxlSize.width * 0.5, boxlSize.height * 0.5));
        box->addChild(rewardDisplay, 10);
        CCCallFunc* showLight = CCCallFunc::create(this, SEL_CallFunc(&DailySigninRewardPanel::showLightAndRewardNum));
        rewardDisplay->runAction(CCSequence::create(CCDelayTime::create(0.3), CCScaleTo::create(0.3, 1.2f), showLight, NULL));
    }
}

void DailySigninRewardPanel::showLightAndRewardNum() {
    CCSprite* light = CCSprite::create("pic/tryst/tryst_light.png");
    light->setPosition(ccp(boxlSize.width * 0.5, boxlSize.height * 0.5));
    light->runAction(CCRepeatForever::create(CCRotateBy::create(3, -360)));
    box->addChild(light, rewardDisplay->getZOrder() - 1);
    
    CCLabelTTF* lblNum = CCLabelTTF::create(CCString::createWithFormat("%d", this->num)->getCString(), DISPLAY->fangzhengFont(), 22);
    lblNum->setAnchorPoint(ccp(0.5, 0.5));
    lblNum->setColor(ccc3(179, 85, 123));
    lblNum->setPosition(ccp(rewardDisplay->getContentSize().width * 0.5, rewardDisplay->getContentSize().height * 0.2));
    rewardDisplay->addChild(lblNum);
    
    couldClose = true;
}

