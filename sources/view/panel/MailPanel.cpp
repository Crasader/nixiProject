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
#include "AudioManager.h"
#include "Reward.h"
#include "Loading2.h"

#define CELL_WIDTH          542
#define CELL_HEIGHT         228


MailPanel::~MailPanel() {
}

bool MailPanel::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
//        CCSprite* mask = CCSprite::create("res/pic/mask.png");
//        mask->setPosition(DISPLAY->center());
//        this->addChild(mask);
        
        _content = CCLayer::create();
//        _content->setScale(0.1);
//        _content->setVisible(false);
        this->addChild(_content);
        
        _bg = CCSprite::create("res/pic/panel/mail/mail_bg.png");
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
        
        CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        _content->addChild(txt_close);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        return true;
    }
    else {
        return false;
    }
}

void MailPanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&MailPanel::hanle_mail_oper), "HTTP_FINISHED_701", NULL);
    
//    this->do_enter();
    this->whether_mailbox_empty();
    
    this->scheduleOnce(SEL_SCHEDULE(&MailPanel::keyBackStatus), .8f);
}
void MailPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void MailPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool MailPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _bg->boundingBox().containsPoint(location)) {
//        this->do_exit();
        remove();
    }
    
    return false;
}

#pragma mark - export

void MailPanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void MailPanel::whether_mailbox_empty() {
    if (this->numberOfCellsInTableView(NULL) == 0) {
        CCSprite* empty = CCSprite::create("res/pic/haoyouScene/message_ren.png");
        empty->setPosition(DISPLAY->center());
        _content->addChild(empty);
    }
}

void MailPanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.5f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
//    _content->runAction(CCEaseBounceOut::create(spawn));
    _content->runAction(CCEaseElasticOut::create(spawn));
}

void MailPanel::do_exit() {
    float duration = 0.4f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&MailPanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(CCEaseElasticIn::create(spawn));
}

void MailPanel::remove() {
    AUDIO->goback_effect();
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void MailPanel::config_cell(CCTableViewCell* cell, int idx) {
    CCSprite* plate = CCSprite::create("res/pic/panel/mail/mail_plate.png");
    plate->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    cell->addChild(plate);
    
    CCArray* mails = DATA->getMail()->mails();
    MailItem* item = (MailItem* )mails->objectAtIndex(idx);
    
    CCLabelTTF* lbl_title = CCLabelTTF::create(item->title.c_str(), DISPLAY->fangzhengFont(), 24);
    lbl_title->setColor(ccc3(140, 82, 82));
    lbl_title->setAnchorPoint(ccp(0, 0.5));
    lbl_title->setPosition(ccp(CELL_WIDTH * 0.12, CELL_HEIGHT * 0.82));
    cell->addChild(lbl_title);
    
    CCLabelTTF* lbl_content = CCLabelTTF::create(item->content.c_str(), DISPLAY->fangzhengFont(), 22, CCSizeMake(CELL_WIDTH * (1 - 0.14 * 2), CELL_HEIGHT * 0.3), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    lbl_content->setColor(ccORANGE);
    lbl_content->setAnchorPoint(ccp(0, 0.5));
    lbl_content->setPosition(ccp(CELL_WIDTH * 0.14, CELL_HEIGHT * 0.6));
    cell->addChild(lbl_content);
    
    // reward
    CCArray* reward_icons = CCArray::createWithCapacity(4);
    CCArray* reward_num = CCArray::createWithCapacity(4);
    Reward* reward = item->reward;
    if (reward->coin > 0) {
        CCSprite* coin = CCSprite::create("res/pic/panel/mail/mail_coin.png");
        reward_icons->addObject(coin);
        reward_num->addObject(CCString::createWithFormat("%d", reward->coin));
    }
    if (reward->diam > 0) {
        CCSprite* diam = CCSprite::create("res/pic/panel/mail/mail_diam.png");
        reward_icons->addObject(diam);
        reward_num->addObject(CCString::createWithFormat("%d", reward->diam));
    }
    if (reward->energy > 0) {
        CCSprite* energy = CCSprite::create("res/pic/panel/mail/mail_energy.png");
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
        CCLabelTTF* lbl_num = CCLabelTTF::create(num->getCString(), DISPLAY->fangzhengFont(), 20);
        lbl_num->setColor(ccORANGE);
        lbl_num->setAnchorPoint(ccp(0, 0.5));
        lbl_num->setPosition(node->getPosition() + ccp(22, -8));
        cell->addChild(lbl_num);
    }
    cell->setUserObject(reward_icons);
    
    CCSprite* delete1 = CCSprite::create("res/pic/panel/mail/mail_btn_delete.png");
    CCSprite* delete2 = CCSprite::create("res/pic/panel/mail/mail_btn_delete.png");
    delete2->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* btn_delete = CCMenuItemSprite::create(delete1, delete2, this, SEL_MenuHandler(&MailPanel::on_mail_delete));
    btn_delete->setUserData(&(item->id));
    
    CCSprite* take1 = CCSprite::create("res/pic/panel/mail/mail_btn_take.png");
    CCSprite* take2 = CCSprite::create("res/pic/panel/mail/mail_btn_take.png");
    take2->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* btn_take = CCMenuItemSprite::create(take1, take2, this, SEL_MenuHandler(&MailPanel::on_mail_take));
    btn_take->setUserData(&(item->id));
    /* 去掉回复按钮
    CCSprite* reply1 = CCSprite::create("res/pic/panel/mail/mail_btn_reply.png");
    CCSprite* reply2 = CCSprite::create("res/pic/panel/mail/mail_btn_reply.png");
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

void MailPanel::on_mail_delete(cocos2d::CCMenuItem *btn) {
    LOADING->show_loading();
    int* id = (int*)btn->getUserData();
    NET->response_mail_701(*id, 2);
    if (this->numberOfCellsInTableView(NULL) == 0) {
        CCSprite* empty = CCSprite::create("res/pic/haoyouScene/message_ren.png");
        empty->setPosition(DISPLAY->center());
        _content->addChild(empty);
    }
}

void MailPanel::on_mail_take(cocos2d::CCMenuItem *btn) {
    LOADING->show_loading();
    int* id = (int*)btn->getUserData();
    NET->response_mail_701(*id, 1);
}

void MailPanel::hanle_mail_oper(cocos2d::CCObject *pObj) {
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

void MailPanel::display_reward_take(int mail_id) {
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
                            CCCallFunc::create(this, SEL_CallFunc(&MailPanel::take_reward_done)), NULL);
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

void MailPanel::take_reward_done() {
    _tv->reloadData();
    LOADING->remove();
    this->whether_mailbox_empty();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}



#pragma mark - CCTableViewDataSource

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


void MailPanel::keyBackClicked(){
    num_child++;
    CCLog("===== MailPanel  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}
