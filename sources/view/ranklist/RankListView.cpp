//
//  RankListView.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#include "RankListView.h"
#include "DataManager.h"
#include "AudioManager.h"
#include "DisplayManager.h"
#include "NetManager.h"
#include "ConfigManager.h"

#include "RankListCell.h"

#include "PromptLayer.h"


const float CELL_WIDTH = 252;
const float CELL_HEIGHT = 96;

const int ITEM_CELL_TAG = 123;

void RankListView::reload() {
    _tv->reloadData();
    int count = this->numberOfCellsInTableView(_tv);
    for (int i = 0; i < count; i++) {
        CCTableViewCell* cell = _tv->cellAtIndex(i);
        if (cell) {
            CCLOG("Has cell at index = %d", i);
            CCPoint origPos = cell->getPosition();
            CCSequence* seq = CCSequence::create(CCPlace::create(ccp(origPos.x + CELL_WIDTH, origPos.y)), CCDelayTime::create(i * 0.1), CCMoveTo::create(0.7, ccp(origPos.x , origPos.y)), NULL);
            cell->runAction(seq);
        }
    }
}

RankListView::~RankListView() {

}

bool RankListView::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    _datasource = NULL;
    _isShowCompetition = false;
    _selectedIndex = 0;
    _emptyTip = NULL;
    
    _panel = CCSprite::create("pic/ranklist/rl_panel.png");
    CCSize panelSize = _panel->getContentSize();
    _panel->setAnchorPoint(ccp(0.5, 0));
    _panel->setPosition(ccp(DISPLAY->W() - panelSize.width * 0.5, DISPLAY->H() * 0.15));
    this->addChild(_panel);
    
    {
        CCMenuItem* titleCompetition = CCMenuItemImage::create("pic/ranklist/rl_title_1.png", "pic/ranklist/rl_title_1.png");
        CCMenuItem* titleCollection = CCMenuItemImage::create("pic/ranklist/rl_title_2.png", "pic/ranklist/rl_title_2.png");
        CCMenuItemToggle* toggleTitle = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&RankListView::onTitleToggle), titleCompetition, titleCollection, NULL);
        CCMenu* menuTitle = CCMenu::createWithItem(toggleTitle);
        menuTitle->setPosition(ccp(panelSize.width * 0.52, panelSize.height * 0.95));
        _panel->addChild(menuTitle);
    }

    {
        _tv = CCTableView::create(this, CCSizeMake(CELL_WIDTH, 686));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->ignoreAnchorPointForPosition(false);
        _tv->setAnchorPoint(ccp(0, 0));
        _tv->setPosition(ccp(5, 2));
        _tv->setDelegate(this);
        _panel->addChild(_tv);
    }
    
    return true;
}

void RankListView::updateSelfPlate(unsigned int idx) {
    if (_menuSelf) {
        _menuSelf->removeFromParent();
        _menuSelf = NULL;
    }
    
    int selfIndex = idx;
    
    CCSprite* sel1 = this->createPlate(selfIndex);
    CCSprite* sel2 = this->createPlate(selfIndex);
    
    CCMenuItem* btnSelf1 = CCMenuItemSprite::create(sel1, sel1);
    CCMenuItem* btnSelf2 = CCMenuItemSprite::create(sel2, sel2);
    
    CCSprite* sel5 = CCSprite::create("pic/ranklist/rl_bar_selected.png");
    sel5->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
    btnSelf2->addChild(sel5, -1);
    
    CCSize panelSize = _panel->getContentSize();
    _toggleSelf = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&RankListView::onSelfBarToggle), btnSelf1, btnSelf2, NULL);
    _menuSelf = CCMenu::createWithItem(_toggleSelf);
    _menuSelf->setPosition(ccp(panelSize.width * 0.52, panelSize.height * 0.85));
    _panel->addChild(_menuSelf);
}

