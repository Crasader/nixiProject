//
//  ChatTableView.cpp
//  tiegao
//
//  Created by stevenLi on 16/8/1.
//
//

#include "ChatTableView.h"
#include "DisplayManager.h"
#include "ChatComp.h"
#include "DataManager.h"
#include "NetManager.h"

#include "UnderlineLabel.h"
#include "PromptLayer.h"

//const float CELL_WIDTH = 400;
const float CELL_WIDTH = 480;
const float CELL_HEIGHT = 60;

ChatTableView::~ChatTableView(){
    
}

bool ChatTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    cell_height = CELL_HEIGHT;

    pTableView = CCTableView::create(this, CCSizeMake(CELL_WIDTH, CELL_HEIGHT*6.5));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setDelegate(this);
//    pTableView->setBounceable(false);
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->ignoreAnchorPointForPosition(false);
    pTableView->setAnchorPoint(CCPoint(0.5, 0));
    pTableView->setPosition(ccp(0, 0));
    
    pTableView->reloadData();
    pTableView->setContentOffset(ccp(0, 0));
    
    
    this->addChild(pTableView);
    
    return true;
}

void ChatTableView::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&ChatTableView::insertMessage), "NEW_CHAT", NULL);
    nc->addObserver(this, SEL_CallFuncO(&ChatTableView::addfriend_callback_803), "HTTP_FINISHED_803", NULL);
}

void ChatTableView::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCLayer::onExit();
}

void ChatTableView::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag) {
    if (button_type == AH_BUTTON_TYPE_YESNO) {
        if (button_tag == AH_BUTTON_TAG_YES) {
            CCString* otherId = (CCString* )box->getUserObject();
            if (otherId) {
                NET->send_message_803(otherId->getCString(), 1);
            }
        }
    }
    
    box->animation_out();
}

void ChatTableView::addfriend_callback_803(CCObject* pObj) {
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "成功添加好友~!");
}

void ChatTableView::insertMessage(CCObject *pObj){
//    ChatItem* chat = (ChatItem*)pObj;
//    insert_name = chat->name.c_str();
//    insert_chat = chat->chat.c_str();

//    this->updateCellPosition();
//    pTableView->insertCellAtIndex(DATA->getChat()->getItems()->count());
//    pTableView->insertCellAtIndex(0);
//    pTableView->setContentOffset(CCPoint(0, -73));
    
    
    
//    pTableView->reloadData();
//    pTableView->setContentOffset(ccp(0, 0));
//    if (DATA->getChat()->getItems()->count() <= 6) {
//        pTableView->setTouchEnabled(false);
//    }else{
//        pTableView->setTouchEnabled(true);
//    }
    pTableView->reloadData();
    pTableView->setContentOffset(ccp(0, 0));
    pTableView->setTouchEnabled(true);

    
    
//    if (DATA->getChat()->getItems()->count() == 1) {
//        pTableView->reloadData();
//        pTableView->setContentOffset(ccp(0, 0));
//    }else{
//        pTableView->insertCellAtIndex(0);
//    }
}

//void ChatTableView::updateCellPosition(){
//    for (int i = 0; i < DATA->getChat()->getItems()->count() - 1; i++) {
//        CCTableViewCell* cell = pTableView->cellAtIndex(i);
//        cell->setPosition(ccp(0, CELL_HEIGHT));
//    }
//    pTableView->insertCellAtIndex(0);
//}

CCTableViewCell* ChatTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = table->dequeueCell();
    if (pCell) {
        pCell->removeAllChildren();
    }else{
        pCell = new CCTableViewCell();
    }
    int length = DATA->getChat()->getItems()->count();
    if (length > 100) {
        this->config_cell(pCell, idx + length - 100);
    }else {
        this->config_cell(pCell, idx);
    }
    
    
    return pCell;
}

