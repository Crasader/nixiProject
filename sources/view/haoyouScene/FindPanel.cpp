//
//  FindPanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/21.
//
//

#include "FindPanel.h"
#include "DisplayManager.h"
#include "MMCursorTextField.h"
#include "CursorTextField.h"

FindPanel::~FindPanel(){
    
}

bool FindPanel::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->initView();
    return true;
}

void FindPanel::initView(){
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    find_panel = CCSprite::create("res/pic/haoyoupaihang/find_kuang.png");
    find_panel->setPosition(DISPLAY->center());
    this->addChild(find_panel);
    
    
    CursorTextField* m_text = CursorTextField::cursorTextFieldWithPlaceHolder("点击进行输入...", CCSizeMake(250, 40), kCCTextAlignmentLeft, DISPLAY->font(), 23);
    m_text->setTextColor(ccc3(154, 138, 147));
    m_text->setPosition(ccp(find_panel->getContentSize().width/2, find_panel->getContentSize().height*.5));
    m_text->setMaxTextBytes(170);
    //    m_text->setDelegate(this);
    find_panel->addChild(m_text);
    
//    CCTextFieldTTF* name = CCTextFieldTTF::textFieldWithPlaceHolder("请输入", CCSizeMake(250, 40), kCCTextAlignmentLeft, DISPLAY->font(), 23);
//    name->setPosition(ccp(find_panel->getContentSize().width/2, find_panel->getContentSize().height*.5));
//    find_panel->addChild(name);
    
    CCSprite* find_spr = CCSprite::create("res/pic/haoyoupaihang/find_btn_find.png");
    CCSprite* find_spr2 = CCSprite::create("res/pic/haoyoupaihang/find_btn_find.png");
    find_spr2->setScale(1.02f);
    CCMenuItemSprite* item_find = CCMenuItemSprite::create(find_spr, find_spr2, this, menu_selector(FindPanel::btn_find_callback));
    item_find->setPosition(ccp(find_panel->getContentSize().width/2 - 90, find_panel->getContentSize().height*.2f));
    CCMenu* menu_find = CCMenu::create(item_find, NULL);
    menu_find->setPosition(CCPointZero);
    find_panel->addChild(menu_find);
    
    CCSprite* back_spr = CCSprite::create("res/pic/haoyoupaihang/find_btn_back.png");
    CCSprite* back_spr2 = CCSprite::create("res/pic/haoyoupaihang/find_btn_back.png");
    back_spr2->setScale(1.02f);
    CCMenuItemSprite* item_back = CCMenuItemSprite::create(back_spr, back_spr2, this, menu_selector(FindPanel::btn_back_callback));
    item_back->setPosition(ccp(find_panel->getContentSize().width/2 + 90, find_panel->getContentSize().height*.2f));
    CCMenu* menu_back = CCMenu::create(item_back, NULL);
    menu_back->setPosition(CCPointZero);
    find_panel->addChild(menu_back);
}

void FindPanel::btn_find_callback(){
    this->removeFromParentAndCleanup(true);
}

void FindPanel::btn_back_callback(){
    this->removeFromParentAndCleanup(true);
}

bool FindPanel::onCursorTextFieldInsertText(CursorTextField* sender, const char* newText, int newTextByte) {
    
    return false;
}


void FindPanel::onEnter(){
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    this->setKeypadEnabled(true);
}

void FindPanel::onExit(){
    this->setKeypadEnabled(false);
    CCLayer::onExit();
}

//bool FindPanel::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
//    CCPoint location = pTouch->getLocation();
//    if (! find_panel->boundingBox().containsPoint(location)) {
//        this->btn_find_callback();
//    }
//    
//    return true;
//}

void FindPanel::onCursorTextFieldAttachWithIME(CursorTextField * sender) {
    
    CCFiniteTimeAction* _actionMove = CCMoveTo::create(.18f, ccp(find_panel->getPosition().x, find_panel->getPosition().y + 100));
    find_panel->runAction(CCSequence::create(_actionMove, NULL));
}

void FindPanel::onCursorTextFieldDetachWithIME(CursorTextField * sender) {
    
    CCFiniteTimeAction* _actionMove = CCMoveTo::create(.18f, ccp(find_panel->getPosition().x, find_panel->getPosition().y - 100));
    find_panel->stopAllActions();
    find_panel->runAction(CCSequence::create(_actionMove, NULL));
}
