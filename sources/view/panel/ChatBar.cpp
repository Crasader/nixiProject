//
//  ChatBar.cpp
//  tiegao
//
//  Created by mac on 16/9/28.
//
//

#include "ChatBar.h"

#include "DisplayManager.h"
#include "DataManager.h"


#pragma mark - Export

void ChatBar::update_num(int num) {
    CCString* strNum = CCString::createWithFormat("%d", num);
    if (_num && num < 100) {
        _num->set_new_number(strNum->getCString());
        if (_isIdle) {
            this->display_next_chat();
        }
    }
}

void ChatBar::update_display() {
    int count = DATA->getChat()->getItems()->count();
    int newCount = DATA->getChat()->getNewChatCount();
    _curDisplayIndex = count - newCount -1;
    _curDisplayIndex = MAX(_curDisplayIndex, 0);
    if (newCount != 0) {
        this->display_chat(_curDisplayIndex);
    }
}


#pragma mark - Inherit

ChatBar::~ChatBar() {
    
}

bool ChatBar::init() {
    if (CCLayer::init()) {
        _curDisplayIndex = 0;
        _isIdle = true;
        
        CCSprite* bar1 = CCSprite::create("pic/panel/chat_bar/chat_bar.png");
        CCSprite* bar2 = CCSprite::create("pic/panel/chat_bar/chat_bar.png");
        bar2->setScale(1.01);
        _bar = CCMenuItemSprite::create(bar1, bar2, this, SEL_MenuHandler(&ChatBar::onClicked));
        _bar->setPosition(ccp(0, DISPLAY->halfH() - 78));
        _container = CCMenu::createWithItem(_bar);
        this->addChild(_container);
        
        _content = CCNode::create();
        _bar->addChild(_content);
        
        return true;
    }
    else {
        return false;
    }
}

void ChatBar::onEnter() {
    CCLayer::onEnter();
    
    this->show_nmu_plate();
}

void ChatBar::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

#pragma mark - Inner

void ChatBar::do_enter() {
}

void ChatBar::do_exit() {
}

void ChatBar::remove() {
    this->removeFromParentAndCleanup(true);
}

void ChatBar::show_nmu_plate() {
    _plate = CCSprite::create("pic/panel/chat_bar/chat_num_plate.png");
    _plate->setPosition(DISPLAY->center() + ccp(_bar->getContentSize().width * (-0.5), _bar->getPositionY()) + ccp(0, -22));
    this->addChild(_plate);
    
    CCSize plateSize = _plate->getContentSize();
    
    
    CCSprite* halo = CCSprite::create("pic/panel/chat_bar/chat_halo.png");
    halo->setPosition(ccp(plateSize.width * 0.5, plateSize.height * 0.5));
    _plate->addChild(halo);
    halo->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1), CCDelayTime::create(0.4), CCFadeIn::create(1), CCDelayTime::create(0.4), NULL)));
    
    
    CCSprite* circle0 = CCSprite::create("pic/panel/chat_bar/chat_circle_0.png");
    circle0->setPosition(ccp(plateSize.width * 0.5, plateSize.height * 0.5));
    _plate->addChild(circle0);
    circle0->runAction(CCRepeatForever::create(CCSequence::create(CCHide::create(), CCDelayTime::create(0.7), CCShow::create(), CCDelayTime::create(2.1), NULL)));
    
    
    CCSprite* circle1 = CCSprite::create("pic/panel/chat_bar/chat_circle_1.png");
    circle1->setPosition(ccp(plateSize.width * 0.5, plateSize.height * 0.5));
    _plate->addChild(circle1);
    circle1->runAction(CCRepeatForever::create(CCSequence::create(CCHide::create(), CCDelayTime::create(1.4), CCShow::create(), CCDelayTime::create(1.4), NULL)));
    
    CCString* startNum = CCString::createWithFormat("%d", DATA->getChat()->getNewChatCount());
    _num = FlashNumberLabel::create_with_atlas("res/pic/baseScene/base_number4.png", startNum->getCString(), 0, .01f);
    _num->setAnchorPoint(ccp(.5f, .5f));
    _num->setScale(0.9);
    _num->set_delegate(this); // 设置代理
    _num->setPosition(ccp(plateSize.width * 0.5, plateSize.height * 0.5));
    _plate->addChild(_num);
}

