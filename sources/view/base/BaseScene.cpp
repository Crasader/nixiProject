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
#include "Loading.h"
#include "PurchasePanel.h"

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
    
    return true;
}

void BaseScene::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::updataMoney), "UpdataMoney", NULL);
    nc->addObserver(this, SEL_CallFuncO(&BaseScene::show_purchase_panel), "HTTP_FINISHED_100", NULL);
}
void BaseScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCLayer::onExit();
}

void BaseScene::init_UI(){
        
    // 姓名框
    CCSprite* nameSpr1 = CCSprite::create("res/pic/baseScene/base_name_bar.png");
    CCSprite* nameSpr2 = CCSprite::create("res/pic/baseScene/base_name_bar.png");
    nameItem = CCMenuItemSprite::create(nameSpr1, nameSpr2);
    if ((DISPLAY->ScreenWidth() - 640) == 0) {
        nameItem->setPosition(ccp(DISPLAY->ScreenWidth()* .115f, DISPLAY->ScreenHeight()* .97f));
    }else{
        nameItem->setPosition(ccp(DISPLAY->ScreenWidth()* .1f, DISPLAY->ScreenHeight()* .97f));
    }
    
    CCLabelTTF* nameLabel = CCLabelTTF::create(DATA->getShow()->nickname(), DISPLAY->fangzhengFont(), 18, CCSizeMake(130, 18), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel->setPosition(ccp(nameItem->getContentSize().width* .55f, nameItem->getContentSize().height* .5f));
    nameLabel->setColor(ccc3(113, 89, 102));
    nameItem->addChild(nameLabel);
    
    
    // 体力框
    CCSprite* tiliSpr1 = CCSprite::create("res/pic/baseScene/base_bar.png");
    CCSprite* tiliSpr2 = CCSprite::create("res/pic/baseScene/base_bar.png");
    tiliItem = CCMenuItemSprite::create(tiliSpr1, tiliSpr2, this, menu_selector(BaseScene::tiliCallBack));
    if ((DISPLAY->ScreenWidth() - 640) == 0) {
        tiliItem->setPosition(ccp(DISPLAY->ScreenWidth()* .38f, DISPLAY->ScreenHeight()* .965f));
    }else{
        tiliItem->setPosition(ccp(DISPLAY->ScreenWidth()* .48f, DISPLAY->ScreenHeight()* .965f));
    }
    CCSprite* tiliIconSpr = CCSprite::create("res/pic/clothesScene/gj_xin.png");
    tiliIconSpr->setScale(1.1f);
    tiliIconSpr->setPosition(ccp(tiliItem->getContentSize().width* .04f, tiliItem->getContentSize().height* .5f));
    tiliItem->addChild(tiliIconSpr);

    CCString* energyStr = CCString::createWithFormat("%d/100", DATA->getPlayer()->energy);
    CCLabelTTF* energyLabel = CCLabelTTF::create(energyStr->getCString(), DISPLAY->fangzhengFont(), 22, CCSizeMake(110, 22), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    energyLabel->setPosition(ccp(tiliItem->getContentSize().width* .48f, tiliItem->getContentSize().height* .5f));
    energyLabel->setColor(ccc3(113, 89, 102));
    tiliItem->addChild(energyLabel);
    
    // 钻石框
    CCSprite* goldSpr1 = CCSprite::create("res/pic/baseScene/base_bar.png");
    CCSprite* goldSpr2 = CCSprite::create("res/pic/baseScene/base_bar.png");
    goldSpr2->setScale(DISPLAY->btn_scale());
    goldItem = CCMenuItemSprite::create(goldSpr1, goldSpr2, this, menu_selector(BaseScene::goldCallBack));
    if ((DISPLAY->ScreenWidth() - 640) == 0) {
        goldItem->setPosition(ccp(DISPLAY->ScreenWidth()* .63f, DISPLAY->ScreenHeight()* .965f));
    }else{
        goldItem->setPosition(ccp(DISPLAY->ScreenWidth()* .69f, DISPLAY->ScreenHeight()* .965f));
    }
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
    if ((DISPLAY->ScreenWidth() - 640) == 0) {
        coinItem->setPosition(ccp(DISPLAY->ScreenWidth()* .88f, DISPLAY->ScreenHeight()* .965f));
    }else{
        coinItem->setPosition(ccp(DISPLAY->ScreenWidth()* .9f, DISPLAY->ScreenHeight()* .965f));
    }
    CCSprite* coinIconSpr = CCSprite::create("res/pic/clothesScene/gj_coin.png");
    coinIconSpr->setPosition(ccp(coinItem->getContentSize().width* .04f, coinItem->getContentSize().height* .54f));
    coinItem->addChild(coinIconSpr);
//    DATA->getPlayer()->coin;//金币
//    DATA->getPlayer()->coin = 98765432;
    CCString* coinStr = CCString::createWithFormat("%d", DATA->getPlayer()->coin);
    m_lbl_coin = FlashNumberLabel1::create_with_atlas("res/pic/baseScene/base_number.png", coinStr->getCString(), 0, .01f);
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
    barMenu->setPosition(CCPointZero);
    this->addChild(barMenu, 10);
}
void BaseScene::updataMoney(){
    CCString* coinStr = CCString::createWithFormat("%d", DATA->getPlayer()->coin);
    m_lbl_coin->set_new_number(coinStr->getCString());
    
    CCString* goldStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
    m_lbl_gold->set_new_number2(goldStr->getCString());
}
void BaseScene::hideBaseScene(){
    barMenu->setVisible(false);
}

void BaseScene::tiliCallBack(CCObject* pSender){
    
}

void BaseScene::goldCallBack(CCObject* pSender){
    if (DATA->getIAP()->has_init_products()) {
        this->show_purchase_panel();
    }
    else {
        LOADING->show_loading();
        NET->all_products_100();
    }
}

void BaseScene::coinCallBack(CCObject* pSender){
    
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
    PurchasePanel* panel = PurchasePanel::create();
    panel->show_from(goldItem->getPosition());
}