CCSprite* RankListView::createPlate(unsigned int idx) {
    CCSprite* plate = CCSprite::create("pic/ranklist/rl_bar_self.png");
    
    float cellWidth = CELL_WIDTH;
    float cellHeight = CELL_HEIGHT;
    float halfCellHeight = cellHeight * 0.5;
    
    if (_isShowCompetition) {
        CompetitionItem* item = DATA->getCompetition()->getSelf();
        
        const char* nickname = item->getNickname().c_str();
        CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        name->setPosition(ccp(cellWidth * 0.75f, cellHeight * 0.68f));
        name->setColor(ccc3(109, 98, 96));
        name->setTag(0x10500);
        plate->addChild(name);
        
        CCSprite* flag = CCSprite::create("pic/ranklist/rl_text_competition.png");
        flag->setPosition(ccp(cellWidth * 0.8f, cellHeight* .375));
        flag->setAnchorPoint(ccp(1, 0.5));
        flag->setTag(0x10111);
        plate->addChild((flag));
        
        CCString* collect_str = CCString::createWithFormat("%d", DATA->getCompetition()->getSelf()->getScore());
        CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 20);
        cloth_count->setPosition(flag->getPosition() + ccp(2, -1.5));
        cloth_count->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count->setTag(0x10300);
        plate->addChild(cloth_count);
    }
    else {
        ShowComp* show = DATA->getShow();
        
        const char* nickname = show->nickname();
        CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        name->setPosition(ccp(cellWidth * 0.75f, cellHeight * 0.68f));
        name->setColor(ccc3(109, 98, 96));
        name->setTag(0x10500);
        plate->addChild(name);
        
        CCSprite* flag = CCSprite::create("pic/haoyoupaihang/text_collected_nor.png");
        flag->setPosition(ccp(cellWidth * 0.8f, cellHeight* .375));
        flag->setAnchorPoint(ccp(1, 0.5));
        flag->setTag(0x10111);
        plate->addChild((flag));
        
        CCString* collect_str = CCString::createWithFormat("%d", show->collected());
        CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 20);
        cloth_count->setPosition(flag->getPosition() + ccp(2, -1.5));
        cloth_count->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count->setTag(0x10300);
        plate->addChild(cloth_count);
    }
    
    int num = idx + 1;
    float numSprPosX = cellWidth * 0.17f;
    if (num <= 0) {
        CCSprite* spr = CCSprite::create("pic/haoyoupaihang/weishangbang.png");
        spr->setPosition(ccp(numSprPosX, cellHeight* .5f));
        plate->addChild(spr);
    }
    else if (num < 10) {
        CCSprite* spr = RankListCell::getNumSprite(num);
        spr->setPosition(ccp(numSprPosX, cellHeight* .5f));
        plate->addChild(spr);
    }
    else if (num < 100) {
        CCSprite* spr1 = RankListCell::getNumSprite((int)floor(num/10));
        spr1->setPosition(ccp(numSprPosX - 7, halfCellHeight));
        this->addChild(spr1);
        CCSprite* spr2 = RankListCell::getNumSprite((int)floor(num%10));
        spr2->setPosition(ccp(numSprPosX + 7, halfCellHeight));
        plate->addChild(spr2);
    }
    else if (num < 1000) {
        int baiwei = (int)floor(num / 100);
        CCSprite* spr1 = RankListCell::getNumSprite(baiwei);
        spr1->setPosition(ccp(numSprPosX - 12, halfCellHeight));
        plate->addChild(spr1);
        
        int shiwei = (int)floor((num - baiwei * 100) / 10);
        CCSprite* spr2 = RankListCell::getNumSprite(shiwei);
        spr2->setPosition(ccp(numSprPosX, halfCellHeight));
        plate->addChild(spr2);
        
        int gewei = (int)floor(num % 10);
        CCSprite* spr3 = RankListCell::getNumSprite(gewei);
        spr3->setPosition(ccp(numSprPosX + 12, halfCellHeight));
        plate->addChild(spr3);
    }
    
    return plate;
}

void RankListView::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&RankListView::onAddFriend), "ON_ADD_FRIEND", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&RankListView::tobeFriend_callback_803), "HTTP_FINISHED_803", NULL);
    
    if (CONFIG->pkSwitchTipCount() > 0) {
        this->showSwitchTip();
    }
}

