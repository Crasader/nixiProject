//
//  WelfareComp.cpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#include "WelfareComp.h"
#include "AppUtil.h"

void WelfareStatisItem::config(Value json) {
    this->id = json["id"].asString();
    this->goal = json["goal"].asInt();
    this->status = json["status"].asInt();
}

void WelfareItem::config(Value json) {
    this->id = json["id"].asString();
    this->name = json["name"].asString();
    this->status = json["status"].asInt();
    this->progress = 2;//json["progress"].asInt();
    this->goal = json["goal"].asInt();
    this->rewardType = json["type"].asString();
    this->rewardNum = json["num"].asInt();
}


#pragma mark -

int WelfareComp::obtainTotalProgress() {
    return _totalProgress;
}

WelfareStatisItem* WelfareComp::fetchStatisItem(int idx) {
    CCString* strId = CCString::createWithFormat("%d", idx + 1);
    return (WelfareStatisItem* )_statis->objectForKey(strId->getCString());
}

int WelfareComp::itemCount() {
    return _items->count();
}

WelfareItem* WelfareComp::fetchWelfareItem(int idx) {
//    CCString* strId = CCString::createWithFormat("%d", idx + 1);
//    return (WelfareItem* )_items->objectForKey(strId->getCString());
    return (WelfareItem* )(_items->objectAtIndex(idx));
}

WelfareComp::~WelfareComp() {
    CC_SAFE_DELETE(_statis);
    CC_SAFE_DELETE(_items);
}

void WelfareComp::update_statis(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    _totalProgress = json["total_progress"].asInt();
    
    CCDictionary* statis = CCDictionary::create();
    
    for (int i = 0; i < 3; i++) {
        WelfareStatisItem* item = WelfareStatisItem::create();
        CCString* strId = CCString::createWithFormat("%d", i + 1);
        item->config(json[strId->getCString()]);
        statis->setObject(item, strId->getCString());
    }
    
    CC_SAFE_RELEASE(_statis);
    _statis = statis;
    CC_SAFE_RETAIN(_statis);
}

void WelfareComp::update_items(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    CCArray* items = CCArray::create();
    std::vector<std::string> keys = json.getMemberNames();
    for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
        std::string key = (std::string)* iterator;
        CSJson::Value value = json.get(key, value.jsonNull);
        
        WelfareItem* item = WelfareItem::create();
        item->config(value);
//        CCLOG("id = %s", item->id.c_str());
        items->addObject(item);
    }
    
//    int size = items->count();
//    for (int i = 0; i < size - 1; ++i) {
//        WelfareItem* pA = (WelfareItem* )items->objectAtIndex(i);
//        for (int j = i + 1; j < size; ++j) {
//            WelfareItem* pB = (WelfareItem* )items->objectAtIndex(j);
//            if (pA->status > pB->status) {
//                items->exchangeObjectAtIndex(i, j);
//            }
//        }
//    }
    
    CC_SAFE_RELEASE(_items);
    _items = items;
//    CC_SAFE_RETAIN(_items);
    _items->retain();
}

bool WelfareComp::init() {
    
    return true;
}

