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
#include "AudioManager.h"
#include "Loading2.h"
#include "PromptLayer.h"
#include "LoginView.h"
#include "RegisterView.h"
#include "RecommendView.h"
#include "CreateName.h"
//#include "MainScene.h"
#include "NXMainScene.h"

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
        
         CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/loginScene/login_hua.plist");
        
        CCSprite* bg = CCSprite::create("res/pic/loginScene/login_bg2.png");
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
        
        CCSprite* logo = CCSprite::create("res/pic/loginScene/login_logo.png");
        logo->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.13));
        this->addChild(logo);
        
        CCSprite* guangSpr = CCSprite::create("res/pic/loginScene/login_guang.png");
        guangSpr->setPosition(ccp(bg->boundingBox().size.width* .18f, bg->boundingBox().size.height* .22f));
        bg->addChild(guangSpr, 5);
        
        CCFadeTo* fadeTo1 = CCFadeTo::create(.5f, 100);
        CCFadeTo* fadeTo2 = CCFadeTo::create(.5f, 255);
        //    CCBlink* blink = CCBlink::create(1.f, 2);
        CCSequence* seq = CCSequence::create(fadeTo1, fadeTo2, NULL);
        guangSpr->runAction(CCRepeatForever::create(seq));
        
        
        CCString* strVersion = CCString::createWithFormat("v%s - %d", CONFIG->version.c_str(), CONFIG->netId);
        CCLabelTTF* lblVersion = CCLabelTTF::create(strVersion->getCString(), DISPLAY->fangzhengFont(), 20.f);
        lblVersion->setColor(ccORANGE);
        lblVersion->setPosition(ccp(DISPLAY->W() * 0.85, DISPLAY->H() * 0.055));
        this->addChild(lblVersion);
        
        for (int i = 1; i <= 6; i++) {
            this->init_snow(i);
        }
        
//        CCSprite* mask = CCSprite::create("res/pic/mask.png");
//        mask->setPosition(DISPLAY->center());
//        this->addChild(mask);
        
        _views = NULL;
        _temp_account_pwd = NULL;
        _container = NULL;
        
        return true;
    }
    else {
        return false;
    }
}

void LoginScene::init_snow(int huaIndex){
    CCParticleSnow * snow = CCParticleSnow::create();
    // 自定义雪花图片
    CCString* snowStr = CCString::createWithFormat("res/pic/loginScene/login_hua%d.png", huaIndex);
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(snowStr->getCString());
    snow->setTexture(texture);
    // 粒子总数
    snow->setTotalParticles(700);
    // duration
    snow->setDuration(kCCParticleDurationInfinity);
    // set gravity mode.
    snow->setEmitterMode(kCCParticleModeGravity);
    snow->setAngle(-90);
    snow->setAngleVar(5);
    // Gravity Mode: radial acceleration
    snow->setRadialAccel(0);
    snow->setRadialAccelVar(0);
    // 下雪时间秒
    snow->setDuration(kCCParticleDurationInfinity);
    // 雪花开始大小
    if (huaIndex == 1) {
        snow->setStartSize(150.0f);
    }else if (huaIndex == 2){
        snow->setStartSize(125.0f);
    }else if (huaIndex == 3){
        snow->setStartSize(125.0f);
    }else if (huaIndex == 4){
        snow->setStartSize(77.0f);
    }else if (huaIndex == 5){
        snow->setStartSize(98.0f);
    }else if (huaIndex == 6){
        snow->setStartSize(107.0f);
    }
    // 雪花大小浮动
    snow->setStartSizeVar(30.0f);
    // 雪花结束大小
    snow->setEndSize(kCCParticleStartSizeEqualToEndSize);
    snow->setEndSizeVar(0);
    // 初始速度
    snow->setSpeed(30);
    snow->setSpeedVar(5);
    // Gravity Mode: radial
    snow->setRadialAccel(0);
    snow->setRadialAccelVar(1);
    // Gravity mode: tangential
    snow->setTangentialAccel(0);
    snow->setTangentialAccelVar(1);
    // 下降速度 y负数表示下落 x负数表示向西吹
    snow->setGravity(ccp(0, -.8f));
    // 每个雪花生命时间
    snow->setLife(30);
    snow->setLifeVar(10);
    // 释放雪花速度
    snow->setEmissionRate(.2);
    // 动画完成后自动从上层layer中移除粒子
    snow->setAutoRemoveOnFinish(true);
    // 释放粒子位置 如果你想在调用的时候设置位置，那么可以注视下面的代码
    snow->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight() + 15));
    snow->setPosVar(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .1f));
    this->addChild(snow, 50);
}

void LoginScene::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::show_registview), "shift_to_registerview", NULL);
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::show_loginview), "shift_to_loginview", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::start_login), "start_login", NULL);
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::start_regist), "start_regist", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&LoginScene::get_info_999), "HTTP_FINISHED_999", NULL);
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
    
