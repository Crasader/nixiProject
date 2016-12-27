//
//  WelfarePanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "WelfarePanel.h"

#include "DisplayManager.h"
#include "AudioManager.h"

#include "DailyWelfareLayer.h"
#include "AchievementLayer.h"

const int LAYER_ZORDER = 10;

#pragma mark - Export

void WelfarePanel::show() {
    WelfarePanel* panel = WelfarePanel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}

#pragma mark - Supper

WelfarePanel::~WelfarePanel() {
}

bool WelfarePanel::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _panel = CCSprite::create("res/pic/welfare/welfare_panel.png");
        _panel->setPosition(DISPLAY->center());
        this->addChild(_panel);
        
        CCSize panelSize = _panel->getContentSize();
        
        _dailywelfare = NULL;
        _achievement = NULL;
        
        CCMenuItemImage* dailyItem1 = CCMenuItemImage::create("pic/welfare/panel_btn_daily1.png", "pic/welfare/panel_btn_daily1.png");
        CCMenuItemImage* dailyItem2 = CCMenuItemImage::create("pic/welfare/panel_btn_daily2.png", "pic/welfare/panel_btn_daily2.png");
        CCMenuItemToggle* daily = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&WelfarePanel::onDailyToggle), dailyItem1, dailyItem2, NULL);
        
        CCMenuItemImage* achieveItem1 = CCMenuItemImage::create("pic/welfare/panel_btn_achieve1.png", "pic/welfare/panel_btn_achieve1.png");
        CCMenuItemImage* achieveItem2 = CCMenuItemImage::create("pic/welfare/panel_btn_achieve2.png", "pic/welfare/panel_btn_achieve2.png");
        CCMenuItemToggle* achieve = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&WelfarePanel::onAchieveToggle), achieveItem1, achieveItem2, NULL);
        
        CCMenu* menu = CCMenu::create(daily, achieve, NULL);
        menu->alignItemsVertically();
        menu->setPosition(menu->getPosition() + ccp(panelSize.width * 0.488, panelSize.height * 0.28));
        this->addChild(menu);
        
        CCSprite* txt_close = CCSprite::create("pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        this->addChild(txt_close);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        _dailywelfare = DailyWelfareLayer::create();
        this->addChild(_dailywelfare, LAYER_ZORDER);
        daily->setSelectedIndex(1);
        
        return true;
    }
    else {
        return false;
    }
}

void WelfarePanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&WelfarePanel::remove), "NEED_REMOVE", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&WelfarePanel::keyBackStatus), .8f);
    
}

void WelfarePanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool WelfarePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint touchLocation = pTouch->getLocation();
    CCLOG("WelfarePanel::ccTouchBegan()");
    if (_dailywelfare->isVisible()) {
        if (! _dailywelfare->getBoundary().containsPoint(touchLocation)) {
            remove();
        }
    }
    
    return true;
}

void WelfarePanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

#pragma mark - inner

void WelfarePanel::remove() {
    AUDIO->goback_effect();
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void WelfarePanel::keyBackClicked(){
    num_child++;
    CCLog("===== WelfarePanel  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}

void WelfarePanel::onDailyToggle(CCMenuItemToggle *btn) {
    bool isVisible = btn->getSelectedIndex() == 1 ? true : false;
    _dailywelfare->setVisible(isVisible);

}

void WelfarePanel::onAchieveToggle(CCMenuItemToggle *btn) {
    if (! _achievement) {
        _achievement = AchievementLayer::create();
        this->addChild(_achievement, LAYER_ZORDER);
    }
}

