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
    
    
//    _friends = DATA->getSocial()->friends()->allKeys();
    _friends = DATA->getSocial()->sortedFriends();
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
    
    pTableView = CCTableView::create(this, CCSizeMake(275, 6*130));
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
    CCNotificationCenter::sharedNotificationCenter()->postNotification("Small");
    
    if (selectedIndex == -1) {
        selectedIndex = cell->getIdx();
        DATA->getSocial()->setSelectedFriend(selectedIndex);
        
        // 需要变大
        sprNode = (CCSprite*)cell->getChildByTag(selectedIndex);
        CCSprite* bg2 = (CCSprite*)sprNode->getChildByTag(0x10000);
        CCTexture2D* tet2;
        if (selectedIndex == 0) {
            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/first_bg_sel.png");
        }else if (selectedIndex == 1){
            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/second_bg_sel.png");
        }else if (selectedIndex == 2){
            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/third_bg_sel.png");
        }else{
            tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/other_bg_sel.png");
        }
        
        bg2->setTexture(tet2);
        
        if (bg2->getChildByTag(0x10500) != NULL) {
            bg2->removeChildByTag(0x10500);
        }
        
//        CCString* show_id2 = (CCString*)_friends->objectAtIndex(selectedIndex);
//        ShowComp* show2 = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id2->getCString());
        ShowComp* show2 = (ShowComp*)_friends->objectAtIndex(selectedIndex);
        
        const char* nickname2 = show2->nickname();
        int collected2 = show2->collected();
        
        CCLabelTTF* name2 = CCLabelTTF::create(nickname2, DISPLAY->fangzhengFont(), 24, CCSizeMake(160, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        name2->setPosition(ccp(bg2->getContentSize().width* .60f, bg2->getContentSize().height* .68));
        name2->setColor(ccc3(234, 106, 106));
        name2->setTag(0x10500);
        bg2->addChild(name2);
        
        CCMenu* menu_send2 = (CCMenu*)bg2->getChildByTag(0x10400);
        menu_send2->setVisible(true);
        
        if (bg2->getChildByTag(0x10300) != NULL) {
            bg2->removeChildByTag(0x10300);
        }
        
        CCString* collected_str2 = CCString::createWithFormat("服装收集: %d", collected2);
        CCLabelTTF* cloth_count2 = CCLabelTTF::create(collected_str2->getCString(), DISPLAY->fangzhengFont(), 16);
        cloth_count2->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count2->setPosition(ccp(bg2->getContentSize().width * .40, bg2->getContentSize().height* .365f));
        cloth_count2->setTag(0x10300);
        bg2->addChild(cloth_count2);
        
        int num2 = selectedIndex + 1;
        if (num2 >= 1 && num2 <= 9) {
            if (bg2->getChildByTag(0x10800) != NULL) {
                bg2->removeChildByTag(0x10800);
            }
            
            CCSprite* spr = this->getNumSprite(num2);
            spr->setPosition(ccp(bg2->getContentSize().width* .15f, bg2->getContentSize().height* .5f));
            spr->setTag(0x10800);
            bg2->addChild(spr);
        }else{
            if (bg2->getChildByTag(0x10600) != NULL) {
                bg2->removeChildByTag(0x10600);
            }
            if (bg2->getChildByTag(0x10700) != NULL) {
                bg2->removeChildByTag(0x10700);
            }
            
            CCSprite* spr1 = this->getNumSprite((int)floor(num2/10));
            spr1->setPosition(ccp(bg2->getContentSize().width* .15f - 10, bg2->getContentSize().height* .5f));
            spr1->setTag(0x10600);
            bg2->addChild(spr1);
            CCSprite* spr2 = this->getNumSprite((int)floor(num2%10));
            spr2->setPosition(ccp(bg2->getContentSize().width* .15f + 10, bg2->getContentSize().height* .5f));
            spr2->setTag(0x10700);
            bg2->addChild(spr2);
        }
        
        CCLayer* layer = CCLayer::create();
        layer->setTouchEnabled(true);
        layer->setTouchSwallowEnabled(true);
        layer->setTag(10000);
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("ExitMan");

    }else{
        if (selectedIndex != cell->getIdx()) {
            // 需要变小
            CCSprite* bg1 = (CCSprite*)sprNode->getChildByTag(0x10000);
            CCTexture2D* tet;
            if (selectedIndex == 0) {
                tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/first_bg_nor.png");
            }else if (selectedIndex == 1){
                tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/second_bg_nor.png");
            }else if (selectedIndex == 2){
                tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/third_bg_nor.png");
            }else{
                tet = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/other_bg_nor.png");
            }
            
            bg1->setTexture(tet);
            
            if (bg1->getChildByTag(0x10500) != NULL) {
                bg1->removeChildByTag(0x10500);
            }
            
//            CCString* show_id = (CCString*)_friends->objectAtIndex(selectedIndex);
//            ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
            ShowComp* show = (ShowComp*)_friends->objectAtIndex(selectedIndex);
            const char* nickname = show->nickname();
            int collected1 = show->collected();
            
            CCLabelTTF* name1 = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 22, CCSizeMake(160, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
            name1->setPosition(ccp(bg1->getContentSize().width* .68f, bg1->getContentSize().height* .68));
            name1->setColor(ccc3(234, 106, 106));
            name1->setTag(0x10500);
            bg1->addChild(name1);
            
            if (bg1->getChildByTag(0x10300) != NULL) {
                bg1->removeChildByTag(0x10300);
            }
            
            CCString* collected_str1 = CCString::createWithFormat("服装收集: %d", collected1);
            CCLabelTTF* cloth_count1 = CCLabelTTF::create(collected_str1->getCString(), DISPLAY->fangzhengFont(), 16);
            cloth_count1->setAnchorPoint(CCPoint(0, 0.5));
            cloth_count1->setPosition(ccp(bg1->getContentSize().width * .47f, bg1->getContentSize().height* .38f));
            cloth_count1->setTag(0x10300);
            bg1->addChild(cloth_count1);
            
            int num1 = selectedIndex + 1;
            if (num1 >= 1 && num1 <= 9) {
                if (bg1->getChildByTag(0x10800) != NULL) {
                    bg1->removeChildByTag(0x10800);
                }
                
                CCSprite* spr = this->getNumSprite(num1);
                spr->setPosition(ccp(bg1->getContentSize().width* .25f, bg1->getContentSize().height* .5f));
                spr->setTag(0x10800);
                bg1->addChild(spr);
            }else{
                if (bg1->getChildByTag(0x10600) != NULL) {
                    bg1->removeChildByTag(0x10600);
                }
                if (bg1->getChildByTag(0x10700) != NULL) {
                    bg1->removeChildByTag(0x10700);
                }
                
                CCSprite* spr1 = this->getNumSprite((int)floor(num1/10));
                spr1->setPosition(ccp(bg1->getContentSize().width* .25f - 10, bg1->getContentSize().height* .5f));
                spr1->setTag(0x10600);
                bg1->addChild(spr1);
                CCSprite* spr2 = this->getNumSprite((int)floor(num1%10));
                spr2->setPosition(ccp(bg1->getContentSize().width* .25f + 10, bg1->getContentSize().height* .5f));
                spr2->setTag(0x10700);
                bg1->addChild(spr2);
            }
            
            CCMenu* menu_send = (CCMenu*)bg1->getChildByTag(0x10400);
            menu_send->setVisible(false);
            
            // 记录需要变大节点
            selectedIndex = cell->getIdx();
            DATA->getSocial()->setSelectedFriend(selectedIndex);
            
            // 需要变大
            sprNode = (CCSprite*)cell->getChildByTag(selectedIndex);
            CCSprite* bg2 = (CCSprite*)sprNode->getChildByTag(0x10000);
            CCTexture2D* tet2;
            if (selectedIndex == 0) {
                tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/first_bg_sel.png");
            }else if (selectedIndex == 1){
                tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/second_bg_sel.png");
            }else if (selectedIndex == 2){
                tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/third_bg_sel.png");
            }else{
                tet2 = CCTextureCache::sharedTextureCache()->addImage("res/pic/haoyoupaihang/other_bg_sel.png");
            }
            
            bg2->setTexture(tet2);
            
            if (bg2->getChildByTag(0x10500) != NULL) {
                bg2->removeChildByTag(0x10500);
            }
            
//            CCString* show_id2 = (CCString*)_friends->objectAtIndex(selectedIndex);
//            ShowComp* show2 = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id2->getCString());
            ShowComp* show2 = (ShowComp*)_friends->objectAtIndex(selectedIndex);
            
            const char* nickname2 = show2->nickname();
            int collected2 = show2->collected();
            
            CCLabelTTF* name2 = CCLabelTTF::create(nickname2, DISPLAY->fangzhengFont(), 24, CCSizeMake(160, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
            name2->setPosition(ccp(bg2->getContentSize().width* .60f, bg2->getContentSize().height* .68));
            name2->setColor(ccc3(234, 106, 106));
            name2->setTag(0x10500);
            bg2->addChild(name2);
            
            CCMenu* menu_send2 = (CCMenu*)bg2->getChildByTag(0x10400);
            menu_send2->setVisible(true);
            
            if (bg2->getChildByTag(0x10300) != NULL) {
                bg2->removeChildByTag(0x10300);
            }
            
            CCString* collected_str2 = CCString::createWithFormat("服装收集: %d", collected2);
            CCLabelTTF* cloth_count2 = CCLabelTTF::create(collected_str2->getCString(), DISPLAY->fangzhengFont(), 16);
            cloth_count2->setAnchorPoint(CCPoint(0, 0.5));
            cloth_count2->setPosition(ccp(bg2->getContentSize().width * .40, bg2->getContentSize().height* .365f));
            cloth_count2->setTag(0x10300);
            bg2->addChild(cloth_count2);
            
            int num2 = selectedIndex + 1;
            if (num2 >= 1 && num2 <= 9) {
                if (bg2->getChildByTag(0x10800) != NULL) {
                    bg2->removeChildByTag(0x10800);
                }
                
                CCSprite* spr = this->getNumSprite(num2);
                spr->setPosition(ccp(bg2->getContentSize().width* .15f, bg2->getContentSize().height* .5f));
                spr->setTag(0x10800);
                bg2->addChild(spr);
            }else{
                if (bg2->getChildByTag(0x10600) != NULL) {
                    bg2->removeChildByTag(0x10600);
                }
                if (bg2->getChildByTag(0x10700) != NULL) {
                    bg2->removeChildByTag(0x10700);
                }
                
                CCSprite* spr1 = this->getNumSprite((int)floor(num2/10));
                spr1->setPosition(ccp(bg2->getContentSize().width* .15f - 10, bg2->getContentSize().height* .5f));
                spr1->setTag(0x10600);
                bg2->addChild(spr1);
                CCSprite* spr2 = this->getNumSprite((int)floor(num2%10));
                spr2->setPosition(ccp(bg2->getContentSize().width* .15f + 10, bg2->getContentSize().height* .5f));
                spr2->setTag(0x10700);
                bg2->addChild(spr2);
            }
            
            CCLayer* layer = CCLayer::create();
            layer->setTouchEnabled(true);
            layer->setTouchSwallowEnabled(true);
            layer->setTag(10000);
            CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000);
            CCNotificationCenter::sharedNotificationCenter()->postNotification("ExitMan");
        }
        
    }
    
}

void HaoyouRankTableView::updateTabelView(){
    selectedIndex = -1;
    pTableView->reloadData();
}

//每个cell的size
cocos2d::CCSize HaoyouRankTableView::cellSizeForTable(cocos2d::extension::CCTableView *table){
    return CCSizeMake(275, 124);
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
    CCSprite* bg;
    if (index == 0) {
        bg = CCSprite::create("res/pic/haoyoupaihang/first_bg_sel.png");
    }else if(index == 1){
        bg = CCSprite::create("res/pic/haoyoupaihang/second_bg_sel.png");
    }else if (index == 2){
        bg = CCSprite::create("res/pic/haoyoupaihang/third_bg_sel.png");
    }else{
        bg = CCSprite::create("res/pic/haoyoupaihang/other_bg_sel.png");
    }
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(CCPointZero);
    bg->setTag(0x10000);
    spr->addChild(bg);
    
//    CCString* show_id = (CCString*)_friends->objectAtIndex(index);
//    ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
    ShowComp* show = (ShowComp*)_friends->objectAtIndex(index);
    const char* nickname = show->nickname();
    int collected = show->collected();
    
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 24, CCSizeMake(160, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(bg->getContentSize().width* .6f, bg->getContentSize().height* .68));
    name->setColor(ccc3(234, 106, 106));
    name->setTag(0x10500);
    bg->addChild(name);
    
    CCString* collect_str = CCString::createWithFormat("服装收集: %d", collected);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 16);
    cloth_count->setAnchorPoint(CCPoint(0, 0.5));
    cloth_count->setPosition(ccp(bg->getContentSize().width * .40, bg->getContentSize().height* .365f));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
    int num = index + 1;
    if (num >= 1 && num <= 9) {
        CCSprite* spr = this->getNumSprite(num);
        spr->setPosition(ccp(bg->getContentSize().width* .15f, bg->getContentSize().height* .5f));
        spr->setTag(0x10800);
        bg->addChild(spr);
    }else{
        CCSprite* spr1 = this->getNumSprite((int)floor(num/10));
        spr1->setPosition(ccp(bg->getContentSize().width* .15f - 10, bg->getContentSize().height* .5f));
        spr1->setTag(0x10600);
        bg->addChild(spr1);
        CCSprite* spr2 = this->getNumSprite((int)floor(num%10));
        spr2->setPosition(ccp(bg->getContentSize().width* .15f + 10, bg->getContentSize().height* .5f));
        spr2->setTag(0x10700);
        bg->addChild(spr2);
    }
    
    CCSprite* tili_spr1 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
    CCSprite* tili_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
    CCSprite* tili_spr3 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    CCSprite* tili_spr4 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    tili_spr2->setScale(1.02f);
    CCMenuItemSprite* item_send;
    if (show->issend == 0) {
        item_send = CCMenuItemSprite::create(tili_spr1, tili_spr2, this, menu_selector(HaoyouRankTableView::sendTili));
        item_send->setTag(index);
//        item_send->setUserObject(CCString::create(show_id->getCString()));
        item_send->setUserObject(CCString::create(show->getShowID()));
        CCMenu* menu_send = CCMenu::create(item_send, NULL);
        menu_send->setPosition(ccp(bg->getContentSize().width - tili_spr1->getContentSize().width/2 -10, 20));
        menu_send->setTag(0x10400);
        bg->addChild(menu_send);
    }else{
        item_send = CCMenuItemSprite::create(tili_spr3, tili_spr4, this, NULL);
        item_send->setTag(index);
//        item_send->setUserObject(show_id);
        item_send->setUserObject(CCString::create(show->getShowID()));
        CCMenu* menu_send = CCMenu::create(item_send, NULL);
        menu_send->setPosition(ccp(bg->getContentSize().width - tili_spr3->getContentSize().width/2 -10, 20));
        menu_send->setTag(0x10400);
        bg->addChild(menu_send);
    }
}
void HaoyouRankTableView::smallSprite(int index, CCSprite* spr){
    CCSprite* bg;
    if (index == 0) {
        bg = CCSprite::create("res/pic/haoyoupaihang/first_bg_nor.png");
    }else if(index == 1){
        bg = CCSprite::create("res/pic/haoyoupaihang/second_bg_nor.png");
    }else if (index == 2){
        bg = CCSprite::create("res/pic/haoyoupaihang/third_bg_nor.png");
    }else{
        bg = CCSprite::create("res/pic/haoyoupaihang/other_bg_nor.png");
    }
    bg->setAnchorPoint(CCPointZero);
    bg->setTag(0x10000);
    spr->addChild(bg);
    
//    CCString* show_id = (CCString*)_friends->objectAtIndex(index);
//    ShowComp* show = (ShowComp*)DATA->getSocial()->friends()->objectForKey(show_id->getCString());
    ShowComp* show = (ShowComp*)_friends->objectAtIndex(index);
    const char* nickname = show->nickname();
    int collected = show->collected();
    
    
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), NAME_FONT_SIZE, CCSizeMake(160, 30),  kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(bg->getContentSize().width* .68f, bg->getContentSize().height* .68));
    name->setColor(ccc3(234, 106, 106));
    name->setTag(0x10500);
    bg->addChild(name);
    
    CCString* collect_str = CCString::createWithFormat("服装收集: %d", collected);
    CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 16);
    cloth_count->setAnchorPoint(CCPoint(0, 0.5f));
    cloth_count->setPosition(ccp(bg->getContentSize().width * .47f, bg->getContentSize().height* .38f));
    cloth_count->setTag(0x10300);
    bg->addChild(cloth_count);
    
    int num = index + 1;
    if (num >= 1 && num <= 9) {
        CCSprite* spr = this->getNumSprite(num);
        spr->setPosition(ccp(bg->getContentSize().width* .25f, bg->getContentSize().height* .5f));
        spr->setTag(0x10800);
        bg->addChild(spr);
    }else{
        CCSprite* spr1 = this->getNumSprite((int)floor(num/10));
        spr1->setPosition(ccp(bg->getContentSize().width* .25f - 10, bg->getContentSize().height* .5f));
        spr1->setTag(0x10600);
        bg->addChild(spr1);
        CCSprite* spr2 = this->getNumSprite((int)floor(num%10));
        spr2->setPosition(ccp(bg->getContentSize().width* .25f + 10, bg->getContentSize().height* .5f));
        spr2->setTag(0x10700);
        bg->addChild(spr2);
    }
    
    
    CCSprite* tili_spr1 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
    CCSprite* tili_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_send_tili.png");
    CCSprite* tili_spr3 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    CCSprite* tili_spr4 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    tili_spr2->setScale(1.02f);
    CCMenuItemSprite* item_send;
    if (show->issend == 0) {
        item_send = CCMenuItemSprite::create(tili_spr1, tili_spr2, this, menu_selector(HaoyouRankTableView::sendTili));
        item_send->setTag(index);
//        item_send->setUserObject(ccs(show_id->getCString()));
        item_send->setUserObject(ccs(show->getShowID()));
        CCMenu* menu_send = CCMenu::create(item_send, NULL);
        menu_send->setPosition(ccp(bg->getContentSize().width - tili_spr1->getContentSize().width/2 -10, 20));
        menu_send->setTag(0x10400);
        menu_send->setVisible(false);
        bg->addChild(menu_send);
    }else{
        item_send = CCMenuItemSprite::create(tili_spr3, tili_spr4, this, NULL);
        item_send->setTag(index);
//        item_send->setUserObject(ccs(show_id->getCString()));
        item_send->setUserObject(ccs(show->getShowID()));
        CCMenu* menu_send = CCMenu::create(item_send, NULL);
        menu_send->setPosition(ccp(bg->getContentSize().width - tili_spr3->getContentSize().width/2 -10, 20));
        menu_send->setTag(0x10400);
        menu_send->setVisible(false);
        bg->addChild(menu_send);
    }
    
}

CCSprite* HaoyouRankTableView::getNumSprite(int num){
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

void HaoyouRankTableView::sendTili(CCMenuItem* btn){    
    CCString* other_id = (CCString*)btn->getUserObject();
    CCNode* bg = btn->getParent()->getParent();
    bg->removeChildByTag(0x10400);
    CCDictionary* friends = DATA->getSocial()->friends();
    ShowComp* other = (ShowComp*)friends->objectForKey(other_id->getCString());
    other->issend = 1;
    
    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/send_finish.png");
    CCMenuItemSprite* item_add;
    item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
    CCMenu* menu_add = CCMenu::create(item_add, NULL);
    menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 20));
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
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&HaoyouRankTableView::updateTabelView), "UpdateRank", NULL);
}

void HaoyouRankTableView::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCLayer::onExit();
}