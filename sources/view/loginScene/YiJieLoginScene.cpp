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
    nc->addObserver(this, SEL_CallFuncO(&YiJieLoginScene::fast_login_callback_900), "HTTP_FINISHED_900", NULL);
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
    
    
    // 测试
    CCLabelTTF* lblTest = CCLabelTTF::create("我是测试易接的~!", DISPLAY->fangzhengFont(), 30);
    lblTest->setPosition(DISPLAY->center());
    this->addChild(lblTest);
    

    CCString* strVersion = CCString::createWithFormat("v%s - %d", CONFIG->version.c_str(), CONFIG->netId);
    CCLabelTTF* lblVersion = CCLabelTTF::create(strVersion->getCString(), DISPLAY->fangzhengFont(), 20.f);
    lblVersion->setColor(ccORANGE);
    lblVersion->setPosition(ccp(DISPLAY->W() * 0.85, DISPLAY->H() * 0.04));
    this->addChild(lblVersion);
    
    bool autoLogin = DATA->getAutoLogin();
    if (autoLogin && CONFIG->has_saved_uuid()) {
        CCLog("非 首次登录~");
        LOADING->show_loading();
        DATA->setLoginType(1);
        NET->fast_login_900(CONFIG->saved_uuid().c_str(), CONFIG->channelId);
    }
    else { // 首次登入走这里
        CCLog("首次登入走这里");
    }
}



void YiJieLoginScene::show_nicknameview() {
    CreateName* layer = CreateName::create();
    this->addChild(layer);
}

void YiJieLoginScene::fast_login_callback_900(CCObject *pObj) {
    LOADING->remove();
    if (! CONFIG->has_saved_uuid()) {
        CONFIG->save_uuid(DATA->getLogin()->obtain_UUID());
    }
    
    NET->login_game_server_902();
}

void YiJieLoginScene::game_login_callback_902(CCObject *pObj) {
    const char* nickname = DATA->getShow()->nickname();
    if (strcmp(nickname, "") == 0) {
        LOADING->remove();

        this->show_nicknameview();
        AUDIO->first_run_config();
        AUDIO->play_main_bgm();
    }
    else {
        CCScene* scene = MainScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void YiJieLoginScene::save_nickname_callback_904(CCObject *pObj) {
    CCScene* scene = MainScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
