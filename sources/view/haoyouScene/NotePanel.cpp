//
//  NotePanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/20.
//
//

#include "NotePanel.h"
#include "DisplayManager.h"
#include "MMCursorTextField.h"
#include "CursorTextField.h"

NotePanel::~NotePanel(){
    
}

bool NotePanel::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->initView();
    return true;
}

void NotePanel::onEnter(){
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    //    this->setKeypadEnabled(true);
    
//    this->scheduleUpdate();
    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&NotePanel::note_callback_809), "HTTP_FINISHED_809", NULL);
}

void NotePanel::onExit(){
    //    this->setKeypadEnabled(false);
//    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCLayer::onExit();
}

void NotePanel::initView(){
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    note_panel = CCSprite::create("res/pic/haoyoupaihang/note_panel.png");
    note_panel->setPosition(DISPLAY->center());
    this->addChild(note_panel);
    
//    CCString* title_str = CCString::createWithFormat("发给 %s 的纸条", "女总裁");
    CCLabelTTF* title = CCLabelTTF::create("发给 女总裁 的纸条:", DISPLAY->font(), 33);
    title->setPosition(ccp(note_panel->getContentSize().width/2 - 50, note_panel->getContentSize().height*.9));
    title->setColor(ccc3(110, 92, 118));
    note_panel->addChild(title);
    
    CursorTextField* m_text = CursorTextField::cursorTextFieldWithPlaceHolder("点击进行输入...", CCSizeMake(350, 350), kCCTextAlignmentLeft, DISPLAY->font(), 23);
    m_text->setTextColor(ccc3(154, 138, 147));
    m_text->setPosition(ccp(note_panel->getContentSize().width/2, note_panel->getContentSize().height*.5));
    m_text->setMaxTextBytes(200);
//    m_text->setDelegate(this);
    note_panel->addChild(m_text);
    
    CCLabelTTF* word = CCLabelTTF::create("字数: ", DISPLAY->font(), 19);
    word->setPosition(ccp(note_panel->getContentSize().width*.75f - 30, note_panel->getContentSize().height*.2f));
    note_panel->addChild(word);
    
//    _word_count = CCLabelTTF::create("字数: 0/50", DISPLAY->font(), 19);
//    _word_count->setPosition(ccp(note_panel->getContentSize().width*.75f, note_panel->getContentSize().height*.2f));
//    _word_count->setColor(ccc3(135, 108, 123));
//    note_panel->addChild(_word_count);
    
    CCSprite* send_spr = CCSprite::create("res/pic/haoyoupaihang/btn_send.png");
    CCSprite* send_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_send.png");
    send_spr2->setScale(1.02f);
    CCMenuItemSprite* item_send = CCMenuItemSprite::create(send_spr, send_spr2, this, menu_selector(NotePanel::btn_send_callback));
    item_send->setPosition(ccp(note_panel->getContentSize().width/2, note_panel->getContentSize().height*.1f));
    CCMenu* menu_send = CCMenu::create(item_send, NULL);
    menu_send->setPosition(CCPointZero);
    note_panel->addChild(menu_send);
    
//    CCSprite* cost_spr = CCSprite::create("res/pic/haoyoupaihang/cost_bar.png");
//    cost_spr->setPosition(ccp(note_panel->getContentSize().width - cost_spr->getContentSize().width/2 - 15, cost_spr->getContentSize().height/2 + 15));
//    note_panel->addChild(cost_spr);
    
    CCSprite* tips = CCSprite::create("res/pic/txt_close.png");
    tips->setPosition(ccp(DISPLAY->ScreenWidth()/2, DISPLAY->ScreenHeight()*.25f));
    this->addChild(tips);
}

//void NotePanel::update(float dt)(){
//    if(_word_count){
//        this->removeChild(_word_count);
//    }
//    
//    _word_count = CCLabelTTF::create("字数: 0/50", DISPLAY->font(), 19);
//    _word_count->setPosition(ccp(note_panel->getContentSize().width*.75f, note_panel->getContentSize().height*.2f));
//    _word_count->setColor(ccc3(135, 108, 123));
//    note_panel->addChild(_word_count);
//}

void NotePanel::btn_send_callback(){
    this->removeFromParentAndCleanup(true);
}

void NotePanel::note_callback_809(){
    
}

bool NotePanel::onCursorTextFieldInsertText(CursorTextField* sender, const char* newText, int newTextByte) {
    
    return false;
}

bool NotePanel::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
    CCPoint location = pTouch->getLocation();
    if (! note_panel->boundingBox().containsPoint(location)) {
        this->btn_send_callback();
    }
    
    return true;
}

void NotePanel::onCursorTextFieldAttachWithIME(CursorTextField * sender) {
    
    CCFiniteTimeAction* _actionMove = CCMoveTo::create(.18f, ccp(note_panel->getPosition().x, note_panel->getPosition().y + 100));
    note_panel->runAction(CCSequence::create(_actionMove, NULL));
}

void NotePanel::onCursorTextFieldDetachWithIME(CursorTextField * sender) {
    
    CCFiniteTimeAction* _actionMove = CCMoveTo::create(.18f, ccp(note_panel->getPosition().x, note_panel->getPosition().y - 100));
    note_panel->stopAllActions();
    note_panel->runAction(CCSequence::create(_actionMove, NULL));
}
