//
//  NXBaseScene.cpp
//  tiegao
//
//  Created by mac on 17-6-29.
//
//

#include "NXBaseScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NetManager.h"
#include "WSManager.h"

#include "Loading2.h"
#include "SettingPanel.h"
#include "PurchasePanel.h"
#include "EnergyBuyPanel.h"
#include "CoinExchangePanel.h"
#include "ConfigManager.h"
#include "SpecialManager.h"
#include "PromptLayer.h"
#include "JNIController.h"

#include "ChatBar.h"
#include "ChatPanel.h"
#include "AppUtil.h"


NXBaseScene::~NXBaseScene(){
    
}
CCScene* NXBaseScene::scene(){
    CCScene* scene = CCScene::create();
    CCLayer* layer = NXBaseScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool NXBaseScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->init_UI();
    
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(false);
    this->setTouchEnabled(true);
    
    return true;
}

void NXBaseScene::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::updataMoney), "UpdataMoney", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::updatePhaseProgress), "UpdatePhaseProgress", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::show_purchase_panel), "HTTP_FINISHED_100", NULL);
    
    //
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::on_reset_nickname), "ON_RESET_NICKNAME", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_reset_nickname_907), "HTTP_FINISHED_907", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_take_gift_333), "HTTP_FINISHED_333", NULL);
    
    // 从外部调用，打开充值面板
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::goldCallBack), "NEED_SHOW_PURCHASEPANEL", NULL);
    // 从外部调用，打开金币兑换面板
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::coinCallBack), "NEED_SHOW_COIN_EXCHANGE", NULL);
    // 从外部调用，打开体力购买面板
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::tiliCallBack), "NEED_SHOW_BUY_ENERGY", NULL);
    //
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_need_coin_fly), "NEED_COIN_FLY", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_need_gold_fly), "NEED_GOLD_FLY", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_need_energy_fly), "NEED_ENERGY_FLY", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_need_piece_fly), "NEED_PIECE_FLY", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_coin_fly_completed), "COIN_FLY_COMPLETED", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_gold_fly_completed), "GOLD_FLY_COMPLETED", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_energy_fly_completed), "ENERGY_FLY_COMPLETED", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::nc_piece_fly_completed), "PIECE_FLY_COMPLETED", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::push_Android), "Push_Android", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::push_Android2), "Push_Android2", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::show_chat_panel), "ON_CHAT_BAR_CLICKED", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXBaseScene::on_chat_panel_close), "ON_CHAT_PANEL_CLOSE", NULL);
}

void NXBaseScene::onExitTransitionDidStart() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void NXBaseScene::onExit() {
    this->unscheduleAllSelectors();
    
    if (_isChatPanelShow) {
        DATA->getChat()->setNewChatCount(0);
    }
    
    CCLayer::onExit();
}

bool NXBaseScene::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent) {
    CCLOG("NXBaseScene::ccTouchBegan() ...");
    SPECIAL->showSpotAt(this->getParent(), pTouch->getLocation(), 1);
    return false;
}

void NXBaseScene::init_UI(){
    
    dikuangSpr = CCSprite::create("res/nxpic/NXBaseScene/NXBase_dikuang1.png");
    dikuangSpr->setAnchorPoint(ccp(.5f, 1.f));
    dikuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 20.f, DISPLAY->ScreenHeight() - 7.f));
    this->addChild(dikuangSpr, 10);
    
    // 姓名框
    CCSprite* nameSpr1 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_namekuang.png");
    CCSprite* nameSpr2 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_namekuang.png");
    nameItem = CCMenuItemSprite::create(nameSpr1, nameSpr2, this, menu_selector(NXBaseScene::nicknameCallBack));
    nameItem->setAnchorPoint(ccp(0, 1));
    nameItem->setPosition(ccp(10.f, dikuangSpr->getContentSize().height - 11.f));
    CCString* nameStr = CCString::createWithFormat("%s", DATA->getShow()->nickname());
