//
//  LogoScene.cpp
//  tiegao
//
//  Created by mac on 16-8-8.
//
//

#include "LogoScene.h"
#include "LoginScene.h"
#include "ConfigManager.h"
#include "YiJieLoginScene.h"


LogoScene::~LogoScene(){
    
}

bool LogoScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    this->creat_view();
    
    return true;
}
CCScene* LogoScene::scene(){
    CCScene* scene = CCScene::create();
    LogoScene* layer = LogoScene::create();
    scene->addChild(layer);
    
    return scene;
}

void LogoScene::onEnter(){
    CCLayer::onEnter();
    
}
void LogoScene::onExit(){
    
    CCLayer::onExit();
}

void LogoScene::creat_view(){
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bgSpr = CCSprite::create("res/pic/logo.png");
    bgSpr->setPosition(ccp(winsize.width* .5f, winsize.height* .5f));
    this->addChild(bgSpr);
    
    this->scheduleOnce(SEL_SCHEDULE(&LogoScene::next_scene), 1.f);
}
void LogoScene::next_scene(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCScene* pScene = LoginScene::scene();
    CCDirector::sharedDirector()->replaceScene(pScene);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (CONFIG->baiOrYijie == 0) {// 白包
        CCScene* pScene = LoginScene::scene();
        CCDirector::sharedDirector()->replaceScene(pScene);
    }else if (CONFIG->baiOrYijie == 1){// 易接
        CCScene* pScene = YiJieLoginScene::scene();
        CCDirector::sharedDirector()->replaceScene(pScene);
    }
#endif
}





