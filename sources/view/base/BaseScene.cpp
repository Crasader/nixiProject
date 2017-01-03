//
//  BaseScene.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "BaseScene.h"
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


BaseScene::~BaseScene(){
    
}
CCScene* BaseScene::scene(){
    CCScene* scene = CCScene::create();
    CCLayer* layer = BaseScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool BaseScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->init_UI();
    
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(false);
    this->setTouchEnabled(true);
    
    return true;
}

void BaseScene::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::updataMoney), "UpdataMoney", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::updatePhaseProgress), "UpdatePhaseProgress", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::show_purchase_panel), "HTTP_FINISHED_100", NULL);
    
    //
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::on_reset_nickname), "ON_RESET_NICKNAME", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_reset_nickname_907), "HTTP_FINISHED_907", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_take_gift_333), "HTTP_FINISHED_333", NULL);
    
    // 从外部调用，打开充值面板
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::goldCallBack), "NEED_SHOW_PURCHASEPANEL", NULL);
    // 从外部调用，打开金币兑换面板
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::coinCallBack), "NEED_SHOW_COIN_EXCHANGE", NULL);
    // 从外部调用，打开体力购买面板
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::tiliCallBack), "NEED_SHOW_BUY_ENERGY", NULL);
    //
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_need_coin_fly), "NEED_COIN_FLY", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_need_gold_fly), "NEED_GOLD_FLY", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_need_energy_fly), "NEED_ENERGY_FLY", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_need_piece_fly), "NEED_PIECE_FLY", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_coin_fly_completed), "COIN_FLY_COMPLETED", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_gold_fly_completed), "GOLD_FLY_COMPLETED", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_energy_fly_completed), "ENERGY_FLY_COMPLETED", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::nc_piece_fly_completed), "PIECE_FLY_COMPLETED", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::push_Android), "Push_Android", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::show_chat_panel), "ON_CHAT_BAR_CLICKED", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::on_chat_panel_close), "ON_CHAT_PANEL_CLOSE", NULL);
}

void BaseScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    if (_isChatPanelShow) {
        DATA->getChat()->setNewChatCount(0);
    }
    
    CCLayer::onExit();
}

bool BaseScene::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent) {
    CCLOG("BaseScene::ccTouchBegan() ...");
    SPECIAL->showSpotAt(this->getParent(), pTouch->getLocation(), 1);
    return false;
}