//    NET->get_info_999();
    
    bool autoLogin = DATA->getAutoLogin();
    bool hasSavedAccount = CONFIG->has_saved_account();
    if (autoLogin && CONFIG->saved_login_type() == 1) { // UUID登录
        if (CONFIG->has_saved_uuid()) {
            LOADING->show_loading();
            //        DATA->setLoginType(1);
            CONFIG->save_login_type(1);
            NET->fast_login_900(CONFIG->saved_uuid().c_str(), CONFIG->channelId);
        }
        else {
            DATA->setAutoLogin(false);
            this->create_views();
            this->show_registview();
            this->show_loginview();
        }
    }
    else if (autoLogin && CONFIG->saved_login_type() == 2) {
        if (hasSavedAccount) {
            LOADING->show_loading();
            //        DATA->setLoginType(2);
            CONFIG->save_login_type(2);
            NET->account_login_901(CONFIG->saved_account().c_str(), CONFIG->saved_password().c_str());
        }
        else {
            DATA->setAutoLogin(false);
            this->create_views();
            this->show_registview();
            this->show_loginview();
        }
    }
    else {
        DATA->setAutoLogin(false);
        this->create_views();
        this->show_registview();
        this->show_loginview();
    }
}

void LoginScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCEGLView::sharedOpenGLView()->setIMEKeyboardState(false);
    
    CCLayer::onExit();
}

bool LoginScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCLOG("LoginScene::ccTouchBegan()~");
    return true;
}

#pragma - inner API

//void LoginScene::slide_in_logo() {
//    CCSprite* logo = CCSprite::create("res/pic/loginScene/login_logo.png");
//    logo->setPosition(ccp(DISPLAY->halfW(), -100));
//    this->addChild(logo);
//    
//    logo->runAction(CCEaseExponentialIn::create(CCMoveTo::create(0.6f, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.12f))));
//}

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
    
    CCLayer* layer = (CCLayer*)_views->objectForKey("nickname");
    _container->addChild(layer);
}

void LoginScene::fast_login(CCMenuItem *pObj) {
//    RecommendView::show(this->getScene());
    pObj->setEnabled(false);
    LOADING->show_loading();

    CONFIG->save_login_type(1);
    NET->fast_login_900(DATA->getLogin()->obtain_UUID(), CONFIG->channelId);
}

void LoginScene::start_login(CCObject *pObj) {
    CCDictionary* dic = (CCDictionary* )pObj;
    CCString* account = (CCString* )dic->objectForKey("account");
    CCString* password = (CCString* )dic->objectForKey("password");
    CC_SAFE_RELEASE(_temp_account_pwd);
    _temp_account_pwd = dic;
    _temp_account_pwd->retain();
    
    LOADING->show_loading();
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
    NET->account_regist_903(account->getCString(), password->getCString(), CONFIG->channelId);
}
                                                      
void LoginScene::get_info_999(CCObject *pObj) {
    CCDictionary* info = (CCDictionary* )pObj;
    CCString* copyRight1 = (CCString* )info->objectForKey("copy_right1");
    CCString* copyRight2 = (CCString* )info->objectForKey("copy_right2");
    
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    ccColor3B color = ccc3(41, 21, 11);
    
    CCLabelTTF* lblCopyRight1 = CCLabelTTF::create(copyRight1->getCString(), "Arial", 16.f);
    lblCopyRight1->setAnchorPoint(ccp(0.5, 0.5));
    lblCopyRight1->setColor(color);
    lblCopyRight1->setPosition(ccp(winsize.width * 0.5, winsize.height * 0.025));
    this->addChild(lblCopyRight1);
    
    CCLabelTTF* lblCopyRight2 = CCLabelTTF::create(copyRight2->getCString(), "Arial", 16.f);
    lblCopyRight2->setAnchorPoint(ccp(0.5, 0.5));
    lblCopyRight2->setColor(color);
    lblCopyRight2->setPosition(ccp(winsize.width * 0.5, winsize.height * 0.01));
    this->addChild(lblCopyRight2);
}


void LoginScene::fast_login_callback_900(CCObject *pObj) {
    LOADING->remove();
    
    if (! CONFIG->has_saved_uuid()) {
        CONFIG->save_uuid(DATA->getLogin()->obtain_UUID());
    }
    
    NET->login_game_server_902();
}

void LoginScene::account_login_callback_901(CCObject *pObj) {
    if (! DATA->getAutoLogin()) {
        CONFIG->save_login_type(2);
        const char* account = ((CCString*)_temp_account_pwd->objectForKey("account"))->getCString();
        CONFIG->save_account(account);
        const char* password = ((CCString*)_temp_account_pwd->objectForKey("password"))->getCString();
        CONFIG->save_password(password);
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
        AUDIO->first_run_config();
        AUDIO->play_main_bgm();
    }
    else {
        CCScene* scene = NXMainScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void LoginScene::account_regist_callback_903(CCObject *pObj) {
    LOADING->remove();
    
    CONFIG->save_login_type(2);
    const char* account = ((CCString*)_temp_account_pwd->objectForKey("account"))->getCString();
    CONFIG->save_account(account);
    const char* password = ((CCString*)_temp_account_pwd->objectForKey("password"))->getCString();
    CONFIG->save_password(password);
#warning "计划添加帐号密码谨记提示!"
    
    LOADING->show_loading();
    NET->account_login_901(CONFIG->saved_account().c_str(), CONFIG->saved_password().c_str());
}

void LoginScene::save_nickname_callback_904(CCObject *pObj) {
    CCLog("<><><><><> LoginScene::save_nickname_callback_904");
    
    // talkingData
    DATA->onEvent("启动事件", "昵称界面", "完成昵称界面");
    
    CCScene* scene = NXMainScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
