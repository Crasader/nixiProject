//
//  NoteTableView.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/29.
//
//

#include "NoteTableView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "NotePanel.h"
#include "PromptLayer.h"


NoteTableView::NoteTableView()
{
    winSize = CCDirector::sharedDirector()->getWinSize();
}

NoteTableView::~NoteTableView()
{
    
}

CCScene* NoteTableView::scene()
{
    CCScene *scene = CCScene::create();
    
    NoteTableView *layer = NoteTableView::create();
    
    scene->addChild(layer);
    
    return scene;
}

void NoteTableView::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(NoteTableView::addfriend_callback_803), "HTTP_FINISHED_805", NULL);
    
}
void NoteTableView::onExit(){
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeAllObservers(this);
    
    CCLayer::onExit();
}

bool NoteTableView::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    offsetFloat = 0;
    sliderBool = false;
    nowIndex = 0;
    
    m_bTable = true;
    m_bSlider = true;
    _number = 0;
    tableFloat = 0;
    clickIndex = -1;
    
    paperArr = DATA->getPaper()->papers();
    
    _number = paperArr->count();
    if (_number > NEI_MAXNUM) {
        _number = NEI_MAXNUM;
    }
    
    diSpr = CCSprite::create();
    this->addChild(diSpr);
    
    this->scheduleOnce(SEL_SCHEDULE(&NoteTableView::creat_View), 1.f);
    
    return true;
}

void NoteTableView::creat_View(){
    
    // 单个cell的高度 × cell的总数 - CCTableView的高度
    //    if ((1 - def_theProportion) > 0) {
    //        slider->setMinimumValue(-(92 * _number - 638));
    //        slider->setMaximumValue(0 + 6);
    //    }else{
    //        slider->setMinimumValue(-(92 * _number - 624));
    //        slider->setMaximumValue(0 + 20);
    //    }
    pTableView = CCTableView::create(this, CCSizeMake(522, 300* 3));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setPosition(ccp(59, 119));
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    this->addChild(pTableView, 1);
    
    offsetFloat = pTableView->getContentOffset().y;
    
    slider = SliderBar::createSliderBar("res/pic/haoyouScene/message_bar2.png", "res/pic/haoyouScene/message_bar1.png", CCSizeMake(911, 11), CCSizeMake(99, 25));
    slider->setPosition(ccp(DISPLAY->ScreenWidth()*.95f, DISPLAY->ScreenHeight()* .5f));
    slider->setRotation(-90);
    slider->setTag(1);
    this->addChild(slider, 2);
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    
    if (_number <= 0) {
        CCSprite* renSpr = CCSprite::create("res/pic/haoyouScene/message_ren.png");
        renSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        this->addChild(renSpr, 10);
    }
    
}

// 滑动条回调
void NoteTableView::valueChanged( CCObject *sender, CCControlEvent controlEvent )
{
    m_bTable = false;
    if ( m_bSlider )
    {
        CCTableView* _tableView = (CCTableView* )diSpr->getChildByTag(0);
        CCControlSlider* _slider = (CCControlSlider* )diSpr->getChildByTag(1);
        _tableView->setContentOffset(CCSizeMake(0, _slider->getValue()));
    }
    m_bTable = true;
    
}

//CCTableViewDelegate继承自CCScrollViewDelegate  拖动CCTableView触发
void NoteTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    CCPoint pos = view->getContentOffset();
    CCPoint _pos = view->getContentSize() - view->getViewSize();
    float percent = -(pos.y/_pos.y);
    slider->setValue(percent);
}

//点击哪个cell
void NoteTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    
    
}

//每个cell的size
cocos2d::CCSize NoteTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(522, 300);
}

