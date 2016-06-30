//
//  LoginScene.cpp
//  tiegao
//
//  Created by mac on 16/6/17.
//
//

#include "LoginScene.h"
#include "ConfigManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "DisplayManager.h"
#include "Loading2.h"
#include "PromptLayer.h"
#include "LoginView.h"
#include "RegisterView.h"
#include "CreateName.h"
#include "MainScene.h"

LoginScene::~LoginScene() {
    CC_SAFE_DELETE(_views);
    CC_SAFE_DELETE(_temp_account_pwd);
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
        
        CCSprite* bg = CCSprite::create("res/pic/loginScene/login_bg.png");
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _views = NULL;
        _temp_account_pwd = NULL;
        _container = NULL;
        
        return true;
    }
    else {
        return false;
    }
}

void LoginScene::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::show_registview), "shift_to_registerview", NULL);
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::show_loginview), "shift_to_loginview", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::start_login), "start_login", NULL);
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::start_regist), "start_regist", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::fast_login_callback_900), "HTTP_FINISHED_900", NULL);
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::account_login_callback_901), "HTTP_FINISHED_901", NULL);
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::game_login_callback_902), "HTTP_FINISHED_902", NULL);
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::account_regist_callback_903), "HTTP_FINISHED_903", NULL);
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::save_nickname_callback_904), "HTTP_FINISHED_904", NULL);
    
    NetEnv netenv = NET->obtain_net_env();
    std::string env_info;
    switch (netenv) {
        case e_NetEnv_NotReachable: {
            env_info = "没有网络!~";
            PromptLayer* prompt = PromptLayer::create();
            prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "没有网络, 请检查设置!~");
        } break;
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
    
    this->slide_in_logo();
    
    if (CONFIG->has_saved_uuid()) {
        LOADING->show_loading();
        DATA->setLoginType(2);
        NET->fast_login_900(CONFIG->saved_uuid().c_str());
    }
    else if (CONFIG->has_saved_account()) {
        LOADING->show_loading();
        DATA->setLoginType(2);
        NET->account_login_901(CONFIG->saved_account().c_str(), CONFIG->saved_password().c_str());
    }
    else {
        this->create_views();
        this->show_registview();
        this->show_loginview();
    }
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
    _views->setObject(CreateName::create(), "nickname");
    
    _container = CCLayer::create();
    this->addChild(_container);
}

void LoginScene::show_loginview() {
    _container->removeAllChildrenWithCleanup(true);
    
    CCLayer* layer = (CCLayer* )_views->objectForKey("login");
    _container->addChild(layer);
    
    CCSprite* fast1 = CCSprite::create("res/pic/loginScene/login_btn_fast.png");
    CCSprite* fast2 = CCSprite::create("res/pic/loginScene/login_btn_fast.png");
    fast2->setScale(DISPLAY->btn_scale());
    CCMenuItem* btn_fast = CCMenuItemSprite::create(fast1, fast2, this, SEL_MenuHandler(&LoginScene::fast_login));
    CCMenu* menu_fast = CCMenu::createWithItem(btn_fast);
    menu_fast->setPosition(menu_fast->getPosition() - ccp(0, DISPLAY->H() * 0.16));
    _container->addChild(menu_fast);
}

void LoginScene::show_registview() {
    _container->removeAllChildrenWithCleanup(true);
    
    CCLayer* layer = (CCLayer* )_views->objectForKey("regist");
    _container->addChild(layer);
}

void LoginScene::show_nicknameview() {
    if (_container) {
        _container->removeAllChildrenWithCleanup(true);
    }
    
    CCLayer* layer = (CCLayer* )_views->objectForKey("nickname");
    _container->addChild(layer);
}

void LoginScene::fast_login(CCMenuItem *pObj) {
    LOADING->show_loading();
    DATA->setLoginType(1);
    NET->fast_login_900(DATA->getLogin()->obtain_UUID());
}

void LoginScene::start_login(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary* )pObj;
    CCString* account = (CCString* )dic->objectForKey("account");
    CCString* password = (CCString* )dic->objectForKey("password");
    CC_SAFE_RELEASE(_temp_account_pwd);
    _temp_account_pwd = dic;
    _temp_account_pwd->retain();
    
    LOADING->show_loading();
    DATA->setLoginType(1);
    NET->account_login_901(account->getCString(), password->getCString());
}

void LoginScene::start_regist(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary* )pObj;
    CCString* account = (CCString* )dic->objectForKey("account");
    CCString* password = (CCString* )dic->objectForKey("password");
    CC_SAFE_RELEASE(_temp_account_pwd);
    _temp_account_pwd = dic;
    _temp_account_pwd->retain();
    
    LOADING->show_loading();
    NET->account_regist_903(account->getCString(), password->getCString());
}

void LoginScene::fast_login_callback_900(CCObject *pObj) {
    LOADING->remove();
    if (! CONFIG->has_saved_uuid()) {
        CONFIG->save_uuid(DATA->getLogin()->obtain_UUID());
    }
    
    NET->login_game_server_902();
}

void LoginScene::account_login_callback_901(CCObject *pObj) {
    if (! CONFIG->has_saved_account()) {
        CONFIG->save_account(((CCString*)_temp_account_pwd->objectForKey("account"))->getCString());
        CONFIG->save_password(((CCString*)_temp_account_pwd->objectForKey("password"))->getCString());
    }
    
    NET->login_game_server_902();
}

void LoginScene::game_login_callback_902(CCObject *pObj) {
    const char* nickname = DATA->getShow()->nickname();
    if (strcmp(nickname, "") == 0) {
        LOADING->remove();
//        this->addChild(CreateName::create());
        if (_container == NULL) {
            this->create_views();
        }
        
        this->show_nicknameview();
    }
    else {
        CCScene* scene = MainScene::scene();
        CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void LoginScene::account_regist_callback_903(CCObject *pObj) {
    LOADING->remove();
    
    CONFIG->save_account(((CCString*)_temp_account_pwd->objectForKey("account"))->getCString());
    CONFIG->save_password(((CCString*)_temp_account_pwd->objectForKey("password"))->getCString());
#warning "计划添加帐号密码谨记提示!"
    
    LOADING->show_loading();
    NET->account_login_901(CONFIG->saved_account().c_str(), CONFIG->saved_password().c_str());
}

void LoginScene::save_nickname_callback_904(CCObject *pObj) {
    CCScene* scene = MainScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
