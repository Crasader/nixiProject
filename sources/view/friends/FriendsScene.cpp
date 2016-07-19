//
//  FriendsScene.cpp
//  tiegao
//
//  Created by mac on 16/7/19.
//
//

#include "FriendsScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NetManager.h"
#include "AudioManager.h"
#include "Loading2.h"

#include "FriendsListView.h"
#include "ShowerView.h"

#include "HaoyouScene.h"
#include "StrangerScene.h"


#pragma mark - Export API

CCScene* FriendsScene::scene() {
    CCLayer* layer = FriendsScene::create();
    CCScene* rtn = CCScene::create();
    rtn->addChild(layer);
    return rtn;
}



#pragma mark - Super API

FriendsScene::~FriendsScene() {
}

bool FriendsScene::init() {
    if (BaseScene::init()) {
        _listView = NULL;
        _showerView = NULL;
        
        _data = DATA->getSocial()->sortedFriends();
        
        this->create_UI();
        
        this->create_self_panel();
        this->update_self_panel(DATA->getShow());

        this->create_show_view();
        
        if (_data->count() <= 1) {
            // 没有好友，提示去添加好友
            this->create_empty_prompt();
            // 显示自己
            _btnSelfPanel->setSelectedIndex(1);
            this->on_btn_self_panel(_btnSelfPanel);
        }
        else {
            this->create_listview();
            this->nc_change_shower(CCInteger::create(0));
        }
        
        return true;
    }
    else {
        return false;
    }
}

void FriendsScene::onEnter() {
    BaseScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&FriendsScene::nc_change_shower), "ON_CHANGE_SHOWER", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&FriendsScene::nc_goto_strangers_802), "HTTP_FINISHED_802", NULL);
    nc->addObserver(this, SEL_CallFuncO(&FriendsScene::nc_take_energy_807), "HTTP_FINISHED_807", NULL);
}

void FriendsScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    BaseScene::onExit();
}


#pragma mark - Inner API

void FriendsScene::create_UI() {
    CCSprite* background = CCSprite::create("res/pic/haoyoupaihang/main_bg.png");
    background->setPosition(ccp(DISPLAY->ScreenWidth()*.5, DISPLAY->ScreenHeight()*.5));
    this->addChild(background);
    
    //name_bar
    CCSprite* name_bar = CCSprite::create("res/pic/qingjingScene/qj_dikuang1.png");
    name_bar->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .93f));
    this->addChild(name_bar);
    
    CCLabelTTF* room_name = CCLabelTTF::create("好友", DISPLAY->fangzhengFont(), 22);
    room_name->setPosition(ccp(name_bar->getContentSize().width* .5f, name_bar->getContentSize().height* .5f - 4));
    name_bar->addChild(room_name);
    
    //分享
    CCSprite* share_spr = CCSprite::create("res/pic/haoyoupaihang/share.png");
    CCSprite* share_spr2 = CCSprite::create("res/pic/haoyoupaihang/share.png");
    share_spr2->setScale(1.02f);
    CCMenuItemSprite* item_share = CCMenuItemSprite::create(share_spr, share_spr2, this, menu_selector(FriendsScene::btn_no_realize));
    item_share->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .88f));
    CCMenu* menu_share = CCMenu::create(item_share, NULL);
    menu_share->setPosition(CCPointZero);
    this->addChild(menu_share);
    
    //纸条
    CCSprite* note_spr = CCSprite::create("res/pic/haoyoupaihang/btn_zhitiao.png");
    CCSprite* note_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_zhitiao.png");
    note_spr2->setScale(1.02f);
    CCMenuItemSprite* item_note = CCMenuItemSprite::create(note_spr, note_spr2, this, menu_selector(FriendsScene::btn_no_realize));
    item_note->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .2f));
    CCMenu* menu_note = CCMenu::create(item_note, NULL);
    menu_note->setPosition(CCPointZero);
    this->addChild(menu_note);
    
    //返回
    CCSprite* back_spr = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* back_spr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    back_spr2->setScale(1.02f);
    CCMenuItemSprite* item_back = CCMenuItemSprite::create(back_spr, back_spr2, this, menu_selector(FriendsScene::on_btn_back_to_social));
    item_back->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    CCMenu* menu_back = CCMenu::create(item_back, NULL);
    menu_back->setPosition(CCPointZero);
    this->addChild(menu_back);
}

void FriendsScene::create_listview() {
    _listView = FriendsListView::create();
    this->addChild(_listView);
}

void FriendsScene::create_self_panel() {
    _selfPanelNormal = CCMenuItemImage::create("pic/haoyoupaihang/self_bg_nor.png", "pic/haoyoupaihang/self_bg_nor.png");
    _selfPanelSelected = CCMenuItemImage::create("pic/haoyoupaihang/self_bg_sel.png", "pic/haoyoupaihang/self_bg_sel.png");
    _btnSelfPanel = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&FriendsScene::on_btn_self_panel), _selfPanelNormal, _selfPanelSelected, NULL);
    _btnSelfPanel->setAnchorPoint(ccp(1, 0.5));
    _btnSelfPanel->setPosition(ccp(DISPLAY->W(), DISPLAY->H() * 0.12));
    
    _nodeNormal = CCNode::create();
    _selfPanelNormal->addChild(_nodeNormal);
    
    _nodeSelected = CCNode::create();
    _selfPanelSelected->addChild(_nodeSelected);
    
    CCMenu* menu = CCMenu::createWithItem(_btnSelfPanel);
    menu->ignoreAnchorPointForPosition(false);
    this->addChild(menu);
}

