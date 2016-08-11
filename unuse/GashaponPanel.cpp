//
//  GashaponPanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "GashaponPanel.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "TransactionScene.h"
#include "PromptLayer.h"
#include "Loading2.h"

#pragma mark - Export

void GashaponPanel::show() {
    GashaponPanel* panel = GashaponPanel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}

#pragma mark - Supper

GashaponPanel::~GashaponPanel() {
    
}

bool GashaponPanel::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("res/pic/panel/gashapon/gashapon_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        _content->addChild(txt_close);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        this->update_content();
        
        return true;
    }
    else {
        return false;
    }
}

void GashaponPanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&GashaponPanel::nc_take_energy_301), "HTTP_FINISHED_301", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&GashaponPanel::keyBackStatus), .8f);
}

void GashaponPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void GashaponPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

bool GashaponPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        remove();
    }
    
    return true;
}

#pragma mark - inner

void GashaponPanel::update_content() {
    _panel->removeAllChildrenWithCleanup(true);
    CCSize panelSize = _panel->getContentSize();
    
    CCArray* templates = DATA->getOperation()->getGashaponTemplate();
    int count = templates->count();
    for (int i = 0; i < count; i++) {
        CCSprite* sptPlate = CCSprite::create("pic/panel/gashapon/gashapon_plate.png");
        float width = panelSize.width * 0.5 + panelSize.width * 0.28 * (i % 3 - 1);
        float height = panelSize.height * (0.82 - i / 3 * 0.24);
        sptPlate->setPosition(ccp(width, height));
        _panel->addChild(sptPlate);
        // 底框
        CCDictionary* item = (CCDictionary*)templates->objectAtIndex(i);
        string uri = item->valueForKey("uri")->getCString();
        CCString* filePath = DATA->clothes_icon_path_with_uri(uri);
        CCSprite* icon = CCSprite::create(filePath->getCString());
        icon->setScale(0.6);
        icon->setPosition(ccp(sptPlate->getContentSize().width * 0.5, sptPlate->getContentSize().height * 0.5));
        sptPlate->addChild(icon);
        // price
        CCSprite* sptPrice1 = CCSprite::create("pic/panel/gashapon/gashapon_price.png");
        CCSprite* sptPrice2 = CCSprite::create("pic/panel/gashapon/gashapon_price.png");
        sptPrice2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btnExcharge = CCMenuItemSprite::create(sptPrice1, sptPrice2);
        CCMenu* menu = CCMenu::createWithItem(btnExcharge);
        menu->setPosition(ccp(sptPlate->getContentSize().width * 0.5, -10));
        sptPlate->addChild(menu);
        
        CCInteger* price = (CCInteger*)item->objectForKey("price");
        CCString* strPrice = CCString::createWithFormat("%d", price->getValue());
        CCLabelTTF* lbl1 = CCLabelTTF::create(strPrice->getCString(), DISPLAY->fangzhengFont(), 16.f);
        lbl1->setPosition(ccp(sptPrice1->getContentSize().width * 0.6, sptPrice1->getContentSize().height * 0.65));
        lbl1->setAnchorPoint(ccp(1, 0.5));
        btnExcharge->addChild(lbl1);
        
        CCSprite* sptPiece = CCSprite::create("pic/panel/gashapon/piece.png");
        sptPiece->setPosition(ccp(sptPrice1->getContentSize().width * 0.72, lbl1->getPositionY()));
        sptPiece->setScale(0.9);
        btnExcharge->addChild(sptPiece);
        
        CCLabelTTF* lbl2 = CCLabelTTF::create("点击兑换", DISPLAY->fangzhengFont(), 15.5f);
        lbl2->setPosition(ccp(sptPrice1->getContentSize().width * 0.5, sptPrice1->getContentSize().height * 0.35));
        btnExcharge->addChild(lbl2);
        
        // 获得碎片
        CCSprite* sptTxtBottom = CCSprite::create("pic/panel/gashapon/gashapon_bar.png");
        sptTxtBottom->setPosition(ccp(panelSize.width * 0.62, panelSize.height * 0.4));
        _panel->addChild(sptTxtBottom);
        CCSprite* sptTxt = CCSprite::create("pic/panel/gashapon/gashapon_txt.png");
        sptTxt->setPosition(ccp(sptTxtBottom->getContentSize().width * 0.4, sptTxtBottom->getContentSize().height * 0.5));
        sptTxtBottom->addChild(sptTxt);
        
        
//        CCSprite* sptPiece2 = CCSprite::create("pic/panel/gashapon/piece.png");
//        sptPiece2->setPosition(ccp(lblPiece->getPositionX() + lblPiece->getContentSize().width, lblPiece->getPositionY()));
//        sptTxtBottom->addChild(sptPiece2);
        
        // free
        CCSprite* free1 = CCSprite::create("pic/panel/gashapon/gashapon_btn_free.png");
        CCSprite* free2 = CCSprite::create("pic/panel/gashapon/gashapon_btn_free.png");
        free2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btnFree = CCMenuItemSprite::create(free1, free2);
        btnFree->setPosition(ccp(panelSize.width * 0.265, panelSize.height * 0.22));
        
        // 10
        CCSprite* mult1 = CCSprite::create("pic/panel/gashapon/gashapon_btn_mult.png");
        CCSprite* mult2 = CCSprite::create("pic/panel/gashapon/gashapon_btn_mult.png");
        mult2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btnMult = CCMenuItemSprite::create(mult1, mult2);
        btnMult->setPosition(ccp(panelSize.width * 0.735, panelSize.height * 0.22));
        
        CCMenu* menuTry = CCMenu::create(btnFree, btnMult, NULL);
        menuTry->ignoreAnchorPointForPosition(false);
        _panel->addChild(menuTry);
        
        // 状态
        string id = item->valueForKey("id")->getCString();
        if (DATA->getOperation()->has_owned_gashapon(id)) {
            
        }
        else {
            
        }
    }
}

void GashaponPanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void GashaponPanel::on_purchase() {
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_PURCHASEPANEL");
}

void GashaponPanel::on_purchase_achievement() {

}

void GashaponPanel::on_signin7() {
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_SIGNIN7");
}

void GashaponPanel::on_take_energy(CCMenuItem *btn) {
    LOADING->show_loading();
    NET->take_energy_reward_301();
}

void GashaponPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1) {
        return;
    }
    
    this->remove();
}

void GashaponPanel::nc_take_energy_301(CCObject *pObj) {

}