//    CCString* nameStr = CCString::createWithFormat("ABCDEFGHIJK");
    _nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 24, CCSizeMake(170, 24), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    _nameLabel->setPosition(ccp(nameItem->getContentSize().width* .65f, nameItem->getContentSize().height* .5f));
    _nameLabel->setColor(ccWHITE);
    nameItem->addChild(_nameLabel);
    
    
    // 头像框
    CCSprite* touTempSpr1 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_toukuang1.png");
    CCSprite* touTempSpr2 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_toukuang1.png");
    touItem = CCMenuItemSprite::create(touTempSpr1, touTempSpr2, this, NULL);
    touItem->setPosition(ccp(18.f, dikuangSpr->getContentSize().height* .4f));
    
    //要显示的头像
//    CCSprite* touSpr = CCSprite::create("？？？？");
//    touSpr->setPosition(ccp(touItem->getContentSize().width* .5f, touItem->getContentSize().height* .5f));
//    touItem->addChild(touSpr);
    
    CCSprite* touSpr2 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_toukuang2.png");
    touSpr2->setPosition(ccp(touItem->getContentSize().width* .5f, touItem->getContentSize().height* .5f));
    touItem->addChild(touSpr2);
    
    // 等级框
    CCString* levelStr = CCString::createWithFormat("res/nxpic/NXBaseScene/NXBase_%d.png", DATA->getPlayer()->phase);
    CCSprite* levelSpr = CCSprite::create(levelStr->getCString());
    levelSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .03f, dikuangSpr->getContentSize().height* .04f));
    dikuangSpr->addChild(levelSpr, 15);
    
    
    // 体力框
    CCSprite* tiliKuangSpr1 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_dikuang2.png");
    CCSprite* tiliKuangSpr2 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_dikuang2.png");
    tiliItem = CCMenuItemSprite::create(tiliKuangSpr1, tiliKuangSpr2, this, NULL);
    tiliItem->setAnchorPoint(ccp(1, 1));
    tiliItem->setPosition(ccp(dikuangSpr->getContentSize().width - 13.f, dikuangSpr->getContentSize().height - 11.f));
    
    CCSprite* tiliSpr1 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_tili.png");
    tiliSpr1->setPosition(ccp(tiliItem->getContentSize().width* .09f, tiliItem->getContentSize().height* .5f));
    tiliItem->addChild(tiliSpr1);
    CCSprite* tiliAddSpr1 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_add.png");
    CCSprite* tiliAddSpr2 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_add.png");
    tiliAddSpr2->setScale(1.02f);
    CCMenuItem* tiliAddItem = CCMenuItemSprite::create(tiliAddSpr1, tiliAddSpr2, this, menu_selector(NXBaseScene::tiliCallBack));
    tiliAddItem->setPosition(ccp(tiliItem->getContentSize().width* .9f, tiliItem->getContentSize().height* .5f));
    CCMenu* tiliAddMenu = CCMenu::create(tiliAddItem, NULL);
    tiliAddMenu->setPosition(CCPointZero);
    tiliItem->addChild(tiliAddMenu);
    // tili进度
    this->updatePhaseProgress();
    
    
    // 钻石框
    CCSprite* goldSpr = CCSprite::create("res/nxpic/NXBaseScene/NXBase_zuanshi.png");
    goldSpr->setScale(1.f);
    goldSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .18f, dikuangSpr->getContentSize().height* .25f));
    dikuangSpr->addChild(goldSpr);