void ChatTableView::config_cell(CCTableViewCell *pCell, int index){
    ChatItem* chat = (ChatItem*)DATA->getChat()->getItems()->objectAtIndex(index);
    const char* insert_name = chat->name.c_str();
    const char* insert_chat = chat->chat.c_str();
    
    
    CCLabelTTF* text = CCLabelTTF::create(insert_chat, DISPLAY->fangzhengFont(), 18);
    float lab_size_height = 0;
    float lab_size_width = 0;
    float cellHeight = 0;
    if (text->getContentSize().width > CELL_WIDTH* .95f - 10) {
        lab_size_height = 40;
        lab_size_width = CELL_WIDTH* .95f - 10;
        cellHeight = 80;
    }else if(text->getContentSize().width < 21){
        lab_size_height = 20;
        lab_size_width = 21;
        cellHeight = 60;
    }else{
        lab_size_height = 20;
        lab_size_width = text->getContentSize().width;
        cellHeight = 60;
    }
    
    CCString* str = CCString::createWithFormat("%s:", insert_name);
    UnderlineLabel* nickname = UnderlineLabel::create(str->getCString(), DISPLAY->fangzhengFont(), 20);
    nickname->setBehindInterval(-6);
    nickname->setUnderlineSize(3);
    nickname->setUnderlineHeight(2);
    nickname->setUnderlineColor(ccc4(112, 146, 192, 250));
    if (! chat->id.empty() && chat->id.compare(DATA->getLogin()->obtain_sid()) != 0) {
        nickname->setUnderlineEnabled(true);
        pCell->setUserObject(ccs(chat->id));
    }
    else {
        nickname->setUnderlineEnabled(false);
        pCell->setUserObject(ccs(""));
    }
//    CCLabelTTF* nickname = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 20);
    nickname->setAnchorPoint(ccp(0, 0.5));
    nickname->setColor(ccc3(112, 146, 192));
    nickname->setPosition(ccp(CELL_WIDTH* .025f, cellHeight - 15));
    pCell->addChild(nickname);
    
    //===喇叭消息===
    if(chat->channel == 1) {
        nickname->setColor(ccRED);
        CCSprite* laba = CCSprite::create("res/pic/panel/chat/notice.png");
        laba->setPosition(ccp(nickname->getPositionX() + nickname->getContentSize().width + laba->getContentSize().width / 2, nickname->getPositionY()));
        laba->setScale(0.8);
        pCell->addChild(laba);
    }
    
    
    
//    CCSprite* line_spr = CCSprite::create("res/pic/panel/chat/line.png");
//    line_spr->setPosition(ccp(CELL_WIDTH* .5f   , 1));
//    pCell->addChild(line_spr);
    
    
    CCScale9Sprite* text_bg = CCScale9Sprite::create("res/pic/panel/chat/text_bg.png", CCRectMake(0, 0, 37, 29), CCRectMake(11, 5, 21, 19));
    text_bg->setContentSize(CCSizeMake(lab_size_width + 10, lab_size_height + 10));
    text_bg->setAnchorPoint(ccp(0, 0.5));
    text_bg->setPosition(ccp(CELL_WIDTH * .025f, text_bg->getContentSize().height / 2 + 2));
    
    CCLabelTTF* message = CCLabelTTF::create(insert_chat, DISPLAY->fangzhengFont(), 18, CCSizeMake(lab_size_width, lab_size_height + 2), kCCTextAlignmentLeft);
    message->setColor(ccc3(108, 83, 96));
    message->setAnchorPoint(CCPoint(0, 0.5));
    message->setPosition(ccp(11, text_bg->getContentSize().height* .5f));
    
    text_bg->addChild(message);
    
    pCell->addChild(text_bg);
}

void ChatTableView::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell){
    int idx = cell->getIdx();
    ChatItem* chat = (ChatItem*)DATA->getChat()->getItems()->objectAtIndex(idx);
    if (! chat->id.empty() && chat->id.compare(DATA->getLogin()->obtain_sid()) != 0) {
        CCString* msg = CCString::createWithFormat("是否添加 '%s' 为好友？", chat->name.c_str());
        AHMessageBox* mb = AHMessageBox::create_with_message(msg->getCString(), this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
        mb->setUserObject(ccs(chat->id));
        mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
    }
}

CCSize ChatTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(0, 0);
}

unsigned int ChatTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return MIN(100, DATA->getChat()->getItems()->count());
}

CCSize ChatTableView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    int length = DATA->getChat()->getItems()->count();
    int index;
    if(length > 100) {
        index = idx + length - 100;
    }else {
        index = idx;
    }
    ChatItem* chat = (ChatItem*)DATA->getChat()->getItems()->objectAtIndex(index);
    const char* insert_chat = chat->chat.c_str();
    
    CCLabelTTF* text = CCLabelTTF::create(insert_chat, DISPLAY->fangzhengFont(), 18);
    if (text->getContentSize().width > CELL_WIDTH* .95f - 10) {
        cell_height = 80;
    }else if(text->getContentSize().width < 21){
        cell_height = 60;
    }else{
        cell_height = 60;
    }
    return CCSizeMake(CELL_WIDTH, cell_height);
}

void ChatTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView *view){
    
}

void ChatTableView::scrollViewDidZoom(cocos2d::extension::CCScrollView *view){
    
}
