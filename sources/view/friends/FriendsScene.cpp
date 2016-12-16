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
#include "PromptLayer.h"

#include "FriendsListView.h"
#include "ShowerView.h"

#include "HaoyouScene.h"
#include "StrangerScene.h"
#include "NotePanel.h"

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
        num_child = 0;
        
        CCSprite* background = CCSprite::create("res/pic/haoyoupaihang/main_bg.png");
        background->setPosition(ccp(DISPLAY->ScreenWidth()*.5, DISPLAY->ScreenHeight()*.5));
        this->addChild(background);
        
        _curIndex = -1;
        _listView = NULL;
        _showerView = NULL;
        
        _data = DATA->getSocial()->sortedFriends();
        
        this->create_show_view();
        this->create_UI();
        
        this->create_self_panel();
        this->update_self_panel(DATA->getShow());

        
        this->obtain_self_ranking();
        
        if (_data->count() <= 1) {
            // 没有好友，提示去添加好友
            this->create_empty_prompt();
            // 显示自己
            _btnSelfPanel->setSelectedIndex(1);
            this->on_btn_self_panel(_btnSelfPanel);
        }
        else {
            _curIndex = 0;
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
    nc->addObserver(this, SEL_CallFuncO(&FriendsScene::nc_send_energy_803), "HTTP_FINISHED_803", NULL);
    nc->addObserver(this, SEL_CallFuncO(&FriendsScene::nc_take_energy_807), "HTTP_FINISHED_807", NULL);
    nc->addObserver(this, SEL_CallFuncO(&FriendsScene::nc_friend_break_813), "HTTP_FINISHED_813", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&FriendsScene::keyBackStatus), .8f);
}

void FriendsScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void FriendsScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    BaseScene::onExit();
}


#pragma mark - Inner API

void FriendsScene::create_UI() {
    
    //name_bar
    CCSprite* name_bar = CCSprite::create("res/pic/qingjingScene/qj_dikuang1.png");
    name_bar->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .93f));
    this->addChild(name_bar);
    
    CCLabelTTF* room_name = CCLabelTTF::create("好友", DISPLAY->fangzhengFont(), 22);
    room_name->setPosition(ccp(name_bar->getContentSize().width* .5f, name_bar->getContentSize().height* .5f - 4));
    name_bar->addChild(room_name);
    {
    //纸条
    CCSprite* note_spr = CCSprite::create("res/pic/haoyoupaihang/btn_zhitiao.png");
    CCSprite* note_spr2 = CCSprite::create("res/pic/haoyoupaihang/btn_zhitiao.png");
    note_spr2->setScale(1.02f);
    _btnPaper = CCMenuItemSprite::create(note_spr, note_spr2, this, menu_selector(FriendsScene::on_btn_send_paper));
    _btnPaper->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .3f));
    CCMenu* menu_note = CCMenu::create(_btnPaper, NULL);
    menu_note->setPosition(CCPointZero);
    this->addChild(menu_note);
    }{
    // 删除
    CCSprite* delete_spr = CCSprite::create("res/pic/haoyouScene/btn_delete.png");
    CCSprite* delete_spr2 = CCSprite::create("res/pic/haoyouScene/btn_delete.png");
    delete_spr->setScale(1.02f);
    _btnDelete = CCMenuItemSprite::create(delete_spr, delete_spr2, this, SEL_MenuHandler(&FriendsScene::on_btn_delete_friend));
    _btnDelete->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .2f));
    CCMenu* menu_delete = CCMenu::create(_btnDelete, NULL);
    menu_delete->setPosition(CCPointZero);
    this->addChild(menu_delete);
    }{
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
    _nodeNormal->setContentSize(_selfPanelNormal->getContentSize());
    
    _nodeSelected = CCNode::create();
    _selfPanelSelected->addChild(_nodeSelected);
    _nodeSelected->setContentSize(_selfPanelSelected->getContentSize());
    
    CCMenu* menu = CCMenu::createWithItem(_btnSelfPanel);
    menu->ignoreAnchorPointForPosition(false);
    this->addChild(menu);
}

