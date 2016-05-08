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

    return true;
}

void TestScene::onEnter() {
    CCScene::onEnter();
    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
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
    
    this->create_view();
}

void TestScene::onExit() {
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCScene::onExit();
}

void TestScene::create_view() {
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bg = CCSprite::create("pic/test_bg.png");
    bg->setPosition(ccp(win_size.width * 0.5, win_size.height * 0.5));
    this->addChild(bg);
    
    CCMenuItemFont* btn_fast_login = CCMenuItemFont::create("游客登入", this, SEL_MenuHandler(&TestScene::fast_login));
    CCMenu* menu = CCMenu::createWithItem(btn_fast_login);
    menu->setColor(ccRED);
    menu->alignItemsVerticallyWithPadding(10);
    this->addChild(menu);
}

void TestScene::fast_login() {
    NetManager::Inst()->fast_login_9008(DataManager::Inst()->obtain_UUID());
}