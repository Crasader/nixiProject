//
//  HaoyouRankTableView.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/18.
//
//

#include "HaoyouRankTableView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "PromptLayer.h"
#include "Loading2.h"
#include "NetManager.h"
#include "HaoyouRankLayer.h"

const int NAME_FONT_SIZE = 20;

HaoyouRankTableView::~HaoyouRankTableView(){}

bool HaoyouRankTableView::init(){
    if(!CCLayer::init()){
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/haoyoupaihang/panel.plist");
    
    
    _friends = DATA->getSocial()->friends()->allKeys();
    if (_friends == NULL) {
        _friends = CCArray::create();
    }
    _friends->retain();
    allNumber = _friends->count();
    selectedIndex = 0;
    if (allNumber == 0) {
        DATA->getSocial()->setSelectedFriend(-1);
    }else{
        DATA->getSocial()->setSelectedFriend(selectedIndex);
    }
    
    pTableView = CCTableView::create(this, CCSizeMake(248, 6*138));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setAnchorPoint(CCPointZero);
    pTableView->setPosition(CCPointZero);
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    
    this->addChild(pTableView, 1);
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    
    if (allNumber <= 0) {
        CCSprite* spr = CCSprite::create("res/pic/haoyoupaihang/panel_normal.png");
        CCLabelTTF* lab = CCLabelTTF::create("还没有好友呢，赶快去陌生人里寻找小伙伴吧~", DISPLAY->fangzhengFont(), 20, CCSizeMake(150, 300), kCCTextAlignmentLeft);
        lab->setColor(ccc3(135, 108, 123));
        lab->setPosition(ccp(pTableView->getContentSize().width + spr->getContentSize().width * .5f + 30, DISPLAY->ScreenHeight()* .5f));
        this->addChild(lab);
    }
    
    return true;
}

//CCTableViewDelegate继承自CCScrollViewDelegate
void HaoyouRankTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    
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
void HaoyouRankTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    if (selectedIndex != cell->getIdx()) {
        // 需要变小
        CCSprite* bg1 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCString* bg_str1 = CCString::createWithFormat("panel_normal.png");
        if(selectedIndex == 0){
            CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str1->getCString());
            bg1->setDisplayFrame(frame1);
            bg1->setPosition(ccp(10, 0));
            
            if (bg1->getChildByTag(0x10100) != NULL) {
                bg1->removeChildByTag(0x10100);
            }
            CCSprite* head1 = CCSprite::create("res/pic/haoyoupaihang/first.png");
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
            
            CCString* show_id = (CCString*)_friends->objectAtIndex(selectedIndex);
            ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
            const char* nickname = show->nickname();
            CCLabelTTF* name1 = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 10, name_bg1->getContentSize().height/2));
            name1->setTag(0x10500);
            name_bg1->addChild(name1);
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(false);
            
            
        }else if (selectedIndex == 1){
            CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str1->getCString());
            bg1->setDisplayFrame(frame1);
            bg1->setPosition(ccp(10, 0));
            
            if (bg1->getChildByTag(0x10100) != NULL) {
                bg1->removeChildByTag(0x10100);
            }
            CCSprite* head1 = CCSprite::create("res/pic/haoyoupaihang/second.png");
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
            
            CCString* show_id = (CCString*)_friends->objectAtIndex(selectedIndex);
            ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
            const char* nickname = show->nickname();
            CCLabelTTF* name1 = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 10, name_bg1->getContentSize().height/2));
            name1->setTag(0x10500);
            name_bg1->addChild(name1);
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(false);
            
            
        }else if (selectedIndex == 2){
            CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str1->getCString());
            bg1->setDisplayFrame(frame1);
            bg1->setPosition(ccp(10, 0));
            
            if (bg1->getChildByTag(0x10100) != NULL) {
                bg1->removeChildByTag(0x10100);
            }
            CCSprite* head1 = CCSprite::create("res/pic/haoyoupaihang/thrid.png");
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
            
            CCString* show_id = (CCString*)_friends->objectAtIndex(selectedIndex);
            ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
            const char* nickname = show->nickname();
            CCLabelTTF* name1 = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 10, name_bg1->getContentSize().height/2));
            name1->setTag(0x10500);
            name_bg1->addChild(name1);
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(false);
            
            
        }else{
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
            
            CCString* show_id = (CCString*)_friends->objectAtIndex(selectedIndex);
            ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
            const char* nickname = show->nickname();
            CCLabelTTF* name1 = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 10, name_bg1->getContentSize().height/2));
            name1->setTag(0x10500);
            name_bg1->addChild(name1);
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(false);
            
            CCLabelAtlas* rank_num;
            CCString* str_num = CCString::createWithFormat("%d", selectedIndex + 1);
            if (selectedIndex < 9) {
                rank_num =CCLabelAtlas::create(str_num->getCString(), "res/pic/haoyoupaihang/num_single_normal.png", 25, 34, '4');
            }else{
                rank_num = CCLabelAtlas::create(str_num->getCString(), "res/pic/haoyoupaihang/num_double_normal.png", 14, 21, '0');
            }
            rank_num->setAnchorPoint(ccp(0.5, 0.5));
            rank_num->setPosition(ccp(head1->getContentSize().width/2, head1->getContentSize().height/2));
            rank_num->setTag(0x20100);
            head1->addChild(rank_num);
        }
        
        // 记录需要变大节点
        selectedIndex = cell->getIdx();
        DATA->getSocial()->setSelectedFriend(selectedIndex);
        
        // 需要变大
        sprNode = (CCSprite*)cell->getChildByTag(selectedIndex);
        CCSprite* bg2 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCString* bg_str2 = CCString::createWithFormat("panel_selected.png");
        if(selectedIndex == 0){
            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str2->getCString());
            bg2->setDisplayFrame(frame2);
            bg2->setPosition(ccp(0, 0));
            
            if (bg2->getChildByTag(0x10100) != NULL) {
                bg2->removeChildByTag(0x10100);
            }
            CCSprite* head2 = CCSprite::create("res/pic/haoyoupaihang/first_selected.png");
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
            
            CCString* show_id2 = (CCString*)_friends->objectAtIndex(selectedIndex);
            ShowComp* show2 = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id2->getCString());
            const char* nickname2 = show2->nickname();
            CCLabelTTF* name2 = CCLabelTTF::create(nickname2, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
            name2->setTag(0x10500);
            name_bg2->addChild(name2);
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(true);
            
            
        }else if (selectedIndex == 1){
            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str2->getCString());
            bg2->setDisplayFrame(frame2);
            bg2->setPosition(ccp(0, 0));
            
            if (bg2->getChildByTag(0x10100) != NULL) {
                bg2->removeChildByTag(0x10100);
            }
            CCSprite* head2 = CCSprite::create("res/pic/haoyoupaihang/second_selected.png");
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
            
            CCString* show_id2 = (CCString*)_friends->objectAtIndex(selectedIndex);
            ShowComp* show2 = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id2->getCString());
            const char* nickname2 = show2->nickname();
            CCLabelTTF* name2 = CCLabelTTF::create(nickname2, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
            name2->setTag(0x10500);
            name_bg2->addChild(name2);
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(true);
            
            
        }else if (selectedIndex == 2){
            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str2->getCString());
            bg2->setDisplayFrame(frame2);
            bg2->setPosition(ccp(0, 0));
            
            if (bg2->getChildByTag(0x10100) != NULL) {
                bg2->removeChildByTag(0x10100);
            }
            CCSprite* head2 = CCSprite::create("res/pic/haoyoupaihang/thrid_selected.png");
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
            
            CCString* show_id2 = (CCString*)_friends->objectAtIndex(selectedIndex);
            ShowComp* show2 = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id2->getCString());
            const char* nickname2 = show2->nickname();
            CCLabelTTF* name2 = CCLabelTTF::create(nickname2, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
            name2->setTag(0x10500);
            name_bg2->addChild(name2);
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(true);
            
            
        }else{
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
            
            CCString* show_id2 = (CCString*)_friends->objectAtIndex(selectedIndex);
            ShowComp* show2 = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id2->getCString());
            const char* nickname2 = show2->nickname();
            CCLabelTTF* name2 = CCLabelTTF::create(nickname2, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
            name2->setTag(0x10500);
            name_bg2->addChild(name2);
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(true);
            
            CCLabelAtlas* rank_num;
            CCString* str_num = CCString::createWithFormat("%d", selectedIndex + 1);
            if (selectedIndex < 9) {
                rank_num =CCLabelAtlas::create(str_num->getCString(), "res/pic/haoyoupaihang/num_single_selected.png", 29, 39, '4');
            }else{
                rank_num = CCLabelAtlas::create(str_num->getCString(), "res/pic/haoyoupaihang/num_double_selected.png", 16, 23, '0');
            }
            rank_num->setAnchorPoint(ccp(0.5, 0.5));
            rank_num->setPosition(ccp(head2->getContentSize().width/2, head2->getContentSize().height/2));
            rank_num->setTag(0x20100);
            head2->addChild(rank_num);
        }
        
        CCLayer* layer = CCLayer::create();
        layer->setTouchEnabled(true);
        layer->setTouchSwallowEnabled(true);
        layer->setTag(10000);
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ExitMan");
    }
}

