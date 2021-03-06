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
#include "PromptLayer.h"
#include "AudioManager.h"
#include "NetManager.h"
#include "AppUtil.h"
#include "JNIController.h"
#include "Loading2.h"


const float totalRank_z_oder = 20.f;

TotalRankScene:: ~TotalRankScene(){}

bool TotalRankScene::init(){
    if(!BaseScene::init()){
        return false;
    }
    
    num_child = 0;
        
    enterBool = false;
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    _rankers = DATA->getRanking()->ranking();
    int count = _rankers->count();
    
    if (count == 0) {
        rank_self = -1;
    }else{
        const char* id_self = DATA->getLogin()->obtain_sid();
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(_rankers, pObj){
            ShowComp* show_rank = (ShowComp*)pObj;
            if (show_rank->getShowID().compare(id_self) == 0) {
                rank_self = (int)_rankers->indexOfObject(pObj) + 1;
                break;
            }else{
                rank_self = -1;
            }
        }
    }

    
    isadd_fst = false;
    isadd_sed = false;
    isadd_thd = false;
    
    this->createView();
    
    if (count == 0) {
        
        // 自己的衣着
        myClothesTemp = DATA->getShow()->ondress(); //DATA->getClothes()->MyClothesTemp();
    }else{
        if (count == 1){
            this->create_first();
        }else if (count == 2) {
            this->create_first();
            this->create_second();
        }else{
            this->create_first();
            this->create_second();
            this->create_third();
        }
        
        this->create_self();
        
        ShowComp* show = (ShowComp*)DATA->getRanking()->ranking()->objectAtIndex(0);
        // 排名第一的
        myClothesTemp = show->ondress();
    }
    
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
    nc->addObserver(this, SEL_CallFuncO(&TotalRankScene::_321CallBack), "HTTP_FINISHED_321", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&TotalRankScene::small_callback), "Small", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TotalRankScene::exitMan), "ExitMan", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&TotalRankScene::keyBackStatus), .8f);
}
void TotalRankScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void TotalRankScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    BaseScene::onExit();
}

void TotalRankScene::createView(){
    
    CCSprite* background = CCSprite::create("res/pic/haoyoupaihang/rank_bg_1.png");
    background->setPosition(ccp(DISPLAY->ScreenWidth()*.5, DISPLAY->ScreenHeight()*.5));
    this->addChild(background);
    
    //name_bar
    CCSprite* name_bar = CCSprite::create("res/pic/qingjingScene/qj_dikuang1.png");
    name_bar->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .93f));
    this->addChild(name_bar);
    
    CCLabelTTF* room_name = CCLabelTTF::create("排行榜", DISPLAY->fangzhengFont(), 22);
    room_name->setPosition(ccp(name_bar->getContentSize().width* .5f, name_bar->getContentSize().height* .5f - 4));
    name_bar->addChild(room_name);
    
    //分享
    CCSprite* shareSpr1;
    CCSprite* shareSpr2;
    if (DATA->getNews()->dailyShareCount == 0) {
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2->setScale(1.02f);
    }else{
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2->setScale(1.02f);
    }
    CCMenuItemSprite* item_share = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(TotalRankScene::btn_share_callback));
    item_share->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .88f));
    CCMenu* menu_share = CCMenu::create(item_share, NULL);
    menu_share->setPosition(CCPointZero);
    menu_share->setTag(0x334455);
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
    CCSprite* back_spr = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* back_spr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    back_spr2->setScale(1.02f);
    CCMenuItemSprite* item_back = CCMenuItemSprite::create(back_spr, back_spr2, this, menu_selector(TotalRankScene::btn_back_callback));
    item_back->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menu_back = CCMenu::create(item_back, NULL);
    menu_back->setPosition(CCPointZero);
    this->addChild(menu_back, totalRank_z_oder);
    
    
    this->initTotalRank();
    
}

