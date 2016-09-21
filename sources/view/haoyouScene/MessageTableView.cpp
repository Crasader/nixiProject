//
//  MessageTableView.cpp
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#include "MessageTableView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "NetManager.h"

#include "Loading2.h"
#include "PromptLayer.h"

MessageTableView::MessageTableView()
{
	winSize = CCDirector::sharedDirector()->getWinSize();
}

MessageTableView::~MessageTableView()
{
    
}

CCScene* MessageTableView::scene()
{
    CCScene *scene = CCScene::create();
    
    MessageTableView *layer = MessageTableView::create();
    
    scene->addChild(layer);
    
    return scene;
}

void MessageTableView::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(MessageTableView::_805CallBack), "HTTP_FINISHED_805", NULL);
    
}
void MessageTableView::onExit(){
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeAllObservers(this);
    
    CCLayer::onExit();
}

bool MessageTableView::init()
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
    
    messageArr = DATA->getMessage()->messages();
    
    _number = messageArr->count();
    if (_number > NEI_MAXNUM) {
        _number = NEI_MAXNUM;
    }
    
    diSpr = CCSprite::create();
    this->addChild(diSpr);
    
//    this->scheduleOnce(SEL_SCHEDULE(&MessageTableView::creat_View), 1.f);
    this->creat_View();
    
    return true;
}

void MessageTableView::creat_View(){
	
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
//    pTableView->setPosition(ccp(59, 119));
    pTableView->setPosition(ccp(DISPLAY->halfW() - 265, 119));
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    this->addChild(pTableView, 1);
    
    offsetFloat = pTableView->getContentOffset().y;
    
    slider = SliderBar::createSliderBar("res/pic/haoyouScene/message_bar2.png", "res/pic/haoyouScene/message_bar1.png", CCSizeMake(911, 11), CCSizeMake(99, 25));
    slider->setPosition(ccp(DISPLAY->ScreenWidth()*.95f, DISPLAY->ScreenHeight()* .5f));
    if (_number <= 0) {
        slider->setRotation(90);
    }else{
        slider->setRotation(-90);
    }
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
void MessageTableView::valueChanged( CCObject *sender, CCControlEvent controlEvent )
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
void MessageTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    CCPoint pos = view->getContentOffset();
    CCPoint _pos = view->getContentSize() - view->getViewSize();
    float percent = -(pos.y/_pos.y);
    slider->setValue(percent);
}

//点击哪个cell
void MessageTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    
    
}

//每个cell的size
cocos2d::CCSize MessageTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(522, 300);
}

//生成cell
cocos2d::extension::CCTableViewCell* MessageTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    
    MessageItem* item = (MessageItem* )messageArr->objectAtIndex(idx);
    
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
    
    CCString* titleStr = CCString::createWithFormat("%s发给您的消息", item->sender.c_str());
    CCLabelTTF* titleLabel = CCLabelTTF::create(titleStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .8f, 25), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    titleLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .92f));
    titleLabel->setColor(ccc3(116, 106, 153));
    kuangSpr->addChild(titleLabel);
    
    CCString* contentStr;
    if (item->type == e_Msg_Friend_Ask) {
        contentStr = CCString::createWithFormat("%s希望加您为好友,希望您能同意.", item->sender.c_str());
    }else if (item->type == e_Msg_Send_Energy){
        contentStr = CCString::createWithFormat("您的好友%s赠送了您体力,请前往领取呦~!", item->sender.c_str());
    }else if (item->type == e_Msg_Friend_Deletem){
        contentStr = CCString::createWithFormat("%s删除您的好友.", item->sender.c_str());
    }
    CCLabelTTF* contentLabel = CCLabelTTF::create(contentStr->getCString(), DISPLAY->fangzhengFont(), 23, CCSizeMake(kuangSpr->getContentSize().width* .9f, kuangSpr->getContentSize().height* .6f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    contentLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
    contentLabel->setColor(ccc3(142, 131, 157));
    kuangSpr->addChild(contentLabel);
    
    
    CCSprite* deleteSpr1 = CCSprite::create("res/pic/haoyouScene/message_delete.png");
    CCSprite* deleteSpr2 = CCSprite::create("res/pic/haoyouScene/message_delete.png");
    deleteSpr2->setScale(1.02f);
    CCMenuItem* deleteItem = CCMenuItemSprite::create(deleteSpr1, deleteSpr2, this, menu_selector(MessageTableView::deleteCallBack));
    if (item->type == e_Msg_Friend_Ask) {
        deleteItem->setPosition(ccp(kuangSpr->getContentSize().width* .3f, kuangSpr->getContentSize().height* .31f));
    }else if (item->type == e_Msg_Send_Energy){
        deleteItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .31f));
    }else if (item->type == e_Msg_Friend_Deletem){
        deleteItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .31f));
    }
    deleteItem->setTag(idx);
    
    CCSprite* agreedSpr1 = CCSprite::create("res/pic/haoyouScene/message_agreed.png");
    CCSprite* agreedSpr2 = CCSprite::create("res/pic/haoyouScene/message_agreed.png");
    agreedSpr2->setScale(1.02f);
    CCMenuItem* agreedItem = CCMenuItemSprite::create(agreedSpr1, agreedSpr2, this, menu_selector(MessageTableView::agreedCallBack));
    agreedItem->setPosition(ccp(kuangSpr->getContentSize().width* .7f, kuangSpr->getContentSize().height* .31f));
    agreedItem->setTag(idx);
    
    CCMenu* menu;
    if (item->type == e_Msg_Friend_Ask) {
        menu = CCMenu::create(deleteItem, agreedItem, NULL);
    }else if (item->type == e_Msg_Send_Energy){
        menu = CCMenu::create(deleteItem, NULL);
    }else if (item->type == e_Msg_Friend_Deletem){
        menu = CCMenu::create(deleteItem, NULL);
    }
    menu->setPosition(CCPointZero);
    menu->setTag(idx);
    kuangSpr->addChild(menu);
    
    return pCell;
}
void MessageTableView::agreedCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    MessageItem* messageItem = (MessageItem* )messageArr->objectAtIndex(item->getTag());
    LOADING->show_loading();
    NET->response_message_805(messageItem->id, 1);
}
void MessageTableView::deleteCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    MessageItem* messageItem = (MessageItem* )messageArr->objectAtIndex(item->getTag());
    LOADING->show_loading();
    NET->response_message_805(messageItem->id, 2);
}
void MessageTableView::_805CallBack(CCObject* pSender){
    LOADING->remove();
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(this->getScene(), "操作成功~!");
    this->updateTableView();
}

//cell的数量
unsigned int MessageTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return _number;
}

void MessageTableView::updateTableView(){
    if (this->getChildByTag(1) != NULL) {
        this->removeChildByTag(1);
    }
    
    messageArr = DATA->getMessage()->messages();
    _number = messageArr->count();
    
    slider = SliderBar::createSliderBar("res/pic/haoyouScene/message_bar2.png", "res/pic/haoyouScene/message_bar1.png", CCSizeMake(911, 11), CCSizeMake(99, 25));
    slider->setPosition(ccp(DISPLAY->ScreenWidth()*.95f, DISPLAY->ScreenHeight()* .5f));
    if (_number <= 0) {
        slider->setRotation(90);
    }else{
        slider->setRotation(-90);
    }
    slider->setTag(1);
    this->addChild(slider, 2);
    
    pTableView->reloadData();
    
    if (_number <= 0) {
        CCSprite* renSpr = CCSprite::create("res/pic/haoyouScene/message_ren.png");
        renSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        this->addChild(renSpr, 10);
    }
}





