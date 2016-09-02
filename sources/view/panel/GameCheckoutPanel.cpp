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
#include "HomeLayer.h"
#include "TaskScene.h"
#include "NetManager.h"

#pragma mark - Export

void GameCheckoutPanel::show(CCNode* parent, string gameId, int score, int history, CCDictionary* firstInfo) {
    GameCheckoutPanel* panel = GameCheckoutPanel::createWithScore(gameId, score, history, firstInfo);
    parent->addChild(panel);
}


#pragma mark - Super

GameCheckoutPanel::~GameCheckoutPanel() {
}

GameCheckoutPanel* GameCheckoutPanel::createWithScore(string gameId, int score, int history, CCDictionary* firstInfo) {
    GameCheckoutPanel* rtn = new GameCheckoutPanel();
    if (rtn && rtn->initWithScore(gameId, score, history, firstInfo)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}

bool GameCheckoutPanel::initWithScore(string gameId, int score, int history, CCDictionary* firstInfo) {
    if (! CCLayer::init()) {
        return false;
    }
    
    gameScore = 0;
    gameScore = score;
    
    _couldRemove = false;

    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    _panel = CCSprite::create("pic/panel/game_checkout/panel.png");
    _panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75));
    _panel->setVisible(false);
    this->addChild(_panel);
    
    CCSize panelSize = _panel->boundingBox().size;
    
    CCSprite* title = CCSprite::create("pic/panel/game_checkout/title_plate.png");
    title->setPosition(ccp(panelSize.width* .5f, panelSize.height));
    _panel->addChild(title);
    
    int scorePadding = 70;
    
    int firstScore = ((CCInteger*)firstInfo->objectForKey("score"))->getValue();
    
    if (score <= history) {
        CCLabelTTF* lblTitle = CCLabelTTF::create("通 关", DISPLAY->fangzhengFont(), 42);
        lblTitle->setPosition(title->getPosition() + ccp(0, 3));
        _panel->addChild(lblTitle);
    }
    else if (score >= firstScore) {
        CCLabelTTF* lblTitle = CCLabelTTF::create("人生巅峰", DISPLAY->fangzhengFont(), 40);
        lblTitle->setPosition(title->getPosition() + ccp(0, 3));
        _panel->addChild(lblTitle);
    }
    else {
        CCLabelTTF* lblTitle = CCLabelTTF::create("超越自己", DISPLAY->fangzhengFont(), 40);
        lblTitle->setPosition(title->getPosition() + ccp(0, 3));
        _panel->addChild(lblTitle);
    }
    
    if (firstInfo && firstInfo->count() > 0) {
        CCString* name = (CCString*)firstInfo->objectForKey("name");
        CCLabelTTF* lblFirstName = CCLabelTTF::create(name->getCString(), DISPLAY->fangzhengFont(), 28);
        lblFirstName->setPosition(ccp(panelSize.width * 0.48f, panelSize.height * 0.72));
        lblFirstName->setColor(ccc3(255, 129, 129));
        _panel->addChild(lblFirstName);
        
        
        CCLabelTTF* lbl1 = CCLabelTTF::create("最高得分：", DISPLAY->fangzhengFont(), 28);
        lbl1->setPosition(ccp(panelSize.width * 0.45f, panelSize.height * 0.65));
        lbl1->setColor(ccc3(223, 154, 154));
        _panel->addChild(lbl1);
        
        
        CCString* strScore = CCString::createWithFormat("%d", firstScore);
        CCLabelAtlas* lblFirstScore = CCLabelAtlas::create(strScore->getCString(), "pic/baseScene/base_number.png", 14, 20, '0');
        lblFirstScore->setAnchorPoint(ccp(0, 0.5));
        lblFirstScore->setPosition(ccp(lbl1->getPositionX() + scorePadding, lbl1->getPositionY()));
        lblFirstScore->setScale(1.2);
        _panel->addChild(lblFirstScore);
    }
    
    CCLabelTTF* lbl2 = CCLabelTTF::create("最高得分：", DISPLAY->fangzhengFont(), 28);
    lbl2->setPosition(ccp(panelSize.width * 0.45f, panelSize.height * 0.365));
    lbl2->setColor(ccc3(223, 154, 154));
    _panel->addChild(lbl2);
    
    CCString* strHistoryScore = CCString::createWithFormat("%d", history);
    CCLabelAtlas* lblHistory = CCLabelAtlas::create(strHistoryScore->getCString(), "pic/baseScene/base_number.png", 14, 20, '0');
    lblHistory->setAnchorPoint(ccp(0, 0.5));
    lblHistory->setPosition(ccp(lbl2->getPositionX() + scorePadding, lbl2->getPositionY()));
    lblHistory->setScale(1.2);
    _panel->addChild(lblHistory);
    
    
    CCLabelTTF* lbl3 = CCLabelTTF::create("当前得分：", DISPLAY->fangzhengFont(), 28);
    lbl3->setPosition(ccp(panelSize.width * 0.45f, panelSize.height * 0.29));
    lbl3->setColor(ccc3(223, 154, 154));
    _panel->addChild(lbl3);
    
    CCString* strCurScore = CCString::createWithFormat("%d", score);
    CCLabelAtlas* lblCurScore = CCLabelAtlas::create(strCurScore->getCString(), "pic/baseScene/base_number.png", 14, 20, '0');
    lblCurScore->setAnchorPoint(ccp(0, 0.5));
    lblCurScore->setPosition(ccp(lbl3->getPositionX() + scorePadding, lbl3->getPositionY()));
    lblCurScore->setScale(1.2);
    _panel->addChild(lblCurScore);
    
    CCSprite* barBottom = CCSprite::create("pic/panel/game_checkout/bar_bottom.png");
    barBottom->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.14));
    _panel->addChild(barBottom);
    
    CCSize barSize = barBottom->getContentSize();
    
    CCProgressTimer* bar = CCProgressTimer::create(CCSprite::create("pic/panel/game_checkout/bar.png"));
    bar->setType(kCCProgressTimerTypeBar);
    bar->setAnchorPoint(ccp(0, 0.5));
    bar->setPosition(ccp(0, barSize.height * 0.5));
    bar->setMidpoint(ccp(0, 1));
    bar->setBarChangeRate(ccp(1, 0));
    barBottom->addChild(bar);
    
    CCSprite* barTop = CCSprite::create("pic/panel/game_checkout/bar_covering.png");
    barTop->setPosition(barBottom->getPosition());
    _panel->addChild(barTop);
    
    CCSprite* light = CCSprite::create("pic/panel/game_checkout/light.png");
    light->setPosition(ccp(0, barSize.height * 0.5));
    barBottom->addChild(light);
    
    float dur = 1.0;
    float per = (float)score / firstScore;
    bar->runAction(CCProgressFromTo::create(dur, 0, per * 100));
    
    CCSequence* seq = CCSequence::create(CCMoveBy::create(dur, ccp(barSize.width * per, 0)), CCDelayTime::create(1.0), CCCallFunc::create(this, SEL_CallFunc(&GameCheckoutPanel::on_anim_finish)), NULL);
    light->runAction(seq);
    
    {
        CCString* strScore = CCString::createWithFormat("%d", firstScore);
        CCLabelAtlas* lblFirstScore = CCLabelAtlas::create(strScore->getCString(), "pic/baseScene/base_number.png", 14, 20, '0');
        lblFirstScore->setAnchorPoint(ccp(0.8, 0.5));
        lblFirstScore->setPosition(ccp(barSize.width, barSize.height * 1.7));
        bar->addChild(lblFirstScore);
        
        CCString* strCurScore = CCString::createWithFormat("%d", score);
        CCLabelAtlas* lblCurScore = CCLabelAtlas::create(strCurScore->getCString(), "pic/baseScene/base_number.png", 14, 20, '0');
        lblCurScore->setAnchorPoint(ccp(0.8, 0.5));
        lblCurScore->setPosition(ccp(barSize.width * per, barSize.height * (- 0.7)));
        bar->addChild(lblCurScore);
    }
    
    return true;
}

void GameCheckoutPanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(GameCheckoutPanel::_605CallBack), "HTTP_FINISHED_605", NULL);
    
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
        if (_couldRemove) {
            this->remove();
        }
    }
    
    return true;
}

#pragma mark - Inner

void GameCheckoutPanel::remove() {
//    this->do_exit();
//    this->removeFromParentAndCleanup(true);
    if (DATA->getTaskGameBool2() || DATA->getTaskGameBool3() || DATA->getTaskGameBool5()) {
        LOADING->show_loading();
        if (DATA->getTaskGameBool2()) {
            NET->commit_extra_mission_605(DATA->getTaskTempID(), 2, gameScore);
        }else if (DATA->getTaskGameBool3()){
            NET->commit_extra_mission_605(DATA->getTaskTempID(), 3, gameScore);
        }else if (DATA->getTaskGameBool5()){
            NET->commit_extra_mission_605(DATA->getTaskTempID(), 5, gameScore);
        }
    }else{
        CCScene* scene = HomeLayer::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}
void GameCheckoutPanel::_605CallBack(CCObject* pObj){
    LOADING->remove();
    
    DATA->setTaskPhase(DATA->getPlayer()->phase);
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
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

void GameCheckoutPanel::on_anim_finish() {
    _couldRemove = true;
}