//    DATA->getPlayer()->diam;//钻石
//    DATA->getPlayer()->diam = 98765432;
    CCString* goldStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
    m_lbl_gold = FlashNumberLabel::create_with_atlas("res/nxpic/NXBaseScene/base_number.png", goldStr->getCString(), 0, .01f);
    m_lbl_gold->setScale(1.1f);
    m_lbl_gold->setAnchorPoint(ccp(.5f, .5f));
    m_lbl_gold->set_delegate(this); // 设置代理
    m_lbl_gold->setPosition(ccp(dikuangSpr->getContentSize().width* .32, dikuangSpr->getContentSize().height* .25f));
    dikuangSpr->addChild(m_lbl_gold);
    
    CCSprite* goldAddSpr1 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_add.png");
    goldAddSpr1->setScale(.9f);
    CCSprite* goldAddSpr2 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_add.png");
    tiliAddSpr2->setScale(.92f);
    goldItem = CCMenuItemSprite::create(goldAddSpr1, goldAddSpr2, this, menu_selector(NXBaseScene::goldCallBack));
    goldItem->setPosition(ccp(dikuangSpr->getContentSize().width* .48f, dikuangSpr->getContentSize().height* .25f));

    // 金币框
    CCSprite* coinSpr = CCSprite::create("res/nxpic/NXBaseScene/NXBase_jinbi.png");
    coinSpr->setScale(1.f);
    coinSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .6f, dikuangSpr->getContentSize().height* .25f));
    dikuangSpr->addChild(coinSpr);
//    DATA->getPlayer()->coin;//金币
//    DATA->getPlayer()->coin = 98765432;
    CCString* coinStr = CCString::createWithFormat("%d", DATA->getPlayer()->coin);
    m_lbl_coin = FlashNumberLabel::create_with_atlas("res/nxpic/NXBaseScene/base_number.png", coinStr->getCString(), 0, .01f);
    m_lbl_coin->setScale(1.1f);
    m_lbl_coin->setAnchorPoint(ccp(.5f, .5f));
    m_lbl_coin->set_delegate(this); // 设置代理
    m_lbl_coin->setPosition(ccp(dikuangSpr->getContentSize().width* .75f, dikuangSpr->getContentSize().height* .25f));
    dikuangSpr->addChild(m_lbl_coin);
    
    CCSprite* coinAddSpr1 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_add.png");
    coinAddSpr1->setScale(.9f);
    CCSprite* coinAddSpr2 = CCSprite::create("res/nxpic/NXBaseScene/NXBase_add.png");
    coinAddSpr2->setScale(.92f);
    coinItem = CCMenuItemSprite::create(coinAddSpr1, coinAddSpr2, this, menu_selector(NXBaseScene::coinCallBack));
    coinItem->setPosition(ccp(dikuangSpr->getContentSize().width* .9f, dikuangSpr->getContentSize().height* .25f));
    
    barMenu = CCMenu::create(nameItem, tiliItem, touItem, goldItem, coinItem, NULL);
    barMenu->setPosition(CCPointZero);
    dikuangSpr->addChild(barMenu, 10);
    
    
