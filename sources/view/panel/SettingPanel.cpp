//
//  SettingPanel.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "SettingPanel.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "DataManager.h"

SettingPanel::~SettingPanel() {
}

bool SettingPanel::init() {
    if (CCLayer::init()) {
        
        _content = CCLayer::create();
//        _content->setScale(0.1);
        this->addChild(_content);
//        _content->setVisible(false);
        
        _panel = CCSprite::create("res/pic/panel/setting/set_panel.png");
        _panel->setPosition(ccp(DISPLAY->W() * 0.33, DISPLAY->H() * 0.155));
        _panel->setAnchorPoint(CCPointZero);
        _content->addChild(_panel);
        
        CCSize panelSize = _panel->boundingBox().size;
        
        CCMenuItemImage* btn_music_off = CCMenuItemImage::create("res/pic/panel/setting/set_music_off.png", "res/pic/panel/setting/set_music_off.png");
        CCMenuItemImage* btn_music_on = CCMenuItemImage::create("res/pic/panel/setting/set_music_on.png", "res/pic/panel/setting/set_music_on.png");
        CCMenuItemToggle* toggle_music = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&SettingPanel::on_music), btn_music_off, btn_music_on, NULL);
        toggle_music->setSelectedIndex((int)AUDIO->is_music_on());
        
        CCMenuItemImage* btn_effect_off = CCMenuItemImage::create("res/pic/panel/setting/set_effect_off.png", "res/pic/panel/setting/set_effect_off.png");
        CCMenuItemImage* btn_effect_on = CCMenuItemImage::create("res/pic/panel/setting/set_effect_on.png", "res/pic/panel/setting/set_effect_on.png");
        CCMenuItemToggle* toggle_effect = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&SettingPanel::on_effect), btn_effect_off, btn_effect_on, NULL);
        toggle_effect->setSelectedIndex((int)AUDIO->is_effect_on());
        
        CCMenu* menu = CCMenu::create(toggle_music, toggle_effect, NULL);
        menu->setPosition(ccp(panelSize.width * 0.5, panelSize.height * 0.52));
        menu->alignItemsHorizontallyWithPadding(panelSize.width * 0.18);
        _panel->addChild(menu);
        
        CCSprite* back_nor = CCSprite::create("res/pic/panel/setting/relogin.png");
        CCSprite* back_sel = CCSprite::create("res/pic/panel/setting/relogin.png");
        back_sel->setScale(1.02f);
        CCMenuItemSprite* item_back = CCMenuItemSprite::create(back_nor, back_sel, this, menu_selector(SettingPanel::on_back));
        item_back->setPosition(ccp(panelSize.width * 0.8, panelSize.height * 0.18));
        CCMenu* menu_back = CCMenu::create(item_back, NULL);
        menu_back->setPosition(CCPointZero);
        _panel->addChild(menu_back);
        
        return true;
    }
    else {
        return false;
    }
}

void SettingPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
//    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&SettingPanel::keyBackStatus), .8f);
}
void SettingPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void SettingPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool SettingPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

#pragma mark - export

void SettingPanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    _panel->setPosition(_enter_pos + ccp(20, 25));
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void SettingPanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.1f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    _content->runAction(spawn);
}

void SettingPanel::do_exit() {
    float duration = 0.1f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&SettingPanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(spawn);
}

void SettingPanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void SettingPanel::on_music(cocos2d::CCMenuItem *btn) {
    //    CCDirector::sharedDirector()->replaceScene(scene);
    CCMenuItemToggle* item = (CCMenuItemToggle*)btn;
    int index = item->getSelectedIndex();
    CCLOG("music getSelectedIndex = %d", index);
    AUDIO->set_music_on((bool)index);
}

void SettingPanel::on_effect(cocos2d::CCMenuItem *btn) {
    CCMenuItemToggle* item = (CCMenuItemToggle*)btn;
    int index = item->getSelectedIndex();
    CCLOG("effect getSelectedIndex = %d", index);
    AUDIO->set_effect_on((bool)index);
}

void SettingPanel::on_back(CCMenuItem *btn){
    DATA->setAutoLogin(false);
    DATA->relogin();
}

void SettingPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}
