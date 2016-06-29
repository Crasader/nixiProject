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
    CC_SAFE_DELETE(_arr_friends);
    CC_SAFE_DELETE(_energy_send);
    CC_SAFE_DELETE(_energy_receive);
}

bool SocialComp::init() {
    _strangers = NULL;
    _friends = NULL;
    _arr_friends = NULL;
    _energy_send = NULL;
    _energy_receive = NULL;
    
    return true;
}

void SocialComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("SocialComp::init_with_json() json object error.");
        return;
    }
    
    _energy_token = json["energy_token"].asInt();
    
    CCArray* arr_friends = AppUtil::array_with_json(json["firends"]);
    CC_SAFE_RELEASE(_arr_friends);
    _arr_friends = arr_friends;
    _arr_friends->retain();
    
    CCArray* arr_send = AppUtil::array_with_json(json["energy_send"]);
    CC_SAFE_RELEASE(_energy_send);
    _energy_send = arr_send;
    _energy_send->retain();
    
    CCArray* arr_receive = AppUtil::array_with_json(json["energy_receive"]);
    CC_SAFE_RELEASE(_energy_receive);
    _energy_receive = arr_receive;
    _energy_receive->retain();
}

void SocialComp::init_friends(Value json) {
    if (json.type() == nullValue) {
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
    if (json.type() == nullValue) {
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

CCDictionary* SocialComp::strangers() {
    return _strangers;
}

CCDictionary* SocialComp::friends() const {
    return _friends;
}

bool SocialComp::is_friend(const char *other_sid) {
    CCObject* pObj = _friends->objectForKey(other_sid);
    if (pObj) {
        return true;
    }
    
    return false;
}

bool SocialComp::has_send_energy(const char *other_sid) {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_energy_send, pObj) {
        CCString* otherId = (CCString*)pObj;
        if (otherId->compare(other_sid) == 0) {
            return true;
        }
    }
    
    return false;
}

int SocialComp::energy_could_take() {
    return _energy_receive->count() - _energy_token;
}

ShowComp* SocialComp::getSelectedStrangerByIndex(int idx) {
    CCArray* arrKeys = _strangers->allKeys();
    int count = arrKeys->count();
    if (0 <= idx && idx < count) {
        CCString* key = (CCString*)arrKeys->objectAtIndex(idx);
        ShowComp* show = (ShowComp*)_strangers->objectForKey(key->getCString());
        return show;
    }
    else {
        return NULL;
    }
}

const char* SocialComp::getSelectedStrangerIDbyIndex(int idx){
    CCArray* arrKeys = _strangers->allKeys();
    int count = arrKeys->count();
    if (0 <= idx && idx < count) {
        CCString* key = (CCString*)arrKeys->objectAtIndex(idx);
        return key->getCString();
    }
    else {
        return NULL;
    }
}

ShowComp* SocialComp::getSelectedFriendByIndex(int idx) {
    CCArray* arrKeys = _friends->allKeys();
    int count = arrKeys->count();
    if (0 <= idx && idx < count) {
        CCString* key = (CCString*)arrKeys->objectAtIndex(idx);
        ShowComp* show = (ShowComp*)_friends->objectForKey(key->getCString());
        return show;
    }
    else {
        return NULL;
    }
}

const char* SocialComp::getSelectedFriendIDbyIndex(int idx){
    CCArray* arrKeys = _friends->allKeys();
    int count = arrKeys->count();
    if (0 <= idx && idx < count) {
        CCString* key = (CCString*)arrKeys->objectAtIndex(idx);
        return key->getCString();
    }
    else {
        return NULL;
    }
}