//    if (DATA->getClothesBool()) {
//        CCSprite* debrisIconSpr = CCSprite::create("res/pic/clothesScene/gj_debris.png");
//        debrisIconSpr->setScale(1.1f);
//        debrisIconSpr->setPosition(ccp(tiliItem->getContentSize().width* .04f, tiliItem->getContentSize().height* .5f));
//        tiliItem->addChild(debrisIconSpr);
//        
//        CCString* debrisStr = CCString::createWithFormat("%d", DATA->getOperation()->getPiece());
//        m_lbl_debris = FlashNumberLabel::create_with_atlas("res/pic/NXBaseScene/base_number.png", debrisStr->getCString(), 0, .01f);
//        if (DATA->getPlayer()->diam > 9999999) {
//            m_lbl_debris->setAnchorPoint(ccp(0, .5f));
//            m_lbl_debris->set_delegate(this); // 设置代理
//            m_lbl_debris->setPosition(ccp(tiliItem->getContentSize().width* .13f, tiliItem->getContentSize().height* .51f));
//        }else{
//            m_lbl_debris->setAnchorPoint(ccp(.5f, .5f));
//            m_lbl_debris->set_delegate(this); // 设置代理
//            m_lbl_debris->setPosition(ccp(tiliItem->getContentSize().width* .48f, tiliItem->getContentSize().height* .51f));
//        }
//        tiliItem->addChild(m_lbl_debris, 1);
//    }else{
//        
//        
//        tili_num = DATA->getPlayer()->energy;
//        
//        // 倒计时
//        _minute = DATA->getTiliMinute();
//        _second = DATA->getTiliSecond();
//        CCString* timeStr;
//        if (_second == 0) {
//            timeStr = CCString::createWithFormat("%d:00", _minute);
//        }else{
//            if (_second < 10) {
//                timeStr = CCString::createWithFormat("%d:0%d", _minute, _second);
//            }else{
//                timeStr = CCString::createWithFormat("%d:%d", _minute, _second);
//            }
//        }
//        m_time_num = CCLabelTTF::create(timeStr->getCString(), DISPLAY->fangzhengFont(), 20);
//        m_time_num->setPosition(ccp(tiliItem->getContentSize().width* .5f, tiliItem->getContentSize().height* .5f));
//        m_time_num->setColor(ccWHITE);
//        tiliItem->addChild(m_time_num);
//        
//        if (tili_num < def_TiliMax) {
//            m_time_num->setVisible(true);
//            
//            this->schedule(schedule_selector(NXBaseScene::updataTileTime), 1.f);
//        }else{
//            m_time_num->setVisible(false);
//            
//            this->unschedule(schedule_selector(NXBaseScene::updataTileTime));
//        }
//    }
    
    
    
    
//    // 完善资料获取钻石提示
//    if (! DATA->getPlayer()->hasCommitIdentity()) {
//        CCSprite* freeDiam = CCSprite::create("pic/NXBaseScene/free_diam.png");
//        freeDiam->setPosition(ccp(nameSpr1->getContentSize().width * 0.08, nameSpr1->getContentSize().height * 0.75));
//        freeDiam->setScale(0.8f);
//        nameItem->addChild(freeDiam, 10);
//        
//        CCSprite* freeDiam2 = AppUtil::get_self_sprite("pic/NXBaseScene/free_diam.png");
//        freeDiam->addChild(freeDiam2);
//        freeDiam2->runAction(CCRepeatForever::create(AppUtil::action_expand_fade_out(1.5f, 0.5f, 2.f)));
//        //
//        CCSprite* flash = CCSprite::create("pic/NXBaseScene/diam_flash.png");
//        flash->setPosition(ccp(nameSpr1->getContentSize().width * 0.1, nameSpr1->getContentSize().height * 0.6));
//        freeDiam->addChild(flash);
//        CCSequence* seq = CCSequence::create(CCFadeOut::create(0.8), CCDelayTime::create(0.4), CCFadeIn::create(0.8), CCDelayTime::create(0.4), NULL);
//        flash->runAction(CCRepeatForever::create(seq));
//    }
    
    // ChatBar
    _chatBar = ChatBar::create();
    _chatBar->setVisible(false);
    this->addChild(_chatBar, 100);
    
    _isChatPanelShow = false;
    _isChatBarShow = false;
}

void NXBaseScene::updataTileTime(float dt){
    if (tili_num >= def_TiliMax) {
        tili_num = def_TiliMax;
        this->unschedule(schedule_selector(NXBaseScene::updataTileTime));
        
        DATA->closeTiliTime();
        
        m_time_num->setVisible(false);
    }else{
        tili_num = DATA->getPlayer()->energy;
        CCString* str = CCString::createWithFormat("%d/%d", tili_num, def_TiliMax);
        m_tili_num->setString(str->getCString());
        
        CCString* str1;
        _minute = DATA->getTiliMinute();
        _second = DATA->getTiliSecond();
        if (_second == 0) {
            str1 = CCString::createWithFormat("%d:00", _minute);
        }else{
            if (_second < 10) {
                str1 = CCString::createWithFormat("%d:0%d", _minute, _second);
            }else{
                str1 = CCString::createWithFormat("%d:%d", _minute, _second);
            }
        }
        m_time_num->setString(str1->getCString());
    }
}