void BaseScene::init_UI(){
    // 姓名框
    CCSprite* nameSpr1 = CCSprite::create("res/pic/baseScene/base_name_bar.png");
    CCSprite* nameSpr2 = CCSprite::create("res/pic/baseScene/base_name_bar.png");
    nameItem = CCMenuItemSprite::create(nameSpr1, nameSpr2, this, menu_selector(BaseScene::nicknameCallBack));
    nameItem->setAnchorPoint(ccp(0, 1));
    nameItem->setPosition(ccp(-DISPLAY->ScreenWidth()* .5f + 2, DISPLAY->ScreenHeight()* .5f - 5));
    CCString* nameStr = CCString::createWithFormat("%s", DATA->getShow()->nickname());
    _nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(130, 18), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    _nameLabel->setPosition(ccp(nameItem->getContentSize().width* .545f, nameItem->getContentSize().height* .5f));
    _nameLabel->setColor(ccc3(113, 89, 102));
    nameItem->addChild(_nameLabel);
    
    
    // 体力框
    if (DATA->getClothesBool()) {
        CCSprite* debrisSpr1 = CCSprite::create("res/pic/baseScene/base_bar.png");
        CCSprite* debrisSpr2 = CCSprite::create("res/pic/baseScene/base_bar.png");
        tiliItem = CCMenuItemSprite::create(debrisSpr1, debrisSpr2, this, NULL);
        tiliItem->setAnchorPoint(ccp(1, 1));
        tiliItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f - 2, DISPLAY->ScreenHeight()* .5f - 5));
        CCSprite* debrisIconSpr = CCSprite::create("res/pic/clothesScene/gj_debris.png");
        debrisIconSpr->setScale(1.1f);
        debrisIconSpr->setPosition(ccp(tiliItem->getContentSize().width* .04f, tiliItem->getContentSize().height* .5f));
        tiliItem->addChild(debrisIconSpr);
        
        
        CCString* debrisStr = CCString::createWithFormat("%d", DATA->getOperation()->getPiece());
        m_lbl_debris = FlashNumberLabel3::create_with_atlas3("res/pic/baseScene/base_number.png", debrisStr->getCString(), 0, .01f);
        if (DATA->getPlayer()->diam > 9999999) {
            m_lbl_debris->setAnchorPoint(ccp(0, .5f));
            m_lbl_debris->set_delegate3(this); // 设置代理
            m_lbl_debris->setPosition(ccp(tiliItem->getContentSize().width* .13f, tiliItem->getContentSize().height* .51f));
        }else{
            m_lbl_debris->setAnchorPoint(ccp(.5f, .5f));
            m_lbl_debris->set_delegate3(this); // 设置代理
            m_lbl_debris->setPosition(ccp(tiliItem->getContentSize().width* .48f, tiliItem->getContentSize().height* .51f));
        }
        tiliItem->addChild(m_lbl_debris, 1);
    }else{
        CCSprite* tiliSpr1 = CCSprite::create("res/pic/baseScene/base_bar.png");
        CCSprite* tiliSpr2 = CCSprite::create("res/pic/baseScene/base_bar.png");
        tiliItem = CCMenuItemSprite::create(tiliSpr1, tiliSpr2, this, menu_selector(BaseScene::tiliCallBack));
        tiliItem->setAnchorPoint(ccp(1, 1));
        tiliItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f - 2, DISPLAY->ScreenHeight()* .5f - 5));
        CCSprite* tiliIconSpr = CCSprite::create("res/pic/clothesScene/gj_xin.png");
        tiliIconSpr->setScale(1.1f);
        tiliIconSpr->setPosition(ccp(tiliItem->getContentSize().width* .04f, tiliItem->getContentSize().height* .5f));
        tiliItem->addChild(tiliIconSpr);
        
        tili_num = DATA->getPlayer()->energy;
        CCString* tiliStr = CCString::createWithFormat("%d/%d", tili_num, def_TiliMax);
        m_tili_num = CCLabelTTF::create(tiliStr->getCString(), DISPLAY->fangzhengFont(), 22, CCSizeMake(110, 22), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
        m_tili_num->setPosition(ccp(tiliItem->getContentSize().width* .48f, tiliItem->getContentSize().height* .5f));
        m_tili_num->setColor(ccc3(113, 89, 102));
        tiliItem->addChild(m_tili_num);
        // 倒计时
        timeKuangSpr = CCSprite::create("res/pic/baseScene/base_timekuang.png");
        timeKuangSpr->setPosition(ccp(tiliItem->getContentSize().width* .65f, -13));
        tiliItem->addChild(timeKuangSpr);
        
        _minute = DATA->getTiliMinute();
        _second = DATA->getTiliSecond();
        CCString* timeStr;
        if (_second == 0) {
            timeStr = CCString::createWithFormat("%d:00", _minute);
        }else{
            if (_second < 10) {
                timeStr = CCString::createWithFormat("%d:0%d", _minute, _second);
            }else{
                timeStr = CCString::createWithFormat("%d:%d", _minute, _second);
            }
        }
        m_time_num = CCLabelTTF::create(timeStr->getCString(), DISPLAY->fangzhengFont(), 20);
        m_time_num->setPosition(ccp(timeKuangSpr->getContentSize().width* .5f, timeKuangSpr->getContentSize().height* .5f));
        m_time_num->setColor(ccWHITE);
        timeKuangSpr->addChild(m_time_num);
        
        if (tili_num < def_TiliMax) {
            m_time_num->setVisible(true);
            timeKuangSpr->setVisible(true);
            
            this->schedule(schedule_selector(BaseScene::updataTileTime), 1.f);
        }else{
            m_time_num->setVisible(false);
            timeKuangSpr->setVisible(false);
            
            this->unschedule(schedule_selector(BaseScene::updataTileTime));
        }
    }
    
    
    // 钻石框
    CCSprite* goldSpr1 = CCSprite::create("res/pic/baseScene/base_bar.png");
    CCSprite* goldSpr2 = CCSprite::create("res/pic/baseScene/base_bar.png");
//    goldSpr2->setScale(DISPLAY->btn_scale());
    goldItem = CCMenuItemSprite::create(goldSpr1, goldSpr2, this, menu_selector(BaseScene::goldCallBack));
    goldItem->setAnchorPoint(ccp(1, 1));
    goldItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f - 2 - tiliItem->getContentSize().width* 1.08f, DISPLAY->ScreenHeight()* .5f - 5));
    CCSprite* goldIconSpr = CCSprite::create("res/pic/clothesScene/gj_gold.png");
    goldIconSpr->setPosition(ccp(goldItem->getContentSize().width* .04f, goldItem->getContentSize().height* .52f));
    goldItem->addChild(goldIconSpr);
