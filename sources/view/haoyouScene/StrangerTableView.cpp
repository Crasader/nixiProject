//
//  StrangerTableView.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#include "StrangerTableView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "StrangerScene.h"
#include "NetManager.h"
#include "Loading2.h"

const float NAME_FONT_SIZE = 22;

StrangerTableView::~StrangerTableView(){}

bool StrangerTableView::init(){
    if(!CCLayer::init()){
        return false;
    }
    
    
    _stangers = DATA->getSocial()->strangers()->allKeys();
    if (_stangers == NULL) {
        _stangers = CCArray::create();
    }
    _stangers->retain();
    allNumber =  _stangers->count();
    selectedIndex = 0;
    if(allNumber == 0){
        DATA->getSocial()->setSelectedStranger(-1);
    }else{
        DATA->getSocial()->setSelectedStranger(selectedIndex);
    }
    
    
    pTableView = CCTableView::create(this, CCSizeMake(304, 7.5*110));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setAnchorPoint(CCPointZero);
    pTableView->setPosition(CCPointZero);
//    pTableView->setBounceable(false);
    pTableView->setDelegate(this);
    
    this->addChild(pTableView);
    
//    CCSprite* di_spr = CCSprite::create("res/pic/haoyoupaihang/di_bar.png");
//    di_spr->setAnchorPoint(CCPoint(1, 0));
//    di_spr->setPosition(ccp(pTableView->getContentSize().width, 0));
//    this->addChild(di_spr);
//    if (allNumber == 0) {
//        di_spr->setVisible(false);
//    }
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    
    
    return true;
}

void StrangerTableView::onEnter(){
    CCLayer::onEnter();
}

void StrangerTableView::onExit(){
    CCLayer::onExit();
}

//CCTableViewDelegate继承自CCScrollViewDelegate
void StrangerTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
//    
//    CCPoint contOffsetPos = pTableView->getContentOffset();
//    if (allNumber > 6) {
//        if (contOffsetPos.y < pTableView->minContainerOffset().y) {
//            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->minContainerOffset().y));
//        }else if (contOffsetPos.y > pTableView->maxContainerOffset().y){
//            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->maxContainerOffset().y));
//        }
//    }
    CCPoint contOffsetPos = view->getContentOffset();
    if (this->numberOfCellsInTableView(pTableView) > 7) {
        if (contOffsetPos.y < view->minContainerOffset().y) {
            view->setContentOffset(CCPoint(contOffsetPos.x, view->minContainerOffset().y));
        }else if (contOffsetPos.y > view->maxContainerOffset().y){
            view->setContentOffset(CCPoint(contOffsetPos.x, view->maxContainerOffset().y));
        }
    }
}

