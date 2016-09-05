//
//  EnergyLargessPanel.cpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#include "EnergyLargessPanel.h"
//
//  EnergyLargessPanel.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "EnergyLargessPanel.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "Loading2.h"

#pragma mark - Export

void EnergyLargessPanel::show(CCNode* parent) {
    EnergyLargessPanel* panel = EnergyLargessPanel::create();
    parent->addChild(panel);
}


#pragma mark - Super

EnergyLargessPanel::~EnergyLargessPanel() {
}

bool EnergyLargessPanel::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("pic/panel/energylargess/energylargess_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        CCSize panelSize = _panel->boundingBox().size;
        
        CCSprite* take1 = CCSprite::create("pic/panel/energylargess/energylargess_btn_take.png");
        CCSprite* take2 = CCSprite::create("pic/panel/energylargess/energylargess_btn_take.png");
        take2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btnTake = CCMenuItemSprite::create(take1, take2, this, SEL_MenuHandler(&EnergyLargessPanel::on_take));
        _menu = CCMenu::createWithItem(btnTake);
        _menu->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.1));
        _panel->addChild(_menu);
        
        return true;
    }
    else {
        return false;
    }
}

void EnergyLargessPanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&EnergyLargessPanel::nc_take_energy_301), "HTTP_FINISHED_301", NULL);
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->scheduleOnce(SEL_SCHEDULE(&EnergyLargessPanel::keyBackStatus), .8f);
    
    this->update_state();
}

void EnergyLargessPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void EnergyLargessPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool EnergyLargessPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return false;
}

void EnergyLargessPanel::nc_take_energy_301(CCObject *pObj) {
    LOADING->remove();
    this->update_state();
    
    CCDictionary* dic = CCDictionary::create();
    dic->setObject( (CCInteger*)pObj, "num");
    dic->setObject(CCString::createWithFormat("{%f,%f}", DISPLAY->halfW(), DISPLAY->H() * 0.4), "from");
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_ENERGY_FLY", dic);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}

#pragma mark - Inner

void EnergyLargessPanel::update_state() {
    int energy1 = DATA->getNews()->energy1;
    int energy2 = DATA->getNews()->energy2;
    if (energy1 == 1 || energy2 == 1) {
        
    }
    else {
        _menu->setEnabled(false);
        _menu->setColor(ccGRAY);
    }
}

void EnergyLargessPanel::remove() {
    AUDIO->goback_effect();
    
    this->removeFromParentAndCleanup(true);
}

void EnergyLargessPanel::on_take(CCMenuItem *) {
    LOADING->show_loading();
    NET->take_energy_reward_301();
}

void EnergyLargessPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}
