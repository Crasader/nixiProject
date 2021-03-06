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
#include "NetManager.h"
#include "PromptLayer.h"

const float NAME_FONT_SIZE = 22;

TotalRankTableView::~TotalRankTableView(){}

bool TotalRankTableView::init(){
    if(!CCLayer::init()){
        return false;
    }
    
    
    _rankers = DATA->getRanking()->ranking();
    if (_rankers == NULL) {
        _rankers = CCArray::create();
    }
    _rankers->retain();
    if (_rankers->count() <= 3) {
        allNumber = 0;
    }else{
        allNumber =  _rankers->count() - 3;
    }
    
    selectedIndex = -1;
    
    pTableView = CCTableView::create(this, CCSizeMake(304, 4.5*110));
    pTableView->setDirection(kCCScrollViewDirectionVertical);
    pTableView->setAnchorPoint(CCPointZero);
    pTableView->setPosition(CCPointZero);
    pTableView->setDelegate(this);
    pTableView->setTag(0);
    
    this->addChild(pTableView);
    
//    CCSprite* di_spr = CCSprite::create("res/pic/haoyoupaihang/di_bar.png");
//    di_spr->setAnchorPoint(CCPoint(1, 0));
//    di_spr->setPosition(ccp(pTableView->getContentSize().width, -1));
//    this->addChild(di_spr);
//    if (allNumber == 0) {
//        di_spr->setVisible(false);
//    }
    
    pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    pTableView->reloadData();
    return true;
}

void TotalRankTableView::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&TotalRankTableView::tobeFriend_callback_803), "HTTP_FINISHED_803", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&TotalRankTableView::updateTabelView), "UpdateRank", NULL);
}

