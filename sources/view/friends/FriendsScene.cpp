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
#include "AudioManager.h"

#include "FriendsListView.h"
#include "Shower.h"

#include "HaoyouScene.h"


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
        this->create_UI();

        this->create_listview();
        
        this->create_self_panel();
        this->update_self_panel(DATA->getShow());
        
        this->create_show_view(NULL);
        
        return true;
    }
    else {
        return false;
    }
}

void FriendsScene::onEnter() {
    BaseScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
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
    FriendsListView* view = FriendsListView::create();
    this->addChild(view);
}

void FriendsScene::create_self_panel() {
    _selfPanelNormal = CCMenuItemImage::create("pic/haoyoupaihang/self_bg_nor.png", "pic/haoyoupaihang/self_bg_nor.png");
    _selfPanelSelected = CCMenuItemImage::create("pic/haoyoupaihang/self_bg_sel.png", "pic/haoyoupaihang/self_bg_sel.png");
    CCMenuItemToggle* btnSelfPanel = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&FriendsScene::on_btn_self_panel), _selfPanelNormal, _selfPanelSelected, NULL);
    btnSelfPanel->setAnchorPoint(ccp(1, 0.5));
    btnSelfPanel->setPosition(ccp(DISPLAY->W(), DISPLAY->H() * 0.12));
    
    _nodeNormal = CCNode::create();
    _selfPanelNormal->addChild(_nodeNormal);
    
    _nodeSelected = CCNode::create();
    _selfPanelSelected->addChild(_nodeSelected);
    
    CCMenu* menu = CCMenu::createWithItem(btnSelfPanel);
    menu->ignoreAnchorPointForPosition(false);
    this->addChild(menu);
}

void FriendsScene::create_show_view(ShowComp *show) {
    Shower* me = Shower::create();
    this->addChild(me);
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

void FriendsScene::on_btn_self_panel(CCMenuItem *menuItem) {
    
}

void FriendsScene::btn_no_realize(CCMenuItem* menuItem) {
    
}