void FriendsScene::create_show_view() {
    _showerView = ShowerView::create();
    this->addChild(_showerView);
}

void FriendsScene::create_empty_prompt() {
    CCLabelTTF* lab = CCLabelTTF::create("暂时还没好友\n请去陌生人添加", DISPLAY->fangzhengFont(), 20, CCSizeMake(150, 250), kCCTextAlignmentCenter);
    lab->setColor(ccc3(135, 108, 123));
    lab->setPosition(ccp(DISPLAY->ScreenWidth()* .89f, DISPLAY->ScreenHeight()* .5f + 100));
    this->addChild(lab);
    
    CCSprite* spr = CCSprite::create("pic/haoyouScene/hy_stranger.png");
    CCSprite* spr2 = CCSprite::create("pic/haoyouScene/hy_stranger.png");
    spr2->setScale(DISPLAY->btn_scale());
    CCMenuItemSprite* item_spr = CCMenuItemSprite::create(spr, spr2, this, menu_selector(FriendsScene::on_btn_goto_starngers));
    CCMenu* menu_spr = CCMenu::create(item_spr, NULL);
    menu_spr->setPosition(ccp(DISPLAY->ScreenWidth()* .88f, DISPLAY->ScreenHeight()* .58f));
    this->addChild(menu_spr);
}

void FriendsScene::update_self_panel(ShowComp* self) {
    if (! self) {
        return;
    }
    
    const char* nickname_self = self->nickname();
    if (_nodeNormal) {
        _nodeNormal->removeAllChildrenWithCleanup(true);
        CCLabelTTF* name = CCLabelTTF::create(nickname_self, DISPLAY->fangzhengFont(), 22, CCSizeMake(160, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        name->setPosition(ccp(_selfPanelNormal->getContentSize().width* .68f, _selfPanelNormal->getContentSize().height* .68f));
        name->setColor(ccc3(234, 106, 106));
        _nodeNormal->addChild(name);
        
        CCString* collect_str = CCString::createWithFormat("服装收集: %d", DATA->getShow()->collected());
        CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 16);
        cloth_count->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count->setPosition(ccp(_selfPanelNormal->getContentSize().width * .47f, _selfPanelNormal->getContentSize().height* .36f));
        _nodeNormal->addChild(cloth_count);
    }
    if (_nodeSelected) {
        _nodeSelected->removeAllChildrenWithCleanup(true);
        float scaleRate = 1.2f;
        CCLabelTTF* name = CCLabelTTF::create(nickname_self, DISPLAY->fangzhengFont(), 22, CCSizeMake(160, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        name->setPosition(ccp(_selfPanelSelected->getContentSize().width* .68f, _selfPanelSelected->getContentSize().height* .68f));
        name->setColor(ccc3(234, 106, 106));
        name->setScale(scaleRate);
        _nodeSelected->addChild(name);
        
        CCString* collect_str = CCString::createWithFormat("服装收集: %d", DATA->getShow()->collected());
        CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 16);
        cloth_count->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count->setPosition(ccp(_selfPanelSelected->getContentSize().width * .42f, _selfPanelSelected->getContentSize().height* .34f));
        cloth_count->setScale(scaleRate);
        _nodeSelected->addChild(cloth_count);
    }
}

void FriendsScene::on_btn_back_to_social(CCMenuItem *menuItem) {
    AUDIO->goback_effect();
    CCScene* scene = HaoyouScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void FriendsScene::on_btn_self_panel(CCMenuItemToggle *menuItem) {
    int selectedIndex = menuItem->getSelectedIndex();
    if (selectedIndex == 1) {
        _btnSelfPanel->setEnabled(false);       // 关闭按钮
        ShowComp* selfShow = DATA->getShow();
        _showerView->change_shower(selfShow->ondress());
    }
    else {
        _btnSelfPanel->setEnabled(true);        // 开启按钮
    }
}

void FriendsScene::on_btn_goto_starngers(CCMenuItem* menuItem) {
    LOADING->show_loading();
    NET->recommend_stranger_802();
}

void FriendsScene::nc_change_shower(CCObject *pObj) {
    _btnSelfPanel->setSelectedIndex(0);
    this->on_btn_self_panel(_btnSelfPanel);
    
    CCInteger* value = (CCInteger*)pObj;
    CCLOG("FriendsScene::nc_change_shower(idx = %d)", value->getValue());
    if (_showerView) {
        ShowComp* show = (ShowComp*)_data->objectAtIndex(value->getValue());
        _showerView->change_shower(show->ondress());
    }
}

void FriendsScene::nc_goto_strangers_802(CCObject *pObj) {
    LOADING->remove();
    CCScene* scene = CCScene::create();
    StrangerScene* layer = StrangerScene::create();
    layer->setEnterType("my_friend");
    scene->addChild(layer);
    
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void FriendsScene::nc_take_energy_807(CCObject *pObj) {
    
}

void FriendsScene::btn_no_realize(CCMenuItem* menuItem) {
    
}
