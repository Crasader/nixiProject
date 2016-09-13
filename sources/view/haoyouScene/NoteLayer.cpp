//
//  NoteLayer.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/29.
//
//

#include "NoteLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "Loading2.h"
#include "HaoyouScene.h"
#include "NoteTableView.h"
#include "AudioManager.h"

NoteLayer::~NoteLayer(){
    
}

bool NoteLayer::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    num_child = 0;
    
    this->create_view();
    
    return true;
}

void NoteLayer::onEnter(){
    BaseScene::onEnter();
    
    
    this->scheduleOnce(SEL_SCHEDULE(&NoteLayer::keyBackStatus), .8f);
}
void NoteLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void NoteLayer::onExit(){
    
    BaseScene::onExit();
}

cocos2d::CCScene* NoteLayer::scene(){
    CCScene* scene = CCScene::create();
    NoteLayer* layer = NoteLayer::create();
    scene->addChild(layer);
    
    return scene;
}

void NoteLayer::create_view(){
    
    bgSpr = CCSprite::create("res/pic/qingjingScene/qj_bg.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    // 返回
    CCSprite* backSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(NoteLayer::backCallback));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .037f));
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 10);
    
    NoteTableView* layer = NoteTableView::create();
    layer->setTag(0x77777);
    this->addChild(layer, 50);
}
void NoteLayer::backCallback(CCObject* pSender){
    AUDIO->goback_effect();
    
    CCScene* scene = HaoyouScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void NoteLayer::keyBackClicked(){
    num_child++;
    CCLog("===== NoteLayer  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->backCallback(NULL);
}

void NoteLayer::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    
}