void TotalRankTableView::onExit(){
    CCLayer::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
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
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("Small");
    
    if (selectedIndex == -1) {
        selectedIndex = cell->getIdx() + 3;
        DATA->getSocial()->setSelectedRanker(selectedIndex);
        // 需要变大
        sprNode = (CCSprite*)cell->getChildByTag(selectedIndex);
        CCSprite* bg2 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCTexture2D* tet2;
//        if (selectedIndex == 0) {
//            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/first_bg_sel.png");
//        }else if (selectedIndex == 1) {
//            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/second_bg_sel.png");
//        }else if (selectedIndex == 2) {
//            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/third_bg_sel.png");
//        }else{
            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/other_bg_sel.png");
//        }
        
        bg2->setTexture(tet2);
//        bg2->setPosition(ccp(10, 0));
        
        
        
        ShowComp* show_2 = (ShowComp* )_rankers->objectAtIndex(selectedIndex);
        const char* nickname_2 = show_2->nickname();
        
        int collected2 = show_2->collected();
        
        if (bg2->getChildByTag(0x10500) != NULL) {
            bg2->removeChildByTag(0X10500, true);
        }
        
        CCLabelTTF* name2 = CCLabelTTF::create(nickname_2, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
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
        
        if (bg2->getChildByTag(0x10111) != NULL) {
            bg2->removeChildByTag(0x10111, true);
        }
        
        CCSprite* flag2 = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
        flag2->setPosition(ccp(bg2->getContentSize().width* .5f, bg2->getContentSize().height* .36));
        flag2->setTag(0x10111);
        bg2->addChild((flag2));
        
        int num = selectedIndex + 1;
        if (num >= 4 && num <= 9) {
            if (bg2->getChildByTag(0x10400) != NULL) {
                bg2->removeChildByTag(0x10400, true);
            }
            
            CCSprite* spr = this->getNumSprite(num);
            spr->setPosition(ccp(bg2->getContentSize().width* .16f, bg2->getContentSize().height* .5f));
            spr->setTag(0x10400);
            bg2->addChild(spr);
        }else{
            if (bg2->getChildByTag(0x10600) != NULL) {
                bg2->removeChildByTag(0x10600, true);
            }
            if (bg2->getChildByTag(0x10700) != NULL) {
                bg2->removeChildByTag(0x10700, true);
            }
            
            CCSprite* spr1 = this->getNumSprite((int)floor(num/10));
            spr1->setPosition(ccp(bg2->getContentSize().width* .16f - 8, bg2->getContentSize().height* .5f));
            spr1->setTag(0x10600);
            bg2->addChild(spr1);
            CCSprite* spr2 = this->getNumSprite((int)floor(num%10));
            spr2->setPosition(ccp(bg2->getContentSize().width* .16f + 8, bg2->getContentSize().height* .5f));
            spr2->setTag(0x10700);
            bg2->addChild(spr2);
        }
        
        CCMenu* menu_add2 = (CCMenu*)bg2->getChildByTag(0x10900);
        if (menu_add2 != NULL) {
            menu_add2->setVisible(true);
        }
        
        
        CCLayer* layer = CCLayer::create();
        layer->setTouchEnabled(true);
        layer->setTouchSwallowEnabled(true);
        layer->setTag(10000);
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ExitMan");
        
    }else if (selectedIndex != cell->getIdx() + 3) {
        // 需要变小
        CCSprite* bg1 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCTexture2D* tet;
//        if (selectedIndex == 0) {
//            tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/first_bg_nor.png");
//        }else if (selectedIndex == 1) {
//            tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/second_bg_nor.png");
//        }else if (selectedIndex == 2) {
//            tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/third_bg_nor.png");
//        }else{
            tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/other_bg_nor.png");
//        }
        
        bg1->setTexture(tet);
        
        
        
        ShowComp* show = (ShowComp* )_rankers->objectAtIndex(selectedIndex);
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
        
        if(bg1->getChildByTag(0x10111) != NULL) {
            bg1->removeChildByTag(0x10111, true);
        }
        
        CCSprite* flag1 = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
        flag1->setPosition(ccp(bg1->getContentSize().width* .6f, bg1->getContentSize().height* .38));
        flag1->setTag(0x10111);
        bg1->addChild((flag1));
        
            
        int num1 = selectedIndex + 1;
        if (num1 >= 4 && num1 <= 9) {
            if (bg1->getChildByTag(0x10400) != NULL) {
                bg1->removeChildByTag(0x10400, true);
            }
            
            CCSprite* spr = this->getNumSprite(num1);
            spr->setPosition(ccp(bg1->getContentSize().width* .325f, bg1->getContentSize().height* .5f));
            spr->setTag(0x10400);
            bg1->addChild(spr);
        }else{
            if (bg1->getChildByTag(0x10600) != NULL) {
                bg1->removeChildByTag(0x10600, true);
            }
            if (bg1->getChildByTag(0x10700) != NULL) {
                bg1->removeChildByTag(0x10700, true);
            }
            
            CCSprite* spr1 = this->getNumSprite((int)floor(num1/10));
            spr1->setPosition(ccp(bg1->getContentSize().width* .325f - 8, bg1->getContentSize().height* .5f));
            spr1->setTag(0x10600);
            bg1->addChild(spr1);
            CCSprite* spr2 = this->getNumSprite((int)floor(num1%10));
            spr2->setPosition(ccp(bg1->getContentSize().width* .325f + 8, bg1->getContentSize().height* .5f));
            spr2->setTag(0x10700);
            bg1->addChild(spr2);
        }
        
        CCMenu* menu_add = (CCMenu*)bg1->getChildByTag(0x10900);
        if (menu_add != NULL) {
            menu_add->setVisible(false);
        }
        
        
        // 记录需要变大节点
        selectedIndex = cell->getIdx() + 3;
        DATA->getSocial()->setSelectedRanker(selectedIndex);
        
        // 需要变大
        sprNode = (CCSprite*)cell->getChildByTag(selectedIndex);
        CCSprite* bg2 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCTexture2D* tet2;
//        if (selectedIndex == 0) {
//            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/first_bg_sel.png");
//        }else if (selectedIndex == 1) {
//            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/second_bg_sel.png");
//        }else if (selectedIndex == 2) {
//            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/third_bg_sel.png");
//        }else{
            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/other_bg_sel.png");
//        }
        
        bg2->setTexture(tet2);
//            bg2->setPosition(ccp(10, 0));
        

        
        ShowComp* show_2 = (ShowComp* )_rankers->objectAtIndex(selectedIndex);
        const char* nickname_2 = show_2->nickname();
        int collected2 = show_2->collected();
        
        if (bg2->getChildByTag(0x10500) != NULL) {
            bg2->removeChildByTag(0X10500, true);
        }
        
        CCLabelTTF* name2 = CCLabelTTF::create(nickname_2, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
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
        
        if (bg2->getChildByTag(0x10111) != NULL) {
            bg2->removeChildByTag(0x10111, true);
        }
        
        CCSprite* flag2 = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
        flag2->setPosition(ccp(bg2->getContentSize().width* .5f, bg2->getContentSize().height* .36));
        flag2->setTag(0x10111);
        bg2->addChild((flag2));
        
        int num2 = selectedIndex + 1;
        if (num2 >= 4 && num2 <= 9) {
            if (bg2->getChildByTag(0x10400) != NULL) {
                bg2->removeChildByTag(0x10400, true);
            }
            
            CCSprite* spr = this->getNumSprite(num2);
            spr->setPosition(ccp(bg2->getContentSize().width* .16f, bg2->getContentSize().height* .5f));
            spr->setTag(0x10400);
            bg2->addChild(spr);
        }else{
            if (bg2->getChildByTag(0x10600) != NULL) {
                bg2->removeChildByTag(0x10600, true);
            }
            if (bg2->getChildByTag(0x10700) != NULL) {
                bg2->removeChildByTag(0x10700, true);
            }
            
            CCSprite* spr1 = this->getNumSprite((int)floor(num2/10));
            spr1->setPosition(ccp(bg2->getContentSize().width* .16f - 8, bg2->getContentSize().height* .5f));
            spr1->setTag(0x10600);
            bg2->addChild(spr1);
            CCSprite* spr2 = this->getNumSprite((int)floor(num2%10));
            spr2->setPosition(ccp(bg2->getContentSize().width* .16f + 8, bg2->getContentSize().height* .5f));
            spr2->setTag(0x10700);
            bg2->addChild(spr2);
        }
        
        CCMenu* menu_add2 = (CCMenu*)bg2->getChildByTag(0x10900);
        if (menu_add2 != NULL) {
            menu_add2->setVisible(true);
        }
        
        CCLayer* layer = CCLayer::create();
        layer->setTouchEnabled(true);
        layer->setTouchSwallowEnabled(true);
        layer->setTag(10000);
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ExitMan");
    }
}

void TotalRankTableView::updateTabelView(){
    selectedIndex = -1;
    pTableView->reloadData();
}

//每个cell的size
cocos2d::CCSize TotalRankTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(304, 110);
}

//生成cell
cocos2d::extension::CCTableViewCell* TotalRankTableView::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int index){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    CCSprite* spr = CCSprite::create();
    unsigned int idx = index;
    
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
    
    if (selectedIndex == idx + 3) {
        bigSprite(idx, spr);
    }else{
        smallSprite(idx, spr);
    }
    
    spr->setTag(idx + 3);
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
    
    if (selectedIndex == idx + 3) {
        sprNode = pCell->getChildByTag(selectedIndex);
    }
    
    return pCell;
}