//点击哪个cell
void StrangerTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    if (selectedIndex != cell->getIdx()) {
        CCLayer* layer = CCLayer::create();
        layer->setTouchEnabled(true);
        layer->setTouchSwallowEnabled(true);
        layer->setTag(10000);
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000);
        
        // 需要变小
        CCSprite* bg1 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCTexture2D* tet;
        tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/other_bg_nor.png");
        
        bg1->setTexture(tet);
    
        
        
        CCString* show_id = (CCString* )_stangers->objectAtIndex(selectedIndex);
        ShowComp* show = (ShowComp* )DATA->getSocial()->strangers()->objectForKey(show_id->getCString());
        const char* nickname = show->nickname();
        int collected1 = show->collected();
        
        if (bg1->getChildByTag(0x10500) != NULL) {
            bg1->removeChildByTag(0X10500, true);
        }
        
        CCLabelTTF* name1 = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
            name1->setPosition(ccp(bg1->getContentSize().width* .75f, bg1->getContentSize().height* .68));
            name1->setColor(ccc3(109, 98, 96));
            name1->setTag(0x10500);
            bg1->addChild(name1);
        
        if (bg1->getChildByTag(0x10300) != NULL) {
            bg1->removeChildByTag(0x10300, true);
        }
        
        CCString* collected_str1 = CCString::createWithFormat("%d", collected1);
        CCLabelTTF* cloth_count1 = CCLabelTTF::create(collected_str1->getCString(), DISPLAY->fangzhengFont(), 20);
        cloth_count1->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count1->setPosition(ccp(bg1->getContentSize().width * .78f, bg1->getContentSize().height* .36f));
        cloth_count1->setTag(0x10300);
        bg1->addChild(cloth_count1);
        
        if (bg1->getChildByTag(0x10111) != NULL) {
            bg1->removeChildByTag(0X10111, true);
        }
        
        CCSprite* flag1 = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
        flag1->setPosition(ccp(bg1->getContentSize().width* .6f, bg1->getContentSize().height* .38));
        flag1->setTag(0x10111);
        bg1->addChild((flag1));
        

        CCMenu* menu_add = (CCMenu*)bg1->getChildByTag(0x10400);
        menu_add->setVisible(false);
        
        
        // 记录需要变大节点
        selectedIndex = cell->getIdx();
        DATA->getSocial()->setSelectedStranger(selectedIndex);
        
        // 需要变大
        sprNode = (CCSprite*)cell->getChildByTag(selectedIndex);
        CCSprite* bg2 = (CCSprite*)sprNode->getChildByTag(0x10000);

        CCTexture2D* tet2;
        tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/other_bg_sel.png");
        bg2->setTexture(tet2);
        
        
        
        CCString* show_id2 = (CCString* )_stangers->objectAtIndex(selectedIndex);
        ShowComp* show2 = (ShowComp* )DATA->getSocial()->strangers()->objectForKey(show_id2->getCString());
        const char* nickname2 = show2->nickname();
        int collected2 = show2->collected();
        
        if (bg2->getChildByTag(0x10500) != NULL) {
            bg2->removeChildByTag(0X10500, true);
        }
        
            CCLabelTTF* name2 = CCLabelTTF::create(nickname2, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
            name2->setPosition(ccp(bg2->getContentSize().width* .63f, bg2->getContentSize().height* .76));
            name2->setColor(ccc3(109, 98, 96));
            name2->setTag(0x10500);
            bg2->addChild(name2);
        
        if (bg2->getChildByTag(0x10300) != NULL) {
            bg2->removeChildByTag(0x10300, true);
        }
        
        CCString* collected_str2 = CCString::createWithFormat("%d", collected2);
        CCLabelTTF* cloth_count2 = CCLabelTTF::create(collected_str2->getCString(), DISPLAY->fangzhengFont(), 22);
        cloth_count2->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count2->setPosition(ccp(bg2->getContentSize().width * .75, bg2->getContentSize().height* .34f));
        cloth_count2->setTag(0x10300);
        bg2->addChild(cloth_count2);
        
        if (bg2->getChildByTag(0X10111) != NULL) {
            bg2->removeChildByTag(0x10111, true);
        }
        
        CCSprite* flag2 = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
        flag2->setPosition(ccp(bg2->getContentSize().width* .5f, bg2->getContentSize().height* .36));
        flag2->setTag(0x10111);
        bg2->addChild((flag2));
        
        
        CCMenu* menu_add2 = (CCMenu*)bg2->getChildByTag(0x10400);
        menu_add2->setVisible(true);
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ExitMan");
    }
}

//每个cell的size
cocos2d::CCSize StrangerTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(304, 110);
}

