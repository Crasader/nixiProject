//
//  TotalRankTableView.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#include "TotalRankTableView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "TotalRankScene.h"

const float NAME_FONT_SIZE = 20;

TotalRankTableView::~TotalRankTableView(){}

bool TotalRankTableView::init(){
    if(!CCLayer::init()){
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/haoyoupaihang/panel.plist");
    
    allNumber = 0;
    selectedIndex = NULL;
    
    allNumber = 20;
    
    pTableView = CCTableView::create(this, CCSizeMake(248, 3*138));
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
void TotalRankTableView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view){
    
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
void TotalRankTableView::tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell){
    
    if (selectedIndex == NULL) {
        selectedIndex = cell->getIdx() + 3;
        
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
        
        CCLabelTTF* name2 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
        name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
        name2->setTag(0x10500);
        name_bg2->addChild(name2);
        
        CCLabelAtlas* rank_num2;
        CCString* str_num2 = CCString::createWithFormat("%d", selectedIndex + 1);
        if (selectedIndex < 9) {
            rank_num2 =CCLabelAtlas::create(str_num2->getCString(), "res/pic/haoyoupaihang/num_single_selected.png", 29, 39, '4');
        }else{
            rank_num2 = CCLabelAtlas::create(str_num2->getCString(), "res/pic/haoyoupaihang/num_double_selected.png", 16, 23, '0');
        }
        rank_num2->setAnchorPoint(ccp(0.5, 0.5));
        rank_num2->setPosition(ccp(head2->getContentSize().width/2, head2->getContentSize().height/2));
        rank_num2->setTag(0x20100);
        head2->addChild(rank_num2);
    }else {
    
    if (selectedIndex != cell->getIdx()) {
        // 需要变小
        CCSprite* bg1 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCString* bg_str1 = CCString::createWithFormat("panel_normal.png");
//        if(selectedIndex == 0){
//            CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str1->getCString());
//            bg1->setDisplayFrame(frame1);
//            bg1->setPosition(ccp(10, 0));
//            
//            if (bg1->getChildByTag(0x10100) != NULL) {
//                bg1->removeChildByTag(0x10100);
//            }
//            CCSprite* head1 = CCSprite::create("res/pic/haoyoupaihang/first.png");
//            head1->setPosition(ccp(head1->getContentSize().width + 4, bg1->getContentSize().height/2 + 5));
//            head1->setTag(0x10100);
//            bg1->addChild(head1);
//            
//            if (bg1->getChildByTag(0x10200) != NULL) {
//                bg1->removeChildByTag(0x10200);
//            }
//            CCSprite* name_bg1 = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
//            name_bg1->setPosition(ccp(bg1->getContentSize().width - name_bg1->getContentSize().width/2, 100));
//            name_bg1->setTag(0x10200);
//            bg1->addChild(name_bg1);
//            
//            CCLabelTTF* name1 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
//            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 10, name_bg1->getContentSize().height/2));
//            name1->setTag(0x10500);
//            name_bg1->addChild(name1);
//            
//            
//            
//        }else if (selectedIndex == 1){
//            CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str1->getCString());
//            bg1->setDisplayFrame(frame1);
//            bg1->setPosition(ccp(10, 0));
//            
//            if (bg1->getChildByTag(0x10100) != NULL) {
//                bg1->removeChildByTag(0x10100);
//            }
//            CCSprite* head1 = CCSprite::create("res/pic/haoyoupaihang/second.png");
//            head1->setPosition(ccp(head1->getContentSize().width + 4, bg1->getContentSize().height/2 + 5));
//            head1->setTag(0x10100);
//            bg1->addChild(head1);
//            
//            if (bg1->getChildByTag(0x10200) != NULL) {
//                bg1->removeChildByTag(0x10200);
//            }
//            CCSprite* name_bg1 = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
//            name_bg1->setPosition(ccp(bg1->getContentSize().width - name_bg1->getContentSize().width/2, 100));
//            name_bg1->setTag(0x10200);
//            bg1->addChild(name_bg1);
//            
//            CCLabelTTF* name1 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
//            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 10, name_bg1->getContentSize().height/2));
//            name1->setTag(0x10500);
//            name_bg1->addChild(name1);
//            
//            
//        }else if (selectedIndex == 2){
//            CCSpriteFrame* frame1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str1->getCString());
//            bg1->setDisplayFrame(frame1);
//            bg1->setPosition(ccp(10, 0));
//            
//            if (bg1->getChildByTag(0x10100) != NULL) {
//                bg1->removeChildByTag(0x10100);
//            }
//            CCSprite* head1 = CCSprite::create("res/pic/haoyoupaihang/thrid.png");
//            head1->setPosition(ccp(head1->getContentSize().width + 4, bg1->getContentSize().height/2 + 5));
//            head1->setTag(0x10100);
//            bg1->addChild(head1);
//            
//            if (bg1->getChildByTag(0x10200) != NULL) {
//                bg1->removeChildByTag(0x10200);
//            }
//            CCSprite* name_bg1 = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
//            name_bg1->setPosition(ccp(bg1->getContentSize().width - name_bg1->getContentSize().width/2, 100));
//            name_bg1->setTag(0x10200);
//            bg1->addChild(name_bg1);
//            
//            CCLabelTTF* name1 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
//            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 10, name_bg1->getContentSize().height/2));
//            name1->setTag(0x10500);
//            name_bg1->addChild(name1);
//            
//            
//        }else{
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
            
            CCLabelTTF* name1 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name1->setPosition(ccp(name_bg1->getContentSize().width/2 - 10, name_bg1->getContentSize().height/2));
            name1->setTag(0x10500);
            name_bg1->addChild(name1);
            
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
//        }
        
        // 记录需要变大节点
        selectedIndex = cell->getIdx() + 3;
        
        // 需要变大
        sprNode = (CCSprite*)cell->getChildByTag(selectedIndex);
        CCSprite* bg2 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCString* bg_str2 = CCString::createWithFormat("panel_selected.png");
//        if(selectedIndex == 0){
//            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str2->getCString());
//            bg2->setDisplayFrame(frame2);
//            bg2->setPosition(ccp(0, 0));
//            
//            if (bg2->getChildByTag(0x10100) != NULL) {
//                bg2->removeChildByTag(0x10100);
//            }
//            CCSprite* head2 = CCSprite::create("res/pic/haoyoupaihang/first_selected.png");
//            head2->setPosition(ccp(head2->getContentSize().width - 8, bg2->getContentSize().height/2 + 3));
//            head2->setTag(0x10100);
//            bg2->addChild(head2);
//            
//            
//            
//            if (bg2->getChildByTag(0x10200) != NULL) {
//                bg2->removeChildByTag(0x10200);
//            }
//            CCSprite* name_bg2 = CCSprite::create("res/pic/haoyoupaihang/namebar_selected.png");
//            name_bg2->setPosition(ccp(bg2->getContentSize().width - name_bg2->getContentSize().width/2, 105));
//            name_bg2->setTag(0x10200);
//            bg2->addChild(name_bg2);
//            
//            CCLabelTTF* name2 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
//            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
//            name2->setTag(0x10500);
//            name_bg2->addChild(name2);
//            
//            
//            
//        }else if (selectedIndex == 1){
//            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str2->getCString());
//            bg2->setDisplayFrame(frame2);
//            bg2->setPosition(ccp(0, 0));
//            
//            if (bg2->getChildByTag(0x10100) != NULL) {
//                bg2->removeChildByTag(0x10100);
//            }
//            CCSprite* head2 = CCSprite::create("res/pic/haoyoupaihang/second_selected.png");
//            head2->setPosition(ccp(head2->getContentSize().width - 8, bg2->getContentSize().height/2 + 3));
//            head2->setTag(0x10100);
//            bg2->addChild(head2);
//            
//            if (bg2->getChildByTag(0x10200) != NULL) {
//                bg2->removeChildByTag(0x10200);
//            }
//            CCSprite* name_bg2 = CCSprite::create("res/pic/haoyoupaihang/namebar_selected.png");
//            name_bg2->setPosition(ccp(bg2->getContentSize().width - name_bg2->getContentSize().width/2, 105));
//            name_bg2->setTag(0x10200);
//            bg2->addChild(name_bg2);
//            
//            CCLabelTTF* name2 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
//            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
//            name2->setTag(0x10500);
//            name_bg2->addChild(name2);
//            
//            
//        }else if (selectedIndex == 2){
//            CCSpriteFrame* frame2 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bg_str2->getCString());
//            bg2->setDisplayFrame(frame2);
//            bg2->setPosition(ccp(0, 0));
//            
//            if (bg2->getChildByTag(0x10100) != NULL) {
//                bg2->removeChildByTag(0x10100);
//            }
//            CCSprite* head2 = CCSprite::create("res/pic/haoyoupaihang/thrid_selected.png");
//            head2->setPosition(ccp(head2->getContentSize().width - 8, bg2->getContentSize().height/2 + 3));
//            head2->setTag(0x10100);
//            bg2->addChild(head2);
//            
//            if (bg2->getChildByTag(0x10200) != NULL) {
//                bg2->removeChildByTag(0x10200);
//            }
//            CCSprite* name_bg2 = CCSprite::create("res/pic/haoyoupaihang/namebar_selected.png");
//            name_bg2->setPosition(ccp(bg2->getContentSize().width - name_bg2->getContentSize().width/2, 105));
//            name_bg2->setTag(0x10200);
//            bg2->addChild(name_bg2);
//            
//            CCLabelTTF* name2 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
//            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
//            name2->setTag(0x10500);
//            name_bg2->addChild(name2);
//            
//            
//        }else{
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
            
            CCLabelTTF* name2 = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
            name2->setPosition(ccp(name_bg2->getContentSize().width/2, name_bg2->getContentSize().height/2));
            name2->setTag(0x10500);
            name_bg2->addChild(name2);
            
            CCLabelAtlas* rank_num2;
            CCString* str_num2 = CCString::createWithFormat("%d", selectedIndex + 1);
            if (selectedIndex < 9) {
                rank_num2 =CCLabelAtlas::create(str_num2->getCString(), "res/pic/haoyoupaihang/num_single_selected.png", 29, 39, '4');
            }else{
                rank_num2 = CCLabelAtlas::create(str_num2->getCString(), "res/pic/haoyoupaihang/num_double_selected.png", 16, 23, '0');
            }
            rank_num2->setAnchorPoint(ccp(0.5, 0.5));
            rank_num2->setPosition(ccp(head2->getContentSize().width/2, head2->getContentSize().height/2));
            rank_num2->setTag(0x20100);
            head2->addChild(rank_num2);
//        }
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
cocos2d::CCSize TotalRankTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(248, 138);
}

//生成cell
cocos2d::extension::CCTableViewCell* TotalRankTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int index){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    unsigned int idx = index + 3;
    
//    if(idx == 0){
//        if (selectedIndex == idx) {// 大的
//            bigSprite(idx, spr);
//        }else{// 小的
//            smallSprite(idx, spr);
//        }
//    }else if(idx == 1){
//        if (selectedIndex == idx) {// 大的
//            bigSprite(idx, spr);
//        }else{// 小的
//            smallSprite(idx, spr);
//        }
//        
//    }else if(idx == 2){
//        if (selectedIndex == idx) {// 大的
//            bigSprite(idx, spr);
//        }else{// 小的
//            smallSprite(idx, spr);
//        }
//        
//    }else{
//        if (selectedIndex == idx) {// 大的
//            bigSprite(idx, spr);
//        }else{// 小的
//            smallSprite(idx, spr);
//        }
//        
//        
//    }
    smallSprite(idx, spr);
    
    spr->setTag(idx);
    pCell->addChild(spr);
    
    
//    if(idx == 0){
//        if (selectedIndex == idx) {// 大的
//            sprNode = pCell->getChildByTag(selectedIndex);
//        }
//    }else if (idx == 1){
//        if (selectedIndex == idx) {// 大的
//            sprNode = pCell->getChildByTag(selectedIndex);
//        }
//    }else if (idx == 2){
//        if (selectedIndex == idx) {// 大的
//            sprNode = pCell->getChildByTag(selectedIndex);
//        }
//    }else{
//        if (selectedIndex == idx) {// 大的
//            sprNode = pCell->getChildByTag(selectedIndex);
//        }
//    }
    
    return pCell;
}

void TotalRankTableView::bigSprite(int index, CCSprite* spr){
    CCSprite* bg = CCSprite::createWithSpriteFrameName("panel_selected.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(CCPointZero);
    bg->setTag(0x10000);
    spr->addChild(bg);
    
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
    
    CCLabelTTF* name = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(name_bg->getContentSize().width/2, name_bg->getContentSize().height/2));
    name->setTag(0x10500);
    name_bg->addChild(name);
    
    CCLabelTTF* cloth_count = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count->setPosition(ccp(bg->getContentSize().width * .8, bg->getContentSize().height/2));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
}
void TotalRankTableView::smallSprite(int index, CCSprite* spr){
    CCSprite* bg = CCSprite::createWithSpriteFrameName("panel_normal.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(ccp(10, 0));
    bg->setTag(0x10000);
    spr->addChild(bg);
    
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
    
    CCLabelTTF* name = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(name_bg->getContentSize().width/2 - 10, name_bg->getContentSize().height/2));
    name->setTag(0x10500);
    name_bg->addChild(name);
    
    CCLabelTTF* cloth_count = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count->setPosition(ccp(bg->getContentSize().width * .8, bg->getContentSize().height/2));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);

    
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

unsigned int TotalRankTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}

void TotalRankTableView::onEnter(){
    CCLayer::onEnter();
}

void TotalRankTableView::onExit(){
    CCLayer::onExit();
}