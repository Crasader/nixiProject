//
//  MailPanel.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "MailPanel.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NetManager.h"

#define CELL_WIDTH          542
#define CELL_HEIGHT         228

MailCell::~MailCell() {
}

bool MailCell::init() {
    if (CCSprite::init()) {
        
        return true;
    }
    else {
        return false;
    }
}

void MailCell::onEnter() {
    CCSprite::onEnter();
}

void MailCell::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCSprite::onExit();
}

void MailCell::config_with_module(MailItem* item) {
    
}


// --------------------------------------------------------

MailPanel::~MailPanel() {
}

bool MailPanel::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("pic/panel/mail/mail_mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        CCSprite* bg = CCSprite::create("pic/panel/mail/mail_bg.png");
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
        // pic size 542, 788
        float panelW = CELL_WIDTH;
        float panelH = 670;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->setPosition(DISPLAY->center() - ccp(panelW * 0.5, panelH * 0.52));
        this->addChild(_tv);
        
        return true;
    }
    else {
        return false;
    }
}

void MailPanel::onEnter() {
    CCLayer::onEnter();
}

void MailPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void MailPanel::show() {
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

void MailPanel::do_enter() {

}

void MailPanel::do_exit() {

}

void MailPanel::config_cell(CCTableViewCell* cell, int idx) {
    CCSprite* plate = CCSprite::create("pic/panel/mail/mail_plate.png");
    plate->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    cell->addChild(plate);
    
    ccColor3B color = ccORANGE;
    CCArray* mails = DATA->getMail()->mails();
    MailItem* item = (MailItem* )mails->objectAtIndex(idx);
    
    CCLabelTTF* lbl_title = CCLabelTTF::create(item->title.c_str(), "Arial", 24);
    lbl_title->setColor(color);
    lbl_title->setAnchorPoint(ccp(0, 0.5));
    lbl_title->setPosition(ccp(CELL_WIDTH * 0.12, CELL_HEIGHT * 0.82));
    cell->addChild(lbl_title);
    
    CCLabelTTF* lbl_content = CCLabelTTF::create(item->content.c_str(), "TRENDS.ttf", 22, CCSizeMake(CELL_WIDTH * (1 - 0.14 * 2), CELL_HEIGHT * 0.3), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    lbl_content->setColor(color);
    lbl_content->setAnchorPoint(ccp(0, 0.5));
    lbl_content->setPosition(ccp(CELL_WIDTH * 0.14, CELL_HEIGHT * 0.6));
    cell->addChild(lbl_content);
    
    CCSprite* delete1 = CCSprite::create("pic/panel/mail/mail_btn_delete.png");
    CCSprite* delete2 = CCSprite::create("pic/panel/mail/mail_btn_delete.png");
    delete2->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* btn_delete = CCMenuItemSprite::create(delete1, delete2, this, SEL_MenuHandler(&MailPanel::on_mail_delete));
    btn_delete->setUserData(&(item->id));
    
    CCSprite* take1 = CCSprite::create("pic/panel/mail/mail_btn_take.png");
    CCSprite* take2 = CCSprite::create("pic/panel/mail/mail_btn_take.png");
    take2->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* btn_take = CCMenuItemSprite::create(take1, take2, this, SEL_MenuHandler(&MailPanel::on_mail_take));
    btn_take->setUserData(&(item->id));
    
    CCSprite* reply1 = CCSprite::create("pic/panel/mail/mail_btn_reply.png");
    CCSprite* reply2 = CCSprite::create("pic/panel/mail/mail_btn_reply.png");
    reply2->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* btn_reply = CCMenuItemSprite::create(reply1, reply2);
    btn_reply->setEnabled(false);
    
    CCMenu* menu = CCMenu::create(btn_delete, btn_take, btn_reply, NULL);
    menu->alignItemsHorizontallyWithPadding(CELL_WIDTH * 0.13);
    menu->setPosition(ccp(CELL_WIDTH * 0.49, CELL_HEIGHT * 0.18));
    cell->addChild(menu);
}

void MailPanel::on_mail_delete(cocos2d::CCMenuItem *btn) {
    
}

void MailPanel::on_mail_take(cocos2d::CCMenuItem *btn) {
    int* id = (int*)btn->getUserData();
    NET->response_mail_701(*id, 1);
}

#pragma - CCTableViewDataSource

CCSize MailPanel::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return this->cellSizeForTable(table);
}

CCSize MailPanel::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCTableViewCell* MailPanel::tableCellAtIndex(CCTableView *table, unsigned int idx) {
//    CCTableViewCell* cell = table->dequeueCell();
//    if (cell) {
//        cell->removeAllChildren();
//    }
//    else {
//        cell = new CCTableViewCell();
//    }
    CCTableViewCell* cell = new CCTableViewCell();
    this->config_cell(cell, idx);
    return cell;
}

unsigned int MailPanel::numberOfCellsInTableView(CCTableView *table) {
    CCArray* mails = DATA->getMail()->mails();
    return mails->count();
}