//每个cell的size
cocos2d::CCSize HaoyouRankTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(248, 138);
}

//生成cell
cocos2d::extension::CCTableViewCell* HaoyouRankTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx){
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

void HaoyouRankTableView::bigSprite(int index, CCSprite* spr){
    CCSprite* bg = CCSprite::createWithSpriteFrameName("panel_selected.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(CCPointZero);
    bg->setTag(0x10000);
    spr->addChild(bg);
    
    CCString* show_id = (CCString*)_friends->objectAtIndex(index);
    ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
    const char* nickname = show->nickname();
    int collected = show->collected();
    
    CCSprite* head;
    if (index == 0) {
        head = CCSprite::create("res/pic/haoyoupaihang/first_selected.png");
    }else if (index == 1){
        head = CCSprite::create("res/pic/haoyoupaihang/second_selected.png");
    }else if (index == 2){
        head = CCSprite::create("res/pic/haoyoupaihang/thrid_selected.png");
    }else{
        head = CCSprite::create("res/pic/haoyoupaihang/cell_head_selected.png");
    }
    head->setPosition(ccp(head->getContentSize().width - 8, bg->getContentSize().height/2 + 3));
    head->setTag(0x10100);
    bg->addChild(head);
    
    CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_selected.png");
    name_bg->setPosition(ccp(bg->getContentSize().width - name_bg->getContentSize().width/2, 105));
    name_bg->setTag(0x10200);
    bg->addChild(name_bg);
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(name_bg->getContentSize().width/2, name_bg->getContentSize().height/2));
    name->setTag(0x10500);
    name_bg->addChild(name);
    
    CCString* collect_str = CCString::createWithFormat("%d", collected);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count->setPosition(ccp(bg->getContentSize().width * .8, bg->getContentSize().height/2));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
//    SocialComp* social = DATA->getSocial();
//    if (social->has_send_energy(show_id->getCString())) {
//        CCSprite* tili_spr = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
//        CCSprite* tili_spr2 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
//        CCMenuItemSprite* item_tili = CCMenuItemSprite::create(tili_spr, tili_spr2);
//        item_tili->setEnabled(false);
//        CCMenu* menu_tili = CCMenu::create(item_tili, NULL);
//        menu_tili->setPosition(ccp(bg->getContentSize().width - tili_spr->getContentSize().width/2 - 10, 38));
//        bg->addChild(menu_tili);
//    }
//    else {
//        CCSprite* tili_spr = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
//        CCSprite* tili_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
//        tili_spr2->setScale(1.02f);
//        CCMenuItemSprite* item_tili = CCMenuItemSprite::create(tili_spr, tili_spr2, this, menu_selector(HaoyouRankTableView::sendTili));
//        item_tili->setUserObject(show_id);
//        CCMenu* menu_tili = CCMenu::create(item_tili, NULL);
//        menu_tili->setPosition(ccp(bg->getContentSize().width - tili_spr->getContentSize().width/2 - 10, 38));
//        menu_tili->setTag(0x10400);
//        bg->addChild(menu_tili);
//    }
    
    CCSprite* tili_spr1 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
    CCSprite* tili_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
    CCSprite* tili_spr3 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    CCSprite* tili_spr4 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    tili_spr2->setScale(1.02f);
    CCMenuItemSprite* item_send;
    if (show->issend == 0) {
        item_send = CCMenuItemSprite::create(tili_spr1, tili_spr2, this, menu_selector(HaoyouRankTableView::sendTili));
        item_send->setTag(index);
        item_send->setUserData(show_id);
        CCMenu* menu_send = CCMenu::create(item_send, NULL);
        menu_send->setPosition(ccp(bg->getContentSize().width - tili_spr1->getContentSize().width/2 -10, 38));
        menu_send->setTag(0x10400);
        bg->addChild(menu_send);
    }else{
        item_send = CCMenuItemSprite::create(tili_spr3, tili_spr4, this, NULL);
        item_send->setTag(index);
        item_send->setUserData(show_id);
        CCMenu* menu_send = CCMenu::create(item_send, NULL);
        menu_send->setPosition(ccp(bg->getContentSize().width - tili_spr3->getContentSize().width/2 -10, 38));
        menu_send->setTag(0x10400);
        bg->addChild(menu_send);
    }
}
void HaoyouRankTableView::smallSprite(int index, CCSprite* spr){
    CCSprite* bg = CCSprite::createWithSpriteFrameName("panel_normal.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(ccp(10, 0));
    bg->setTag(0x10000);
    spr->addChild(bg);
    
    CCString* show_id = (CCString*)_friends->objectAtIndex(index);
    ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
    const char* nickname = show->nickname();
    int collected = show->collected();
    
    CCSprite* head;
    if (index == 0) {
        head = CCSprite::create("res/pic/haoyoupaihang/first.png");
    }else if (index == 1){
        head = CCSprite::create("res/pic/haoyoupaihang/second.png");
    }else if (index == 2){
        head = CCSprite::create("res/pic/haoyoupaihang/thrid.png");
    }else{
        head = CCSprite::create("res/pic/haoyoupaihang/cell_head_normal.png");
    }
    head->setPosition(ccp(head->getContentSize().width + 4, bg->getContentSize().height/2 + 5));
    head->setTag(0x10100);
    bg->addChild(head);
    
    CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
    name_bg->setPosition(ccp(bg->getContentSize().width - name_bg->getContentSize().width/2, 100));
    name_bg->setTag(0x10200);
    bg->addChild(name_bg);
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30),  kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(name_bg->getContentSize().width/2 - 10, name_bg->getContentSize().height/2));
    name->setTag(0x10500);
    name_bg->addChild(name);
    
    CCString* collect_str = CCString::createWithFormat("%d", collected);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count->setPosition(ccp(bg->getContentSize().width * .8, bg->getContentSize().height/2));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
//    SocialComp* social = DATA->getSocial();
//    if (social->has_send_energy(show_id->getCString())) {
//        CCSprite* tili_spr = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
//        CCSprite* tili_spr2 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
//        CCMenuItemSprite* item_tili = CCMenuItemSprite::create(tili_spr, tili_spr2);
//        item_tili->setEnabled(false);
//        CCMenu* menu_tili = CCMenu::create(item_tili, NULL);
//        menu_tili->setPosition(ccp(bg->getContentSize().width - tili_spr->getContentSize().width/2 - 10, 38));
//        bg->addChild(menu_tili);
//    }
//    else {
//        CCSprite* tili_spr = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
//        CCSprite* tili_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
//        tili_spr2->setScale(1.02f);
//        CCMenuItemSprite* item_tili = CCMenuItemSprite::create(tili_spr, tili_spr2, this, menu_selector(HaoyouRankTableView::sendTili));
//        item_tili->setUserObject(show_id);
//        CCMenu* menu_tili = CCMenu::create(item_tili, NULL);
//        menu_tili->setPosition(ccp(bg->getContentSize().width - tili_spr->getContentSize().width/2 - 10, 38));
//        menu_tili->setTag(0x10400);
//        bg->addChild(menu_tili);
//    }
    
    CCSprite* tili_spr1 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
    CCSprite* tili_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
    CCSprite* tili_spr3 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    CCSprite* tili_spr4 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    tili_spr2->setScale(1.02f);
    CCMenuItemSprite* item_send;
    if (show->issend == 0) {
        item_send = CCMenuItemSprite::create(tili_spr1, tili_spr2, this, menu_selector(HaoyouRankTableView::sendTili));
        item_send->setTag(index);
        item_send->setUserData(show_id);
        CCMenu* menu_send = CCMenu::create(item_send, NULL);
        menu_send->setPosition(ccp(bg->getContentSize().width - tili_spr1->getContentSize().width/2 -10, 38));
        menu_send->setTag(0x10400);
        bg->addChild(menu_send);
    }else{
        item_send = CCMenuItemSprite::create(tili_spr3, tili_spr4, this, NULL);
        item_send->setTag(index);
        item_send->setUserData(show_id);
        CCMenu* menu_send = CCMenu::create(item_send, NULL);
        menu_send->setPosition(ccp(bg->getContentSize().width - tili_spr3->getContentSize().width/2 -10, 38));
        menu_send->setTag(0x10400);
        bg->addChild(menu_send);
    }
    
    if (index > 2) {
        CCLabelAtlas* rank_num;
        CCString* str_num = CCString::createWithFormat("%d", index + 1);
        if (index < 9) {
            rank_num =CCLabelAtlas::create(str_num->getCString(), "res/pic/haoyoupaihang/num_single_normal.png", 25, 34, '4');
        }else{
            rank_num = CCLabelAtlas::create(str_num->getCString(), "res/pic/haoyoupaihang/num_double_normal.png", 14, 21, '0');
        }
        rank_num->setAnchorPoint(ccp(0.5, 0.5));
        rank_num->setPosition(ccp(head->getContentSize().width/2, head->getContentSize().height/2));
        rank_num->setTag(0x20100);
        head->addChild(rank_num);
    }
}

