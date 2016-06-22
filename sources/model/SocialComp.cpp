//
//  SocialComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "SocialComp.h"
#include "AppUtil.h"
#include "ShowComp.h"

SocialComp::~SocialComp() {
    CC_SAFE_DELETE(_strangers);
    CC_SAFE_DELETE(_friends);
    CC_SAFE_DELETE(_interaction);
}

bool SocialComp::init() {
    _strangers = NULL;
    _friends = NULL;
    _interaction = NULL;
    
    return true;
}

void SocialComp::init_with_json(Value json) {
    if (json.type() == nullValue && !json.isObject()) {
        CCLOG("SocialComp::init_with_json() json object error.");
        return;
    }
    
    CCDictionary* dic = AppUtil::dictionary_with_json(json["firends"]);
    CC_SAFE_RELEASE(_interaction);
    _interaction = dic;
    _interaction->retain();
}

void SocialComp::init_friends(Value json) {
    if (json.type() == nullValue && !json.isObject()) {
        CCLOG("SocialComp::init_friends() json object error.");
        return;
    }
    
    CCDictionary* dic = CCDictionary::create();
    std::vector<std::string> keys = json.getMemberNames();
    for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
        std::string key = (std::string)* iterator;
        CSJson::Value value = json.get(key, value.jsonNull);
        if (value != value.jsonNull) {
            ShowComp* shower = ShowComp::create();
            shower->init_with_json(value);
            dic->setObject(shower, key);
        }
    }
    
    CC_SAFE_RELEASE(_friends);
    _friends = dic;
    _friends->retain();
}

void SocialComp::update_strangers(CSJson::Value json) {
    if (json.type() == nullValue && !json.isObject()) {
        CCLOG("SocialComp::update_strangers() json object error.");
        return;
    }
    
    if (json.isObject()) { // 对象
        CCDictionary* dic = CCDictionary::create();
        std::vector<std::string> keys = json.getMemberNames();
        for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
            std::string key = (std::string)* iterator;
            CSJson::Value value = json.get(key, value.jsonNull);
            if (value != value.jsonNull) {
                ShowComp* shower = ShowComp::create();
                shower->init_with_json(value);
                dic->setObject(shower, key);
            }
        }
        
        CC_SAFE_RELEASE(_strangers);
        _strangers = dic;
        _strangers->retain();
    }
}

CCDictionary* SocialComp::strangers() const {
    return _strangers;
}

CCDictionary* SocialComp::friends() const {
    return _friends;
}
