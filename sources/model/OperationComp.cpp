//
//  OperationComp.cpp
//  tiegao
//
//  Created by mac on 16/8/3.
//
//

#include "OperationComp.h"
#include "AppUtil.h"

#pragma mark - Export

bool OperationComp::has_init_purchase_achievement_template() {
    return (_purchaseAchievementTemplate != NULL);
}

#pragma mark - Inport

void OperationComp::init_purchase_achievement_template(Value json) {
    if (json.type() == nullValue) {
        CCLOG("OperationComp::init_purchase_achievement_template() json object error.");
        return;
    }
    /* item
    "uri": "19111",
    "id": "1",
    "goal": 50
    */
    CCDictionary* dic = AppUtil::dictionary_with_json(json);
    CCArray* keys = dic->allKeys();
    AppUtil::sort_string_array(keys);
    CCArray* arr = CCArray::create();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(arr, pObj) {
        CCString* key = (CCString*)pObj;
        arr->addObject(dic->objectForKey(key->getCString()));
    }
    this->setPurchaseAchievementTemplate(arr);
}

void OperationComp::replace_purchase_achievement(Value json) {
    if (json.type() == nullValue) {
        CCLOG("OperationComp::replace_purchase_achievement() json object error.");
        return;
    }
    
    this->setPurchasedTotal(json["total"].asInt());
    this->setPurchaseAchievementUser(AppUtil::dictionary_with_json(json["pAchievement"]));
}

#pragma mark - Supper

OperationComp::~OperationComp() {
    CC_SAFE_DELETE(_purchaseAchievementTemplate);
    CC_SAFE_DELETE(_purchaseAchievementUser);
}

bool OperationComp::init() {
    _purchaseAchievementTemplate = NULL;
    _purchaseAchievementUser = NULL;
    
    return true;
}

#pragma mark - Inner

