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
    
    // 体力框
    CCSprite* tiliSpr1 = CCSprite::create("res/pic/baseScene/base_tili_bar.png");
    CCSprite* tiliSpr2 = CCSprite::create("res/pic/baseScene/base_tili_bar.png");
    tiliSpr2->setColor(ccGRAY);
    tiliItem = CCMenuItemSprite::create(tiliSpr1, tiliSpr2, this, menu_selector(BaseScene::tiliCallBack));
    
    // 钻石框
    CCSprite* goldSpr1 = CCSprite::create("res/pic/baseScene/base_zuanshi_bar.png");
    CCSprite* goldSpr2 = CCSprite::create("res/pic/baseScene/base_zuanshi_bar.png");
    goldSpr2->setColor(ccGRAY);
    goldItem = CCMenuItemSprite::create(goldSpr1, goldSpr2, this, menu_selector(BaseScene::goldCallBack));
    
    // 金币框
    CCSprite* coinSpr1 = CCSprite::create("res/pic/baseScene/base_jinbi_bar.png");
    CCSprite* coinSpr2 = CCSprite::create("res/pic/baseScene/base_jinbi_bar.png");
    coinSpr2->setColor(ccGRAY);
    coinItem = CCMenuItemSprite::create(coinSpr1, coinSpr2, this, menu_selector(BaseScene::coinCallBack));
    
    barMenu = CCMenu::create(nameItem, tiliItem, goldItem, coinItem, NULL);
    if ((DISPLAY->ScreenWidth() - 640) == 0) {
        barMenu->alignItemsHorizontallyWithPadding(-2.5f);
    }else{
        barMenu->alignItemsHorizontallyWithPadding(15.f);
    }
    barMenu->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .965f));
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










