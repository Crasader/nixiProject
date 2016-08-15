//
//  OperationComp.cpp
//  tiegao
//
//  Created by mac on 16/8/3.
//
//

#include "OperationComp.h"
#include "AppUtil.h"
#include "Shower.h"
#include "ConfigManager.h"

#pragma mark - Export

unsigned OperationComp::cur_purchase_achievement_template_index() {
    int templateMax = _purchaseAchievementTemplate->count();
    if (_purchaseAchievementUser) {
        return MIN(_purchaseAchievementUser->count(), templateMax - 1);
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
    
    int countGroup = arr->count();
    CCArray* suits = CCArray::create();
    for (int i = 0; i < countGroup; i++) {
        CCDictionary* preDress = CCDictionary::create();
        preDress->setObject(CCInteger::create(10000), "1");
        preDress->setObject(CCInteger::create(20000), "2");
        preDress->setObject(CCInteger::create(30000), "3");
        preDress->setObject(CCInteger::create(40000), "4");
        preDress->setObject(CCInteger::create(50000), "5");
        preDress->setObject(CCInteger::create(60000), "6");
        preDress->setObject(CCInteger::create(80000), "8");
        preDress->setObject(CCInteger::create(90000), "9");
        preDress->setObject(CCInteger::create(100000), "10");
        
        CCDictionary* part7 = CCDictionary::create();
        preDress->setObject(part7, "7");
        
        part7->setObject(CCInteger::create(70000), "11");
        part7->setObject(CCInteger::create(70000), "12");
        part7->setObject(CCInteger::create(70000), "13");
        part7->setObject(CCInteger::create(70000), "14");
        part7->setObject(CCInteger::create(70000), "15");
        part7->setObject(CCInteger::create(70000), "16");
        part7->setObject(CCInteger::create(70000), "17");
        part7->setObject(CCInteger::create(70000), "18");
        part7->setObject(CCInteger::create(70000), "19");
        part7->setObject(CCInteger::create(70000), "20");
        
        suits->addObject(preDress);
    }
    
    CCDictionary* allClothes = CONFIG->clothes();
    for (int i = 0; i < countGroup; i++) {
        CCDictionary* item = (CCDictionary*)_purchaseAchievementTemplate->objectAtIndex(i);
        CCDictionary* group = (CCDictionary*)suits->objectAtIndex(i);
        
        CCArray* clothes = (CCArray*)item->objectForKey("clothes");
        int count = clothes->count();
        for (int j = 0; j < count; j++) {
            int clothesId = ((CCInteger*)clothes->objectAtIndex(j))->getValue();
            int itemPart = clothesId / 10000;
            
            CCString* partKey = CCString::createWithFormat("%d", itemPart);
            CCString* id = CCString::createWithFormat("%d", clothesId);
            
            CCArray* partClothes = (CCArray*)allClothes->objectForKey(itemPart);
            CCObject* pObj2 = NULL;
            CCARRAY_FOREACH(partClothes, pObj2) {
                CCDictionary* clothesItem = (CCDictionary*)pObj2;
                CCString* clothesItemId = (CCString*)clothesItem->objectForKey("id");
                if (clothesItemId->compare(id->getCString()) == 0) {
                    if (itemPart == 7) {
                        CCDictionary* part7 = (CCDictionary*)group->objectForKey("7");
                        part7->setObject(CCInteger::create(clothesId), partKey->getCString());
                    }
                    else {
                        group->setObject(CCInteger::create(clothesId), partKey->getCString());
                    }
                    break;
                }
            }
        }
    }
    
    this->setPASuits(suits);
//    Shower* shower = Shower::create();
//    shower->ondress((CCDictionary*)suits->objectAtIndex(0));
//    CCDirector::sharedDirector()->getRunningScene()->addChild(shower);
}

void OperationComp::init_costs(Value json) {
    if (json.type() == nullValue) {
        CCLOG("OperationComp::init_costs() json object error.");
        return;
    }
    
    this->setSignleCost(json["single_cost"].asInt());
    this->setTenCost(json["ten_cost"].asInt());
}

void OperationComp::replace_purchase_achievement(Value json) {
    if (json.type() == nullValue) {
        CCLOG("OperationComp::replace_purchase_achievement() json object error.");
        return;
    }
    
    this->setPurchasedTotal(json["total"].asInt());
    this->setPurchaseAchievementUser(AppUtil::array_with_json(json["record"]));
}

void OperationComp::init_gashapon_template(Value json) {
    if (json.type() == nullValue) {
        CCLOG("OperationComp::init_gashapon_template() json object error.");
        return;
    }
    /* item::
     "71724": {
     "price": 300,
     "part": 11,
     "id": "71724",
     "group": 1
     },
     */
    
    CCDictionary* dic = AppUtil::dictionary_with_json(json);
    CCDictElement* pElem = NULL;
    int countGroup = 0;
    CCDICT_FOREACH(dic, pElem) {
        CCDictionary* item = (CCDictionary*)pElem->getObject();
        int itemGroup = ((CCInteger*)item->objectForKey("group"))->getValue();
        if (itemGroup > countGroup) {
            countGroup = itemGroup;
        }
    }
    
    CCArray* suits = CCArray::create();
    for (int i = 0; i < countGroup; i++) {
        CCDictionary* preDress = CCDictionary::create();
        preDress->setObject(CCInteger::create(10000), "1");
        preDress->setObject(CCInteger::create(20000), "2");
        preDress->setObject(CCInteger::create(30000), "3");
        preDress->setObject(CCInteger::create(40000), "4");
        preDress->setObject(CCInteger::create(50000), "5");
        preDress->setObject(CCInteger::create(60000), "6");
        preDress->setObject(CCInteger::create(80000), "8");
        preDress->setObject(CCInteger::create(90000), "9");
        preDress->setObject(CCInteger::create(100000), "10");
        
        CCDictionary* part7 = CCDictionary::create();
        preDress->setObject(part7, "7");
        
        part7->setObject(CCInteger::create(70000), "11");
        part7->setObject(CCInteger::create(70000), "12");
        part7->setObject(CCInteger::create(70000), "13");
        part7->setObject(CCInteger::create(70000), "14");
        part7->setObject(CCInteger::create(70000), "15");
        part7->setObject(CCInteger::create(70000), "16");
        part7->setObject(CCInteger::create(70000), "17");
        part7->setObject(CCInteger::create(70000), "18");
        part7->setObject(CCInteger::create(70000), "19");
        part7->setObject(CCInteger::create(70000), "20");
        
        
        suits->addObject(preDress);
    }
    
    CCDictElement* pElem2 = NULL;
    CCDICT_FOREACH(dic, pElem2) {
        CCDictionary* item = (CCDictionary*)pElem2->getObject();
        int itemGroup = ((CCInteger*)item->objectForKey("group"))->getValue();
        int itemPart = ((CCInteger*)item->objectForKey("part"))->getValue();
        
        CCDictionary* group = (CCDictionary*)suits->objectAtIndex(itemGroup - 1);
        CCString* partKey = CCString::createWithFormat("%d", itemPart);
        CCString* id = ccs(pElem2->getStrKey());
        if (itemPart < 11) {
            group->setObject(CCInteger::create(id->intValue()), partKey->getCString());
        }
        else {
            CCDictionary* part7 = (CCDictionary*)group->objectForKey("7");
            part7->setObject(CCInteger::create(id->intValue()), partKey->getCString());
        }
    }
    
    this->setSuits(suits);
    this->setGashaponTemplate(dic);
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

void OperationComp::init_extra(Value json){
    if (json.type() == nullValue) {
        CCLOG("OperationComp::init_extra() json object error.");
        return;
    }
    
    this->setExtra(json.asInt());
    CCLog("<><><><><> extra == %d", _extra);
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