//生成cell
cocos2d::extension::CCTableViewCell* StrangerTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    
    if(idx == 0){
        if (selectedIndex == idx) {// 大的
            bigSprite(idx, spr);
        }else{// 小的
            smallSprite(idx, spr);
        }
    }else if(idx == 1){
        if (selectedIndex == idx) {// 大的
            bigSprite(idx, spr);
        }else{// 小的
            smallSprite(idx, spr);
        }
        
    }else if(idx == 2){
        if (selectedIndex == idx) {// 大的
            bigSprite(idx, spr);
        }else{// 小的
            smallSprite(idx, spr);
        }
        
    }else{
        if (selectedIndex == idx) {// 大的
            bigSprite(idx, spr);
        }else{// 小的
            smallSprite(idx, spr);
        }
        
        
    }
    
    spr->setTag(idx);
    pCell->addChild(spr);
    
    if(idx == 0){
        if (selectedIndex == idx) {// 大的
            sprNode = pCell->getChildByTag(selectedIndex);
        }
    }else if (idx == 1){
        if (selectedIndex == idx) {// 大的
            sprNode = pCell->getChildByTag(selectedIndex);
        }
    }else if (idx == 2){
        if (selectedIndex == idx) {// 大的
            sprNode = pCell->getChildByTag(selectedIndex);
        }
    }else{
        if (selectedIndex == idx) {// 大的
            sprNode = pCell->getChildByTag(selectedIndex);
        }
    }
    
    return pCell;
}

