//
//  NotePanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/20.
//
//

#include "NotePanel.h"
#include "DisplayManager.h"
#include "CursorTextField.h"
#include "Loading2.h"
#include "NetManager.h"
//#include "HaoyouRankLayer.h"
#include "DataManager.h"
#include "PromptLayer.h"

NotePanel::~NotePanel(){
    
}

bool NotePanel::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    erase_pos = 1000000;
    
    return true;
}

void NotePanel::onEnter(){
    CCLayer::onEnter();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    //    this->setKeypadEnabled(true);
    
    this->scheduleUpdate();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&NotePanel::note_callback_809), "HTTP_FINISHED_809", NULL);
    this->initView();
    
    this->scheduleOnce(SEL_SCHEDULE(&NotePanel::keyBackStatus), .8f);
}
void NotePanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void NotePanel::onExit(){
    //    this->setKeypadEnabled(false);
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCLayer::onExit();
}

void NotePanel::initView(){
    CCSprite* mask = CCSprite::create("res/pic/mask.png");
    mask->setPosition(DISPLAY->center());
    this->addChild(mask);
    
    note_panel = CCSprite::create("res/pic/haoyoupaihang/note_panel.png");
    note_panel->setPosition(DISPLAY->center());
    this->addChild(note_panel);
    

    ShowComp* show = NULL;
    const char* nickname = NULL;
    _index = -1;
    if (!_entranceType.empty() && _entranceType.compare("friend") == 0) {
        _index = DATA->getSocial()->getSelectedFriend();
        show = DATA->getSocial()->getSelectedFriendByIndex(_index);
        nickname = show->nickname();
    }
    else if (!_entranceType.empty() && _entranceType.compare("stranger") == 0) {
        _index = DATA->getSocial()->getSelectedStranger();
        show = DATA->getSocial()->getSelectedStrangerByIndex(_index);
        nickname = show->nickname();
    }
    else if (!_entranceType.empty() && _entranceType.compare("zhitiao") == 0) {
        nickname = DATA->getPaper()->getNickName();
    }
    else if(!_entranceType.empty() && _entranceType.compare("ranker") == 0){
        CCArray* arr = DATA->getRanking()->ranking();
        show = (ShowComp*)arr->objectAtIndex(DATA->getSocial()->getSelectedRanker());
        nickname = show->nickname();
    }
    CCLOG("NickName: %s", nickname);
    CCString* title_str = CCString::createWithFormat("发给 %s 的纸条:", nickname);
    CCLabelTTF* title = CCLabelTTF::create(title_str->getCString(), DISPLAY->fangzhengFont(), 30, CCSizeMake(380, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    title->setPosition(ccp(note_panel->getContentSize().width/2, note_panel->getContentSize().height*.9 + 15));
    title->setColor(ccc3(110, 92, 118));
    note_panel->addChild(title);
    
    
    m_text = CursorTextField::cursorTextFieldWithPlaceHolder("请在这里输入...", CCSizeMake(350, 350), kCCTextAlignmentLeft, DISPLAY->fangzhengFont(), 23);
    m_text->setTextColor(ccc3(154, 138, 147));
    m_text->setPosition(ccp(note_panel->getContentSize().width/2, note_panel->getContentSize().height*.5));
    m_text->setMaxTextBytes(150);
    m_text->setDelegate(this);
    note_panel->addChild(m_text);
        
    CCLabelTTF* word = CCLabelTTF::create("字数: ", DISPLAY->fangzhengFont(), 19);
    word->setPosition(ccp(note_panel->getContentSize().width*.75f - 30, note_panel->getContentSize().height*.2f));
    word->setColor(ccc3(135, 108, 123));
    note_panel->addChild(word);
    
    CCLabelTTF* max_word = CCLabelTTF::create("/50", DISPLAY->fangzhengFont(), 19);
    max_word->setPosition(ccp(note_panel->getContentSize().width*.75f + 35, note_panel->getContentSize().height*.2f));
    max_word->setColor(ccc3(135, 108, 123));
    note_panel->addChild(max_word);
    
    _word_count = CCLabelTTF::create("0", DISPLAY->fangzhengFont(), 19);
    _word_count->setPosition(ccp(note_panel->getContentSize().width*.75f, note_panel->getContentSize().height*.2f));
    _word_count->setColor(ccc3(135, 108, 123));
    note_panel->addChild(_word_count);
    
    CCSprite* send_spr = CCSprite::create("res/pic/common/btn_send.png");
    CCSprite* send_spr2 = CCSprite::create("res/pic/common/btn_send.png");
    send_spr2->setScale(1.02f);
    CCMenuItemSprite* item_send = CCMenuItemSprite::create(send_spr, send_spr2, this, menu_selector(NotePanel::btn_send_callback));
    item_send->setPosition(ccp(note_panel->getContentSize().width/2, note_panel->getContentSize().height*.1f));
    CCMenu* menu_send = CCMenu::create(item_send, NULL);
    menu_send->setPosition(CCPointZero);
    note_panel->addChild(menu_send);

    
    CCSprite* tips = CCSprite::create("res/pic/txt_close.png");
    tips->setAnchorPoint(CCPoint(0.5, 1));
    tips->setPosition(ccp(note_panel->getContentSize().width* .5f, -15));
    note_panel->addChild(tips);
}

void NotePanel::update(float dt){
    if(_word_count){
        _word_count->removeFromParentAndCleanup(true);
    }
    unsigned long length = m_text->getText().length();
    const char* str = m_text->getText().c_str();
    int chs_count = 0;
    int eng_count = 0;
    int cur_count = 0;
    for(int i = 0; i < length; i++){
        if (('0' <= str[i] && str[i] <= '9') || ('A' <= str[i] && str[i] <= 'Z') || ('a' <= str[i] && str[i] <= 'z')  || (32 <= str[i] && str[i] <= 47) || (58 <= str[i] && str[i] <= 64) || (91 <= str[i] && str[i] <= 96) || (123 <= str[i] && str[i] <= 126)) {
            eng_count++;
        }else{
            chs_count++;
        }
        cur_count = (int)ceil(chs_count/3) + eng_count;
        if(cur_count == 50){
            m_text->setMaxTextBytes(i);
//            m_text->setText(m_text->getText().erase(i).c_str());
            break;
        }
        
    }
    
    CCString* count_str = CCString::createWithFormat("%d", cur_count);
    _word_count = CCLabelTTF::create(count_str->getCString(), DISPLAY->fangzhengFont(), 19);
    _word_count->setPosition(ccp(note_panel->getContentSize().width*.75f + 10, note_panel->getContentSize().height*.2f));
    _word_count->setColor(ccc3(135, 108, 123));
    note_panel->addChild(_word_count);
}

void NotePanel::btn_send_callback(){
    if (!_entranceType.empty() && _entranceType.compare("friend") == 0) {
        ShowComp* other = DATA->getSocial()->getSelectedFriendByIndex(_index);
        if (other) {
            LOADING->show_loading();
            NET->send_papar_809(other->getShowID().c_str(), m_text->getText().c_str());
        }
    }
    else if (!_entranceType.empty() && _entranceType.compare("stranger") == 0) {
        const char* id = DATA->getSocial()->getSelectedStrangerIDbyIndex(_index);
        if (id) {
            LOADING->show_loading();
            NET->send_papar_809(id, m_text->getText().c_str());
        }
    }
    else if (!_entranceType.empty() && _entranceType.compare("zhitiao") == 0) {
        const char* id = DATA->getPaper()->getReplyID();
        if (id) {
            LOADING->show_loading();
            NET->send_papar_809(id, m_text->getText().c_str());
        }
    }
    else if(!_entranceType.empty() && _entranceType.compare("ranker") == 0){
        CCArray* arr = DATA->getRanking()->ranking();
        ShowComp* show = (ShowComp*)arr->objectAtIndex(DATA->getSocial()->getSelectedRanker());
        const char* id = show->getShowID().c_str();
        if (id) {
            LOADING->show_loading();
            NET->send_papar_809(id, m_text->getText().c_str());
        }
    }
    
    
}

void NotePanel::note_callback_809(){
    LOADING->remove();
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "纸条发送成功");
    this->removeFromParentAndCleanup(true);
}

bool NotePanel::onCursorTextFieldInsertText(CursorTextField* sender, const char* newText, int newTextByte) {
    
    return false;
}

bool NotePanel::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
    if (!m_text->openBool) {
        CCPoint location = pTouch->getLocation();
        if (! note_panel->boundingBox().containsPoint(location)) {
            this->closeNotePanel();
        }
    }
    
    
    return true;
}

void NotePanel::closeNotePanel(){
    this->removeFromParentAndCleanup(true);
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

void NotePanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->closeNotePanel();
}


