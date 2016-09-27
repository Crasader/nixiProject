//
//  FriendsListView.cpp
//  tiegao
//
//  Created by mac on 16/7/17.
//
//

#include "FriendsListView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NetManager.h"
#include "AudioManager.h"
#include "Loading2.h"

#define CELL_WIDTH          275
#define CELL_HEIGHT         124

const float selectedScale = 1.2f;

#pragma mark - Export API

void FriendsListView::add_ranking_num(CCSprite* plate, int ranking, bool selected) {
    if (0 < ranking && ranking < 10) {
        CCSprite* spr = FriendsListView::num_sprite(ranking);
        if (selected) {
            spr->setPosition(ccp(plate->getContentSize().width* .15f, plate->getContentSize().height* .5f));
            spr->setScale(selectedScale);
        }
        else {
            spr->setPosition(ccp(plate->getContentSize().width* .25f, plate->getContentSize().height* .5f));
        }
        plate->addChild(spr);
    }else{
        CCSprite* spr1 = FriendsListView::num_sprite((int)floor(ranking / 10));
        if (selected) {
            spr1->setPosition(ccp(plate->getContentSize().width* .15f - 9, plate->getContentSize().height* .5f));
            spr1->setScale(selectedScale);
        }
        else {
            spr1->setPosition(ccp(plate->getContentSize().width* .25f - 8, plate->getContentSize().height* .5f));
        }
        plate->addChild(spr1);
        
        CCSprite* spr2 = FriendsListView::num_sprite((int)floor(ranking % 10));
        if (selected) {
            spr2->setPosition(ccp(plate->getContentSize().width* .15f + 9, plate->getContentSize().height* .5f));
            spr2->setScale(selectedScale);
        }
        else {
            spr2->setPosition(ccp(plate->getContentSize().width* .25f + 8, plate->getContentSize().height* .5f));
        }
        plate->addChild(spr2);
    }
}

CCSprite* FriendsListView::num_sprite(int num){
    CCSprite* spr = NULL;
    switch (num) {
        case 0:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_0.png");
            break;
        case 1:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_1.png");
            break;
        case 2:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_2.png");
            break;
        case 3:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_3.png");
            break;
        case 4:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_4.png");
            break;
        case 5:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_5.png");
            break;
        case 6:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_6.png");
            break;
        case 7:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_7.png");
            break;
        case 8:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_8.png");
            break;
        case 9:
            spr =CCSprite::create("res/pic/haoyoupaihang/num_9.png");
            break;
        default:
            spr =CCSprite::create("res/pic/unknown.png");
            break;
    }
    
    return spr;
}

#pragma mark - Super API

FriendsListView::~FriendsListView() {
}

bool FriendsListView::init() {
    if (CCLayer::init()) {
        _seletedIndex = -1;
        _data = DATA->getSocial()->sortedFriends();
        
        float panelW = CELL_WIDTH;
        float panelH = CELL_HEIGHT * 5.5;
        _tv = CCTableView::create(this, CCSizeMake(panelW, panelH));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->ignoreAnchorPointForPosition(false);
        _tv->setAnchorPoint(ccp(1, 0));
        _tv->setPosition(ccp(DISPLAY->W(), DISPLAY->H() * 0.23));
        _tv->setDelegate(this);
        this->addChild(_tv);
        
        // 滚动边框
        CCSprite* bottom_spr = CCSprite::create("res/pic/haoyoupaihang/di_bar.png");
        bottom_spr->setAnchorPoint(ccp(1, 0.5));
        bottom_spr->setPosition(ccp(_tv->getPosition().x, _tv->getPosition().y - 12));
        this->addChild(bottom_spr);
        
        CCSprite* top_spr = CCSprite::create("res/pic/haoyoupaihang/di_bar.png");
        top_spr->setFlipY(true);
        top_spr->setAnchorPoint(ccp(1, 0.5));
        top_spr->setPosition(ccp(_tv->getPosition().x, bottom_spr->getPosition().y + panelH + 15));
        this->addChild(top_spr);
        
        float barScale = CELL_WIDTH / bottom_spr->getContentSize().width * 0.975;
        bottom_spr->setScale(barScale);
        top_spr->setScale(barScale);
        
        return true;
    }
    else {
        return false;
    }
}