void NXBaseScene::updataMoney(){
    
    
    CCString* coinStr = CCString::createWithFormat("%d", DATA->getPlayer()->coin);
    if (m_lbl_coin != NULL) {
        m_lbl_coin->set_new_number(coinStr->getCString());
    }
    
    
    CCString* goldStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
    if (m_lbl_gold != NULL) {
        m_lbl_gold->set_new_number(goldStr->getCString());
    }
    
    
    if (DATA->getClothesBool()) {
        CCString* debrisStr = CCString::createWithFormat("%d", DATA->getOperation()->getPiece());
        if (m_lbl_debris != NULL) {
            m_lbl_debris->set_new_number(debrisStr->getCString());
        }
        
    }else{
        uint energy = DATA->getPlayer()->energy;
        tili_num = energy;
        CCString* tiliStr = CCString::createWithFormat("%d/%d", tili_num, def_TiliMax);
        if (m_tili_num != NULL) {
            m_tili_num->setString(tiliStr->getCString());
        }
        
        
        if (energy >= def_TiliMax) {
            this->unschedule(schedule_selector(NXBaseScene::updataTileTime));
            
            DATA->closeTiliTime();
            
            m_time_num->setVisible(false);
        }else{
            _minute = DATA->getTiliMinute();
            _second = DATA->getTiliSecond();
            
            this->schedule(schedule_selector(NXBaseScene::updataTileTime), 1.f);
            
            m_time_num->setVisible(true);
        }
    }
}

void NXBaseScene::hideBaseScene(){
    dikuangSpr->setVisible(false);
}

void NXBaseScene::openBaseScene(){
    dikuangSpr->setVisible(true);
}

void NXBaseScene::closeBaseMenu(){
    barMenu->setEnabled(false);
}

void NXBaseScene::nicknameCallBack(CCObject* pSender) {
    // talkingData
    DATA->onEvent("点击事件", "上导航", "点击改名与设置");
    
    int resetCost = DATA->getShow()->resetCost();
    SettingPanel* panel = NULL;
    if (0 == resetCost) {
        panel = SettingPanel::create("首次免费");
    }
    else {
        CCString* strCost = CCString::createWithFormat("%d钻石", resetCost);
        panel = SettingPanel::create(strCost->getCString());
    }
    
    if (NULL != panel) {
        this->getScene()->addChild(panel);
    }
}

void NXBaseScene::tiliCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "上导航", "点击体力");
    
    this->show_energybuy_panel();
}

void NXBaseScene::goldCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "上导航", "点击钻石");
    
    if (DATA->getPurchase()->has_init_products()) {
        this->show_purchase_panel();
    }
    else {
        LOADING->show_loading();
        NET->all_products_100();
    }
}

void NXBaseScene::coinCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "上导航", "点击金币");
    
    CoinExchangePanel* panel = CoinExchangePanel::create();
    panel->show();
}

void NXBaseScene::will_number_scroll(){
    
}

void NXBaseScene::did_number_stoped(){
    
}

void NXBaseScene::show_purchase_panel() {
    LOADING->remove();
    PurchasePanel* panel = PurchasePanel::create();
    panel->show_from(ccp(DISPLAY->W() * 0.6, DISPLAY->H() * 0.95));
}

void NXBaseScene::show_energybuy_panel() {
    EnergyBuyPanel* panel = EnergyBuyPanel::create();
    panel->show();
}