void TotalRankScene::create_self(){
    //self
    
    CCSprite* self_spr = CCSprite::create("res/pic/haoyoupaihang/self_bg_nor.png");
    CCSprite* self_spr2 = CCSprite::create("res/pic/haoyoupaihang/self_bg_sel.png");
    item_self = CCMenuItemSprite::create(self_spr, self_spr2, this, menu_selector(TotalRankScene::btn_toBig_callback));
    item_self->setPosition(ccp(DISPLAY->ScreenWidth() - self_spr->getContentSize().width/2, DISPLAY->ScreenHeight()* .08f));
    item_self ->setUserObject(CCInteger::create(-100));
    
    
    const char* nickname_self = DATA->getShow()->nickname();
    CCLabelTTF* name = CCLabelTTF::create(nickname_self, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name->setPosition(ccp(self_spr->getContentSize().width* .75f, self_spr->getContentSize().height* .68f));
    name->setColor(ccc3(109, 98, 96));
    name->setTag(100);
    self_spr->addChild(name);
    
    CCLabelTTF* name2 = CCLabelTTF::create(nickname_self, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name2->setPosition(ccp(self_spr2->getContentSize().width* .63f, self_spr2->getContentSize().height* .76f));
    name2->setColor(ccc3(109, 98, 96));
    name2->setTag(100);
    self_spr2->addChild(name2);
    
    CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
    flag->setPosition(ccp(self_spr->getContentSize().width * .6f, self_spr->getContentSize().height * 0.38));
    self_spr->addChild(flag);
    
    CCSprite* flag2 = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
    flag2->setPosition(ccp(self_spr2->getContentSize().width * .50f, self_spr2->getContentSize().height * 0.36));
    self_spr2->addChild(flag2);
    
    CCString* collect_str = CCString::createWithFormat("%d", DATA->getShow()->collected());
    CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 20);
    cloth_count->setAnchorPoint(CCPoint(0, 0.5));
    cloth_count->setPosition(ccp(self_spr->getContentSize().width * .78f, self_spr->getContentSize().height* .36f));
    cloth_count->setTag(200);
    self_spr->addChild(cloth_count);
    
    CCLabelTTF* cloth_count2 = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 22);
    cloth_count2->setAnchorPoint(CCPoint(0, 0.5));
    cloth_count2->setPosition(ccp(self_spr2->getContentSize().width * .75f, self_spr2->getContentSize().height* .34f));
    cloth_count2->setTag(200);
    self_spr2->addChild(cloth_count2);
    
    if (rank_self == -1) {
        CCSprite* spr = CCSprite::create("res/pic/haoyoupaihang/weishangbang.png");
        spr->setPosition(ccp(self_spr->getContentSize().width* .325f, self_spr->getContentSize().height* .5f));
        spr->setTag(300);
        self_spr->addChild(spr);
        
        CCSprite* spr2 = CCSprite::create("res/pic/haoyoupaihang/weishangbang.png");
        spr2->setPosition(ccp(self_spr2->getContentSize().width* .16f, self_spr2->getContentSize().height* .5f));
        spr2->setTag(300);
        self_spr2->addChild(spr2);
        
    }else{
        if (rank_self >= 1 && rank_self <= 9) {
            CCSprite* spr = this->getNumSprite(rank_self);
            spr->setPosition(ccp(self_spr->getContentSize().width* .325f, self_spr->getContentSize().height* .5f));
            spr->setTag(300);
            self_spr->addChild(spr);
            
            CCSprite* spr2 = this->getNumSprite(rank_self);
            spr2->setPosition(ccp(self_spr2->getContentSize().width* .16f, self_spr2->getContentSize().height* .5f));
            spr2->setTag(300);
            self_spr2->addChild(spr2);
        }else{
            CCSprite* spr1 = this->getNumSprite((int)floor(rank_self/10));
            spr1->setPosition(ccp(self_spr->getContentSize().width* .325f - 8, self_spr->getContentSize().height* .5f));
            spr1->setTag(300);
            self_spr->addChild(spr1);
            CCSprite* spr2 = this->getNumSprite((int)floor(rank_self%10));
            spr2->setPosition(ccp(self_spr->getContentSize().width* .325f + 8, self_spr->getContentSize().height* .5f));
            spr2->setTag(400);
            self_spr->addChild(spr2);
            
            CCSprite* spr3 = this->getNumSprite((int)floor(rank_self/10));
            spr3->setPosition(ccp(self_spr->getContentSize().width* .16f - 8, self_spr->getContentSize().height* .5f));
            spr3->setTag(300);
            self_spr2->addChild(spr3);
            CCSprite* spr4 = this->getNumSprite((int)floor(rank_self%10));
            spr4->setPosition(ccp(self_spr2->getContentSize().width* .16f + 8, self_spr2->getContentSize().height* .5f));
            spr4->setTag(400);
            self_spr2->addChild(spr4);
        }
    }
    
    
    
    CCMenu* menu_rank = CCMenu::create(item_self, NULL);
    menu_rank->setPosition(CCPointZero);
    this->addChild(menu_rank, totalRank_z_oder);
    
    if (_rankers->count() == 0) {
        this->btn_toBig_callback(item_self);
        item_self->setEnabled(false);
    }
}

