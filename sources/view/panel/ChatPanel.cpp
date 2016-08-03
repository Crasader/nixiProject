//
//  ChatPanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/7/29.
//
//

#include "ChatPanel.h"
#include "DisplayManager.h"
#include "ChatTableView.h"
#include "WSManager.h"


ChatPanel::~ChatPanel(){
    
}

bool ChatPanel::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    this->initChatPanel();
    
    
    
    return true;
}

void ChatPanel::onEnter(){
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
}

void ChatPanel::onExit(){
    CCLayer::onExit();
}

void ChatPanel::initChatPanel(){
    _panel_bg = CCSprite::create("res/pic/panel/chat/panel_bg.png");
    _panel_bg->setPosition(ccp(DISPLAY->halfW(), DISPLAY->halfH()));
    this->addChild(_panel_bg);
    
    this->initTopMessage();
    
    ChatTableView* chatView = ChatTableView::create();
    chatView->setPosition(ccp(_panel_bg->getContentSize().width* .5f, _panel_bg->getContentSize().height* .10f));
    _panel_bg->addChild(chatView);
    
    _input_bg = CCSprite::create("res/pic/panel/chat/input.png");
    _input_bg->setPosition(ccp(_panel_bg->getContentSize().width* .4f, 45));
    _panel_bg->addChild(_input_bg);
    
    CCSprite* send_spr = CCSprite::create("res/pic/panel/chat/btn_send.png");
    CCSprite* send_spr2 = CCSprite::create("res/pic/panel/chat/btn_send.png");
    send_spr2->setScale(1.02f);
    CCMenuItemSprite* item_send = CCMenuItemSprite::create(send_spr, send_spr2, this, menu_selector(ChatPanel::btn_sendMessage));
    CCMenu* menu_send = CCMenu::create(item_send, NULL);
    menu_send->setPosition(ccp(_panel_bg->getContentSize().width* .87f, _input_bg->getPositionY()));
    _panel_bg->addChild(menu_send);

    _input_text = CCTextFieldTTF::textFieldWithPlaceHolder("请输入...", DISPLAY->fangzhengFont(), 26);
    _input_text->setAnchorPoint(CCPoint(0, 0.5));
    _input_text->setColor(ccBLACK);
    _input_text->setDelegate(this);
    
    CCSprite* stencil = CCSprite::create();
    stencil->setTextureRect(CCRect(0, 0, _input_bg->getContentSize().width - 2, _input_bg->getContentSize().height));
    stencil->setColor(ccRED);
    
    _node = CCClippingNode::create(stencil);
    _node->setPosition(ccp(_input_bg->getPositionX(), _input_bg->getPositionY()));
    _node->setInverted(false);
    _node->addChild(_input_text);
    _input_text->setPosition(ccp(- _input_bg->getContentSize().width* .5f, 0));
    _panel_bg->addChild(_node);
    
    
}

void ChatPanel::initTopMessage(){
    CCSprite* bg = CCSprite::create("res/pic/panel/chat/text_bg_1.png");
    bg->setPosition(ccp(_panel_bg->getContentSize().width* .5f, _panel_bg->getContentSize().height* .89f));
    _panel_bg->addChild(bg);
    
    CCLabelTTF* nickname = CCLabelTTF::create("昵称七个字:", DISPLAY->fangzhengFont(), 17);
    nickname->setColor(ccc3(248, 83, 18));
    nickname->setAnchorPoint(CCPointZero);
    nickname->setPosition(ccp(bg->getContentSize().width* .05f, bg->getContentSize().height* .55f));
    
    CCSprite* nick_bg = CCSprite::create("res/pic/panel/chat/name_bg.png");
    nick_bg->setAnchorPoint(CCPointZero);
    nick_bg->setScaleX(nickname->getContentSize().width / nick_bg->getContentSize().width);
    nick_bg->setPosition(ccp(bg->getContentSize().width* .05f, bg->getContentSize().height* .58f));
    
    bg->addChild(nick_bg);
    bg->addChild(nickname);
    
    CCLabelTTF* message = CCLabelTTF::create("消息五十个字以内才行消息五十个字以内才行消息五十个字以内才行消息五十个字以内才行消息五十个字以内才行", DISPLAY->fangzhengFont(), 14, CCSizeMake(bg->getContentSize().width* .9f - nickname->getContentSize().width - 5, 60), kCCTextAlignmentLeft);
    message->setColor(ccc3(171, 107, 119));
    message->setAnchorPoint(CCPoint(0, 0.5));
    message->setPosition(ccp(nickname->getPositionX() + nickname->getContentSize().width + 5, bg->getContentSize().height* .39));
    bg->addChild(message);
}

void ChatPanel::btn_sendMessage(CCMenuItem *item){
    
//    WS->send();
}

bool ChatPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if (!_panel_bg->boundingBox().containsPoint(pos)) {
        this->removeFromParentAndCleanup(true);
    }
    return true;
}

void ChatPanel::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void ChatPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = _panel_bg->convertToNodeSpace(pTouch->getLocation());
    if (_input_bg->boundingBox().containsPoint(pos)) {
        _input_text->attachWithIME();
    }else{
        _input_text->detachWithIME();
    }
}

void ChatPanel::show_panel(){
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

bool ChatPanel::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *sender){
    CCFiniteTimeAction* _actionMove = CCMoveTo::create(.18f, ccp(_panel_bg->getPosition().x, _panel_bg->getPosition().y + 350));
    _panel_bg->runAction(CCSequence::create(_actionMove, NULL));
    
    return false;
}

bool ChatPanel::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *sender){
    CCFiniteTimeAction* _actionMove = CCMoveTo::create(.18f, ccp(_panel_bg->getPosition().x, _panel_bg->getPosition().y - 350));
    _panel_bg->stopAllActions();
    _panel_bg->runAction(CCSequence::create(_actionMove, NULL));
    
    return false;
}

bool ChatPanel::onTextFieldInsertText(cocos2d::CCTextFieldTTF *sender, const char *text, int nLen){
    CCString* str = CCString::createWithFormat("%s%s", sender->getString(), text);
    CCLabelTTF* lab = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 26);
    
    if (lab->getContentSize().width >= _input_bg->getContentSize().width) {
        sender->setAnchorPoint(CCPoint(1, 0.5));
        sender->setPosition(ccp(_input_bg->getContentSize().width* .5f - 1, 0));
    }else{
        sender->setAnchorPoint(CCPoint(0, 0.5));
        sender->setPosition(ccp(- _input_bg->getContentSize().width* .5f + 1, 0));
    }
    
    return false;
}

bool ChatPanel::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *sender, const char *delText, int nLen){
    if (sender->getContentSize().width >= _input_bg->getContentSize().width) {
        sender->setAnchorPoint(CCPoint(1, 0.5));
        sender->setPosition(ccp(_input_bg->getContentSize().width* .5f - 1, 0));
    }else{
        sender->setAnchorPoint(CCPoint(0, 0.5));
        sender->setPosition(ccp(- _input_bg->getContentSize().width* .5f + 1, 0));
    }
    
    return false;
}