void ChatBar::display_chat(int index) {
    CCLOG("ChatBar::display_chat(%d)", index);
    CCArray* items = DATA->getChat()->getItems();
    int count = items->count();
    if (index < count - 1 || (count == 1 && index == 0)) {
        ChatItem* item = (ChatItem*)items->objectAtIndex(index);
        if (item) {
            _isIdle = false;
            _curDisplayIndex = index;
            this->display_chat_content(item->name.c_str(), item->chat.c_str());
        }
    }
    else {
        // 隐藏
        _isIdle = true;
    }
}

void ChatBar::display_chat_content(const char *name, const char *content) {
    _content->removeAllChildrenWithCleanup(true);
    
    float barWidth = _bar->getContentSize().width;
    float barHeight = _bar->getContentSize().height;
    
    CCString* nameFormat = CCString::createWithFormat("%s: ", name);
    CCLabelTTF* lblName = CCLabelTTF::create(nameFormat->getCString(), DISPLAY->fangzhengFont(), 24);
    lblName->setColor(DISPLAY->dullBlueColor());
    lblName->setAnchorPoint(ccp(0, 0.5));
    lblName->setPosition(ccp(0, barHeight * 0.5));
    _content->addChild(lblName);
    
    float nameWidth = lblName->getContentSize().width;
    
    CCSprite* stencil = CCSprite::create();
    stencil->setTextureRect(CCRect(nameWidth, 0, barWidth - nameWidth - 6, barHeight));
    stencil->setAnchorPoint(CCPointZero);
    stencil->setPosition(ccp(nameWidth, 0));
    
    CCClippingNode* node = CCClippingNode::create(stencil);
    _content->addChild(node);
    
    CCLabelTTF* lblContent = CCLabelTTF::create(content, DISPLAY->fangzhengFont(), 24);
    lblContent->setColor(DISPLAY->defalutColor());
    lblContent->setAnchorPoint(ccp(0, 0.5));
    node->addChild(lblContent);
    
    CCCallFunc* next = CCCallFunc::create(this, SEL_CallFunc(&ChatBar::display_next_chat));
    
    float showSize = nameWidth + lblContent->getContentSize().width;
    if (showSize < barWidth) {
//        lblContent->setPosition(ccp(nameWidth, barHeight));
//        lblContent->runAction(CCSequence::create(CCMoveBy::create(1.0, ccp(0, barHeight * (-0.5))), CCDelayTime::create(2), next, NULL));
        lblContent->setPosition(ccp(nameWidth, barHeight * 0.5));
        lblContent->runAction(CCSequence::create(CCFadeIn::create(1.0), CCDelayTime::create(2), CCFadeOut::create(1.0), next, NULL));
    }
    else {
        lblContent->setPosition(ccp(nameWidth + barWidth * 0.15, barHeight * 0.5));
        float distance = lblContent->getContentSize().width - lblContent->getPositionX() - nameWidth;
        if (distance <= 0) {
            lblContent->setPosition(ccp(nameWidth, barHeight * 0.5));
            lblContent->runAction(CCSequence::create(CCFadeIn::create(1.0), CCDelayTime::create(2), CCFadeOut::create(1.0), next, NULL));
        }
        else {
            lblContent->runAction(CCSequence::create(CCMoveBy::create(MAX(distance / 34, 1.2), ccp(-distance, 0)), CCDelayTime::create(1.2), next, NULL));
        }
    }
}

void ChatBar::display_next_chat() {
    this->display_chat(_curDisplayIndex + 1);
}

void ChatBar::onClicked(CCMenuItem *btn) {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("ON_CHAT_BAR_CLICKED");
}

void ChatBar::will_number_scroll(){
    
}

void ChatBar::did_number_stoped(){
    
}
