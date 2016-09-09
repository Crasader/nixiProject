//
//  YiJieLoginScene.cpp
//  tiegao
//
//  Created by mac on 16/9/9.
//
//

#include "ConfigManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "DisplayManager.h"
#include "AudioManager.h"

#include "YiJieLoginScene.h"
#include "Loading2.h"
#include "PromptLayer.h"
#include "CreateName.h"
#include "MainScene.h"

YiJieLoginScene::~YiJieLoginScene() {

}

CCScene* YiJieLoginScene::scene() {
    CCScene* scene = CCScene::create();
    scene->addChild(YiJieLoginScene::create());
    return scene;
}

bool YiJieLoginScene::init() {
    CCLog("<><><><><> YiJieLoginScene::init()");
    if (CCLayer::init()) {
        CCSprite* bg = CCSprite::create("res/pic/YiJieLoginScene/login_bg.png");
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
        
        return true;
    }
    else {
        return false;
    }
}

void YiJieLoginScene::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&YiJieLoginScene::nc_yijie_login_909), "HTTP_FINISHED_909", NULL);
    nc->addObserver(this, SEL_CallFuncO(&YiJieLoginScene::save_nickname_callback_904), "HTTP_FINISHED_904", NULL);
    
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    this->setTouchEnabled(true);
    
    this->create_views();
}

void YiJieLoginScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCEGLView::sharedOpenGLView()->setIMEKeyboardState(false);
    
    CCLayer::onExit();
}

//bool YiJieLoginScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
//    CCLOG("YiJieLoginScene::ccTouchBegan()~");
//    return true;
//}

#pragma - inner API

void YiJieLoginScene::create_views() {
    // 需要检查网络状况
    

    CCString* strVersion = CCString::createWithFormat("v%s - %d", CONFIG->version.c_str(), CONFIG->netId);
    CCLabelTTF* lblVersion = CCLabelTTF::create(strVersion->getCString(), DISPLAY->fangzhengFont(), 20.f);
    lblVersion->setColor(ccORANGE);
    lblVersion->setPosition(ccp(DISPLAY->W() * 0.85, DISPLAY->H() * 0.04));
    this->addChild(lblVersion);
}

void YiJieLoginScene::show_nicknameview() {
    CreateName* layer = CreateName::create();
    this->addChild(layer);
}

void YiJieLoginScene::nc_yijie_login_909(CCObject *pObj) {
    LOADING->remove();
    if (! CONFIG->has_saved_uuid()) {
        CONFIG->save_uuid(DATA->getLogin()->obtain_UUID());
    }
    
    NET->login_game_server_902();
}

void YiJieLoginScene::save_nickname_callback_904(CCObject *pObj) {
    CCScene* scene = MainScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