//生成cell
cocos2d::extension::CCTableViewCell* NoteTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    
    PaperItem* item = (PaperItem* )paperArr->objectAtIndex(idx);
    
    CCSprite* kuangSpr = CCSprite::create("res/pic/haoyouScene/message_dikuang.png");
    kuangSpr->setAnchorPoint(CCPointZero);
    kuangSpr->setPosition(CCPointZero);
    kuangSpr->setTag(idx);
    pCell->addChild(kuangSpr);
    
    CCString* indexStr = CCString::createWithFormat("%d", idx+1);
    CCLabelTTF* indexLabel = CCLabelTTF::create(indexStr->getCString(), DISPLAY->fangzhengFont(), 20);
    indexLabel->setPosition(ccp(kuangSpr->getContentSize().width* .06f, kuangSpr->getContentSize().height* .91f));
    indexLabel->setColor(ccc3(116, 106, 153));
    kuangSpr->addChild(indexLabel);
    
    CCString* titleStr = CCString::createWithFormat("%s发给您的纸条", item->sender_name.c_str());
    CCLabelTTF* titleLabel = CCLabelTTF::create(titleStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .8f, 25), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    titleLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .92f));
    titleLabel->setColor(ccc3(116, 106, 153));
    kuangSpr->addChild(titleLabel);
    
    CCString* contentStr;
    contentStr = CCString::createWithFormat(item->content.c_str(), item->sender.c_str());
    
    CCLabelTTF* contentLabel = CCLabelTTF::create(contentStr->getCString(), DISPLAY->fangzhengFont(), 23, CCSizeMake(kuangSpr->getContentSize().width* .9f, kuangSpr->getContentSize().height* .6f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    contentLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
    contentLabel->setColor(ccc3(142, 131, 157));
    kuangSpr->addChild(contentLabel);
    
    
    CCSprite* deleteSpr1 = CCSprite::create("res/pic/haoyouScene/message_delete.png");
    CCSprite* deleteSpr2 = CCSprite::create("res/pic/haoyouScene/message_delete.png");
    deleteSpr2->setScale(1.02f);
    CCMenuItem* deleteItem = CCMenuItemSprite::create(deleteSpr1, deleteSpr2, this, menu_selector(NoteTableView::deleteCallBack));
    deleteItem->setPosition(ccp(kuangSpr->getContentSize().width* .25f, kuangSpr->getContentSize().height* .31f));
    deleteItem->setTag(idx);
    
    CCSprite* replySpr1 = CCSprite::create("res/pic/haoyouScene/reply.png");
    CCSprite* replySpr2 = CCSprite::create("res/pic/haoyouScene/reply.png");
    replySpr2->setScale(1.02f);
    CCMenuItem* replyItem = CCMenuItemSprite::create(replySpr1, replySpr2, this, menu_selector(NoteTableView::replyCallBack));
    replyItem->setPosition(ccp(kuangSpr->getContentSize().width* .75f, kuangSpr->getContentSize().height* .31f));
    replyItem->setTag(idx);
    
    CCSprite* addSpr1 = CCSprite::create("res/pic/haoyouScene/friend.png");
    CCSprite* addSpr2 = CCSprite::create("res/pic/haoyouScene/friend.png");
    addSpr2->setScale(1.02f);
    CCMenuItem* addItem = CCMenuItemSprite::create(addSpr1, addSpr2, this, menu_selector(NoteTableView::addCallBack));
    addItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .31f));
    addItem->setTag(idx);
    
    if (DATA->getSocial()->is_friend(item->sender.c_str())) {
        addItem->setVisible(false);
    }
    
    CCMenu* menu;
    menu = CCMenu::create(deleteItem, replyItem, addItem, NULL);
    menu->setPosition(CCPointZero);
    menu->setTag(idx);
    kuangSpr->addChild(menu);
    
    return pCell;
}
void NoteTableView::replyCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    PaperItem* paperItem = (PaperItem* )paperArr->objectAtIndex(item->getTag());
    DATA->getPaper()->setReplyID(paperItem->sender.c_str());
    DATA->getPaper()->setNickName(paperItem->sender_name.c_str());
    
    NotePanel* panel = NotePanel::create();
    panel->setEntranceType("zhitiao");
    this->addChild(panel, 10000);
}
void NoteTableView::deleteCallBack(CCObject* pSender){
    LOADING->show_loading();
    CCMenuItem* item = (CCMenuItem* )pSender;
    PaperItem* paperItem = (PaperItem* )paperArr->objectAtIndex(item->getTag());
    LOADING->show_loading();
    NET->delete_paper_811(paperItem->id);
}

void NoteTableView::addCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem*)pSender;
    PaperItem* paperItem = (PaperItem*)paperArr->objectAtIndex(item->getTag());
    NET->send_message_803(paperItem->sender.c_str(), 1);
}

void NoteTableView::addfriend_callback_803(CCObject* pSender){
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "好友请求发送成功");
}

void NoteTableView::delete_callback_811(CCObject* pSender){
    LOADING->remove();
    
    this->updateTableView();
}

//cell的数量
unsigned int NoteTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return _number;
}

void NoteTableView::updateTableView(){
    if (this->getChildByTag(1) != NULL) {
        this->removeChildByTag(1);
    }
    
    paperArr = DATA->getPaper()->papers();
    _number = paperArr->count();
    
    slider = SliderBar::createSliderBar("res/pic/haoyouScene/message_bar2.png", "res/pic/haoyouScene/message_bar1.png", CCSizeMake(911, 11), CCSizeMake(99, 25));
    slider->setPosition(ccp(DISPLAY->ScreenWidth()*.95f, DISPLAY->ScreenHeight()* .5f));
    slider->setRotation(-90);
    slider->setTag(1);
    this->addChild(slider, 2);
    
    pTableView->reloadData();
    
    if (_number <= 0) {
        CCSprite* renSpr = CCSprite::create("res/pic/haoyouScene/message_ren.png");
        renSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        this->addChild(renSpr, 10);
    }
}