void NXBaseScene::updatePhaseProgress() {
    tili_num = DATA->getPlayer()->energy;
    
    CCSprite* bottom = CCSprite::create("res/nxpic/NXBaseScene/NXBase_bar1.png");
    bottom->setAnchorPoint(ccp(0.5, 0.5));
    bottom->setPosition(ccp(tiliItem->getContentSize().width* .49f, tiliItem->getContentSize().height* .5f));
    tiliItem->addChild(bottom);
    
    CCSprite* top = CCSprite::create("res/nxpic/NXBaseScene/NXBase_bar2.png");
    _progress = CCProgressTimer::create(top);
    _progress->setAnchorPoint(ccp(0.5, 0.5));
    _progress->setPosition(ccp(tiliItem->getContentSize().width* .49f, tiliItem->getContentSize().height* .5f));
    _progress->setType(kCCProgressTimerTypeBar);
    _progress->setMidpoint(ccp(0, 0.5));
    _progress->setBarChangeRate(ccp(1, 0));
    tiliItem->addChild(_progress);
    
    int percent = floor(100 * tili_num / def_TiliMax);
    _progress->setPercentage(percent);
}

void NXBaseScene::on_reset_nickname(CCObject *pObje) {
    LOADING->show_loading();
    NET->reset_nickname_907(((CCString*)pObje)->getCString());
}

void NXBaseScene::nc_reset_nickname_907(CCObject *pObje) {
    LOADING->remove();
    
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(this->getParent(), "改名成功~!");
    
    _nameLabel->setString(DATA->getShow()->nickname());
    this->updataMoney();
}

void NXBaseScene::nc_take_gift_333(CCObject *pObj) {
    LOADING->remove();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        int coin = ((CCInteger*)dic->objectForKey("coin"))->getValue();
        int diam = ((CCInteger*)dic->objectForKey("diam"))->getValue();
        int energy = ((CCInteger*)dic->objectForKey("energy"))->getValue();
        
        if (diam > 0) {
            CCString* diamStr = CCString::createWithFormat("礼包赠予%d钻石", diam);
            DATA->onReward(diam, diamStr->getCString());
        }
        
        CCString* str = NULL;
        if (coin > 0) {
            if (diam > 0) {
                if (energy > 0) {
                    str = CCString::createWithFormat("成功领取 %d金币、%d钻石、%d体力~!", coin, diam, energy);
                }
                else {
                    str = CCString::createWithFormat("成功领取 %d金币、%d钻石~!", coin, diam);
                }
            }
            else {
                str = CCString::createWithFormat("成功领取 %d金币~!", coin);
            }
        }
        else if (diam > 0) {
            if (energy > 0) {
                str = CCString::createWithFormat("成功领取 %d钻石、%d体力~!", diam, energy);
            }
            else {
                str = CCString::createWithFormat("成功领取 %d钻石~!", diam);
            }
        }
        
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(this->getScene(), str->getCString());
    }
}

void NXBaseScene::nc_need_coin_fly(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        CCInteger* num = (CCInteger*)dic->objectForKey("num");
        CCString* from = (CCString*)dic->objectForKey("from");
        CCPoint end = coinItem->getPosition() + DISPLAY->center() - ccp(60, 50);
        SPECIAL->show_coin_reward(this->getScene(), num->getValue(), CCPointFromString(from->getCString()), end);
    }
}

void NXBaseScene::nc_need_gold_fly(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        CCInteger* num = (CCInteger*)dic->objectForKey("num");
        CCString* from = (CCString*)dic->objectForKey("from");
        CCPoint end = goldItem->getPosition() + DISPLAY->center() - ccp(60, 50);
        SPECIAL->show_gold_reward(this->getScene(), num->getValue(), CCPointFromString(from->getCString()), end);
    }
}

void NXBaseScene::nc_need_energy_fly(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        CCInteger* num = (CCInteger*)dic->objectForKey("num");
        CCString* from = (CCString*)dic->objectForKey("from");
        CCPoint end = tiliItem->getPosition() + DISPLAY->center() - ccp(60, 50);
        SPECIAL->show_energy_reward(this->getScene(), num->getValue(), CCPointFromString(from->getCString()), end);
    }
}

