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

const float CELL_WIDTH = 401;
const float CELL_HEIGHT = 55;

ChatTableView::~ChatTableView(){
    
}

bool ChatTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
        

    pTableView = CCTableView::create(this, CCSizeMake(CELL_WIDTH, CELL_HEIGHT*7));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setDelegate(this);
    pTableView->setVerticalFillOrder(kCCTableViewFillBottomUp);
    pTableView->ignoreAnchorPointForPosition(false);
    pTableView->setAnchorPoint(CCPoint(0.5, 0));
    pTableView->setPosition(ccp(0, 0));
    
    pTableView->reloadData();
    
    
    this->addChild(pTableView);
    
    return true;
}

void ChatTableView::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&ChatTableView::insertMessage), "NEW_CHAT", NULL);
}

void ChatTableView::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    CCLayer::onExit();
}

void ChatTableView::insertMessage(CCObject *pObj){
//    ChatItem* chat = (ChatItem*)pObj;
//    insert_name = chat->name.c_str();
//    insert_chat = chat->chat.c_str();
//    
//    pTableView->insertCellAtIndex(DATA->getChat()->getItems()->count());
//    pTableView->setContentOffset(CCPoint(0, -73));
//    pTableView->_updateCellPositions();
    pTableView->reloadData();
//    this->updateCellPosition();
//    CCLOG("NUM = %d", DATA->getChat()->getItems()->count());
}

CCTableViewCell* ChatTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = table->dequeueCell();
    if (pCell) {
        pCell->removeAllChildren();
    }else{
        pCell = new CCTableViewCell();
    }
    this->config_cell(pCell, idx);
    
    return pCell;
}

void ChatTableView::config_cell(CCTableViewCell *pCell, int index){
    ChatItem* chat = (ChatItem*)DATA->getChat()->getItems()->objectAtIndex(index);
    const char* insert_name = chat->name.c_str();
    CCString* str = CCString::createWithFormat("%s:", insert_name);
    CCLabelTTF* nickname = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 18);
    nickname->setAnchorPoint(ccp(0, 0.5));
    nickname->setColor(ccc3(112, 146, 192));
    nickname->setPosition(ccp(CELL_WIDTH* .025f, CELL_HEIGHT* .50f));
    pCell->addChild(nickname);
    
    
    const char* insert_chat = chat->chat.c_str();
    CCLabelTTF* text = CCLabelTTF::create(insert_chat, DISPLAY->fangzhengFont(), 18);
    float lab_size_height = 0;
    float lab_size_width = 0;
    if (text->getContentSize().width > CELL_WIDTH* .95f - nickname->getContentSize().width - 16) {
        lab_size_height = 40;
        lab_size_width = CELL_WIDTH* .95f - nickname->getContentSize().width - 16;
    }else if(text->getContentSize().width < 21){
        lab_size_height = 20;
        lab_size_width = 21;
    }else{
        lab_size_height = 20;
        lab_size_width = text->getContentSize().width;
    }
    
    
    CCScale9Sprite* text_bg = CCScale9Sprite::create("res/pic/panel/chat/text_bg.png", CCRectMake(0, 0, 37, 29), CCRectMake(11, 5, 21, 19));
    text_bg->setContentSize(CCSizeMake(lab_size_width + 16, lab_size_height + 10));
    text_bg->setAnchorPoint(ccp(0, 0.5));
    text_bg->setPosition(ccp(nickname->getPositionX() + nickname->getContentSize().width, CELL_HEIGHT* .5f));
    
    CCLabelTTF* message = CCLabelTTF::create(insert_chat, DISPLAY->fangzhengFont(), 18, CCSizeMake(lab_size_width, lab_size_height), kCCTextAlignmentLeft);
    message->setColor(ccc3(108, 83, 96));
    message->setAnchorPoint(CCPoint(0, 0.5));
    message->setPosition(ccp(11, text_bg->getContentSize().height* .5f));
    
    text_bg->addChild(message);
    
    pCell->addChild(text_bg);
    
}

void ChatTableView::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell){
    
}

CCSize ChatTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

unsigned int ChatTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return DATA->getChat()->getItems()->count();
}

//CCSize ChatTableView::tableCellSizeForIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
//    return this->cellSizeForTable(table);
//}

void ChatTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView *view){
    
}

void ChatTableView::scrollViewDidZoom(cocos2d::extension::CCScrollView *view){
    
}