//    DATA->getPlayer()->diam;//钻石
//    DATA->getPlayer()->diam = 98765432;
    CCString* goldStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
    m_lbl_gold = FlashNumberLabel2::create_with_atlas2("res/pic/baseScene/base_number.png", goldStr->getCString(), 0, .01f);
    if (DATA->getPlayer()->diam > 9999999) {
        m_lbl_gold->setAnchorPoint(ccp(0, .5f));
        m_lbl_gold->set_delegate2(this); // 设置代理
        m_lbl_gold->setPosition(ccp(goldItem->getContentSize().width* .13f, goldItem->getContentSize().height* .51f));
    }else{
        m_lbl_gold->setAnchorPoint(ccp(.5f, .5f));
        m_lbl_gold->set_delegate2(this); // 设置代理
        m_lbl_gold->setPosition(ccp(goldItem->getContentSize().width* .48f, goldItem->getContentSize().height* .51f));
    }
    goldItem->addChild(m_lbl_gold, 1);
    
    // 金币框
    CCSprite* coinSpr1 = CCSprite::create("res/pic/baseScene/base_bar.png");
    CCSprite* coinSpr2 = CCSprite::create("res/pic/baseScene/base_bar.png");
    coinItem = CCMenuItemSprite::create(coinSpr1, coinSpr2, this, menu_selector(BaseScene::coinCallBack));
    coinItem->setAnchorPoint(ccp(1, 1));
    coinItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f - 2 - tiliItem->getContentSize().width* 1.08f - goldItem->getContentSize().width* 1.08f, DISPLAY->ScreenHeight()* .5f - 5));
    CCSprite* coinIconSpr = CCSprite::create("res/pic/clothesScene/gj_coin.png");
    coinIconSpr->setPosition(ccp(coinItem->getContentSize().width* .04f, coinItem->getContentSize().height* .54f));
    coinItem->addChild(coinIconSpr);
//    DATA->getPlayer()->coin;//金币
//    DATA->getPlayer()->coin = 98765432;
    CCString* coinStr = CCString::createWithFormat("%d", DATA->getPlayer()->coin);
    m_lbl_coin = FlashNumberLabel::create_with_atlas("res/pic/baseScene/base_number.png", coinStr->getCString(), 0, .01f);
    if (DATA->getPlayer()->coin > 9999999) {
        m_lbl_coin->setAnchorPoint(ccp(0, .5f));
        m_lbl_coin->set_delegate(this); // 设置代理
        m_lbl_coin->setPosition(ccp(coinItem->getContentSize().width* .13f, coinItem->getContentSize().height* .51f));
    }else{
        m_lbl_coin->setAnchorPoint(ccp(.5f, .5f));
        m_lbl_coin->set_delegate(this); // 设置代理
        m_lbl_coin->setPosition(ccp(coinItem->getContentSize().width* .48f, coinItem->getContentSize().height* .51f));
    }
    coinItem->addChild(m_lbl_coin, 1);
    
    barMenu = CCMenu::create(nameItem, tiliItem, goldItem, coinItem, NULL);
    this->addChild(barMenu, 10);
    
    // 公司等级进度
    _phaseStar = CCSprite::create("res/pic/baseScene/base_phase.png");
    _phaseStar->setPosition(ccp(-10, 4));
    coinItem->addChild(_phaseStar, 10);

    this->updatePhaseProgress();
    
    
//    // Chat button
//    CCSprite* qipao = CCSprite::create("res/pic/panel/chat/qipao.png");
//    CCSprite* qipao2 = CCSprite::create("res/pic/panel/chat/qipao.png");
//    qipao2->setScale(1.02f);
//    CCMenuItemSprite* item_chat = CCMenuItemSprite::create(qipao, qipao2, this, menu_selector(BaseScene::show_chat_panel));
//    item_chat->setPosition(ccp(DISPLAY->ScreenWidth()* .075f, DISPLAY->ScreenHeight()* .19f));
//    _chatMenu = CCMenu::createWithItem(item_chat);
//    _chatMenu->setPosition(CCPointZero);
//    this->addChild(_chatMenu, 100);
//    _chatMenu->setVisible(false);
    
    
    // ChatBar
    _chatBar = ChatBar::create();
    _chatBar->setVisible(false);
    this->addChild(_chatBar, 100);
    
    _isChatPanelShow = false;
    _isChatBarShow = false;
}