void FriendsListView::onEnter() {
    CCLayer::onEnter();
    
    this->_seletedIndex = 0;
    this->tableCellTouched(_tv, _tv->cellAtIndex(0));
}

void FriendsListView::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

//bool FriendsListView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
//    CCPoint location = pTouch->getLocation();
//    return false;
//}

#pragma mark - Inner API

void FriendsListView::config_cell(CCTableViewCell *cell, int idx) {
    ShowComp* show = (ShowComp*)_data->objectAtIndex(idx);
    if (! show) {
        return;
    }
    
    const char* otherId = show->getShowID().c_str();
    
    CCSprite* plate = NULL;
    if (_seletedIndex == idx) {
        if (idx == 0) {
            plate = CCSprite::create("pic/haoyoupaihang/first_bg_sel.png");
        }
        else if (idx == 1) {
            plate = CCSprite::create("pic/haoyoupaihang/second_bg_sel.png");
        }
        else if (idx == 2) {
            plate = CCSprite::create("pic/haoyoupaihang/third_bg_sel.png");
        }
        else {
            plate = CCSprite::create("pic/haoyoupaihang/other_bg_sel.png");

        }
        
        this->add_ranking_num(plate, idx + 1, true);
        this->add_name(plate, show->nickname(), true);
        this->add_collected(plate, show->collected(), true);
        if (strcasecmp(otherId, DATA->getLogin()->obtain_sid()) == 0) { // 不能给自己送体力
            
        }
        else {
            bool hasSent = DATA->getSocial()->has_send_energy(otherId);
            this->add_send_button(plate, otherId, ! hasSent, true);
        }
    }
    else {
        if (idx == 0) {
            plate = CCSprite::create("pic/haoyoupaihang/first_bg_nor.png");
        }
        else if (idx == 1) {
            plate = CCSprite::create("pic/haoyoupaihang/second_bg_nor.png");
        }
        else if (idx == 2) {
            plate = CCSprite::create("pic/haoyoupaihang/third_bg_nor.png");
        }
        else {
            plate = CCSprite::create("pic/haoyoupaihang/other_bg_nor.png");
            
        }
        
        this->add_ranking_num(plate, idx + 1, false);
        this->add_name(plate, show->nickname(), false);
        this->add_collected(plate, show->collected(), false);
        if (strcasecmp(otherId, DATA->getLogin()->obtain_sid()) == 0) { // 不能给自己送体力
            
        }
        else {
            bool hasSent = DATA->getSocial()->has_send_energy(otherId);
            this->add_send_button(plate, otherId, ! hasSent, false);
        }
    }
    

    plate->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    cell->addChild(plate);
}

void FriendsListView::add_name(CCSprite *plate, const char *nickname, bool selected) {
    CCSize plateSize = plate->getContentSize();
    CCLabelTTF* labelName = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 24, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    labelName->setAnchorPoint(ccp(0, 0.5));
    labelName->setColor(ccc3(234, 106, 106));
    if (selected) {
        labelName->setPosition(ccp(plateSize.width* .24f, plateSize.height* .68));
        labelName->setScale(selectedScale);
    }
    else {
        labelName->setPosition(ccp(plateSize.width* .34f, plateSize.height* .68));
    }
    plate->addChild(labelName);
}

void FriendsListView::add_collected(CCSprite *plate, int collected, bool selected) {
    CCSize plateSize = plate->getContentSize();
    CCString* strCollected = CCString::createWithFormat("服装收集: %d", collected);
    CCLabelTTF* label = CCLabelTTF::create(strCollected->getCString(), DISPLAY->fangzhengFont(), 16);
    label->setAnchorPoint(CCPoint(0, 0.5));
    if (selected) {
        label->setPosition(ccp(plateSize.width * .42f, plateSize.height* .35f));
        label->setScale(selectedScale);
    }
    else {
        label->setPosition(ccp(plateSize.width * .48f, plateSize.height* .365f));
    }
    
    plate->addChild(label);
}

