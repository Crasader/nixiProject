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

AchievementItem* AchievementComp::fetchItem(int idx) {
    return (AchievementItem* )_items->objectAtIndex(idx);
}

bool AchievementComp::init() {
    
    return true;
}