void TotalRankScene::create_first(){
    //first
    
    _curBtn_index = 0;
    DATA->getSocial()->setSelectedRanker(0);
    
    CCSprite* bg_first = CCSprite::create("res/pic/haoyoupaihang/first_bg_nor.png");
    CCSprite* bg_first2 = CCSprite::create("res/pic/haoyoupaihang/first_bg_sel.png");
    item_first = CCMenuItemSprite::create(bg_first, bg_first2, this, menu_selector(TotalRankScene::btn_toBig_callback));
    item_first->setPosition(ccp(DISPLAY->ScreenWidth() - bg_first->getContentSize().width/2, DISPLAY->ScreenHeight()* .16f + 110*7));
    item_first->setUserObject(CCInteger::create(0));
    item_first->setEnabled(false);
    item_first->selected();
    
    ShowComp* show_fst = (ShowComp* )_rankers->objectAtIndex(0);
    const char* nickname_first = show_fst->nickname();
    int collect_first = show_fst->collected();
    
    CCSprite* num_fst_item = CCSprite::create("res/pic/haoyoupaihang/num_1.png");
    num_fst_item->setPosition(ccp(bg_first->getContentSize().width* .325f, bg_first->getContentSize().height* .5f));
    bg_first->addChild(num_fst_item);
    
    CCSprite* num_fst_item2 = CCSprite::create("res/pic/haoyoupaihang/num_1.png");
    num_fst_item2->setPosition(ccp(bg_first2->getContentSize().width* .16f, bg_first2->getContentSize().height* .5f));
    bg_first2->addChild(num_fst_item2);
    
    CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
    flag->setPosition(ccp(bg_first->getContentSize().width * .6f, bg_first->getContentSize().height * 0.38));
    bg_first->addChild(flag);
    
    CCSprite* flag2 = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
    flag2->setPosition(ccp(bg_first2->getContentSize().width * .50f, bg_first2->getContentSize().height * 0.36));
    bg_first2->addChild(flag2);
    
    CCLabelTTF* name_first_item = CCLabelTTF::create(nickname_first, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name_first_item->setPosition(ccp(bg_first->getContentSize().width* .75f, bg_first->getContentSize().height* .68));
    name_first_item->setColor(ccc3(109, 98, 96));
    bg_first->addChild(name_first_item);
    
    CCLabelTTF* name_first_item2 = CCLabelTTF::create(nickname_first, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name_first_item2->setPosition(ccp(bg_first2->getContentSize().width* .63f, bg_first2->getContentSize().height* .76));
    name_first_item2->setColor(ccc3(109, 98, 96));
    bg_first2->addChild(name_first_item2);
    
    CCString* collect_first_str = CCString::createWithFormat("%d", collect_first);
    CCLabelTTF* cloth_count_first_item = CCLabelTTF::create(collect_first_str->getCString(), DISPLAY->fangzhengFont(), 20);
    cloth_count_first_item->setPosition(ccp(bg_first->getContentSize().width * .78, bg_first->getContentSize().height* .36f));
    cloth_count_first_item->setAnchorPoint(CCPoint(0, 0.5));
    bg_first->addChild(cloth_count_first_item);
    
    CCLabelTTF* cloth_count_first_item2 = CCLabelTTF::create(collect_first_str->getCString(), DISPLAY->fangzhengFont(), 22);
    cloth_count_first_item2->setPosition(ccp(bg_first2->getContentSize().width * .75, bg_first2->getContentSize().height* .34f));
    cloth_count_first_item2->setAnchorPoint(CCPoint(0, 0.5));
    bg_first2->addChild(cloth_count_first_item2);
    
    
    
    if (show_fst->getShowID().compare(DATA->getLogin()->obtain_sid()) == 0) {
        //这是我自己
        
    }else {
        
        if (DATA->getSocial()->is_friend(show_fst->getShowID().c_str())) {
            //是好友
            CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
            CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
            spr_fst2->setScale(1.02f);
            CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
            CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
            menu_add_fst->setPosition(ccp(bg_first2->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
            bg_first2->addChild(menu_add_fst);
            item_add_fst->setEnabled(false);
        }
        // 已申请好友
        else if (true == DATA->getSocial()->is_requested(show_fst->getShowID().c_str())) {
            CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
            CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
            spr_fst2->setScale(1.02f);
            CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
            CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
            menu_add_fst->setPosition(ccp(bg_first2->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
            bg_first2->addChild(menu_add_fst);
            item_add_fst->setEnabled(false);
        }
        else{
            //不是好友
            CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
            CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
            spr_fst2->setScale(1.02f);
            CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
            item_add_fst->setUserObject(CCInteger::create(0));
            CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
            menu_add_fst->setPosition(ccp(bg_first2->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
            bg_first2->addChild(menu_add_fst);
        }
    }
    
    CCMenu* menu = CCMenu::create(item_first, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, totalRank_z_oder);
}

void TotalRankScene::create_second(){
    //second
    CCSprite* bg_second = CCSprite::create("res/pic/haoyoupaihang/second_bg_nor.png");
    CCSprite* bg_second2 = CCSprite::create("res/pic/haoyoupaihang/second_bg_sel.png");
    item_second = CCMenuItemSprite::create(bg_second, bg_second2, this, menu_selector(TotalRankScene::btn_toBig_callback));
    item_second->setPosition(ccp(DISPLAY->ScreenWidth() - bg_second->getContentSize().width/2, DISPLAY->ScreenHeight()* .16f + 110*6));
    item_second->setUserObject(CCInteger::create(1));
    
    
    ShowComp* show_sed = (ShowComp*)_rankers->objectAtIndex(1);
    const char* nickname_second = show_sed->nickname();
    int collect_second = show_sed->collected();
    
    
    CCSprite* num_sed = CCSprite::create("res/pic/haoyoupaihang/num_2.png");
    num_sed->setPosition(ccp(bg_second->getContentSize().width* .325f, bg_second->getContentSize().height* .5f));
    bg_second->addChild(num_sed);
    
    CCSprite* num_sed2 = CCSprite::create("res/pic/haoyoupaihang/num_2.png");
    num_sed2->setPosition(ccp(bg_second2->getContentSize().width* .16f, bg_second2->getContentSize().height* .5f));
    bg_second2->addChild(num_sed2);
    
    CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
    flag->setPosition(ccp(bg_second->getContentSize().width * .6f, bg_second->getContentSize().height * 0.38));
    bg_second->addChild(flag);
    
    CCSprite* flag2 = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
    flag2->setPosition(ccp(bg_second2->getContentSize().width * .50f, bg_second2->getContentSize().height * 0.36));
    bg_second2->addChild(flag2);
    
    CCLabelTTF* name_second = CCLabelTTF::create(nickname_second, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name_second->setPosition(ccp(bg_second->getContentSize().width* .75f, bg_second->getContentSize().height* .68));
    name_second->setColor(ccc3(109, 98, 96));
    bg_second->addChild(name_second);
    
    CCLabelTTF* name_second2 = CCLabelTTF::create(nickname_second, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name_second2->setPosition(ccp(bg_second2->getContentSize().width* .63f, bg_second2->getContentSize().height* .76));
    name_second2->setColor(ccc3(109, 98, 96));
    bg_second2->addChild(name_second2);
    
    CCString* collect_second_str = CCString::createWithFormat("%d", collect_second);
    CCLabelTTF* cloth_count_second = CCLabelTTF::create(collect_second_str->getCString(), DISPLAY->fangzhengFont(), 20);
    cloth_count_second->setPosition(ccp(bg_second->getContentSize().width * .78, bg_second->getContentSize().height* .36f));
    cloth_count_second->setAnchorPoint(CCPoint(0, 0.5));
    bg_second->addChild(cloth_count_second);
    
    CCLabelTTF* cloth_count_second2 = CCLabelTTF::create(collect_second_str->getCString(), DISPLAY->fangzhengFont(), 22);
    cloth_count_second2->setPosition(ccp(bg_second2->getContentSize().width * .75, bg_second2->getContentSize().height* .34f));
    cloth_count_second2->setAnchorPoint(CCPoint(0, 0.5));
    bg_second2->addChild(cloth_count_second2);
    
    
    
    if (show_sed->getShowID().compare(DATA->getLogin()->obtain_sid()) == 0) {
        //这是我自己
        
    }else {
        if (DATA->getSocial()->is_friend(show_sed->getShowID().c_str())) {
            //是好友
            CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
            CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
            spr_fst2->setScale(1.02f);
            CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
            //                item_add->setUserObject(CCInteger::create(index + 3));
            CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
            menu_add_fst->setPosition(ccp(bg_second2->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
            bg_second2->addChild(menu_add_fst);
            item_add_fst->setEnabled(false);
//            menu_add_fst->setVisible(false);
        }else{
            bool hasRequested = DATA->getSocial()->is_requested(show_sed->getShowID().c_str());
            //不是好友
            if (hasRequested || isadd_sed) {
                CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
                CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
                spr_fst2->setScale(1.02f);
                CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
                //                item_add->setUserObject(CCInteger::create(index + 3));
                CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
                menu_add_fst->setPosition(ccp(bg_second2->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
                bg_second2->addChild(menu_add_fst);
                item_add_fst->setEnabled(false);
//                menu_add_fst->setVisible(false);
            }else{
                CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
                CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
                spr_fst2->setScale(1.02f);
                CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
                item_add_fst->setUserObject(CCInteger::create(1));
                CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
                menu_add_fst->setPosition(ccp(item_second->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
//                menu_add_fst->setVisible(false);
                bg_second2->addChild(menu_add_fst);
            }
            
        }
    }
    
    
    CCMenu* menu = CCMenu::create(item_second, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, totalRank_z_oder);
}

void TotalRankScene::create_third(){
    CCSprite* bg_third = CCSprite::create("res/pic/haoyoupaihang/third_bg_nor.png");
    CCSprite* bg_third2 = CCSprite::create("res/pic/haoyoupaihang/third_bg_sel.png");
    item_third = CCMenuItemSprite::create(bg_third, bg_third2, this, menu_selector(TotalRankScene::btn_toBig_callback));
    item_third->setPosition(ccp(DISPLAY->ScreenWidth() - bg_third->getContentSize().width/2, DISPLAY->ScreenHeight()* .16f + 110*5));
    item_third->setUserObject(CCInteger::create(2));
    
    
    ShowComp* show_thd = (ShowComp*)_rankers->objectAtIndex(2);
    const char* nickname_third = show_thd->nickname();
    int collect_third = show_thd->collected();
    
    
    CCSprite* num_thd = CCSprite::create("res/pic/haoyoupaihang/num_3.png");
    num_thd->setPosition(ccp(bg_third->getContentSize().width* .325f, bg_third->getContentSize().height* .5f));
    bg_third->addChild(num_thd);
    
    CCSprite* num_thd2 = CCSprite::create("res/pic/haoyoupaihang/num_3.png");
    num_thd2->setPosition(ccp(bg_third2->getContentSize().width* .16f, bg_third2->getContentSize().height* .5f));
    bg_third2->addChild(num_thd2);
    
    CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
    flag->setPosition(ccp(bg_third->getContentSize().width * .6f, bg_third->getContentSize().height * 0.38));
    bg_third->addChild(flag);
    
    CCSprite* flag2 = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
    flag2->setPosition(ccp(bg_third2->getContentSize().width * .50f, bg_third2->getContentSize().height * 0.36));
    bg_third2->addChild(flag2);
    
    CCLabelTTF* name_third = CCLabelTTF::create(nickname_third, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name_third->setPosition(ccp(bg_third->getContentSize().width* .75f, bg_third->getContentSize().height* .68));
    name_third->setColor(ccc3(109, 98, 96));
    bg_third->addChild(name_third);
    
    CCLabelTTF* name_third2 = CCLabelTTF::create(nickname_third, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    name_third2->setPosition(ccp(bg_third2->getContentSize().width* .63f, bg_third2->getContentSize().height* .76));
    name_third2->setColor(ccc3(109, 98, 96));
    bg_third2->addChild(name_third2);
    
    CCString* collect_first_str = CCString::createWithFormat("%d", collect_third);
    CCLabelTTF* cloth_count_third = CCLabelTTF::create(collect_first_str->getCString(), DISPLAY->fangzhengFont(), 20);
    cloth_count_third->setPosition(ccp(bg_third->getContentSize().width * .78, bg_third->getContentSize().height* .36f));
    cloth_count_third->setAnchorPoint(CCPoint(0, 0.5));
    bg_third->addChild(cloth_count_third);
    
    CCLabelTTF* cloth_count_third2 = CCLabelTTF::create(collect_first_str->getCString(), DISPLAY->fangzhengFont(), 22);
    cloth_count_third2->setPosition(ccp(bg_third2->getContentSize().width * .75, bg_third2->getContentSize().height* .34f));
    cloth_count_third2->setAnchorPoint(CCPoint(0, 0.5));
    bg_third2->addChild(cloth_count_third2);
    
    
    if (show_thd->getShowID().compare(DATA->getLogin()->obtain_sid()) == 0) {
        //这是我自己
        
    }else {
        if (DATA->getSocial()->is_friend(show_thd->getShowID().c_str())) {
            //是好友
            CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
            CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/add_finish.png");
            spr_fst2->setScale(1.02f);
            CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
            //                item_add->setUserObject(CCInteger::create(index + 3));
            CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
            menu_add_fst->setPosition(ccp(bg_third2->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
            bg_third2->addChild(menu_add_fst);
            item_add_fst->setEnabled(false);
//            menu_add_fst->setVisible(false);
        }else{
            bool hasRequested = DATA->getSocial()->is_requested(show_thd->getShowID().c_str());
            //不是好友
            if (hasRequested || isadd_sed) {
                CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
                CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
                spr_fst2->setScale(1.02f);
                CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
                //                item_add->setUserObject(CCInteger::create(index + 3));
                CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
                menu_add_fst->setPosition(ccp(bg_third2->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
                bg_third2->addChild(menu_add_fst);
                item_add_fst->setEnabled(false);
//                menu_add_fst->setVisible(false);
            }else{
                CCSprite* spr_fst = CCSprite::create("res/pic/haoyoupaihang/addFriend.png");
                CCSprite* spr_fst2 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
                spr_fst2->setScale(1.02f);
                CCMenuItem* item_add_fst = CCMenuItemSprite::create(spr_fst, spr_fst2, this, menu_selector(TotalRankScene::toBeFriend));
                item_add_fst->setUserObject(CCInteger::create(2));
                CCMenu* menu_add_fst = CCMenu::create(item_add_fst, NULL);
                menu_add_fst->setPosition(ccp(bg_third2->getContentSize().width - spr_fst->getContentSize().width/2 -10, 8));
                bg_third2->addChild(menu_add_fst);
            }
            
        }
    }
    
    CCMenu* menu = CCMenu::create(item_third, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, totalRank_z_oder);
    
}

void TotalRankScene::initTotalRank(){
    CCSprite* spr = CCSprite::create("res/pic/haoyoupaihang/other_bg_nor.png");
    
    tabLayer = TotalRankTableView::create();
    
    tabLayer->setPosition(ccp(DISPLAY->ScreenWidth() - spr->getContentSize().width, DISPLAY->ScreenHeight()* .16f));
    this->addChild(tabLayer, 20);
}

void TotalRankScene::btn_share_callback(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "排行界面", "点击分享");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/share.png");
    
    CCLog("图片 === %s", path.c_str());
    
    rt->saveToFile(path.c_str());
    
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("share.png");
    
    CCLog("图片 === %s", path.c_str());
    
    JNIController::setShareImage(path.c_str());
    rt->saveToFile(path.c_str());
    
    JNIController::showShare();
    this->schedule(SEL_SCHEDULE(&TotalRankScene::shareStatus), .1f);
#endif
    
}
void TotalRankScene::shareStatus(float dt){
    
    if (JNIController::getShareStatus() == 1) {
        JNIController::shareText();
        JNIController::setShareStatus(0);
        
        LOADING->show_loading();
        NET->daily_share_321();
        
        this->unschedule(SEL_SCHEDULE(&TotalRankScene::shareStatus));
    }else if (JNIController::getShareStatus() == 2 || JNIController::getShareStatus() == 3){
        JNIController::shareText();
        JNIController::setShareStatus(0);
        this->unschedule(SEL_SCHEDULE(&TotalRankScene::shareStatus));
    }
}
void TotalRankScene::_321CallBack(CCObject* pSender){
    LOADING->remove();
    
    if (this->getChildByTag(0x99999) != NULL) {
        this->removeChildByTag(0x99999);
    }
    if (this->getChildByTag(0x334455) != NULL) {
        this->removeChildByTag(0x334455);
    }
    CCSprite* shareSpr1;
    CCSprite* shareSpr2;
    if (DATA->getNews()->dailyShareCount == 0) {
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2->setScale(1.02f);
    }else{
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2->setScale(1.02f);
    }
    CCMenuItem* shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(TotalRankScene::btn_share_callback));
    shareItem->setAnchorPoint(ccp(0, .5f));
    shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
    CCMenu* shareMenu = CCMenu::create(shareItem, NULL);
    shareMenu->setPosition(CCPointZero);
    shareMenu->setTag(0x334455);
    this->addChild(shareMenu, 20);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}

void TotalRankScene::btn_note_callback(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "排行界面", "点击纸条");
    
    CCArray* arr = DATA->getRanking()->ranking();
    if (arr->count() == 0) {
        
    }else{
        ShowComp* show = (ShowComp*)arr->objectAtIndex(DATA->getSocial()->getSelectedRanker());
        if (_curBtn_index == -100 || show->getShowID().compare(DATA->getLogin()->obtain_sid()) == 0) {
            PromptLayer* layer = PromptLayer::create();
            layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "不可以给自己发纸条哦");
        }else{
            _panel = NotePanel::create();
            _panel->setEntranceType("ranker");
            this->addChild(_panel, 10000);
        }
    }
    
}

void TotalRankScene::btn_back_callback(CCObject* pSender){
    AUDIO->goback_effect();
    // talkingData
    DATA->onEvent("点击事件", "排行界面", "点击返回");
    
    if (_type == 1) {
        num_child = 0;
        CCScene* scene = MainScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else if (_type == 2) {
        num_child = 0;
        CCScene* scene = HaoyouScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}


void TotalRankScene::btn_toBig_callback(CCMenuItem* btn){
    CCMenuItem* item = (CCMenuItem*)btn;
    item->setEnabled(false);
    item->selected();
    int index = ((CCInteger*)item->getUserObject())->getValue();
    
    
    if(index != -100) {
       DATA->getSocial()->setSelectedRanker(index);
    }
    
    if (_curBtn_index != -1) {
        this->btn_toSmall_callback(index);
        _curBtn_index = index;
    }else{
        _curBtn_index = index;
        CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdateRank");
    }
    
    CCLayer* layer = CCLayer::create();
    layer->setTouchEnabled(true);
    layer->setTouchSwallowEnabled(true);
    layer->setTag(10000);
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000);
    this->exitMan();
}

void TotalRankScene::btn_toSmall_callback(int index){
    if (_curBtn_index == 0) {
//        if (item_first->getChildByTag(0x1000)) {
//            item_first->removeChildByTag(0x1000, true);
//        }
        item_first->setEnabled(true);
        item_first->unselected();
        
//        CCSprite* num = (CCSprite*)item_first->getChildByTag(100);
//        num->setPosition(ccp(item_third->getContentSize().width* .25f, item_third->getContentSize().height* .5f));
//        
//        CCLabelTTF* name = (CCLabelTTF*)item_first->getChildByTag(200);
//        name->setFontSize(22);
//        name->setPosition(ccp(item_third->getContentSize().width* .68f, item_third->getContentSize().height* .68f));
        
    }else if (_curBtn_index == 1){
//        if (item_second->getChildByTag(0x1000)) {
//            item_second->removeChildByTag(0x1000, true);
//        }
        item_second->setEnabled(true);
        item_second->unselected();
        
    }else if (_curBtn_index == 2){
//        if (item_third->getChildByTag(0x1000)) {
//            item_third->removeChildByTag(0x1000, true);
//        }
        item_third->setEnabled(true);
        item_third->unselected();
        
    }else if (_curBtn_index == -100){
//        if (item_self->getChildByTag(0x1000)) {
//            item_self->removeChildByTag(0x1000, true);
//        }
        item_self->setEnabled(true);
        item_self->unselected();
    }
}

void TotalRankScene::small_callback(){
    if (_curBtn_index == 0 || _curBtn_index != NULL) {
        this->btn_toSmall_callback(_curBtn_index);
        _curBtn_index = -1;
    }
}

void TotalRankScene::enterMan(){
    if (_curBtn_index == -100) {
        myClothesTemp = DATA->getShow()->ondress();
    }else{
        ShowComp* show = (ShowComp*)DATA->getRanking()->ranking()->objectAtIndex(DATA->getSocial()->getSelectedRanker());
        myClothesTemp = show->ondress();
    }
    
    
    this->creat_Man();
    this->initClothes();
    
    CCMoveTo* moveTo = CCMoveTo::create(.3f, ccp(0, _ManSpr->getPosition().y));
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TotalRankScene::removeMask));
    CCSequence* seq = CCSequence::create(moveTo, callFunc, NULL);
    _ManSpr->runAction(seq);
}

void TotalRankScene::exitMan(){
    CCPlace* pl = CCPlace::create(ccp(-500, _ManSpr->getPositionY()));
    CCCallFunc* callFunc1 = CCCallFunc::create(this, SEL_CallFunc(&TotalRankScene::removeMan));
    CCCallFunc* callFunc2 = CCCallFunc::create(this, SEL_CallFunc(&TotalRankScene::enterMan));
    CCSequence* seq = CCSequence::create(pl, CCDelayTime::create(.1f), callFunc1, CCDelayTime::create(.1f), callFunc2, NULL);
    _ManSpr->runAction(seq);
    //
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void TotalRankScene::removeMan(){
    _ManSpr->removeAllChildrenWithCleanup(true);
    
}

void TotalRankScene::removeMask(){
    if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(10000)) {
        CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(10000, true);
    }
}

void TotalRankScene::toBeFriend(CCMenuItem *btn){
    CCMenuItem* item = (CCMenuItem* )btn;
    int index = ((CCInteger*)item->getUserObject())->getValue();
    
    CCNode* bg = (CCNode* )item->getParent()->getParent();
    bg->removeChildByTag(0x10400, true);
    if (index == 0) {
        isadd_fst = true;
    }else if (index == 1){
        isadd_sed = true;
    }else if(index == 2){
        isadd_thd = true;
    }
    
    CCArray* rankers = DATA->getRanking()->ranking();
    ShowComp* other = (ShowComp*)rankers->objectAtIndex(index);
    
//    CCSprite* add_spr3 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
//    CCSprite* add_spr4 = CCSprite::create("res/pic/haoyoupaihang/friend_requested.png");
//    CCMenuItemSprite* item_add;
//    item_add = CCMenuItemSprite::create(add_spr3, add_spr4, this, NULL);
//    CCMenu* menu_add = CCMenu::create(item_add, NULL);
//    menu_add->setPosition(ccp(bg->getContentSize().width - add_spr3->getContentSize().width/2 -10, 10));
//    menu_add->setTag(0X10400);
//    bg->addChild(menu_add);
    item->selected();
    
    NET->send_message_803(other->getShowID().c_str(), 1);
}

CCSprite* TotalRankScene::getNumSprite(int num){
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
    int sub_part = 0;
    
    for (int i = Tag_Total_TouFa; i <= Tag_Total_ZhuangRong; i++) {
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
                        sub_part = clothDic->valueForKey("sub_part")->intValue();
                        
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
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_Total_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_Total_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }
                
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
        }else if (i == Tag_Total_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_Total_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _zrSpr1->setTag(Tag_Total_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}

void TotalRankScene::keyBackClicked(){
    num_child++;
    CCLog("===== TotalRankScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->btn_back_callback(NULL);
}

