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
}
void BaseScene::onExit(){
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
    
    // 体力框
    CCSprite* tiliSpr1 = CCSprite::create("res/pic/baseScene/base_bar.png");
    CCSprite* tiliSpr2 = CCSprite::create("res/pic/baseScene/base_bar.png");
    tiliItem = CCMenuItemSprite::create(tiliSpr1, tiliSpr2, this, menu_selector(BaseScene::tiliCallBack));
    if ((DISPLAY->ScreenWidth() - 640) == 0) {
        tiliItem->setPosition(ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* .965f));
    }else{
        tiliItem->setPosition(ccp(DISPLAY->ScreenWidth()* .48f, DISPLAY->ScreenHeight()* .965f));
    }
    CCSprite* tiliIconSpr = CCSprite::create("res/pic/clothesScene/gj_xin.png");
    tiliIconSpr->setScale(1.1f);
    tiliIconSpr->setPosition(ccp(tiliItem->getContentSize().width* .09f, tiliItem->getContentSize().height* .5f));
    tiliItem->addChild(tiliIconSpr);
    
    // 钻石框
    CCSprite* goldSpr1 = CCSprite::create("res/pic/baseScene/base_bar.png");
    CCSprite* goldSpr2 = CCSprite::create("res/pic/baseScene/base_bar.png");
    goldItem = CCMenuItemSprite::create(goldSpr1, goldSpr2, this, menu_selector(BaseScene::goldCallBack));
    if ((DISPLAY->ScreenWidth() - 640) == 0) {
        goldItem->setPosition(ccp(DISPLAY->ScreenWidth()* .64f, DISPLAY->ScreenHeight()* .965f));
    }else{
        goldItem->setPosition(ccp(DISPLAY->ScreenWidth()* .69f, DISPLAY->ScreenHeight()* .965f));
    }
    CCSprite* goldIconSpr = CCSprite::create("res/pic/clothesScene/gj_gold.png");
    goldIconSpr->setPosition(ccp(goldItem->getContentSize().width* .09f, goldItem->getContentSize().height* .52f));
    goldItem->addChild(goldIconSpr);
    
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
    coinIconSpr->setPosition(ccp(coinItem->getContentSize().width* .09f, coinItem->getContentSize().height* .54f));
    coinItem->addChild(coinIconSpr);
    
    barMenu = CCMenu::create(nameItem, tiliItem, goldItem, coinItem, NULL);
    barMenu->setPosition(CCPointZero);
    this->addChild(barMenu, 10);
    
    



}
void BaseScene::hideBaseScene(){
    barMenu->setVisible(false);
}

void BaseScene::tiliCallBack(CCObject* pSender){
    
}
void BaseScene::goldCallBack(CCObject* pSender){
    
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