void FriendsListView::add_send_button(CCSprite* plate, const char* otherId, bool couldSend, bool selected) {
    CCSize plateSize = plate->getContentSize();
    if (couldSend) {
        if (selected) {
            CCSprite* sptSend1 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
            CCSprite* sptSend2 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
            sptSend2->setScale(DISPLAY->btn_scale());
            CCMenuItem* btnSend = CCMenuItemSprite::create(sptSend1, sptSend2, this, SEL_MenuHandler(&FriendsListView::on_btn_send_energy));
            btnSend->setUserObject(ccs(otherId));
            CCMenu* menu = CCMenu::createWithItem(btnSend);
            //        menu->setPosition(ccp(plateSize.width - 50, 24));
            menu->setPosition(ccp(plateSize.width - 55, 18));
            plate->addChild(menu);
        }
    }
    else {
        CCSprite* hasSent = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
        hasSent->setPosition(ccp(plateSize.width - 50, 24));
        if (selected) {
            hasSent->setScale(selectedScale);
            hasSent->setPosition(ccp(plateSize.width - 55, 18));
        }
        
        plate->addChild(hasSent);
    }
}

void FriendsListView::seleted_cell(int idx) {
    int count = this->numberOfCellsInTableView(_tv);
    if (idx < count) {
        int oldIndex = _seletedIndex;
        
        if (oldIndex != idx) {
            _seletedIndex = idx;
            _tv->updateCellAtIndex(_seletedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ON_CHANGE_SHOWER", CCInteger::create(idx));
        }
        
        if (oldIndex != -1) {
            _tv->updateCellAtIndex(oldIndex);
        }
    }
    else {
        _seletedIndex = -1;
    }
}

void FriendsListView::unseleted_cells() {
    if (_seletedIndex >= 0) {
        int oldSeletedIndex = _seletedIndex;
        _seletedIndex = -1;
        _tv->updateCellAtIndex(oldSeletedIndex);
    }
}

void FriendsListView::update_selected_cell() {
    if (_seletedIndex >= 0) {
        _tv->updateCellAtIndex(_seletedIndex);
    }
}

void FriendsListView::on_btn_send_energy(CCMenuItem *menuItem) {
    LOADING->show_loading();
    CCString* otherId = (CCString*)menuItem->getUserObject();
    NET->send_message_803(otherId->getCString(), e_Msg_Send_Energy);
}

#pragma mark - CCTableViewDataSource

CCSize FriendsListView::tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
    return this->cellSizeForTable(table);
}

CCSize FriendsListView::cellSizeForTable(CCTableView *table) {
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

CCTableViewCell* FriendsListView::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell* cell = table->dequeueCell();
    if (cell) {
        cell->removeAllChildren();
    }
    else {
        cell = new CCTableViewCell();
    }
    
    this->config_cell(cell, idx);
    return cell;
}

unsigned int FriendsListView::numberOfCellsInTableView(CCTableView *table) {
    return _data->count();
}

#pragma mark - CCTableViewDelegate

void FriendsListView::tableCellTouched(CCTableView *table, CCTableViewCell *cell) {
    int idx = cell->getIdx();
    this->seleted_cell(idx);
}

void FriendsListView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {
    CCPoint contOffsetPos = view->getContentOffset();
    if (this->numberOfCellsInTableView(_tv) > 5) {
//        _tv->setBounceable(true);
        if (contOffsetPos.y < view->minContainerOffset().y) {
            view->setContentOffset(CCPoint(contOffsetPos.x, view->minContainerOffset().y));
        }else if (contOffsetPos.y > view->maxContainerOffset().y){
            view->setContentOffset(CCPoint(contOffsetPos.x, view->maxContainerOffset().y));
        }
    }
}

void FriendsListView::scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {

}

