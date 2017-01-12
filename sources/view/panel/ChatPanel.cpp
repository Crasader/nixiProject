//
//  ChatPanel.cpp
//  tiegao
//
//  Created by stevenLi on 16/7/29.
//
//

#include "ChatPanel.h"
#include "DisplayManager.h"
#include "FileManager.h"
#include "ChatTableView.h"
#include "WSManager.h"
#include "DataManager.h"
#include "json_lib.h"

#include "PromptLayer.h"
#include "AudioManager.h"

using namespace CSJson;

ChatPanel::~ChatPanel(){
    
}

ChatPanel* ChatPanel::create(bool couldColse) {
    ChatPanel* pRet = new ChatPanel();
    if (pRet && pRet->init(couldColse))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ChatPanel::init(bool couldColse){
    if (!CCLayer::init()) {
        return false;
    }
    
    _couldClose = couldColse;
    isOpen = false;

    this->initChatPanel();
    
    return true;
}

void ChatPanel::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&ChatPanel::nc_on_emoticon), "ON_EMOTICON", NULL);
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    if (_couldClose) {
        this->setTouchSwallowEnabled(true);
    }
    else {
        this->setTouchSwallowEnabled(false);
    }
}

void ChatPanel::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCLayer::onExit();
}

void ChatPanel::initChatPanel(){
    _panel_bg = CCSprite::create("res/pic/panel/chat/chat_bg.png");
    _panel_bg->setAnchorPoint(ccp(0, 0));
    _panel_bg->setPosition(ccp(0, DISPLAY->ScreenHeight()* .165f));
    this->addChild(_panel_bg);
    
    
    CCSprite* qipao = CCSprite::create("res/pic/panel/chat/qipao.png");
    CCSprite* qipao2 = CCSprite::create("res/pic/panel/chat/qipao.png");
    CCMenuItemSprite* item_qipao = CCMenuItemSprite::create(qipao, qipao2, this, menu_selector(ChatPanel::closeChatPanel));
    item_qipao->setPosition(ccp(_panel_bg->getContentSize().width* .10f, 30));
    CCMenu* menu_qipao = CCMenu::create(item_qipao, NULL);
    menu_qipao->setPosition(CCPointZero);
    _panel_bg->addChild(menu_qipao);
    
    _input_bg = CCSprite::create("res/pic/panel/chat/input.png");
    _input_bg->setPosition(ccp(_panel_bg->getContentSize().width* .485f, 30));
    _panel_bg->addChild(_input_bg);
    
    CCSprite* send_spr = CCSprite::create("res/pic/panel/chat/send.png");
    CCSprite* send_spr2 = CCSprite::create("res/pic/panel/chat/send.png");
    CCSprite* send_spr3 = CCSprite::create("res/pic/panel/chat/send.png");
    send_spr2->setScale(1.02f);
    send_spr3->setColor(ccGRAY);
    item_send = CCMenuItemSprite::create(send_spr, send_spr2, send_spr3, this, menu_selector(ChatPanel::btn_sendMessage));

    lab_time = CCLabelAtlas::create("00:00", "res/pic/baseScene/base_number3.png", 14, 20, '0');
    lab_time->setPosition(ccp(item_send->getContentSize().width* .5f, item_send->getContentSize().height* .5f));
    lab_time->setAnchorPoint(CCPoint(0.5, 0.5));
    lab_time->setScale(0.9f);
    lab_time->setVisible(false);
    item_send->addChild(lab_time);
    
    CCMenu* menu_send = CCMenu::create(item_send, NULL);
    menu_send->setPosition(ccp(_panel_bg->getContentSize().width* .89f, _input_bg->getPositionY()));
    _panel_bg->addChild(menu_send);
    

    _input_text = CCTextFieldTTF::textFieldWithPlaceHolder("最多可输入40汉字", DISPLAY->fangzhengFont(), 24);
    _input_text->setAnchorPoint(CCPoint(0, 0.5));
    _input_text->setColor(ccBLACK);
    _input_text->setDelegate(this);
    
//    _input_text = CCEditBox::create(CCSizeMake(_input_bg->getContentSize().width - 2, _input_bg->getContentSize().height), CCScale9Sprite::create("res/pic/loginScene/99.png"));
//    _input_text->setMaxLength(100);
//    _input_text->setFontColor(ccBLACK);
//    _input_text->setPlaceHolder("请输入...");
//    _input_text->setFontName(DISPLAY->fangzhengFont());
////    _input_text->setInputMode(kEditBoxInputModeEmailAddr);
//    _input_text->setReturnType(kKeyboardReturnTypeDone);
//    _input_text->setDelegate(this);
//    _input_text->setAnchorPoint(CCPoint(0, 0.5));
    
    
    CCSprite* stencil = CCSprite::create();
    stencil->setTextureRect(CCRect(0, 0, _input_bg->getContentSize().width - 2, _input_bg->getContentSize().height));
    
    _node = CCClippingNode::create(stencil);
    _node->setPosition(ccp(_input_bg->getPositionX(), _input_bg->getPositionY()));
    _node->setInverted(false);
    _node->addChild(_input_text);
    _input_text->setPosition(ccp(- _input_bg->getContentSize().width* .5f, 0));
    _panel_bg->addChild(_node);
    
    
    this->initTopMessage();
    
    ChatTableView* chatView = ChatTableView::create();
    chatView->setPosition(ccp(_panel_bg->getContentSize().width* .5f, _panel_bg->getContentSize().height* .12f));
//    chatView->pTableView->setContentOffset(ccp(0, 0));
    _panel_bg->addChild(chatView);
}

