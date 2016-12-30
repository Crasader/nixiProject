//
//  AchievementComp.cpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#include "AchievementComp.h"
#include "AppUtil.h"


void AchievementItem::config(Value json) {
    this->setId(json[0].asInt());
    this->setGoal(json[1].asInt());
    this->setNum(json[2].asInt());
}


#pragma mark -

int AchievementComp::itemCount() {
    return _items->count();
}

AchievementComp::~AchievementComp() {
    CC_SAFE_DELETE(_items);
}

void AchievementComp::init_template(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    CCArray* items = CCArray::create();
    std::vector<std::string> keys = json.getMemberNames();
    for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
        std::string key = (std::string)* iterator;
        CSJson::Value value = json.get(key, CSJson::nullValue);
        if (value != CSJson::nullValue) {
            string desc = value["desc"].asString();
            CSJson::Value list = value["list"];
            int count = list.size();
            for (int i = 0; i < count; i++) {
                AchievementItem* item = AchievementItem::create();
                item->config(list[i]);
                CCString* strDesc = CCString::createWithFormat(desc.c_str(), item->getGoal());
                item->setDesc(strDesc->getCString());
                items->addObject(item);
            }
        }
    }
    
    CC_SAFE_RELEASE(_items);
    _items = items;
    CC_SAFE_RETAIN(_items);
    CCLOG("There are %d achievements", _items->count());
}

void AchievementComp::update_user_accumulated(CSJson::Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    CCDictionary* dic = AppUtil::dictionary_with_json(json);
    CC_SAFE_RELEASE(_accumulated);
    _accumulated = dic;
    CC_SAFE_RETAIN(_accumulated);
}

void AchievementComp::update_user_achieved(CSJson::Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    CCArray* arr = AppUtil::array_with_json(json);
    CC_SAFE_RELEASE(_achieved);
    _achieved = arr;
    CC_SAFE_RETAIN(_achieved);
}

void AchievementComp::update_user_finished(CSJson::Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    CCArray* arr = AppUtil::array_with_json(json);
    CC_SAFE_RELEASE(_finished);
    _finished = arr;
    CC_SAFE_RETAIN(_finished);
}

AchievementItem* AchievementComp::fetchItem(int idx) {
    return (AchievementItem* )_items->objectAtIndex(idx);
}

int AchievementComp::fetchItemState(int id) {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_finished, pObj) {
        CCInteger* value = (CCInteger* )pObj;
        if (value->getValue() == id) {
            return -1;
        }
    }
    CCARRAY_FOREACH(_achieved, pObj) {
        CCInteger* value = (CCInteger* )pObj;
        if (value->getValue() == id) {
            return 1;
        }
    }
    
    return 0;
}

int AchievementComp::fetchItemAccumulate(int id) {
    CCString* strAchieveId = CCString::createWithFormat("%d", id / 100);
    CCInteger* accumulate = (CCInteger* )_accumulated->objectForKey(strAchieveId->getCString());
    if (accumulate) {
        return accumulate->getValue();
    }
    else {
        return 0;
    }
}

bool AchievementComp::init() {
    
    return true;
}

