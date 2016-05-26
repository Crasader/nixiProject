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
}

bool SocialComp::init() {
    _strangers = NULL;
    
    return true;
}

void SocialComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("SocialComp::init_with_json() json object error.");
        return;
    }
    
}

void SocialComp::update_stranger(CSJson::Value json) {
    if (!json.isObject()) {
        CCLOG("SocialComp::update_stranger() json object error.");
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