void TotalRankTableView::bigSprite(int index, CCSprite* spr){
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
    bg->setTag(0x10000);
    spr->addChild(bg);
    
    ShowComp* show = (ShowComp* )_rankers->objectAtIndex(index + 3);
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
    cloth_count->setPosition(ccp(bg->getContentSize().width * .75, bg->getContentSize().height* .34f));
    cloth_count->setAnchorPoint(CCPoint(0, 0.5));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
    int num = index + 4;
    if (num >= 4 && num <= 9) {
        CCSprite* spr = this->getNumSprite(num);
        spr->setPosition(ccp(bg->getContentSize().width* .16f, bg->getContentSize().height* .5f));
        spr->setTag(0x10400);
        bg->addChild(spr);
    }else{
        CCSprite* spr1 = this->getNumSprite((int)floor(num/10));
        spr1->setPosition(ccp(bg->getContentSize().width* .16f - 8, bg->getContentSize().height* .5f));
        spr1->setTag(0x10600);
        bg->addChild(spr1);
        CCSprite* spr2 = this->getNumSprite((int)floor(num%10));
        spr2->setPosition(ccp(bg->getContentSize().width* .16f + 8, bg->getContentSize().height* .5f));
        spr2->setTag(0x10700);
        bg->addChild(spr2);
    }
    
    CCSprite* add_spr1 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr2 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
    CCSprite* add_spr5 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCSprite* add_spr6 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    add_spr2->setScale(1.02f);
    CCMenuItemSprite* item_add;
//    if (show->isadd == 0) {
//        item_add = CCMenuItemSprite::create(add_spr1, add_spr2, this, menu_selector(TotalRankTableView::toBeFriend));
//        item_add->setTag(index);
//        item_add->setUserObject(CCInteger::create(index + 3));
//        CCMenu* menu_add = CCMenu::create(item_add, NULL);
//        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr1->getContentSize().width/2 -10, 20));
//        menu_add->setTag(0x10900);
//        bg->addChild(menu_add);
//    }else{
//        item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
//        item_add->setTag(index);
//        item_add->setUserObject(CCInteger::create(index + 3));
//        CCMenu* menu_add = CCMenu::create(item_add, NULL);
//        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 20));
//        menu_add->setTag(0x10900);
//        bg->addChild(menu_add);
//    }
    
    if (show->getShowID().compare(DATA->getLogin()->obtain_sid()) == 0) {
        //这是我自己
        
    }else {
        if (DATA->getSocial()->is_friend(show->getShowID().c_str())) {
            //是好友
            item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
            item_add->setTag(index);
            item_add->setUserObject(CCInteger::create(index + 3));
            CCMenu* menu_add = CCMenu::create(item_add, NULL);
            menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
            menu_add->setTag(0x10900);
            bg->addChild(menu_add);
        }else{
            bool hasRequested = DATA->getSocial()->is_requested(show->getShowID().c_str());
            //不是好友
            if (! hasRequested || show->isadd == 0) {
                item_add = CCMenuItemSprite::create(add_spr1, add_spr2, this, menu_selector(TotalRankTableView::toBeFriend));
                item_add->setTag(index);
                item_add->setUserObject(CCInteger::create(index + 3));
                CCMenu* menu_add = CCMenu::create(item_add, NULL);
                menu_add->setPosition(ccp(bg->getContentSize().width - add_spr1->getContentSize().width/2 -10, 8));
                menu_add->setTag(0x10900);
                bg->addChild(menu_add);
            }else{
                item_add = CCMenuItemSprite::create(add_spr5, add_spr6, this, NULL);
                item_add->setTag(index);
                item_add->setUserObject(CCInteger::create(index + 3));
                CCMenu* menu_add = CCMenu::create(item_add, NULL);
                menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
                menu_add->setTag(0x10900);
                bg->addChild(menu_add);
            }
        }
    }
    
}
void TotalRankTableView::smallSprite(int index, CCSprite* spr){
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
    
    ShowComp* show = (ShowComp*)_rankers->objectAtIndex(index + 3);
    const char* nickname = show->nickname();
    int collect = show->collected();
    
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(bg->getContentSize().width* .75f, bg->getContentSize().height* .68));
    name->setColor(ccc3(109, 98, 96));
    name->setTag(0x10500);
    bg->addChild(name);
    
    CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
    flag->setPosition(ccp(bg->getContentSize().width* .6f, bg->getContentSize().height* .38));
    flag->setTag(0x10111);
    bg->addChild((flag));
    
    CCString* collect_str = CCString::createWithFormat("%d", collect);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 20);
    cloth_count->setPosition(ccp(bg->getContentSize().width * .78f, bg->getContentSize().height* .36f));
    cloth_count->setAnchorPoint(CCPoint(0, 0.5));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);

    int num = index + 4;
    if (num >= 4 && num <= 9) {
        CCSprite* spr = this->getNumSprite(num);
        spr->setPosition(ccp(bg->getContentSize().width* .325f, bg->getContentSize().height* .5f));
        spr->setTag(0x10400);
        bg->addChild(spr);
    }else{
        CCSprite* spr1 = this->getNumSprite((int)floor(num/10));
        spr1->setPosition(ccp(bg->getContentSize().width* .325f - 8, bg->getContentSize().height* .5f));
        spr1->setTag(0x10600);
        bg->addChild(spr1);
        CCSprite* spr2 = this->getNumSprite((int)floor(num%10));
        spr2->setPosition(ccp(bg->getContentSize().width* .325f + 8, bg->getContentSize().height* .5f));
        spr2->setTag(0x10700);
        bg->addChild(spr2);
    }
    
    
    CCSprite* add_spr1 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr2 = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
    CCSprite* add_spr5 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCSprite* add_spr6 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    add_spr2->setScale(1.02f);
    CCMenuItemSprite* item_add;
