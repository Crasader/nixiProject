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

ChatTableView::~ChatTableView(){
    
}

bool ChatTableView::init(){
    if (!CCLayer::init()) {
        return false;
    }
        

    pTableView = CCTableView::create(this, CCSizeMake(526, 73*8));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setVerticalFillOrder(kCCTableViewFillBottomUp);
    pTableView->ignoreAnchorPointForPosition(false);
    pTableView->setAnchorPoint(CCPoint(0.5, 0));
    pTableView->setPosition(ccp(0, 0));
    pTableView->setDelegate(this);
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
    CCLayer::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
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
    CCSprite* bg = CCSprite::create("res/pic/panel/chat/text_bg_2.png");
    bg->setPosition(ccp(bg->getContentSize().width* .5f, bg->getContentSize().height* .5f));
    
    ChatItem* chat = (ChatItem*)DATA->getChat()->getItems()->objectAtIndex(index);
    const char* insert_name = chat->name.c_str();
    CCString* str = CCString::createWithFormat("%s:", insert_name);
    CCLabelTTF* nickname = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 17);
    nickname->setAnchorPoint(CCPointZero);
    nickname->setColor(ccc3(248, 83, 18));
    nickname->setPosition(ccp(bg->getContentSize().width* .05f, bg->getContentSize().height* .55f));
    
    CCSprite* name_bg = CCSprite::create("res/pic/panel/chat/name_bg.png");
    name_bg->setAnchorPoint(CCPointZero);
    name_bg->setPosition(ccp(bg->getContentSize().width* .05f, bg->getContentSize().height* .59f));
    name_bg->setScaleX(nickname->getContentSize().width / name_bg->getContentSize().width);
    
    bg->addChild(name_bg);
    bg->addChild(nickname);
    
    const char* insert_chat = chat->chat.c_str();
    CCLabelTTF* message = CCLabelTTF::create(insert_chat, DISPLAY->fangzhengFont(), 14, CCSizeMake(bg->getContentSize().width* .9f - nickname->getContentSize().width - 5, 60), kCCTextAlignmentLeft);
    message->setColor(ccc3(171, 107, 119));
    message->setAnchorPoint(CCPoint(0, 0.5));
    message->setPosition(ccp(nickname->getPositionX() + nickname->getContentSize().width + 5, bg->getContentSize().height* .39f));
    bg->addChild(message);
    
    pCell->addChild(bg);
}

void ChatTableView::tableCellTouched(cocos2d::extension::CCTableView *table, cocos2d::extension::CCTableViewCell *cell){
    
}

CCSize ChatTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(526, 73);
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