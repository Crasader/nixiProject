//
//  GiftPanel.cpp
//  tiegao
//
//  Created by mac on 16/9/5.
//
//

#include "GiftPanel.h"
//
//  GiftPanel.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "GiftPanel.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "NetManager.h"
#include "DataManager.h"
#include "FileManager.h"
#include "PromptLayer.h"
#include "Loading2.h"

#pragma mark - Export


#pragma mark - Inherit

GiftPanel::~GiftPanel() {
    
}

bool GiftPanel::init() {
    if (CCLayer::init()) {
//        CCSprite* mask = CCSprite::create("res/pic/mask.png");
//        mask->setPosition(DISPLAY->center());
//        this->addChild(mask);
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("pic/panel/nickname_reset/nnr_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        
        CCSize panelSize = _panel->getContentSize();
        
        CCSize size_bar = CCSizeMake(262, 40);
        _eb = CCEditBox::create(CCSizeMake(size_bar.width, size_bar.height), CCScale9Sprite::create("pic/panel/nickname_reset/nnr_bar.png"));
        _eb->setMaxLength(30);
        _eb->setFontColor(DISPLAY->defalutColor());
        _eb->setPlaceHolder("点此输入礼包码");
        _eb->setFontName(DISPLAY->fangzhengFont());
        _eb->setInputMode(kEditBoxInputModeAny);
        _eb->setReturnType(kKeyboardReturnTypeDone);
        _eb->setPosition(ccp(panelSize.width * 0.38, panelSize.height * 0.5));
        _panel->addChild(_eb);
        
        CCSprite* spt1 = CCSprite::create("pic/panel/nickname_reset/nnr_commit.png");
        CCSprite* spt2 = CCSprite::create("pic/panel/nickname_reset/nnr_commit.png");
        spt2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btnCommit = CCMenuItemSprite::create(spt1, spt2, this, SEL_MenuHandler(&GiftPanel::onBtnCommit));
        CCMenu* menu = CCMenu::createWithItem(btnCommit);
        menu->ignoreAnchorPointForPosition(true);
        menu->setPosition(ccp(panelSize.width * 0.83, panelSize.height * 0.5));
        _panel->addChild(menu);
        
        CCLabelTTF* lblCost = CCLabelTTF::create("领取礼包", DISPLAY->fangzhengFont(), 20);
        lblCost->setAnchorPoint(ccp(0.5, 0.5));
        lblCost->setPosition(ccp(spt1->getContentSize().width * 0.5, spt1->getContentSize().height * 0.5));
        btnCommit->addChild(lblCost);
        
        return true;
    }
    else {
        return false;
    }
}

void GiftPanel::onEnter() {
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    //    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&GiftPanel::keyBackStatus), .8f);
}

void GiftPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void GiftPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

bool GiftPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->remove();
    }
    
    return true;
}

#pragma mark - inner

void GiftPanel::do_enter() {
//    _content->setPosition(_enter_pos - DISPLAY->center());
//    _content->setVisible(true);
//    
//    float duration = 0.1f;
//    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
//    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
//    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
//    _content->runAction(spawn);
}

void GiftPanel::do_exit() {
//    float duration = 0.1f;
//    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
//    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&GiftPanel::remove)), NULL);
//    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
//    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
//    _content->runAction(spawn);
}

void GiftPanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void GiftPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}

void GiftPanel::onBtnCommit(CCMenuItem *btn) {
    string text = _eb->getText();
    LOADING->show_loading();
    NET->take_gift_333(0, text.c_str());
    this->remove();
}
