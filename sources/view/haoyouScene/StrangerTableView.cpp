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

StrangerTableView::~StrangerTableView(){}

bool StrangerTableView::init(){
    if(!CCLayer::init()){
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/haoyoupaihang/panel.plist");
    
    _stangers = DATA->getSocial()->strangers()->allKeys();
    _stangers->retain();
    allNumber =  _stangers->count();
    selectedIndex = 0;
    
    
    pTableView = CCTableView::create(this, CCSizeMake(248, 6*138));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setAnchorPoint(CCPointZero);
    pTableView->setPosition(CCPointZero);
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    
    this->addChild(pTableView, 1);
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    return true;
}

//CCTableViewDelegate继承自CCScrollViewDelegate
void StrangerTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    
    CCPoint contOffsetPos = pTableView->getContentOffset();
    if (allNumber > 6) {
        if (contOffsetPos.y < pTableView->minContainerOffset().y) {
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->minContainerOffset().y));
        }else if (contOffsetPos.y > pTableView->maxContainerOffset().y){
            pTableView->setContentOffset(CCPoint(contOffsetPos.x, pTableView->maxContainerOffset().y));
        }
    }
}

//点击哪个cell
void StrangerTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    if (selectedIndex != cell->getIdx()) {
        // 需要变小
        CCSprite* bg1 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCString* bg_str1 = CCString::createWithFormat("panel_normal.png");

            CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str1->getCString());
            bg1->setDisplayFrame(frame1);
            bg1->setPosition(ccp(10, 0));
            
            if (bg1->getChildByTag(0x10100) != NULL) {
                bg1->removeChildByTag(0x10100);
            }
            CCSprite* head1 = CCSprite::create("res/pic/haoyoupaihang/cell_head_normal.png");
            head1->setPosition(ccp(head1->getContentSize().width + 4, bg1->getContentSize().height/2 + 5));
            head1->setTag(0x10100);
            bg1->addChild(head1);
            
            if (bg1->getChildByTag(0x10200) != NULL) {
                bg1->removeChildByTag(0x10200);
            }
            CCSprite* name_bg1 = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
            name_bg1->setPosition(ccp(bg1->getContentSize().width - name_bg1->getContentSize().width/2, 100));
            name_bg1->setTag(0x10200);
            bg1->addChild(name_bg1);
        
        CCString* show_id = (CCString* )_stangers->objectAtIndex(selectedIndex);
        ShowComp* show = (ShowComp* )DATA->getSocial()->strangers()->objectForKey(show_id->getCString());
        const char* nickname = show->nickname();
        
            CCLabelTTF* name1 = CCLabelTTF::create(nickname, DISPLAY->font(), 25, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 15, name_bg1->getContentSize().height/2));
            name1->setTag(0x10500);
            name_bg1->addChild(name1);
        
        CCSprite* n_spr1 = CCSprite::create("res/pic/haoyoupaihang/head_n_normal.png");
        n_spr1->setPosition(ccp(head1->getContentSize().width/2, head1->getContentSize().height/2));
        head1->addChild(n_spr1);
            

        
        // 记录需要变大节点
        selectedIndex = cell->getIdx();
        
        // 需要变大
        sprNode = (CCSprite*)cell->getChildByTag(selectedIndex);
        CCSprite* bg2 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCString* bg_str2 = CCString::createWithFormat("panel_selected.png");

            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str2->getCString());
            bg2->setDisplayFrame(frame2);
            bg2->setPosition(ccp(0, 0));
            
            if (bg2->getChildByTag(0x10100) != NULL) {
                bg2->removeChildByTag(0x10100);
            }
            CCSprite* head2 = CCSprite::create("res/pic/haoyoupaihang/cell_head_selected.png");
            head2->setPosition(ccp(head2->getContentSize().width - 8, bg2->getContentSize().height/2 + 3));
            head2->setTag(0x10100);
            bg2->addChild(head2);
            
            if (bg2->getChildByTag(0x10200) != NULL) {
                bg2->removeChildByTag(0x10200);
            }
            CCSprite* name_bg2 = CCSprite::create("res/pic/haoyoupaihang/namebar_selected.png");
            name_bg2->setPosition(ccp(bg2->getContentSize().width - name_bg2->getContentSize().width/2, 105));
            name_bg2->setTag(0x10200);
            bg2->addChild(name_bg2);
        
        CCString* show_id2 = (CCString* )_stangers->objectAtIndex(selectedIndex);
        ShowComp* show2 = (ShowComp* )DATA->getSocial()->strangers()->objectForKey(show_id2->getCString());
        const char* nickname2 = show2->nickname();
        
            CCLabelTTF* name2 = CCLabelTTF::create(nickname2, DISPLAY->font(), 25, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
            name2->setTag(0x10500);
            name_bg2->addChild(name2);
        
        CCSprite* n_spr2 = CCSprite::create("res/pic/haoyoupaihang/head_n_selected.png");
        n_spr2->setPosition(ccp(head2->getContentSize().width/2, head2->getContentSize().height/2));
        head2->addChild(n_spr2);
        

        
        //        CCNotificationCenter::sharedNotificationCenter()->postNotification("ChangeClothes", (CCObject* )selectedIndex);
    }
}