void BaseScene::updataTileTime(float dt){
    if (tili_num >= def_TiliMax) {
        tili_num = def_TiliMax;
        this->unschedule(schedule_selector(BaseScene::updataTileTime));
        
        DATA->closeTiliTime();
        
        m_time_num->setVisible(false);
        timeKuangSpr->setVisible(false);
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

void BaseScene::updataMoney(){
    
    
    CCString* coinStr = CCString::createWithFormat("%d", DATA->getPlayer()->coin);
    m_lbl_coin->set_new_number(coinStr->getCString());
    
    CCString* goldStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
    m_lbl_gold->set_new_number2(goldStr->getCString());
    
    if (DATA->getClothesBool()) {
        CCString* debrisStr = CCString::createWithFormat("%d", DATA->getOperation()->getPiece());
        m_lbl_debris->set_new_number3(debrisStr->getCString());
    }else{
        uint energy = DATA->getPlayer()->energy;
        tili_num = energy;
        CCString* tiliStr = CCString::createWithFormat("%d/%d", tili_num, def_TiliMax);
        m_tili_num->setString(tiliStr->getCString());
        
        if (energy >= def_TiliMax) {
            this->unschedule(schedule_selector(BaseScene::updataTileTime));
            
            DATA->closeTiliTime();
            
            m_time_num->setVisible(false);
            timeKuangSpr->setVisible(false);
        }else{
            _minute = DATA->getTiliMinute();
            _second = DATA->getTiliSecond();
            
            this->schedule(schedule_selector(BaseScene::updataTileTime), 1.f);
            
            m_time_num->setVisible(true);
            timeKuangSpr->setVisible(true);
        }
    }
}

void BaseScene::hideBaseScene(){
    barMenu->setVisible(false);
    _phaseStar->setVisible(false);
}

void BaseScene::openBaseScene(){
    barMenu->setVisible(true);
    _phaseStar->setVisible(true);
}

void BaseScene::closeBaseMenu(){
    barMenu->setEnabled(false);
}

void BaseScene::nicknameCallBack(CCObject* pSender) {
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

void BaseScene::tiliCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "上导航", "点击体力");
    
    this->show_energybuy_panel();
}

void BaseScene::goldCallBack(CCObject* pSender){
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

void BaseScene::coinCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "上导航", "点击金币");
    
    CoinExchangePanel* panel = CoinExchangePanel::create();
    panel->show();
}

void BaseScene::will_number_scroll(){
    
}

void BaseScene::did_number_stoped(){
    
}

void BaseScene::will_number_scroll2(){
    
}

void BaseScene::did_number_stoped2(){
    
}

void BaseScene::will_number_scroll3(){
    
}

void BaseScene::did_number_stoped3(){
    
}

void BaseScene::show_purchase_panel() {
    LOADING->remove();
    PurchasePanel* panel = PurchasePanel::create();
    panel->show_from(ccp(DISPLAY->W() * 0.6, DISPLAY->H() * 0.95));
}

void BaseScene::show_energybuy_panel() {
    EnergyBuyPanel* panel = EnergyBuyPanel::create();
    panel->show();
}

void BaseScene::updatePhaseProgress() {
    if (! _phaseStar) {
        return;
    }
    
    _phaseStar->removeAllChildrenWithCleanup(true);
    int phase = DATA->getPlayer()->phase;

    CCLabelAtlas* phaseNum = CCLabelAtlas::create(CCString::createWithFormat("%d", phase)->getCString(), "res/pic/baseScene/base_number2.png", 13.9, 17, '0');
    phaseNum->setPosition(ccp(10, 3));
    _phaseStar->addChild(phaseNum);
    
    CCSprite* bottom = CCSprite::create("res/pic/baseScene/base_exp_bar_bottom.png");
    bottom->setAnchorPoint(ccp(0, 0.5));
    bottom->setPosition(ccp(22, 10));
    _phaseStar->addChild(bottom);
    
    CCSprite* top = CCSprite::create("res/pic/baseScene/base_exp_bar_top.png");
    _progress = CCProgressTimer::create(top);
    _progress->setAnchorPoint(ccp(0, 0.5));
    _progress->setPosition(ccp(22, 10));
    _progress->setType(kCCProgressTimerTypeBar);
    _progress->setMidpoint(ccp(0, 0.5));
    _progress->setBarChangeRate(ccp(1, 0));
    _phaseStar->addChild(_progress);
    
    int curMissionIndex = CONFIG->mission_index_in_phase(DATA->getPlayer()->mission, phase);
    int phaseTotalMissionCount = CONFIG->mission_count(phase);
    int percent = floor(100 * curMissionIndex / phaseTotalMissionCount);
    _progress->setPercentage(percent);
}