void FriendsScene::create_show_view(){
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

bool FriendsScene::is_self_sid(const char *sid) {
    const char* selfSid = DATA->getLogin()->obtain_sid();
    return strcmp(selfSid, sid) == 0;
}

int FriendsScene::obtain_self_ranking() {
    int rtn = 0;
    int count = _data->count();
    if (count == 0) {
        return 1;
    }
    else {
        for (int i = 0; i < count; i++) {
            rtn += 1;
            ShowComp* show = (ShowComp*)_data->objectAtIndex(i);
            if (this->is_self_sid(show->getShowID().c_str())) {
                break;
            }
        }
        
        return rtn;
    }
}

void FriendsScene::update_self_panel(ShowComp* self) {
    if (! self) {
        return;
    }
    
    CCSize plateSize = _selfPanelNormal->getContentSize();

    const char* nickname_self = self->nickname();
    //  体力收入
    int energyCount = DATA->getSocial()->energy_could_take();
    // 自己名次
    int selfRanking = this->obtain_self_ranking();
    
    if (_nodeNormal) {
        _nodeNormal->removeAllChildrenWithCleanup(true);
        CCLabelTTF* name = CCLabelTTF::create(nickname_self, DISPLAY->fangzhengFont(), 22, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        name->setPosition(ccp(plateSize.width* .75f, plateSize.height* .68f));
        name->setColor(ccc3(109, 98, 96));
        _nodeNormal->addChild(name);
        
        CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_nor.png");
        flag->setPosition(ccp(plateSize.width* .6f, plateSize.height* .38));
        _nodeNormal->addChild(flag);
        
        CCString* collect_str = CCString::createWithFormat("%d", DATA->getShow()->collected());
        CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 20);
        cloth_count->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count->setPosition(ccp(plateSize.width * .78f, plateSize.height* .36f));
        _nodeNormal->addChild(cloth_count);
        
        // 体力
        CCString* strEnergy = CCString::createWithFormat("收到体力: %d/30", energyCount);
        CCLabelTTF* lblEnergy = CCLabelTTF::create(strEnergy->getCString(), DISPLAY->fangzhengFont(), 14.f);
        lblEnergy->setAnchorPoint(ccp(0, 0.5));
        lblEnergy->setColor(DISPLAY->defalutColor());
        lblEnergy->setPosition(ccp(plateSize.width * .22f, plateSize.height* .16f));
        _nodeNormal->addChild(lblEnergy);
        
        // 名次，借用功能
        FriendsListView::add_ranking_num((CCSprite*)_nodeNormal, selfRanking, false);
    }
    
    if (_nodeSelected) {
        _nodeSelected->removeAllChildrenWithCleanup(true);
        float scaleRate = 1.2f;
        CCLabelTTF* name = CCLabelTTF::create(nickname_self, DISPLAY->fangzhengFont(), 27, CCSizeMake(200, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        name->setPosition(ccp(_selfPanelSelected->getContentSize().width* .63f, _selfPanelSelected->getContentSize().height* .76f));
        name->setColor(ccc3(109, 98, 96));
//        name->setScale(scaleRate);
        _nodeSelected->addChild(name);
        
        CCSprite* flag = CCSprite::create("res/pic/haoyoupaihang/text_collected_sel.png");
        flag->setPosition(ccp(plateSize.width* .5f, plateSize.height* .36));
        _nodeSelected->addChild(flag);
        
        CCString* collect_str = CCString::createWithFormat("%d", DATA->getShow()->collected());
        CCLabelTTF* cloth_count = CCLabelTTF::create(collect_str->getCString(), DISPLAY->fangzhengFont(), 22);
        cloth_count->setAnchorPoint(CCPoint(0, 0.5));
        cloth_count->setPosition(ccp(_selfPanelSelected->getContentSize().width * .75f, _selfPanelSelected->getContentSize().height* .34f));
//        cloth_count->setScale(scaleRate);
        _nodeSelected->addChild(cloth_count);
        
        // 体力
        CCString* strEnergy = CCString::createWithFormat("收到体力：%d", energyCount);
        CCLabelTTF* lblEnergy = CCLabelTTF::create(strEnergy->getCString(), DISPLAY->fangzhengFont(), 14.f);
        lblEnergy->setAnchorPoint(ccp(0, 0.5));
        lblEnergy->setColor(DISPLAY->defalutColor());
        lblEnergy->setScale(scaleRate);
        lblEnergy->setPosition(ccp(plateSize.width * .12f, plateSize.height* .12f));
        _nodeSelected->addChild(lblEnergy);
        //
        CCSprite* spt1 = CCSprite::create("pic/haoyoupaihang/btn_get_tili.png");
        CCSprite* spt2 = CCSprite::create("pic/haoyoupaihang/btn_get_tili.png");
        CCSprite* spt3 = CCSprite::create("pic/haoyoupaihang/btn_get_tili.png");
        spt2->setScale(DISPLAY->btn_scale());
        spt3->setColor(ccGRAY);
        CCMenuItem* btnTake = CCMenuItemSprite::create(spt1, spt2, spt3, this, SEL_MenuHandler(&FriendsScene::on_btn_take_energy));
        CCMenu* menu = CCMenu::createWithItem(btnTake);
        menu->setPosition(ccp(plateSize.width - 55, 8));
        _nodeSelected->addChild(menu);
        if (energyCount <= 0) {
            btnTake->setEnabled(false);
        }
        
        // 名次，借用功能
        FriendsListView::add_ranking_num((CCSprite*)_nodeSelected, selfRanking, true);
    }
}

void FriendsScene::on_btn_back_to_social(CCMenuItem *menuItem) {
    AUDIO->goback_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击返回");
    num_child = 0;
    CCScene* scene = HaoyouScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void FriendsScene::on_btn_self_panel(CCMenuItemToggle *menuItem) {
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击自己");
    
    int selectedIndex = menuItem->getSelectedIndex();
    if (selectedIndex == 1) {
        _btnSelfPanel->setEnabled(false);       // 关闭按钮
        if (_listView) {
            _listView->unseleted_cells();       // 重设listview所有cell状态
        }
        ShowComp* selfShow = DATA->getShow();
        _showerView->change_shower(selfShow->ondress());

        _btnPaper->setVisible(false);           // 不能发纸条给自己
        _btnDelete->setVisible(false);
    }
    else {
        _btnSelfPanel->setEnabled(true);        // 开启按钮
        _btnPaper->setVisible(true);
        _btnDelete->setVisible(true);
    }
}

void FriendsScene::on_btn_goto_starngers(CCMenuItem* menuItem) {
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击陌生人");
    
    LOADING->show_loading();
    NET->recommend_stranger_802();
}

void FriendsScene::on_btn_take_energy(CCMenuItem *menuItem) {
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击收体力");
    
    LOADING->show_loading();
    NET->take_energy_807();
}

void FriendsScene::on_btn_send_paper(CCMenuItem *menuItem) {
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击纸条");
    
    DATA->getSocial()->setSelectedFriend(_curIndex);
    NotePanel* panel = NotePanel::create();
    panel->setEntranceType("friend");
    this->addChild(panel, 10000);
}

void FriendsScene::on_btn_delete_friend(CCMenuItem *menuItem) {
    AUDIO->common_effect();
    
//    DATA->getSocial()->setSelectedFriend(_curIndex);
    ShowComp* show = DATA->getSocial()->getSelectedFriendByIndex(_curIndex);

    CCString* msg = CCString::createWithFormat("请确认是否删除'%s'这个好友?", show->nickname());
    AHMessageBox* mb = AHMessageBox::create_with_message(msg->getCString(), this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
    mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
    

    
//    NotePanel* panel = NotePanel::create();
//    panel->setEntranceType("friend");
//    this->addChild(panel, 10000);
}


void FriendsScene::nc_change_shower(CCObject *pObj) {
    // 恢复自己的面板状态
    _btnSelfPanel->setSelectedIndex(0);
    this->on_btn_self_panel(_btnSelfPanel);
    // 换装
    CCInteger* value = (CCInteger*)pObj;
    _curIndex = value->getValue();
    CCLOG("FriendsScene::nc_change_shower() - _curIndex = %d", _curIndex);
    ShowComp* show = (ShowComp*)_data->objectAtIndex(_curIndex);
    if (_showerView) {
        _showerView->change_shower(show->ondress());
    }
    
    if (this->is_self_sid(show->getShowID().c_str())) {
        _btnPaper->setVisible(false);           // 不能发纸条给自己
        _btnDelete->setVisible(false);
    }
    else {
        _btnPaper->setVisible(true);
        _btnDelete->setVisible(true);
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

void FriendsScene::nc_send_energy_803(CCObject *pObj) {
    LOADING->remove();
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "体力赠送成功~!");
    //
    _listView->update_selected_cell();
}

void FriendsScene::nc_take_energy_807(CCObject *pObj) {
    LOADING->remove();
    this->update_self_panel(DATA->getShow());
    
    CCDictionary* info = (CCDictionary*)pObj;
    CCDictionary* dic = CCDictionary::create();
    int num = ((CCInteger*)info->objectForKey("energy"))->getValue();
    dic->setObject(CCInteger::create(num), "num");
    CCPoint from = ccp(DISPLAY->halfW() + 100, DISPLAY->H() * 0.12);
    dic->setObject(CCString::createWithFormat("{%f,%f}", from.x, from.y), "from");
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_ENERGY_FLY", dic);
}

void FriendsScene::nc_friend_break_813(CCObject* pObj) {
    LOADING->remove();
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "操作成功~!");
    _listView->reload_all();
    //
    _data = DATA->getSocial()->sortedFriends();
    if (_data->count() <= 1) {
        // 没有好友，提示去添加好友
        this->create_empty_prompt();
        // 显示自己
        _btnSelfPanel->setSelectedIndex(1);
        this->on_btn_self_panel(_btnSelfPanel);
        if (_listView) {
            _listView->removeFromParentAndCleanup(true);
        }
    }
    else {
        _curIndex = 0;
        this->nc_change_shower(CCInteger::create(0));
    }
}

void FriendsScene::keyBackClicked(){
    num_child++;
    CCLog("===== FriendsScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->on_btn_back_to_social(NULL);
}

void FriendsScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    if (button_type == AH_BUTTON_TYPE_YESNO) {
        if (button_tag == AH_BUTTON_TAG_YES) {
            // talkingData
            DATA->onEvent("点击事件", "好友界面", "删除好友");
            LOADING->show_loading();
            ShowComp* show = DATA->getSocial()->getSelectedFriendByIndex(_curIndex);
            NET->friend_break_813(show->getShowID().c_str());
        }
    }
    
    box->animation_out();
}