void RankListView::onExit(){
    CCLayer::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void RankListView::showEmptyTip() {
    if (! _emptyTip) {
        _emptyTip = CCNode::create();
        _emptyTip->setPosition(ccp(DISPLAY->W() - CELL_WIDTH * 0.5, DISPLAY->H() * 0.6));
        this->addChild(_emptyTip, 100);
        
        CCLabelTTF* lblTip = CCLabelTTF::create("-* 暂无数据 *-", DISPLAY->fangzhengFont(), 24);
        lblTip->setColor(DISPLAY->dullBlueColor());
        _emptyTip->addChild(lblTip);
    }
    
    _emptyTip->setVisible(true);
}

void RankListView::hideEmptyTip() {
    if (_emptyTip) {
        _emptyTip->setVisible(false);
    }
}

void RankListView::showUpdateDays() {
    if (! _competitionUpdateTip) {
        CCNode* panel = _tv->getParent();
        _competitionUpdateTip = CCSprite::create("pic/ranklist/rl_competition_tip.png");
        _competitionUpdateTip->setPosition(panel->getPosition() + ccp(2, panel->getContentSize().height + 16));
        this->addChild(_competitionUpdateTip);
        
        int day = DATA->getCompetition()->getTheme()->getDay() + 1;
        CCString* strDays = CCString::createWithFormat("%d", day);
        CCLabelTTF* lblTip = CCLabelTTF::create(strDays->getCString(), DISPLAY->fangzhengFont(), 20);
        lblTip->setColor(ccc3(255, 46, 46));
        lblTip->setPosition(ccp(_competitionUpdateTip->getContentSize().width * 0.755, _competitionUpdateTip->getContentSize().height * 0.5));
        _competitionUpdateTip->addChild(lblTip);
    }
    
    _competitionUpdateTip->setVisible(_isShowCompetition);
}

RankListCell* RankListView::createItemCell(unsigned int idx) {
    RankListCell* rtn = NULL;
    if (idx == 0) {
        rtn = RankListCell::create("pic/ranklist/rl_bar_first.png");
    }
    else if (idx == 1) {
        rtn = RankListCell::create("pic/ranklist/rl_bar_second.png");
    }
    else if (idx == 2) {
        rtn = RankListCell::create("pic/ranklist/rl_bar_third.png");
    }
    else {
        rtn = RankListCell::create("pic/ranklist/rl_bar_other.png");
    }
    
    if (_isShowCompetition) {
        CompetitionItem* item = (CompetitionItem* )_datasource->objectAtIndex(idx);
        rtn->configCompetition(idx, item, CELL_WIDTH, CELL_HEIGHT);
    }
    else {
        ShowComp* show = (ShowComp* )_datasource->objectAtIndex(idx);
        rtn->configShower(idx, show, CELL_WIDTH, CELL_HEIGHT);
    }
    
    return rtn;
}

void RankListView::onTitleToggle(CCMenuItemToggle *btn) {
    AUDIO->comfirm_effect();
    
    if (_isShowCompetition) {
        _isShowCompetition = false;
        this->setDatasource(DATA->getRanking()->ranking());
        this->updateSelfPlate(DATA->getRanking()->getSelfRank());
    }
    else {
        _isShowCompetition = true;
        this->setDatasource(DATA->getCompetition()->getRanklist());
        this->updateSelfPlate(DATA->getCompetition()->getSelfRank());
    }
    
    

    if (!_datasource || (_datasource && _datasource->count() == 0)) {
        _selectedIndex = -1;
        _toggleSelf->setSelectedIndex(1);
        _toggleSelf->setEnabled(false);
        
        if (_isShowCompetition) {
            CCObject* item = DATA->getCompetition()->getSelf();
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_CHANGE_COMPETITION", item);
        }
        else {
            CCObject* show = DATA->getShow();
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_CHANGE_SHOWER", show);
        }
        
        this->showEmptyTip();
    }
    else {
        _selectedIndex = 0;
        _toggleSelf->setSelectedIndex(0);
        _toggleSelf->setEnabled(true);
        
        if (_isShowCompetition) {
            CCObject* item = this->getDatasource()->objectAtIndex(_selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_CHANGE_COMPETITION", item);
        }
        else {
            CCObject* show = this->getDatasource()->objectAtIndex(_selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_CHANGE_SHOWER", show);
        }
        
        this->hideEmptyTip();
    }
    
    this->reload();
    this->showUpdateDays();
}

void RankListView::onSelfBarToggle() {
    int oldSelected = _selectedIndex;
    if (_toggleSelf->getSelectedIndex() == 1) {
        if (oldSelected != -1) {
            _selectedIndex = -1;
            _tv->updateCellAtIndex(oldSelected);
        }
        
        _toggleSelf->setEnabled(false);
        
        if (_isShowCompetition) {
            CCObject* item = DATA->getCompetition()->getSelf();
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_CHANGE_COMPETITION", item);
        }
        else {
            CCObject* show = DATA->getShow();
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_CHANGE_SHOWER", show);
        }
    }
}

void RankListView::showSwitchTip() {
    CCArray* arrowAnimations = CCArray::createWithCapacity(8);
    char arrowStr[100] = {};
    for (int i = 9; i <= 13; i++) {
        
        sprintf(arrowStr, "res/pic/guide/quan/guide_quan%d.png", i);
        CCSpriteFrame* arrowFrame = CCSpriteFrame::create(arrowStr, CCRectMake(0, 0, 129, 129));
        arrowAnimations->addObject(arrowFrame);
    }
    CCAnimation* arrowAnimation = CCAnimation::createWithSpriteFrames(arrowAnimations, .2f);
    
    CCSprite* quanSpr = CCSprite::create("res/pic/guide/quan/guide_quan1.png");
    quanSpr->runAction(CCRepeatForever::create(CCAnimate::create(arrowAnimation)));
    quanSpr->setPosition(ccp(_panel->getContentSize().width * 0.6, _panel->getContentSize().height * 0.95));
    _panel->addChild(quanSpr, 100);
    
    CCSprite* arrowSpr = CCSprite::create("res/pic/guide/guide_jiantou1.png");
    arrowSpr->setAnchorPoint(ccp(0, 1));
    arrowSpr->setPosition(ccp(quanSpr->getContentSize().width* .3f, quanSpr->getContentSize().height* .62f));
    quanSpr->addChild(arrowSpr);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(.2f, ccp(quanSpr->getContentSize().width* .45f, quanSpr->getContentSize().height* .48f));
    CCMoveTo* moveTo2 = CCMoveTo::create(.2f, ccp(quanSpr->getContentSize().width* .3f, quanSpr->getContentSize().height* .62f));
    arrowSpr->runAction(CCSequence::create( CCRepeat::create(CCSequence::create(moveTo1, moveTo2, CCDelayTime::create(.6f), NULL), 10), CCRemoveSelf::create(), NULL));
}

void RankListView::onAddFriend(CCInteger* pIdx) {
    int index = pIdx->getValue();
    if (_isShowCompetition) {
        CompetitionItem* other = (CompetitionItem* )_datasource->objectAtIndex(index);
        if (other) {
            NET->send_message_803(other->getId().c_str(), 1);
        }
    }
    else {
        ShowComp* other = (ShowComp* )_datasource->objectAtIndex(index);
        if (other) {
            NET->send_message_803(other->getShowID().c_str(), 1);
        }
    }
}

void RankListView::tobeFriend_callback_803(){
    if (_selectedIndex >= 0) {
        if (_isShowCompetition) {
            CompetitionItem* other = (CompetitionItem* )_datasource->objectAtIndex(_selectedIndex);
            other->setAdded(true);
        }
        else {
            ShowComp* other = (ShowComp* )_datasource->objectAtIndex(_selectedIndex);
            other->isadd = 1;
        }


        _tv->updateCellAtIndex(_selectedIndex);
    }
    
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "成功添加好友~!");
}


#pragma mark - CCScrollViewDatasource

unsigned int RankListView::numberOfCellsInTableView(CCTableView *table){
    if (_datasource) {
        return _datasource->count();
    }
    else {
        return 0;
    }
}

//每个cell的size
cocos2d::CCSize RankListView::cellSizeForTable(CCTableView *table){
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT + 10);
}

//生成cell
CCTableViewCell* RankListView::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    RankListCell* itemCell = this->createItemCell(idx);
    pCell->addChild(itemCell, 2, ITEM_CELL_TAG);
    if (idx == _selectedIndex) {
        itemCell->selected();
    }
    else {
        itemCell->unselected();
    }
    
    return pCell;
}

