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

unsigned OperationComp::cur_purchase_achievement_template_index() {
    if (_purchaseAchievementUser) {
        return _purchaseAchievementUser->count();
    }
    
    return 0;
}

bool OperationComp::has_init_purchase_achievement_template() {
    return (_purchaseAchievementTemplate != NULL);
}

bool OperationComp::has_init_gashapon_template() {
    return (_gashaponTemplate != NULL);
}

bool OperationComp::has_taken_pAchievement(string &id) {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_purchaseAchievementUser, pObj) {
        CCString* strId = (CCString*)pObj;
        if (strId->compare(id.c_str()) == 0) {
            return true;
        }
    }
    
    return false;
}

bool OperationComp::has_owned_gashapon(string &id) {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_gashaponUser, pObj) {
        CCString* strId = (CCString*)pObj;
        if (strId->compare(id.c_str()) == 0) {
            return true;
        }
    }
    
    return false;
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
    CCARRAY_FOREACH(keys, pObj) {
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
    this->setPurchaseAchievementUser(AppUtil::array_with_json(json["pAchievement"]));
}

void OperationComp::init_gashapon_template(Value json) {
    if (json.type() == nullValue) {
        CCLOG("OperationComp::init_gashapon_template() json object error.");
        return;
    }
    /* item
     "uri": "19111",
     "price": 50,
     "id": "1"
     */
    CCDictionary* dic = AppUtil::dictionary_with_json(json);
    CCArray* keys = dic->allKeys();
    AppUtil::sort_string_array(keys);
    CCArray* arr = CCArray::create();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(keys, pObj) {
        CCString* key = (CCString*)pObj;
        arr->addObject(dic->objectForKey(key->getCString()));
    }
    this->setGashaponTemplate(arr);
}

void OperationComp::replace_gashapon_user(Value json) {
    if (json.type() == nullValue) {
        CCLOG("OperationComp::replace_gashapon_user() json object error.");
        return;
    }
    
    this->setPiece(json["piece"].asInt());
    this->setFreePoint(json["free_point"].asInt64());
    this->setGashaponUser(AppUtil::array_with_json(json["owned"]));
}

#pragma mark - Supper

OperationComp::~OperationComp() {
    CC_SAFE_DELETE(_purchaseAchievementTemplate);
    CC_SAFE_DELETE(_purchaseAchievementUser);
    CC_SAFE_DELETE(_gashaponTemplate);
    CC_SAFE_DELETE(_gashaponUser);
}

bool OperationComp::init() {
    _purchaseAchievementTemplate = NULL;
    _purchaseAchievementUser = NULL;
    _gashaponTemplate = NULL;
    _gashaponUser = NULL;
    
    return true;
}

#pragma mark - Inner

