//
//  TotalRankScene.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#include "TotalRankScene.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "HaoyouScene.h"
#include "MainScene.h"


TotalRankScene:: ~TotalRankScene(){}

bool TotalRankScene::init(){
    if(!BaseScene::init()){
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/haoyoupaihang/panel.plist");
    
    enterBool = false;
    
    // 这里读取排行当前第一个人的衣服字典
    myClothesTemp = DATA->getClothes()->MyClothesTemp();
//    const char* curSelected_id = DATA->getSocial()->getSelectedStrangerIDbyIndex(DATA->getSocial()->getSelectedStranger());
//    ShowComp* show = (ShowComp*)DATA->getSocial()->strangers()->objectForKey(curSelected_id);
//    myClothesTemp = show->ondress();
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    this->createView();
    this->creat_Man();
    this->initClothes();
    
    return true;
}

TotalRankScene* TotalRankScene::create_with_type(int type){
    TotalRankScene* scene = TotalRankScene::create();
    scene->init_with_type(type);
    return scene;
}

void TotalRankScene::init_with_type(int type){
    _type = type;
}

void TotalRankScene::onEnter(){
    BaseScene::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    nc->addObserver(this, SEL_CallFuncO(&TotalRankScene::exitMan), "ExitMan", NULL);
    
}

void TotalRankScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    BaseScene::onExit();
}

