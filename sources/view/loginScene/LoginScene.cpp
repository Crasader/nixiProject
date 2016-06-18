//
//  LoginScene.cpp
//  tiegao
//
//  Created by mac on 16/6/17.
//
//

#include "LoginScene.h"
#include "NetManager.h"
#include "DisplayManager.h"

#include "LoginView.h"
#include "RegisterView.h"

LoginScene::~LoginScene() {
    CC_SAFE_DELETE(_views);
}

CCScene* LoginScene::scene() {
    CCScene* scene = CCScene::create();
    scene->addChild(LoginScene::create());
    return scene;
}

bool LoginScene::init() {
    if (CCLayer::init()) {
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        this->setTouchEnabled(true);
        
        _views = NULL;
        
        return true;
    }
    else {
        return false;
    }
}

void LoginScene::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&LoginScene::show_registview), "shift_to_registerview", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&LoginScene::show_loginview), "shift_to_loginview", NULL);
    
    CCSprite* bg = CCSprite::create("res/pic/loginScene/login_bg.png");
    bg->setPosition(DISPLAY->center());
    this->addChild(bg);
    
    NetEnv netenv = NET->obtain_net_env();
    std::string env_info;
    switch (netenv) {
        case e_NetEnv_NotReachable:
            env_info = "没有网络!~";
            break;
        case e_NetEnv_ViaWiFi:
            env_info = "使用 WiFi 接入";
            break;
        case e_NetEnv_ViaWWAN:
            env_info = "使用 WWAN 网络";
            break;
        case e_NetEnv_Unknown:
        default:
            env_info = "未知网络!~";
            break;
    }
    CCLOG("%s", env_info.c_str());
    
    this->create_views();
    this->show_loginview();
    this->slide_in_logo();
}

void LoginScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

bool LoginScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCLOG("LoginScene::ccTouchBegan()~");
    return true;
}

#pragma - inner API

void LoginScene::slide_in_logo() {
    CCSprite* logo = CCSprite::create("res/pic/loginScene/login_logo.png");
    logo->setPosition(ccp(DISPLAY->halfW(), -100));
    this->addChild(logo);
    
    logo->runAction(CCEaseExponentialIn::create(CCMoveTo::create(0.6f, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.12f))));
}

void LoginScene::create_views() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
    CC_SAFE_RELEASE_NULL(_views);
    _views = CCDictionary::create();
    _views->retain();
    
    _views->setObject(LoginView::create(), "login");
    _views->setObject(RegisterView::create(), "regist");
    
    _container = CCLayer::create();
    this->addChild(_container);
}

void LoginScene::show_loginview() {
    _container->removeAllChildren();
    
    CCLayer* layer = (CCLayer* )_views->objectForKey("login");
    _container->addChild(layer);
}

void LoginScene::show_registview() {
    _container->removeAllChildren();
    
    CCLayer* layer = (CCLayer* )_views->objectForKey("regist");
    _container->addChild(layer);
}