#pragma mark - CCTableViewDelegate

//点击哪个cell
void RankListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    AUDIO->right_effect();
    
    int idx = cell->getIdx();
    if (idx != _selectedIndex) {
        int oldSelected = _selectedIndex;
        _selectedIndex = idx;
        table->updateCellAtIndex(oldSelected);
        table->updateCellAtIndex(idx);
        
        // 需要切换按钮状态
        RankListCell* itemCell = (RankListCell* )cell->getChildByTag(ITEM_CELL_TAG);
        if (itemCell) {
        }
        
        // 通知切换shower
        if (_isShowCompetition) {
            CCObject* item = this->getDatasource()->objectAtIndex(_selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_CHANGE_COMPETITION", item);
        }
        else {
            CCObject* show = this->getDatasource()->objectAtIndex(_selectedIndex);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_CHANGE_SHOWER", show);
        }
    }
    
    _toggleSelf->setSelectedIndex(0);
    _toggleSelf->setEnabled(true);
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void RankListView::tableCellHighlight(CCTableView* table, CCTableViewCell* cell) {

}

//松开的时候，取消高亮状态
void RankListView::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell) {

}

#pragma mark - CCScrollViewDelegate

void RankListView::scrollViewDidScroll(CCScrollView* view) {
    CCPoint contOffsetPos = view->getContentOffset();
    if (this->numberOfCellsInTableView(_tv) > 6) {
        if (contOffsetPos.y < view->minContainerOffset().y) {
            view->setContentOffset(CCPoint(contOffsetPos.x, view->minContainerOffset().y));
        }else if (contOffsetPos.y > view->maxContainerOffset().y){
            view->setContentOffset(CCPoint(contOffsetPos.x, view->maxContainerOffset().y));
        }
    }
}

void RankListView::scrollViewDidZoom(CCScrollView* view) {
    
}
