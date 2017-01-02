//
//  AchievementComp.cpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#include "AchievementComp.h"
#include "AppUtil.h"

const int ECHELON_ONE_ORDER = 0;

void AchievementItem::config(Value json) {
    this->setId(json[0].asInt());
    this->setGoal(json[1].asInt());
    this->setNum(json[2].asInt());
}


#pragma mark -

int AchievementComp::itemCount() {
    return _sortedItemKeys->count();
}

AchievementComp::~AchievementComp() {
    CC_SAFE_DELETE(_items);
    CC_SAFE_DELETE(_sortedItemKeys);
}

void AchievementComp::init_template(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    CCDictionary* items = CCDictionary::create();
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
                item->setStatus(0);
                if (i == 0) {
                    item->setOrder(ECHELON_ONE_ORDER);
                }
                else {
                    item->setOrder(item->getId());
                }
                items->setObject(item, item->getId());
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
    
//    CCArray* arr = AppUtil::array_with_json(json);
//    CC_SAFE_RELEASE(_achieved);
//    _achieved = arr;
//    CC_SAFE_RETAIN(_achieved);
    
    _newCount = 0;
    int count = json.size();
    for (int i = 0; i < count; i++) {
        int id = json[i].asInt();
        AchievementItem* item = (AchievementItem* )_items->objectForKey(id);
        if (item) {
            item->setStatus(1);
            _newCount++;
        }
    }
}

void AchievementComp::update_user_finished(CSJson::Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
//    CCArray* arr = AppUtil::array_with_json(json);
//    CC_SAFE_RELEASE(_finished);
//    _finished = arr;
//    CC_SAFE_RETAIN(_finished);
    int count = json.size();
    for (int i = count - 1; i >= 0; i--) {
        int id = json[i].asInt();
        AchievementItem* item = (AchievementItem* )_items->objectForKey(id);
        if (item) {
            item->setStatus(-1);
            item->setOrder(item->getId() * 10);
            //
            AchievementItem* nextItem = (AchievementItem* )_items->objectForKey(item->getId() + 1);
            if (nextItem) {
                nextItem->setOrder(ECHELON_ONE_ORDER);
            }
        }
    }
}

void AchievementComp::update_sorted_item_keys() {
    CCArray* keys = _items->allKeys();
    int count = keys->count();
    
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            int keyI = ( (CCInteger* )keys->objectAtIndex(i) )->getValue();
            int keyJ = ( (CCInteger* )keys->objectAtIndex(j) )->getValue();
            AchievementItem* itemI = (AchievementItem* )_items->objectForKey(keyI);
            AchievementItem* itemJ = (AchievementItem* )_items->objectForKey(keyJ);
            if (itemJ->getOrder() < itemI->getOrder()) {
                keys->exchangeObjectAtIndex(i, j);
            }
        }
    }
    
    CC_SAFE_RELEASE(_sortedItemKeys);
    _sortedItemKeys = keys;
    CC_SAFE_RETAIN(_sortedItemKeys);
//    for (int i = 0; i < count; i++) {
//        CCInteger* id = (CCInteger* )_sortedItemKeys->objectAtIndex(i);
//        CCLOG("index = %d, id = %d", i, id->getValue());
//    }
}

int AchievementComp::getNewCount() {
    return _newCount;
}

AchievementItem* AchievementComp::fetchItem(int idx) {
    CCInteger* itemKey = (CCInteger* )_sortedItemKeys->objectAtIndex(idx);
    return (AchievementItem* )_items->objectForKey(itemKey->getValue());
}

int AchievementComp::fetchItemIndex(int id) {
    int rtn = -1;
    int count = _sortedItemKeys->count();
    for (int i = 0; i < count; i++) {
        CCInteger* idd = (CCInteger* )_sortedItemKeys->objectAtIndex(i);
        if (idd->getValue() == id) {
            rtn = i;
            break;
        }
    }
    
    return rtn;
}

//int AchievementComp::fetchItemState(int id) {
//    CCObject* pObj = NULL;
//    CCARRAY_FOREACH(_finished, pObj) {
//        CCInteger* value = (CCInteger* )pObj;
//        if (value->getValue() == id) {
//            return -1;
//        }
//    }
//    CCARRAY_FOREACH(_achieved, pObj) {
//        CCInteger* value = (CCInteger* )pObj;
//        if (value->getValue() == id) {
//            return 1;
//        }
//    }
//    
//    return 0;
//}

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
    _newCount = 0;
    
    return true;
}