//    if (show->isadd == 0) {
//        item_add = CCMenuItemSprite::create(add_spr1, add_spr2, this, menu_selector(TotalRankTableView::toBeFriend));
//        item_add->setTag(index);
//        item_add->setUserObject(CCInteger::create(index + 3));
//        CCMenu* menu_add = CCMenu::create(item_add, NULL);
//        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr1->getContentSize().width/2 -10, 20));
//        menu_add->setTag(0x10900);
//        menu_add->setVisible(false);
//        bg->addChild(menu_add);
//    }else{
//        item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
//        item_add->setTag(index);
//        item_add->setUserObject(CCInteger::create(index + 3));
//        CCMenu* menu_add = CCMenu::create(item_add, NULL);
//        menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 20));
//        menu_add->setTag(0x10900);
//        menu_add->setVisible(false);
//        bg->addChild(menu_add);
//    }
    
    if (show->getShowID().compare(DATA->getLogin()->obtain_sid()) == 0) {
        //这是我自己
        
    }else {
        if (DATA->getSocial()->is_friend(show->getShowID().c_str())) {
            //是好友
            item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
            item_add->setTag(index);
            item_add->setUserObject(CCInteger::create(index + 3));
            CCMenu* menu_add = CCMenu::create(item_add, NULL);
            menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
            menu_add->setTag(0x10900);
            menu_add->setVisible(false);
            bg->addChild(menu_add);
        }else{
            bool hasRequested = DATA->getSocial()->is_requested(show->getShowID().c_str());
            //不是好友
            if (! hasRequested || show->isadd == 0) {
                item_add = CCMenuItemSprite::create(add_spr1, add_spr2, this, menu_selector(TotalRankTableView::toBeFriend));
                item_add->setTag(index);
                item_add->setUserObject(CCInteger::create(index + 3));
                CCMenu* menu_add = CCMenu::create(item_add, NULL);
                menu_add->setPosition(ccp(bg->getContentSize().width - add_spr1->getContentSize().width/2 -10, 8));
                menu_add->setTag(0x10900);
                menu_add->setVisible(false);
                bg->addChild(menu_add);
            }else{
                item_add = CCMenuItemSprite::create(add_spr5, add_spr6, this, NULL);
                item_add->setTag(index);
                item_add->setUserObject(CCInteger::create(index + 3));
                CCMenu* menu_add = CCMenu::create(item_add, NULL);
                menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
                menu_add->setTag(0x10900);
                menu_add->setVisible(false);
                bg->addChild(menu_add);
            }
        }
    }


}

void TotalRankTableView::toBeFriend(CCMenuItem* btn){
    CCMenuItem* item = (CCMenuItem* )btn;
    int index = ((CCInteger*)item->getUserObject())->getValue();
    
    CCNode* bg = (CCNode* )item->getParent()->getParent();
    bg->removeChildByTag(0x10900, true);
    CCArray* rankers = DATA->getRanking()->ranking();
    ShowComp* other = (ShowComp*)rankers->objectAtIndex(index);
    other->isadd = 1;
    
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
    CCMenuItemSprite* item_add;
    item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
    CCMenu* menu_add = CCMenu::create(item_add, NULL);
    menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 8));
    menu_add->setTag(0X10900);
    bg->addChild(menu_add);
    
    NET->send_message_803(other->getShowID().c_str(), 1);
}

void TotalRankTableView::tobeFriend_callback_803(){
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "成功添加好友~!");
}

CCSprite* TotalRankTableView::getNumSprite(int num){
    CCSprite* spr;
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
            break;
    }
    
    return spr;
}

unsigned int TotalRankTableView::numberOfCellsInTableView(cocos2d::extension::CCTableView *table){
    return allNumber;
}