void StrangerTableView::bigSprite(int index, CCSprite* spr){
    CCSprite* bg;
//    if (index == 0) {
//        bg = CCSprite::create("res/pic/haoyoupaihang/first_bg_sel.png");
//    }else if(index == 1){
//        bg = CCSprite::create("res/pic/haoyoupaihang/second_bg_sel.png");
//    }else if (index == 2){
//        bg = CCSprite::create("res/pic/haoyoupaihang/third_bg_sel.png");
//    }else{
        bg = CCSprite::create("res/pic/haoyoupaihang/other_bg_sel.png");
//    }
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(CCPointZero);
    bg->setPosition(CCPoint(0, 0));
    bg->setTag(0x10000);
    spr->addChild(bg);
    
    CCString* show_id = (CCString* )_stangers->objectAtIndex(index);
    ShowComp* show = (ShowComp* )DATA->getSocial()->strangers()->objectForKey(show_id->getCString());
    const char* nickname = show->nickname();
    int collected = show->collected();

    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(bg->getContentSize().width* .63f, bg->getContentSize().height* .76));
    name->setColor(ccc3(109, 98, 96));
    name->setTag(0x10500);
    bg->addChild(name);
    
    CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
    flag->setPosition(ccp(bg->getContentSize().width* .5f, bg->getContentSize().height* .36));
    flag->setTag(0x10111);
    bg->addChild((flag));
    
    CCString* collected_str = CCString::createWithFormat("%d", collected);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collected_str->getCString(), DISPLAY->fangzhengFont(), 22);
    cloth_count->setAnchorPoint(CCPoint(0, 0.5));
    cloth_count->setPosition(ccp(bg->getContentSize().width * .75, bg->getContentSize().height* .34f));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    

    CCSprite* add_spr1 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr2 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    add_spr2->setScale(1.02f);
    CCMenuItemSprite* item_add;
    bool hasAdd = DATA->getSocial()->is_requested(show_id->getCString());
    if (true == hasAdd) {
        item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
        item_add->setTag(index);
        item_add->setUserData(show_id);
        CCMenu* menu_add = CCMenu::create(item_add, NULL);
        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
        menu_add->setTag(0x10400);
        bg->addChild(menu_add);
    }
    else if (show->isadd == 0) {
        item_add = CCMenuItemSprite::create(add_spr1, add_spr2, this, menu_selector(StrangerTableView::toBeFriend));
        item_add->setTag(index);
        item_add->setUserData(show_id);
        CCMenu* menu_add = CCMenu::create(item_add, NULL);
        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr1->getContentSize().width/2 -10, 8));
        menu_add->setTag(0x10400);
        bg->addChild(menu_add);
    }else{
        item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
        item_add->setTag(index);
        item_add->setUserData(show_id);
        CCMenu* menu_add = CCMenu::create(item_add, NULL);
        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
        menu_add->setTag(0x10400);
        bg->addChild(menu_add);
    }
}
void StrangerTableView::smallSprite(int index, CCSprite* spr){
    CCSprite* bg;
//    if (index == 0) {
//        bg = CCSprite::create("res/pic/haoyoupaihang/first_bg_nor.png");
//    }else if(index == 1){
//        bg = CCSprite::create("res/pic/haoyoupaihang/second_bg_nor.png");
//    }else if (index == 2){
//        bg = CCSprite::create("res/pic/haoyoupaihang/third_bg_nor.png");
//    }else{
        bg = CCSprite::create("res/pic/haoyoupaihang/other_bg_nor.png");
//    }
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(ccp(0, 0));
    bg->setTag(0x10000);
    spr->addChild(bg);
    
    CCString* show_id = (CCString* )_stangers->objectAtIndex(index);
    ShowComp* show = (ShowComp* )DATA->getSocial()->strangers()->objectForKey(show_id->getCString());
    
    const char* nickname = show->nickname();
    int collected = show->collected();
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(bg->getContentSize().width* .75f, bg->getContentSize().height* .68));
    name->setColor(ccc3(109, 98, 96));
    name->setTag(0x10500);
    bg->addChild(name);
    
    CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
    flag->setPosition(ccp(bg->getContentSize().width* .6f, bg->getContentSize().height* .38));
    flag->setTag(0x10111);
    bg->addChild((flag));
    
    CCString* collected_str = CCString::createWithFormat("%d", collected);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collected_str->getCString(), DISPLAY->fangzhengFont(), 20);
    cloth_count->setAnchorPoint(CCPoint(0, 0.5f));
    cloth_count->setPosition(ccp(bg->getContentSize().width * .78f, bg->getContentSize().height* .36f));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
    CCSprite* add_spr1 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr2 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    add_spr2->setScale(1.02f);
    
    CCMenuItemSprite* item_add;

    bool hasAdd = DATA->getSocial()->is_requested(show_id->getCString());
    if (true == hasAdd) {
        item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
        item_add->setTag(index);
        item_add->setUserData(show_id);
        CCMenu* menu_add = CCMenu::create(item_add, NULL);
        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
        menu_add->setTag(0x10400);
        menu_add->setVisible(false);
        bg->addChild(menu_add);
    }
    else if (show->isadd == 0) {
        item_add = CCMenuItemSprite::create(add_spr1, add_spr2, this, menu_selector(StrangerTableView::toBeFriend));
        item_add->setTag(index);
        item_add->setUserData(show_id);
        CCMenu* menu_add = CCMenu::create(item_add, NULL);
        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr1->getContentSize().width/2 -10, 8));
        menu_add->setTag(0x10400);
        menu_add->setVisible(false);
        bg->addChild(menu_add);
    }else{
        item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
        item_add->setTag(index);
        item_add->setUserData(show_id);
        CCMenu* menu_add = CCMenu::create(item_add, NULL);
        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
        menu_add->setTag(0x10400);
        menu_add->setVisible(false);
        bg->addChild(menu_add);
    }
}

void StrangerTableView::toBeFriend(CCMenuItem* btn){
    CCMenuItem* item = (CCMenuItem* )btn;
    CCString* other_id = (CCString*)btn->getUserData();
    
    CCNode* bg = (CCNode* )item->getParent()->getParent();
    bg->removeChildByTag(0x10400);
    CCDictionary* strangers = DATA->getSocial()->strangers();
    ShowComp* other = (ShowComp*)strangers->objectForKey(other_id->getCString());
    other->isadd = 1;
    
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCMenuItemSprite* item_add;
    item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
    CCMenu* menu_add = CCMenu::create(item_add, NULL);
    menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
    menu_add->setTag(0X10400);
    bg->addChild(menu_add);
    
    NET->send_message_803(other_id->getCString(), 1);
}

unsigned int StrangerTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}