void TotalRankScene::createView(){
    float totalRank_z_oder = 20.f;// 人在按钮下
//    float totalRank_z_oder = 9.f;// 人在按钮上
    
    CCSprite* background = CCSprite::create("res/pic/haoyoupaihang/bg.png");
    background->setPosition(ccp(DISPLAY->ScreenWidth()*.5, DISPLAY->ScreenHeight()*.5));
    this->addChild(background);
    
    //分享
    CCSprite* share_spr = CCSprite::create("res/pic/haoyoupaihang/share.png");
    CCSprite* share_spr2 = CCSprite::create("res/pic/haoyoupaihang/share.png");
    share_spr2->setScale(1.02f);
    CCMenuItemSprite* item_share = CCMenuItemSprite::create(share_spr, share_spr2, this, menu_selector(TotalRankScene::btn_share_callback));
    item_share->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .88f));
    CCMenu* menu_share = CCMenu::create(item_share, NULL);
    menu_share->setPosition(CCPointZero);
    this->addChild(menu_share, totalRank_z_oder);
    
    //纸条
    CCSprite* note_spr = CCSprite::create("res/pic/haoyoupaihang/btn_zhitiao.png");
    CCSprite* note_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_zhitiao.png");
    note_spr2->setScale(1.02f);
    CCMenuItemSprite* item_note = CCMenuItemSprite::create(note_spr, note_spr2, this, menu_selector(TotalRankScene::btn_note_callback));
    item_note->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .2f));
    CCMenu* menu_note = CCMenu::create(item_note, NULL);
    menu_note->setPosition(CCPointZero);
    this->addChild(menu_note, totalRank_z_oder);
    
    //返回
    CCSprite* back_spr = CCSprite::create("res/pic/taskScene/task_back.png");
    CCSprite* back_spr2 = CCSprite::create("res/pic/taskScene/task_back.png");
    back_spr2->setScale(1.02f);
    CCMenuItemSprite* item_back = CCMenuItemSprite::create(back_spr, back_spr2, this, menu_selector(TotalRankScene::btn_back_callback));
    item_back->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menu_back = CCMenu::create(item_back, NULL);
    menu_back->setPosition(CCPointZero);
    this->addChild(menu_back, totalRank_z_oder);
    
    CCSprite* self_spr = CCSprite::create("res/pic/haoyoupaihang/panel_self.png");
    self_spr->setPosition(ccp(DISPLAY->ScreenWidth() - self_spr->getContentSize().width/2 + 10, DISPLAY->ScreenHeight()* .08f));
    this->addChild(self_spr, totalRank_z_oder);
    
    int my_rank = 8;
    CCSprite* head;
    if (my_rank == 1) {
        head = CCSprite::create("res/pic/haoyoupaihang/first.png");
    }else if (my_rank == 2){
        head = CCSprite::create("res/pic/haoyoupaihang/second.png");
    }else if (my_rank == 3){
        head = CCSprite::create("res/pic/haoyoupaihang/thrid.png");
    }else{
        head = CCSprite::create("res/pic/haoyoupaihang/cell_head_normal.png");
        CCLabelAtlas* rank_num;
        CCString* str_num = CCString::createWithFormat("%d", my_rank);
        if (my_rank < 10) {
            rank_num =CCLabelAtlas::create(str_num->getCString(), "res/pic/haoyoupaihang/num_single_normal.png", 25, 34, '4');
        }else{
            rank_num = CCLabelAtlas::create(str_num->getCString(), "res/pic/haoyoupaihang/num_double_normal.png", 14, 21, '0');
        }
        rank_num->setAnchorPoint(ccp(0.5, 0.5));
        rank_num->setPosition(ccp(head->getContentSize().width/2, head->getContentSize().height/2));
        head->addChild(rank_num);
    }
    head->setPosition(ccp(head->getContentSize().width - 13, self_spr->getContentSize().height/2 + 3));
    self_spr->addChild(head);
    
    CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
    name_bg->setPosition(ccp(self_spr->getContentSize().width - name_bg->getContentSize().width/2, 90));
    self_spr->addChild(name_bg);
    
    const char* nickname = DATA->getShow()->nickname();
    CCLabelTTF* name = CCLabelTTF::create(nickname, DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(name_bg->getContentSize().width/2 - 10, name_bg->getContentSize().height/2));
    name_bg->addChild(name);
    
    CCString* collect_str = CCString::createWithFormat("%d", DATA->getShow()->collected());
    CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count->setPosition(ccp(self_spr->getContentSize().width * .8, self_spr->getContentSize().height/2));
    self_spr->addChild(cloth_count);
    
    
    //first
    
    _curBtn_index = 0;
    
    CCSprite* bg_first = CCSprite::createWithSpriteFrameName("panel_normal.png");
    CCSprite* bg_first2 = CCSprite::createWithSpriteFrameName("panel_normal.png");
    item_first = CCMenuItemSprite::create(bg_first, bg_first2, this, menu_selector(TotalRankScene::btn_toBig_callback));
    item_first->setPosition(ccp(DISPLAY->ScreenWidth() - bg_first->getContentSize().width/2, DISPLAY->ScreenHeight()* .18f + 138*5.5));
    item_first->setUserObject(CCInteger::create(0));
    item_first->setEnabled(false);
    
    CCSprite* bg = CCSprite::createWithSpriteFrameName("panel_selected.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(CCPointZero);
    bg->setTag(0x1000);
    item_first->addChild(bg);
    
    CCSprite* head_first = CCSprite::create("res/pic/haoyoupaihang/first_selected.png");
    head_first->setPosition(ccp(head_first->getContentSize().width - 8, bg_first->getContentSize().height/2 + 3));
    bg->addChild(head_first);
    
    CCSprite* name_bg_first = CCSprite::create("res/pic/haoyoupaihang/namebar_selected.png");
    name_bg_first->setPosition(ccp(bg_first->getContentSize().width - name_bg_first->getContentSize().width/2, 105));
    bg->addChild(name_bg_first);
    
    CCLabelTTF* name_first = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name_first->setPosition(ccp(name_bg_first->getContentSize().width/2, name_bg_first->getContentSize().height/2));
    name_bg_first->addChild(name_first);
    
    CCLabelTTF* cloth_count_first = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count_first->setPosition(ccp(bg_first->getContentSize().width * .8, bg_first->getContentSize().height/2));
    bg->addChild(cloth_count_first);
    
    
    //second
    
    CCSprite* bg_second = CCSprite::createWithSpriteFrameName("panel_normal.png");
    CCSprite* bg_second2 = CCSprite::createWithSpriteFrameName("panel_normal.png");
    item_second = CCMenuItemSprite::create(bg_second, bg_second2, this, menu_selector(TotalRankScene::btn_toBig_callback));
    item_second->setPosition(ccp(DISPLAY->ScreenWidth() - bg_second->getContentSize().width/2 + 10, DISPLAY->ScreenHeight()* .18f + 138*4.5));
    item_second->setUserObject(CCInteger::create(1));
    
    CCSprite* head_second = CCSprite::create("res/pic/haoyoupaihang/second.png");
    head_second->setPosition(ccp(head_second->getContentSize().width + 4, bg_second->getContentSize().height/2 + 5));
    head_second->setTag(0x2001);
    item_second->addChild(head_second);
    
    CCSprite* name_bg_second = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
    name_bg_second->setPosition(ccp(bg_second->getContentSize().width - name_bg_second->getContentSize().width/2, 105));
    name_bg_second->setTag(0x2002);
    item_second->addChild(name_bg_second);
    
    CCLabelTTF* name_second = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name_second->setPosition(ccp(name_bg_second->getContentSize().width/2 - 10, name_bg_second->getContentSize().height/2));
    name_bg_second->addChild(name_second);
    
    CCLabelTTF* cloth_count_second = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count_second->setPosition(ccp(bg_second->getContentSize().width * .8, bg_second->getContentSize().height/2));
    cloth_count_second->setTag(0x2003);
    item_second->addChild(cloth_count_second);
    
    
    //third
    
    CCSprite* bg_third = CCSprite::createWithSpriteFrameName("panel_normal.png");
    CCSprite* bg_third2 = CCSprite::createWithSpriteFrameName("panel_normal.png");
    item_third = CCMenuItemSprite::create(bg_third, bg_third2, this, menu_selector(TotalRankScene::btn_toBig_callback));
    item_third->setPosition(ccp(DISPLAY->ScreenWidth() - bg_third->getContentSize().width/2 + 10, DISPLAY->ScreenHeight()* .18f + 138*3.5));
    item_third->setUserObject(CCInteger::create(2));
    
    CCSprite* head_third = CCSprite::create("res/pic/haoyoupaihang/thrid.png");
    head_third->setPosition(ccp(head_third->getContentSize().width + 4, bg_third->getContentSize().height/2 + 5));
    head_third->setTag(0x2001);
    item_third->addChild(head_third);
    
    CCSprite* name_bg_third = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
    name_bg_third->setPosition(ccp(bg_third->getContentSize().width - name_bg_third->getContentSize().width/2, 105));
    name_bg_third->setTag(0x2002);
    item_third->addChild(name_bg_third);
    
    CCLabelTTF* name_third = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name_third->setPosition(ccp(name_bg_third->getContentSize().width/2 - 10, name_bg_third->getContentSize().height/2));
    name_bg_third->addChild(name_third);
    
    CCLabelTTF* cloth_count_third = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    cloth_count_third->setPosition(ccp(bg_third->getContentSize().width * .8, bg_third->getContentSize().height/2));
    cloth_count_third->setTag(0x2003);
    item_third->addChild(cloth_count_third);
    
    CCMenu* menu_rank = CCMenu::create(item_first, item_second, item_third, NULL);
    menu_rank->setPosition(CCPointZero);
    this->addChild(menu_rank);
    
    this->initTotalRank();
}