void BaseScene::on_reset_nickname(CCObject *pObje) {
    LOADING->show_loading();
    NET->reset_nickname_907(((CCString*)pObje)->getCString());
}

void BaseScene::nc_reset_nickname_907(CCObject *pObje) {
    LOADING->remove();
    
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(this->getParent(), "改名成功~!");
    
    _nameLabel->setString(DATA->getShow()->nickname());
    this->updataMoney();
}

void BaseScene::nc_take_gift_333(CCObject *pObj) {
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

void BaseScene::nc_need_coin_fly(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        CCInteger* num = (CCInteger*)dic->objectForKey("num");
        CCString* from = (CCString*)dic->objectForKey("from");
        CCPoint end = coinItem->getPosition() + DISPLAY->center() - ccp(60, 50);
        SPECIAL->show_coin_reward(this->getScene(), num->getValue(), CCPointFromString(from->getCString()), end);
    }
}

void BaseScene::nc_need_gold_fly(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        CCInteger* num = (CCInteger*)dic->objectForKey("num");
        CCString* from = (CCString*)dic->objectForKey("from");
        CCPoint end = goldItem->getPosition() + DISPLAY->center() - ccp(60, 50);
        SPECIAL->show_gold_reward(this->getScene(), num->getValue(), CCPointFromString(from->getCString()), end);
    }
}

void BaseScene::nc_need_energy_fly(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        CCInteger* num = (CCInteger*)dic->objectForKey("num");
        CCString* from = (CCString*)dic->objectForKey("from");
        CCPoint end = tiliItem->getPosition() + DISPLAY->center() - ccp(60, 50);
        SPECIAL->show_energy_reward(this->getScene(), num->getValue(), CCPointFromString(from->getCString()), end);
    }
}

void BaseScene::nc_need_piece_fly(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary*)pObj;
    if (dic) {
        CCInteger* num = (CCInteger*)dic->objectForKey("num");
        CCString* from = (CCString*)dic->objectForKey("from");
        CCPoint end = nameItem->getPosition() + DISPLAY->center() - ccp(60, 50);
        SPECIAL->show_piece_reward(this->getScene(), num->getValue(), CCPointFromString(from->getCString()), end);
    }
}

void BaseScene::nc_coin_fly_completed(CCObject *pObj) {
    updataMoney();
}

void BaseScene::nc_gold_fly_completed(CCObject *pObj) {
    updataMoney();
}

void BaseScene::nc_energy_fly_completed(CCObject *pObj) {
    updataMoney();
}

void BaseScene::nc_piece_fly_completed(CCObject *pObj) {

}

void BaseScene::push_Android(CCObject* pObj){
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


#pragma mark - ChatBar

void BaseScene::openChat() {
    DATA->setChatOut(false);
    if (WS->isConnected()) {
        
    }else{
        // talkingData
        DATA->onEvent("连接事件", "主界面", "连接聊天服务器");
        WS->connect();
    }
    
//    _chatMenu->setVisible(true);
    
    this->unschedule(SEL_SCHEDULE(&BaseScene::check_new_chat));
    this->schedule(SEL_SCHEDULE(&BaseScene::check_new_chat), 0.5);
}

void BaseScene::show_chat_bar() {
    _isChatBarShow = true;
    _chatBar->update_display();
    _chatBar->setVisible(true);
    _isChatPanelShow = false;
}

void BaseScene::show_chat_panel() {
    if (! _isChatPanelShow) {
        _chatBar->setVisible(false);
        _isChatBarShow = false;
        _isChatPanelShow = true;
        //
        ChatPanel* panel = ChatPanel::create(true);
        this->addChild(panel, 101);
    }
}

void BaseScene::check_new_chat() {
    static int oldNum = 0;
    int newNum = DATA->getChat()->getNewChatCount();
    
    if (newNum > 0 && ! _isChatPanelShow) {
        if (! _isChatBarShow) {
            this->show_chat_bar();
        }
    }
    
    if (oldNum != newNum) {
        _chatBar->update_num(newNum);
        oldNum = newNum;
    }
}

void BaseScene::on_chat_panel_close() {
    _isChatPanelShow = false;
    DATA->getChat()->setNewChatCount(0);
}


