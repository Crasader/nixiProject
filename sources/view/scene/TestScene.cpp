//
//  TestScene.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "TestScene.h"
#include "DataManager.h"
#include "NetManager.h"

bool TestScene::init() {
    if (!CCScene::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);

    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bg = CCSprite::create("pic/test_bg.png");
    bg->setPosition(ccp(win_size.width * 0.5, win_size.height * 0.5));
    this->addChild(bg);
    
    _content = CCLayer::create();
    this->addChild(_content);
    
    return true;
}

void TestScene::onEnter() {
    CCScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&TestScene::login_game_server), "HTTP_FINISHED_900", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::social_view), "HTTP_FINISHED_902", NULL);
    
    NetEnv netenv = NetManager::Inst()->obtain_net_env();
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
    
    this->login_view();
}

void TestScene::onExit() {
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCScene::onExit();
}

void TestScene::login_view() {
    _content->removeAllChildren();
    
    CCMenuItemFont* btn_fast_login = CCMenuItemFont::create("游客登入", this, SEL_MenuHandler(&TestScene::fast_login));
    CCMenu* menu = CCMenu::createWithItem(btn_fast_login);
    menu->setColor(ccRED);
    menu->alignItemsVerticallyWithPadding(10);
    _content->addChild(menu);
}

void TestScene::social_view() {
    _content->removeAllChildren();
    
    CCMenuItemFont* btn_rtn_login = CCMenuItemFont::create("返回登入", this, SEL_MenuHandler(&TestScene::login_view));
    CCMenuItemFont* btn_recommend_stranger = CCMenuItemFont::create("推荐陌生人", this, SEL_MenuHandler(&TestScene::recommend_stranger));
    
    CCMenu* menu = CCMenu::create(btn_rtn_login, btn_recommend_stranger, NULL);
    menu->setColor(ccORANGE);
    menu->alignItemsVerticallyWithPadding(10);
    _content->addChild(menu);
}

void TestScene::fast_login() {
    NetManager::Inst()->fast_login_900(DataManager::Inst()->getLogin()->obtain_UUID());
}

void TestScene::login_game_server() {
    NetManager::Inst()->login_game_server_902();
}

void TestScene::recommend_stranger() {
    NetManager::Inst()->recommend_stranger_802();
}