//每个cell的size
cocos2d::CCSize StrangerTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(248, 138);
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
    CCSprite* bg = CCSprite::createWithSpriteFrameName("panel_selected.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(CCPointZero);
    bg->setTag(0x10000);
    spr->addChild(bg);
    
    CCString* show_id = (CCString* )_stangers->objectAtIndex(index);
    ShowComp* show = (ShowComp* )DATA->getSocial()->strangers()->objectForKey(show_id->getCString());
    const char* nickname = show->nickname();
    int collected = show->collected();
    
    
    CCSprite* head;
//    if (index == 0) {
//        head = CCSprite::create("res/pic/haoyoupaihang/first_selected.png");
//    }else if (index == 1){
//        head = CCSprite::create("res/pic/haoyoupaihang/second_selected.png");
//    }else if (index == 2){
//        head = CCSprite::create("res/pic/haoyoupaihang/thrid_selected.png");
//    }else{
        head = CCSprite::create("res/pic/haoyoupaihang/cell_head_selected.png");
//    }
    head->setPosition(ccp(head->getContentSize().width - 8, bg->getContentSize().height/2 + 3));
    head->setTag(0x10100);
    bg->addChild(head);
    
    CCSprite* n_spr = CCSprite::create("res/pic/haoyoupaihang/head_n_selected.png");
    n_spr->setPosition(ccp(head->getContentSize().width/2, head->getContentSize().height/2));
    head->addChild(n_spr);
    
    CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_selected.png");
    name_bg->setPosition(ccp(bg->getContentSize().width - name_bg->getContentSize().width/2, 105));
    name_bg->setTag(0x10200);
    bg->addChild(name_bg);
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->font(), 25, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(name_bg->getContentSize().width/2, name_bg->getContentSize().height/2));
    name->setTag(0x10500);
    name_bg->addChild(name);
    
    CCString* collected_str = CCString::createWithFormat("%d", collected);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collected_str->getCString(), DISPLAY->font(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count->setPosition(ccp(bg->getContentSize().width * .8, bg->getContentSize().height/2));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
    CCSprite* add_spr = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr2 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    add_spr2->setScale(1.02f);
    CCMenuItemSprite* item_add = CCMenuItemSprite::create(add_spr, add_spr2, this, menu_selector(StrangerTableView::toBeFriend));
    item_add->setUserData(show_id);
    CCMenu* menu_add = CCMenu::create(item_add, NULL);
    menu_add->setPosition(ccp(bg->getContentSize().width - add_spr->getContentSize().width/2 - 10, 38));
    menu_add->setTag(0x10400);
    bg->addChild(menu_add);
    
    //    CCLabelAtlas
}
void StrangerTableView::smallSprite(int index, CCSprite* spr){
    CCSprite* bg = CCSprite::createWithSpriteFrameName("panel_normal.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(ccp(10, 0));
    bg->setTag(0x10000);
    spr->addChild(bg);
    
    CCString* show_id = (CCString* )_stangers->objectAtIndex(index);
    ShowComp* show = (ShowComp* )DATA->getSocial()->strangers()->objectForKey(show_id->getCString());
    const char* nickname = show->nickname();
    int collected = show->collected();
    
    CCSprite* head;
//    if (index == 0) {
//        head = CCSprite::create("res/pic/haoyoupaihang/first.png");
//    }else if (index == 1){
//        head = CCSprite::create("res/pic/haoyoupaihang/second.png");
//    }else if (index == 2){
//        head = CCSprite::create("res/pic/haoyoupaihang/thrid.png");
//    }else{
        head = CCSprite::create("res/pic/haoyoupaihang/cell_head_normal.png");
//    }
    head->setPosition(ccp(head->getContentSize().width + 4, bg->getContentSize().height/2 + 5));
    head->setTag(0x10100);
    bg->addChild(head);
    
    CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
    name_bg->setPosition(ccp(bg->getContentSize().width - name_bg->getContentSize().width/2, 100));
    name_bg->setTag(0x10200);
    bg->addChild(name_bg);
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->font(), 24, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(name_bg->getContentSize().width/2 - 15, name_bg->getContentSize().height/2));
    name->setTag(0x10500);
    name_bg->addChild(name);
    
    CCString* collected_str = CCString::createWithFormat("%d", collected);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collected_str->getCString(), DISPLAY->font(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count->setPosition(ccp(bg->getContentSize().width * .8, bg->getContentSize().height/2));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
    CCSprite* add_spr = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr2 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    add_spr2->setScale(1.02f);
    CCMenuItemSprite* item_add = CCMenuItemSprite::create(add_spr, add_spr2, this, menu_selector(StrangerTableView::toBeFriend));
    item_add->setUserData(show_id);
    CCMenu* menu_add = CCMenu::create(item_add, NULL);
    menu_add->setPosition(ccp(bg->getContentSize().width - add_spr->getContentSize().width/2 -10, 38));
    menu_add->setTag(0x10400);
    bg->addChild(menu_add);
    
    CCSprite* n_spr = CCSprite::create("res/pic/haoyoupaihang/head_n_normal.png");
    n_spr->setPosition(ccp(head->getContentSize().width/2, head->getContentSize().height/2));
    head->addChild(n_spr);
    

}

void StrangerTableView::toBeFriend(CCMenuItem* btn){
    CCString* other_id = (CCString*)btn->getUserData();
    NET->send_message_803(other_id->getCString(), 1);
}

unsigned int StrangerTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}

void StrangerTableView::onEnter(){
    CCLayer::onEnter();
}

void StrangerTableView::onExit(){
    CCLayer::onExit();
}