void ChatPanel::updateSendTime(){
    time_t now_time = DATA->cur_timestamp();
    time_t last_time = DATA->getLastTime();
    time_t delta_time = DATA->getDeltaTime();
    
    time_t new_delta = delta_time + (now_time - last_time);
    DATA->setDeltaTime(new_delta);
    DATA->setLastTime(now_time);
    int newInterval = DATA->getChatInterval() + 1;
    if (newInterval >= 3) {
        if (new_delta < 10) {
            this->start_updatetime(10);
        }
        
        DATA->setChatInterval(0);
        DATA->setDeltaTime(0);
    }
    else {
        DATA->setChatInterval(newInterval);
    }
}

void ChatPanel::start_updatetime(int secondLeft){
    _timeLeft = (float)secondLeft;
    item_send->setEnabled(false);
    lab_time->setVisible(true);
    
    this->unschedule(SEL_SCHEDULE(&ChatPanel::schedule_count_down));
    this->schedule(SEL_SCHEDULE(&ChatPanel::schedule_count_down));
}

void ChatPanel::schedule_count_down(float dt){
    _timeLeft -= dt;
    if (_timeLeft > 0) {
        int minute = int(_timeLeft) / 60;
        int second = int(_timeLeft) % 60;
        CCString* timeLeft = CCString::createWithFormat("%02d:%02d", minute, second);
        lab_time->setString(timeLeft->getCString());
    }else{
        this->unschedule(SEL_SCHEDULE(&ChatPanel::schedule_count_down));
        item_send->setEnabled(true);
        lab_time->setVisible(false);
    }
}

void ChatPanel::nc_on_emoticon(CCObject *pObj) {
    CCString* emot = (CCString*)pObj;
    string str = _input_text->getString();
    str.append(emot->getCString());
    _input_text->setString(str.c_str());
}

void ChatPanel::initTopMessage(){
    CCSprite* bg = CCSprite::create("res/pic/panel/chat/notice_bg.png");
    bg->setPosition(ccp(_panel_bg->getContentSize().width* .5f, _panel_bg->getContentSize().height - bg->getContentSize().height* .5f));
    _panel_bg->addChild(bg);
    
    CCSprite* notice_spr = CCSprite::create("res/pic/panel/chat/notice.png");
    notice_spr->setPosition(ccp(notice_spr->getContentSize().width* .5f, bg->getContentSize().height* .5f));
    bg->addChild(notice_spr);
    
//    CCLabelTTF* nickname = CCLabelTTF::create("昵称七个字:", DISPLAY->fangzhengFont(), 17);
//    nickname->setColor(ccc3(248, 83, 18));
//    nickname->setAnchorPoint(CCPointZero);
//    nickname->setPosition(ccp(bg->getContentSize().width* .1f, bg->getContentSize().height* .55f));
//    
//    CCSprite* nick_bg = CCSprite::create("res/pic/panel/chat/name_bg.png");
//    nick_bg->setAnchorPoint(CCPointZero);
//    nick_bg->setScaleX(nickname->getContentSize().width / nick_bg->getContentSize().width);
//    nick_bg->setPosition(ccp(bg->getContentSize().width* .1f, bg->getContentSize().height* .58f));
//    
//    bg->addChild(nick_bg);
//    bg->addChild(nickname);
    
    std::string notif = DATA->getNotif();
    if (notif.empty()) {
        notif = "欢迎来到女总的贴身高手，请小伙伴们文明发言。共同营造和谐氛围。";
    }
    CCLabelTTF* message = CCLabelTTF::create(notif.c_str(), DISPLAY->fangzhengFont(), 18, CCSizeMake(bg->getContentSize().width* .95f - notice_spr->getContentSize().width - 5, 70), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    message->setColor(ccc3(178, 117, 254));
    message->setAnchorPoint(CCPoint(0, 0.5));
    message->setPosition(ccp(notice_spr->getContentSize().width + 5, bg->getContentSize().height* .5));
    bg->addChild(message);
}

void ChatPanel::closeChatPanel(){
//    DATA->setChatOut(true);
//    DATA->getChat()->setItems(CCArray::create());
    
    if (_input_text->detachWithIME()) {
        
    }else{
        _input_text->detachWithIME();
    }
    this->removeFromParentAndCleanup(true);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("ON_CHAT_PANEL_CLOSE");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("CLOSE_CHATPANEL"); // 咖啡厅用
}

void ChatPanel::btn_sendMessage(CCMenuItem *item){
    AUDIO->comfirm_effect();
    
    FastWriter writer;
    Value root;
    
    CCString* text_str = CCString::createWithFormat("%s", _input_text->getString());
    
    if (text_str->compare(" ") == 0) {
        
    }
    else if (text_str->compare("  ") == 0) {
        
    }
    else if (text_str->compare("   ") == 0) {
        
    }
    else{
        const char* content = _input_text->getString();
        if (FILEM->is_illegal(content) == true) {
            PromptLayer* tip = PromptLayer::create();
            tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "不能使用不文明及敏感文字~!");
        }else{
            root["name"] = DATA->getShow()->nickname();
            root["chat"] = content;
            root["id"] = DATA->getShow()->getShowID();
            string data = writer.write(root);
            WS->send(data);
        }
    }
    
    _input_text->setString("");
    _input_text->setAnchorPoint(CCPoint(0, 0.5));
    _input_text->setPosition(ccp(- _input_bg->getContentSize().width* .5f, 0));
    
    this->updateSendTime();
}