void TotalRankScene::initTotalRank(){
    CCSprite* spr = CCSprite::create("res/pic/haoyoupaihang/panel_normal.png");
    
    tabLayer = TotalRankTableView::create();
    
    tabLayer->setPosition(ccp(DISPLAY->ScreenWidth() - spr->getContentSize().width, DISPLAY->ScreenHeight()* .18f));
    tabLayer->setTag(0x77777);
    this->addChild(tabLayer, 20);
}

void TotalRankScene::btn_share_callback(CCObject* pSender){
    
}

void TotalRankScene::btn_note_callback(CCObject* pSender){
    _panel = NotePanel::create();
    this->addChild(_panel, 10000);
}

void TotalRankScene::btn_back_callback(CCObject* pSender){
    if (_type == 1) {
        CCScene* scene = MainScene::scene();
        CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else if (_type == 2) {
        CCScene* scene = HaoyouScene::scene();
        CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void TotalRankScene::btn_toBig_callback(CCMenuItem* btn){
    CCMenuItem* item = (CCMenuItem*)btn;
    item->setEnabled(false);
    int index = ((CCInteger*)item->getUserObject())->getValue();
    
    if (item->getChildByTag(0x2001)) {
        item->removeChildByTag(0x2001, true);
    }
    if (item->getChildByTag(0x2002)) {
        item->removeChildByTag(0x2002, true);
    }
    if (item->getChildByTag(0x2003)) {
        item->removeChildByTag(0x2003, true);
    }
    
    item->setPosition(item->getPositionX() - 10, item->getPositionY());
    
    CCSprite* bg = CCSprite::createWithSpriteFrameName("panel_selected.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(CCPointZero);
    bg->setTag(0x1000);
    item->addChild(bg);
    
    CCSprite* head;
    if (index == 0) {
        head = CCSprite::create("res/pic/haoyoupaihang/first_selected.png");
    }else if (index == 1){
        head = CCSprite::create("res/pic/haoyoupaihang/second_selected.png");
    }else if (index == 2){
        head = CCSprite::create("res/pic/haoyoupaihang/thrid_selected.png");
    }
    head->setPosition(ccp(head->getContentSize().width - 8, item->getContentSize().height/2 + 3));
    head->setTag(0x2001);
    item->addChild(head);
    
    CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_selected.png");
    name_bg->setPosition(ccp(item->getContentSize().width - name_bg->getContentSize().width/2, 105));
    name_bg->setTag(0x2002);
    item->addChild(name_bg);
    
    CCLabelTTF* name = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(name_bg->getContentSize().width/2, name_bg->getContentSize().height/2));
    name_bg->addChild(name);
    
    CCLabelTTF* count = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
    count->setPosition(ccp(item->getContentSize().width * .8f, item->getContentSize().height * .5f));
    count->setTag(0x2003);
    item->addChild(count);
    
    this->btn_toSmall_callback(index);
}

void TotalRankScene::btn_toSmall_callback(int index){
    if (_curBtn_index == 0) {
        if (item_first->getChildByTag(0x1000)) {
            item_first->removeChildByTag(0x1000, true);
        }
        if (item_first->getChildByTag(0x2001)) {
            item_first->removeChildByTag(0x2001, true);
        }
        if (item_first->getChildByTag(0x2002)) {
            item_first->removeChildByTag(0x2002, true);
        }
        if (item_first->getChildByTag(0x2003)) {
            item_first->removeChildByTag(0x2003, true);
        }
        
        item_first->setPosition(item_first->getPositionX() + 10, item_first->getPositionY());
        
        CCSprite* head = CCSprite::create("res/pic/haoyoupaihang/first.png");
        head->setPosition(ccp(head->getContentSize().width + 4, item_first->getContentSize().height/2 + 5));
        head->setTag(0x2001);
        item_first->addChild(head);
        
        CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
        name_bg->setPosition(ccp(item_first->getContentSize().width - name_bg->getContentSize().width/2, 105));
        name_bg->setTag(0x2002);
        item_first->addChild(name_bg);
        
        CCLabelTTF* name_third = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
        name_third->setPosition(ccp(name_bg->getContentSize().width/2 - 10, name_bg->getContentSize().height/2));
        name_bg->addChild(name_third);
        
        CCLabelTTF* cloth_count = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
        cloth_count->setPosition(ccp(item_first->getContentSize().width * .8, item_first->getContentSize().height/2));
        cloth_count->setTag(0x2003);
        item_first->addChild(cloth_count);
        
        item_first->setEnabled(true);
    }else if (_curBtn_index == 1){
        if (item_second->getChildByTag(0x1000)) {
            item_second->removeChildByTag(0x1000, true);
        }
        if (item_second->getChildByTag(0x2001)) {
            item_second->removeChildByTag(0x2001, true);
        }
        if (item_second->getChildByTag(0x2002)) {
            item_second->removeChildByTag(0x2002, true);
        }
        if (item_second->getChildByTag(0x2003)) {
            item_second->removeChildByTag(0x2003, true);
        }
        
        item_second->setPosition(item_second->getPositionX() + 10, item_second->getPositionY());
        
        CCSprite* head = CCSprite::create("res/pic/haoyoupaihang/second.png");
        head->setPosition(ccp(head->getContentSize().width + 4, item_second->getContentSize().height/2 + 5));
        head->setTag(0x2001);
        item_second->addChild(head);
        
        CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
        name_bg->setPosition(ccp(item_second->getContentSize().width - name_bg->getContentSize().width/2, 105));
        name_bg->setTag(0x2002);
        item_second->addChild(name_bg);
        
        CCLabelTTF* name_third = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
        name_third->setPosition(ccp(name_bg->getContentSize().width/2 - 10, name_bg->getContentSize().height/2));
        name_bg->addChild(name_third);
        
        CCLabelTTF* cloth_count = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
        cloth_count->setPosition(ccp(item_second->getContentSize().width * .8, item_second->getContentSize().height/2));
        cloth_count->setTag(0x2003);
        item_second->addChild(cloth_count);
        
        item_second->setEnabled(true);
    }else if (_curBtn_index == 2){
        if (item_third->getChildByTag(0x1000)) {
            item_third->removeChildByTag(0x1000, true);
        }
        if (item_third->getChildByTag(0x2001)) {
            item_third->removeChildByTag(0x2001, true);
        }
        if (item_third->getChildByTag(0x2002)) {
            item_third->removeChildByTag(0x2002, true);
        }
        if (item_third->getChildByTag(0x2003)) {
            item_third->removeChildByTag(0x2003, true);
        }
        
        item_third->setPosition(item_third->getPositionX() + 10, item_third->getPositionY());
        
        CCSprite* head = CCSprite::create("res/pic/haoyoupaihang/thrid.png");
        head->setPosition(ccp(head->getContentSize().width + 4, item_third->getContentSize().height/2 + 5));
        head->setTag(0x2001);
        item_third->addChild(head);
        
        CCSprite* name_bg = CCSprite::create("res/pic/haoyoupaihang/namebar_normal.png");
        name_bg->setPosition(ccp(item_third->getContentSize().width - name_bg->getContentSize().width/2, 105));
        name_bg->setTag(0x2002);
        item_third->addChild(name_bg);
        
        CCLabelTTF* name_third = CCLabelTTF::create("游客8A79B648", DISPLAY->fangzhengFont(), 20, CCSizeMake(160, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
        name_third->setPosition(ccp(name_bg->getContentSize().width/2 - 10, name_bg->getContentSize().height/2));
        name_bg->addChild(name_third);
        
        CCLabelTTF* cloth_count = CCLabelTTF::create("1000", DISPLAY->fangzhengFont(), 18, CCSizeMake(150, 20), kCCTextAlignmentCenter);
        cloth_count->setPosition(ccp(item_third->getContentSize().width * .8, item_third->getContentSize().height/2));
        cloth_count->setTag(0x2003);
        item_third->addChild(cloth_count);
        
        item_third->setEnabled(true);
    }
    
    _curBtn_index = index;
}

void TotalRankScene::enterMan(){
//    myClothesTemp =
    
    this->creat_Man();
    this->initClothes();
    
    CCMoveTo* moveTo = CCMoveTo::create(.3f, ccp(_ManSpr->getPosition().x + 500, _ManSpr->getPosition().y));
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TotalRankScene::removeMask));
    CCSequence* seq = CCSequence::create(moveTo, callFunc, NULL);
    _ManSpr->runAction(seq);
}
void TotalRankScene::exitMan(){
    CCMoveTo* moveTo = CCMoveTo::create(.3f, ccp(_ManSpr->getPosition().x - 500, _ManSpr->getPosition().y));
    CCCallFunc* callFunc1 = CCCallFunc::create(this, SEL_CallFunc(&TotalRankScene::removeMan));
    CCCallFunc* callFunc2 = CCCallFunc::create(this, SEL_CallFunc(&TotalRankScene::enterMan));
    CCSequence* seq = CCSequence::create(moveTo, CCDelayTime::create(.1f), callFunc1, CCDelayTime::create(.1f), callFunc2, NULL);
    _ManSpr->runAction(seq);
}
void TotalRankScene::removeMan(){
    _ManSpr->removeAllChildren();
    
//    myClothesTemp = 
}

void TotalRankScene::removeMask(){
    if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10000)) {
        CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(10000, true);
    }
}

void TotalRankScene::creat_Man(){
    float widthFolt = .5f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
    
    if (!enterBool) {
        enterBool = true;
        _ManSpr->setPosition(ccp(_ManSpr->getPosition().x, _ManSpr->getPosition().y));
    }else{
        _ManSpr->setPosition(ccp(_ManSpr->getPosition().x, _ManSpr->getPosition().y));
    }
}
void TotalRankScene::initClothes(){
    float widthFolt = .5f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    
    for (int i = Tag_Total_TouFa; i <= Tag_Total_Bao; i++) {
        if (i == Tag_Total_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_Total_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_Total_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_Total_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_Total_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_Total_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Total_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_Total_WaiTao1);
                _ManSpr->addChild(_wtSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_Total_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_Total_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_Total_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Total_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_Total_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_Total_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_Total_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_Total_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Total_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setScale(scaleFloat);
                _kzSpr1->setFlipX(flipxBool);
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _kzSpr1->setTag(Tag_Total_KuZi1);
                _ManSpr->addChild(_kzSpr1, 290);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_Total_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_Total_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_Total_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Total_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_Total_WaZi1);
                _ManSpr->addChild(_wzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr1->setTag(Tag_Total_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_Total_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_Total_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Total_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_Total_XieZi1);
                _ManSpr->addChild(_xzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_Total_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_Total_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_Total_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Total_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr->addChild(_spSpr1, 50);
                }else{
                    CCDictionary* dic = CONFIG->clothes();// 所有衣服
                    CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                    for (int k = 0; k < clothesArr->count(); k++) {
                        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(k);
                        int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                        if (now_clothes_Id == cloth_id->getValue()) {
                            const CCString* layer1 =  clothDic->valueForKey("layer1");
                            const CCString* layer2 =  clothDic->valueForKey("layer2");
                            const CCString* layer3 =  clothDic->valueForKey("layer3");
                            if (layer1->compare("") != 0) {
                                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_Total_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_Total_Bao1);
                _ManSpr->addChild(_bSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_Total_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_Total_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_Total_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}