//
//  GameCheckoutPanel.cpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#include "GameCheckoutPanel.h"
//
//  GameCheckoutPanel.cpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#include "GameCheckoutPanel.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "AudioManager.h"
#include "Loading2.h"
#include "PromptLayer.h"

#pragma mark - Export

void GameCheckoutPanel::show(CCNode* parent, string gameId, int score, CCDictionary* firstInfo) {
    GameCheckoutPanel* panel = GameCheckoutPanel::createWithScore(gameId, score, firstInfo);
    parent->addChild(panel);
}


#pragma mark - Super

GameCheckoutPanel::~GameCheckoutPanel() {
}

GameCheckoutPanel* GameCheckoutPanel::createWithScore(string gameId, int score, CCDictionary* firstInfo) {
    GameCheckoutPanel* rtn = new GameCheckoutPanel();
    if (rtn && rtn->initWithScore(gameId, score, firstInfo)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool GameCheckoutPanel::initWithScore(string gameId, int score, CCDictionary* firstInfo) {
    if (! CCLayer::init()) {
        return false;
    }

    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    _panel = CCSprite::create("pic/panel/game_checkout/panel.png");
    _panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75));
    _panel->setVisible(false);
    this->addChild(_panel);
    
    CCSize panelSize = _panel->boundingBox().size;
    
    CCSprite* light = CCSprite::create("pic/building/reward/light.png");
    light->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
    _panel->addChild(light);
    
    CCSprite* spots = CCSprite::create("pic/building/reward/spots.png");
    spots->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
    _panel->addChild(spots);
    
    CCSequence* seq = CCSequence::create(CCFadeIn::create(0.6), CCDelayTime::create(0.3), CCFadeOut::create(0.6), CCDelayTime::create(0.5), NULL);
    spots->runAction(CCRepeatForever::create(seq));
    
    CCSprite* title = CCSprite::create("pic/panel/game_checkout/title_plate.png");
    title->setPosition(ccp(panelSize.width* .5f, panelSize.height));
    _panel->addChild(title);
    
    int scorePadding = 70;
    
    if (firstInfo && firstInfo->count() > 0) {
        CCString* name = (CCString*)firstInfo->objectForKey("name");
        CCLabelTTF* lblFirstName = CCLabelTTF::create(name->getCString(), DISPLAY->fangzhengFont(), 28);
        lblFirstName->setPosition(ccp(panelSize.width * 0.48f, panelSize.height * 0.72));
        lblFirstName->setColor(ccc3(255, 129, 129));
        _panel->addChild(lblFirstName);
        
        
        CCLabelTTF* lbl1 = CCLabelTTF::create("最高得分：", DISPLAY->fangzhengFont(), 28);
        lbl1->setPosition(ccp(panelSize.width * 0.45f, panelSize.height * 0.64));
        lbl1->setColor(ccc3(223, 154, 154));
        _panel->addChild(lbl1);
        
        CCInteger* score = (CCInteger*)firstInfo->objectForKey("score");
        CCString* strScore = CCString::createWithFormat("%d", score->getValue());
        CCLabelAtlas* lblFirstScore = CCLabelAtlas::create(strScore->getCString(), "res/pic/baseScene/base_number.png", 14, 20, '0');
        lblFirstScore->setAnchorPoint(ccp(0, 0.5));
        lblFirstScore->setPosition(ccp(lbl1->getPositionX() + scorePadding, lbl1->getPositionY()));
        lblFirstScore->setScale(1.2);
        _panel->addChild(lblFirstScore);
    }
    
    CCLabelTTF* lbl2 = CCLabelTTF::create("最高得分：", DISPLAY->fangzhengFont(), 28);
    lbl2->setPosition(ccp(panelSize.width * 0.45f, panelSize.height * 0.35));
    lbl2->setColor(ccc3(223, 154, 154));
    _panel->addChild(lbl2);
    
    CCDictionary* games = DATA->getHome()->getScores();
    CCString* strHighestScore = CCString::createWithFormat("%d", ((CCInteger*)games->objectForKey(gameId))->getValue());
    CCLabelAtlas* lblHighest = CCLabelAtlas::create(strHighestScore->getCString(), "res/pic/baseScene/base_number.png", 14, 20, '0');
    lblHighest->setAnchorPoint(ccp(0, 0.5));
    lblHighest->setPosition(ccp(lbl2->getPositionX() + scorePadding, lbl2->getPositionY()));
    lblHighest->setScale(1.2);
    _panel->addChild(lblHighest);
    
    
    CCLabelTTF* lbl3 = CCLabelTTF::create("当前得分：", DISPLAY->fangzhengFont(), 28);
    lbl3->setPosition(ccp(panelSize.width * 0.45f, panelSize.height * 0.28));
    lbl3->setColor(ccc3(223, 154, 154));
    _panel->addChild(lbl3);
    
    CCString* strCurScore = CCString::createWithFormat("%d", score);
    CCLabelAtlas* lblCurScore = CCLabelAtlas::create(strCurScore->getCString(), "res/pic/baseScene/base_number.png", 14, 20, '0');
    lblCurScore->setAnchorPoint(ccp(0, 0.5));
    lblCurScore->setPosition(ccp(lbl3->getPositionX() + scorePadding, lbl3->getPositionY()));
    lblCurScore->setScale(1.2);
    _panel->addChild(lblCurScore);
    
    return true;
}

void GameCheckoutPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&GameCheckoutPanel::keyBackStatus), .8f);
}
void GameCheckoutPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void GameCheckoutPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool GameCheckoutPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->remove();
    }
    
    return true;
}

#pragma mark - Inner

void GameCheckoutPanel::remove() {
    this->do_exit();
    this->removeFromParentAndCleanup(true);
}

void GameCheckoutPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLOG("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}

void GameCheckoutPanel::do_enter() {
    _panel->setScale(0.1);
    _panel->setVisible(true);
    
    float duration1 = 0.2f;
    CCMoveTo* moveto1 = CCMoveTo::create(duration1, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.45));
    CCScaleTo* scaleto1 = CCScaleTo::create(duration1, 1.1);
    CCSpawn* spawn1 = CCSpawn::create(moveto1, scaleto1, NULL);
    
    float duration2 = 0.2f;
    CCMoveTo* moveto2 = CCMoveTo::create(duration2, DISPLAY->center());
    CCScaleTo* scaleto2 = CCScaleTo::create(duration2, 1);
    CCSpawn* spawn2 = CCSpawn::create(moveto2, scaleto2, NULL);
    
    _panel->runAction(CCEaseSineIn::create(CCSequence::create(spawn1, spawn2, NULL)));
}

void GameCheckoutPanel::do_exit() {
    
}