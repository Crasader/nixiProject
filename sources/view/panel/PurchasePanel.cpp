//
//  PurchasePanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "PurchasePanel.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NetManager.h"

#include "Reward.h"
#include "Loading.h"

#define CELL_WIDTH          542
#define CELL_HEIGHT         228


PurchasePanel::~PurchasePanel() {
}

bool PurchasePanel::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        _content->setScale(0.1);
        this->addChild(_content);
        _content->setVisible(false);
        
        _bg = CCSprite::create("pic/panel/mail/mail_bg.png");
        _bg->setPosition(DISPLAY->center());
        _content->addChild(_bg);
        
        // pic size 542, 788
        float panelW = CELL_WIDTH;
        float panelH = 670;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->setPosition(DISPLAY->center() - ccp(panelW * 0.5, panelH * 0.52));
        _content->addChild(_tv);
        
        CCSprite* txt_close = CCSprite::create("pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        _content->addChild(txt_close);
        
        return true;
    }
    else {
        return false;
    }
}

void PurchasePanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&PurchasePanel::hanle_mail_oper), "HTTP_FINISHED_701", NULL);
    
    this->do_enter();
}

void PurchasePanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool PurchasePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _bg->boundingBox().containsPoint(location)) {
        this->do_exit();
    }
    
    return true;
}

#pragma mark - export

void PurchasePanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void PurchasePanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.8f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    //    _content->runAction(CCEaseBounceOut::create(spawn));
    _content->runAction(CCEaseElasticOut::create(spawn));
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
}

void PurchasePanel::do_exit() {
    float duration = 0.6f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&PurchasePanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(CCEaseElasticIn::create(spawn));
}

void PurchasePanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void PurchasePanel::config_cell(CCTableViewCell* cell, int idx) {
    CCSprite* plate = CCSprite::create("pic/panel/mail/mail_plate.png");
    plate->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    cell->addChild(plate);
    
    CCArray* mails = DATA->getMail()->mails();
    MailItem* item = (MailItem* )mails->objectAtIndex(idx);
    
    CCLabelTTF* lbl_title = CCLabelTTF::create(item->title.c_str(), DISPLAY->font(), 24);
    lbl_title->setColor(ccc3(140, 82, 82));
    lbl_title->setAnchorPoint(ccp(0, 0.5));
    lbl_title->setPosition(ccp(CELL_WIDTH * 0.12, CELL_HEIGHT * 0.82));
    cell->addChild(lbl_title);
    
    CCLabelTTF* lbl_content = CCLabelTTF::create(item->content.c_str(), DISPLAY->font(), 22, CCSizeMake(CELL_WIDTH * (1 - 0.14 * 2), CELL_HEIGHT * 0.3), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    lbl_content->setColor(ccORANGE);
    lbl_content->setAnchorPoint(ccp(0, 0.5));
    lbl_content->setPosition(ccp(CELL_WIDTH * 0.14, CELL_HEIGHT * 0.6));
    cell->addChild(lbl_content);
    
    // reward
    CCArray* reward_icons = CCArray::createWithCapacity(4);
    CCArray* reward_num = CCArray::createWithCapacity(4);
    Reward* reward = item->reward;
    if (reward->coin > 0) {
        CCSprite* coin = CCSprite::create("pic/panel/mail/mail_coin.png");
        reward_icons->addObject(coin);
        reward_num->addObject(CCString::createWithFormat("%d", reward->coin));
    }
    if (reward->diam > 0) {
        CCSprite* diam = CCSprite::create("pic/panel/mail/mail_diam.png");
        reward_icons->addObject(diam);
        reward_num->addObject(CCString::createWithFormat("%d", reward->diam));
    }
    if (reward->energy > 0) {
        CCSprite* energy = CCSprite::create("pic/panel/mail/mail_energy.png");
        reward_icons->addObject(energy);
        reward_num->addObject(CCString::createWithFormat("%d", reward->energy));
    }
    int icon_count = reward_icons->count();
    for (int i = 0; i < icon_count; i++) {
        CCNode* node = (CCNode* )reward_icons->objectAtIndex(i);
        node->setPosition(ccp(CELL_WIDTH * (0.2 + 0.2 * i), CELL_HEIGHT * 0.4));
        cell->addChild(node);
        //
        CCString* num = (CCString* )reward_num->objectAtIndex(i);
        CCLabelTTF* lbl_num = CCLabelTTF::create(num->getCString(), DISPLAY->font(), 20);
        lbl_num->setColor(ccORANGE);
        lbl_num->setAnchorPoint(ccp(0, 0.5));
        lbl_num->setPosition(node->getPosition() + ccp(22, -8));
        cell->addChild(lbl_num);
    }
    cell->setUserObject(reward_icons);
    
    CCSprite* delete1 = CCSprite::create("pic/panel/mail/mail_btn_delete.png");
    CCSprite* delete2 = CCSprite::create("pic/panel/mail/mail_btn_delete.png");
    delete2->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* btn_delete = CCMenuItemSprite::create(delete1, delete2, this, SEL_MenuHandler(&PurchasePanel::on_mail_delete));
    btn_delete->setUserData(&(item->id));
    
    CCSprite* take1 = CCSprite::create("pic/panel/mail/mail_btn_take.png");
    CCSprite* take2 = CCSprite::create("pic/panel/mail/mail_btn_take.png");
    take2->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* btn_take = CCMenuItemSprite::create(take1, take2, this, SEL_MenuHandler(&PurchasePanel::on_mail_take));
    btn_take->setUserData(&(item->id));
    /* 去掉回复按钮
     CCSprite* reply1 = CCSprite::create("pic/panel/mail/mail_btn_reply.png");
     CCSprite* reply2 = CCSprite::create("pic/panel/mail/mail_btn_reply.png");
     reply2->setScale(DISPLAY->btn_scale());
     CCMenuItemSprite* btn_reply = CCMenuItemSprite::create(reply1, reply2);
     btn_reply->setEnabled(false);
     
     CCMenu* menu = CCMenu::create(btn_delete, btn_take, btn_reply, NULL);
     menu->alignItemsHorizontallyWithPadding(CELL_WIDTH * 0.13);
     menu->setPosition(ccp(CELL_WIDTH * 0.49, CELL_HEIGHT * 0.18));
     cell->addChild(menu);
     */
    CCMenu* menu = CCMenu::create(btn_delete, btn_take, NULL);
    menu->alignItemsHorizontallyWithPadding(CELL_WIDTH * 0.32);
    menu->setPosition(ccp(CELL_WIDTH * 0.49, CELL_HEIGHT * 0.18));
    cell->addChild(menu);
}

void PurchasePanel::on_mail_delete(cocos2d::CCMenuItem *btn) {
    LOADING->show_loading();
    int* id = (int*)btn->getUserData();
    NET->response_mail_701(*id, 2);
}

void PurchasePanel::on_mail_take(cocos2d::CCMenuItem *btn) {
    LOADING->show_loading();
    int* id = (int*)btn->getUserData();
    NET->response_mail_701(*id, 1);
}

void PurchasePanel::hanle_mail_oper(cocos2d::CCObject *pObj) {
    LOADING->stop_loading();
    CCDictionary* dic = (CCDictionary* )pObj;
    int id = ((CCInteger* )dic->objectForKey("id"))->getValue();
    int oper = ((CCInteger* )dic->objectForKey("oper"))->getValue();
    if (oper == 1) {
        this->display_reward_take(id);
    }
    else {
        DATA->getMail()->handle_mail_oper(id, oper);
        _tv->reloadData();
        LOADING->remove();
    }
}

void PurchasePanel::display_reward_take(int mail_id) {
    int index = DATA->getMail()->item_index_by_mail_id(mail_id);
    DATA->getMail()->handle_mail_oper(mail_id, 1);
    
    if (index >= 0) {
        CCTableViewCell* cell = _tv->cellAtIndex(index);
        CCArray* icons = (CCArray* )cell->getUserObject();
        if (icons) {
            int size = icons->count();
            if (size > 0) {
                float duration = 0.4f;
                float scale = 1.2f;
                for (int i = 0; i < size; i++) {
                    CCNode* node = (CCNode* )icons->objectAtIndex(i);
                    if (i == 0) {
                        CCSequence* seq = CCSequence::create(CCScaleTo::create(duration, scale),
                                                             CCCallFunc::create(this, SEL_CallFunc(&PurchasePanel::take_reward_done)), NULL);
                        node->runAction(seq);
                    }
                    else {
                        node->runAction(CCScaleTo::create(duration, scale));
                    }
                }
                return;
            }
        }
    }
    
    _tv->reloadData();
    LOADING->remove();
}

void PurchasePanel::take_reward_done() {
    _tv->reloadData();
    LOADING->remove();
}



#pragma mark - CCTableViewDataSource

CCSize PurchasePanel::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return this->cellSizeForTable(table);
}

CCSize PurchasePanel::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCTableViewCell* PurchasePanel::tableCellAtIndex(CCTableView *table, unsigned int idx) {
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

unsigned int PurchasePanel::numberOfCellsInTableView(CCTableView *table) {
    CCArray* mails = DATA->getMail()->mails();
    return mails->count();
}

