//
//  MessageLayer.cpp
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#include "MessageLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "Loading2.h"
#include "HaoyouScene.h"
#include "MessageTableView.h"
#include "AudioManager.h"

MessageLayer::MessageLayer(){
    
}
MessageLayer::~MessageLayer(){
    
}

bool MessageLayer::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    this->creat_View();
    
    return true;
}

void MessageLayer::onEnter(){
    BaseScene::onEnter();
    
    
    this->scheduleOnce(SEL_SCHEDULE(&MessageLayer::keyBackStatus), .8f);
}
void MessageLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void MessageLayer::onExit(){
    
    BaseScene::onExit();
}

cocos2d::CCScene* MessageLayer::scene(){
    CCScene* scene = CCScene::create();
    MessageLayer* layer = MessageLayer::create();
    scene->addChild(layer);
    
    return scene;
}

void MessageLayer::creat_View(){
    
    bgSpr = CCSprite::create("res/pic/qingjingScene/qj_bg.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    // 返回
    CCSprite* backSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(MessageLayer::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .037f));
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 10);
    
    MessageTableView* layer = MessageTableView::create();
    layer->setTag(0x77777);
    this->addChild(layer, 50);
}
void MessageLayer::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    CCScene* scene = HaoyouScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MessageLayer::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->backCallBack(NULL);
}

void MessageLayer::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    
}