void NXBaseScene::nc_need_piece_fly(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        CCInteger* num = (CCInteger*)dic->objectForKey("num");
        CCString* from = (CCString*)dic->objectForKey("from");
        CCPoint end = nameItem->getPosition() + DISPLAY->center() - ccp(60, 50);
        SPECIAL->show_piece_reward(this->getScene(), num->getValue(), CCPointFromString(from->getCString()), end);
    }
}

void NXBaseScene::nc_coin_fly_completed(CCObject *pObj) {
    updataMoney();
}

void NXBaseScene::nc_gold_fly_completed(CCObject *pObj) {
    updataMoney();
}

void NXBaseScene::nc_energy_fly_completed(CCObject *pObj) {
    updataMoney();
}

void NXBaseScene::nc_piece_fly_completed(CCObject *pObj) {
    
}

void NXBaseScene::push_Android(CCObject* pObj){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JNIController::closePush_Android(1);
    
    CCDictionary* dic = (CCDictionary* )pObj;
    CCString* name = (CCString*)dic->valueForKey("name");
    CCInteger* num = (CCInteger*)dic->objectForKey("num");
    CCInteger* index = CCInteger::create(1);
    
    CCLog("str == %s, num == %d, index == %d", name->getCString(), num->getValue(), index->getValue());
    
    JNIController::push_Android(name->getCString(), num->getValue(), index->getValue());
#endif
}
void NXBaseScene::push_Android2(CCObject* pObj){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCDictionary* dic = (CCDictionary* )pObj;
    CCString* name = (CCString*)dic->valueForKey("name");
    CCInteger* num = (CCInteger*)dic->objectForKey("num");
    CCInteger* index = CCInteger::create(3);
    
    CCLog("str == %s, num == %d, index == %d", name->getCString(), num->getValue(), index->getValue());
    
    JNIController::push_Android(name->getCString(), num->getValue(), index->getValue());
#endif
}


#pragma mark - ChatBar

void NXBaseScene::openChat(bool isOpen) {
    DATA->setChatOut(false);
    if (WS->isConnected()) {
        
    }else{
        // talkingData
        DATA->onEvent("连接事件", "主界面", "连接聊天服务器");
        WS->connect();
    }
    
    _chatBar->isTouchOpen(isOpen);
    
    //    _chatMenu->setVisible(true);
    
    this->unschedule(SEL_SCHEDULE(&NXBaseScene::check_new_chat));
    this->schedule(SEL_SCHEDULE(&NXBaseScene::check_new_chat), 0.5);
}

void NXBaseScene::show_chat_bar() {
    _isChatBarShow = true;
    _chatBar->update_display();
    _chatBar->setVisible(true);
    _isChatPanelShow = false;
}

void NXBaseScene::show_chat_panel() {
    if (! _isChatPanelShow) {
        _chatBar->setVisible(false);
        _isChatBarShow = false;
        _isChatPanelShow = true;
        //
        ChatPanel* panel = ChatPanel::create(true);
        this->addChild(panel, 101);
    }
}

void NXBaseScene::check_new_chat() {
    //    static int oldNum = 0;
    int newNum = DATA->getChat()->getNewShoutCount() - DATA->getChat()->getShoutCursor();
    //    CCLOG("NXBaseScene::check_new_chat() -- newNum = %d", newNum);
    
    if (newNum > 0 && ! _isChatPanelShow) {
        if (! _isChatBarShow) {
            this->show_chat_bar();
        }
    }
    
    _chatBar->update_num(newNum);
    
    //    if (oldNum != newNum ) {
    //        _chatBar->update_num(newNum);
    //        oldNum = newNum;
    //    }
}

void NXBaseScene::on_chat_panel_close() {
    _isChatPanelShow = false;
    DATA->getChat()->setNewChatCount(0);
}