//
//  LoginComp.cpp
//  tiegao
//
//  Created by mac on 16/5/16.
//
//

#include "LoginComp.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "native/CCNative.h"
USING_NS_CC_EXTRA;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

LoginComp::~LoginComp() {
}

bool LoginComp::init() {
    _sid = "";
    _uuid = "";
    _skey = "";
    _gameaddr = "";
    
    return true;
}

void LoginComp::config_UUID() {
    if (!_uuid.empty()) {
        CCLOG("UUID 只需获取一次，请确认是否正确使用???");
        return;
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    _uuid = cocos2d::extra::CCNative::getOpenUDID();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    _uuid = "没有获得";
#endif
}

const char* LoginComp::obtain_UUID() {
    return _uuid.c_str();
}

const char* LoginComp::obtain_sid() {
    return _sid.c_str();
}

const char* LoginComp::obtain_game_addr() {
    return _gameaddr.c_str();
}

void LoginComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("LoginComp::init_with_json() json object error.");
        return;
    }
    
    _sid = json["sid"].asString();
    _skey = json["skey"].asString();
    _gameaddr = json["addr"].asString();
    CCLOG("Game addr: %s", _gameaddr.c_str());
}