bool ChatPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if (_couldClose) {
        if (! _panel_bg->boundingBox().containsPoint(pos) && ! isOpen) {
            this->closeChatPanel();
        }
    }
    else {

    }
    
    return true;
}

void ChatPanel::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void ChatPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = _panel_bg->convertToNodeSpace(pTouch->getLocation());
//    if (_input_bg->boundingBox().containsPoint(pos)) {
//        _input_text->attachWithIME();
//        isCanClose = false;
//    }else{
//        _input_text->detachWithIME();
//        isCanClose = true;
//    }
    if (_input_bg->boundingBox().containsPoint(pos)) {
        if (isOpen) {
            
        }else{
            _input_text->attachWithIME();
        }
    }else {
        _input_text->detachWithIME();
    }
}

void ChatPanel::show_panel(){
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

bool ChatPanel::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *sender){
//    this->setTouchSwallowEnabled(true);
    isOpen = true;
    CCFiniteTimeAction* _actionMove = CCMoveTo::create(.18f, ccp(_panel_bg->getPosition().x, DISPLAY->ScreenHeight()* .165f + 350));
    _panel_bg->runAction(CCSequence::create(_actionMove, NULL));
    
    return false;
}

bool ChatPanel::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *sender){
//    this->setTouchSwallowEnabled(false);
    isOpen = false;
    CCFiniteTimeAction* _actionMove = CCMoveTo::create(.18f, ccp(_panel_bg->getPosition().x, DISPLAY->ScreenHeight()* .165f));
    _panel_bg->stopAllActions();
    _panel_bg->runAction(CCSequence::create(_actionMove, NULL));
    
    return false;
}

bool ChatPanel::onTextFieldInsertText(cocos2d::CCTextFieldTTF *sender, const char *text, int nLen){
    CCString* str = CCString::createWithFormat("%s%s", sender->getString(), text);
    
    if (str->length() > 120) {
        return true;
    }else{
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

//void ChatPanel::editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox){
//    
//}
//
//void ChatPanel::editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox){
//    
//}
//
//void ChatPanel::editBoxTextChanged(cocos2d::extension::CCEditBox *editBox, const std::string &text){
//    CCString* str = CCString::createWithFormat("%s", text.c_str());
//    CCLabelTTF* lab = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 26);
//    if (lab->getContentSize().width >= _input_bg->getContentSize().width) {
//        editBox->setAnchorPoint(CCPoint(1, 0.5));
//        editBox->setContentSize(CCSizeMake(lab->getContentSize().width, _input_bg->getContentSize().height));
//        editBox->setPosition(ccp(_input_bg->getContentSize().width* .5f - 1, 0));
//    }else{
//        editBox->setAnchorPoint(CCPoint(0, 0.5));
//        editBox->setContentSize(CCSizeMake(lab->getContentSize().width, _input_bg->getContentSize().height));
//        editBox->setPosition(ccp(- _input_bg->getContentSize().width* .5f + 1, 0));
//    }
//}
//
//void ChatPanel::editBoxReturn(cocos2d::extension::CCEditBox *editBox){
//    
//}