void HaoyouRankTableView::sendTili(CCMenuItem* btn){    
    CCString* other_id = (CCString*)btn->getUserObject();
//    SocialComp* social = DATA->getSocial();
//    if (social->has_send_energy(other_id->getCString())) {
//        PromptLayer* prompt = PromptLayer::create();
//        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "今日已送出体力，请明日再试!~");
//    }
//    else {
//        LOADING->show_loading();
//        NET->send_message_803(other_id->getCString(), e_Msg_Send_Energy);
//    }
    
    CCMenuItem* item = (CCMenuItem* )btn;
    
    CCNode* bg = (CCNode* )item->getParent()->getParent();
    bg->removeChildByTag(0x10400);
    CCDictionary* strangers = DATA->getSocial()->strangers();
    ShowComp* other = (ShowComp*)strangers->objectForKey(other_id->getCString());
    other->issend = 1;
    
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
    CCMenuItemSprite* item_add;
    item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
    CCMenu* menu_add = CCMenu::create(item_add, NULL);
    menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 38));
    menu_add->setTag(0X10400);
    bg->addChild(menu_add);
    
    NET->send_message_803(other_id->getCString(), e_Msg_Send_Energy);
}

void HaoyouRankTableView::send_message_callback_803(cocos2d::CCObject *pObj) {
//    LOADING->remove();
//    pTableView->reloadData();
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "体力赠送成功");
}

unsigned int HaoyouRankTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}

void HaoyouRankTableView::onEnter(){
    CCLayer::onEnter();
}

void HaoyouRankTableView::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCLayer::onExit();
}