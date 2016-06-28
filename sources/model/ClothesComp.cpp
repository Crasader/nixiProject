//
//  ClothesComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "ClothesComp.h"
#include "AppUtil.h"

ClothesComp::~ClothesComp() {
    CC_SAFE_DELETE(_clothes);
    CC_SAFE_DELETE(_myClothesTemp);
    CC_SAFE_DELETE(_dress);
    CC_SAFE_DELETE(_ornaments);
}

bool ClothesComp::init() {
    has_init_clothes = false;
    
    return true;
}

void ClothesComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("ClothesComp::init_with_json() json is not object.");
        return;
    }
    
    CC_SAFE_RELEASE(_clothes);
    _clothes = AppUtil::dictionary_with_json(json);
    _clothes->retain();
    has_init_clothes = true;
}

void ClothesComp::update_clothes(CSJson::Value json) {
    if (json.type() == nullValue) {
        CCLOG("ClothesComp::update_clothes() json is null.");
        return;
    }
    
//    std::vector<std::string> keys = json.getMemberNames();
//    for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
//        std::string key = (std::string)* iterator;
//        CSJson::Value value = json.get(key, json.jsonNull);
//        if (value != json.jsonNull) {
//            CCArray* arr = (CCArray* )_clothes->objectForKey(key);
//            arr->addObject(CCInteger::create(value.asInt()));
//        }
//    }
    int size = json.size();
    for (int i = 0; i < size; i++) {
        int cloth_id = json[i].asInt();
        int part = cloth_id / 10000;
        CCArray* arr = (CCArray* )_clothes->objectForKey(CCString::createWithFormat("%d", part)->getCString());
        arr->addObject(CCInteger::create(cloth_id));

    }
}

void ClothesComp::init_dressed(CSJson::Value json) {
    if (json.type() == nullValue) {
        CCLOG("ClothesComp::init_dressed() json object error.");
        return;
    }
    
    CC_SAFE_DELETE(_dress);
    _dress = AppUtil::dictionary_with_json(json["ondress"]);
    _dress->retain();
    
    CC_SAFE_RELEASE(_ornaments);
    _ornaments = AppUtil::dictionary_with_json(json["ornaments"]);
    _ornaments->retain();
    
    _dress->setObject(_ornaments, "7");
    
    this->copy_clothesTemp();
}

void ClothesComp::copy_clothesTemp(){
    CC_SAFE_RELEASE(_myClothesTemp);
    _myClothesTemp = CCDictionary::create();
    CCDictionary* dic = CCDictionary::create();
    CCDictElement* pElem = NULL;
    CCDICT_FOREACH(_dress, pElem) {
        const char* key = pElem->getStrKey();
        if (strcmp(key, "7") == 0) {
            CCDictElement* pElem2 = NULL;
            CCDictionary* pElemDic = (CCDictionary* )pElem->getObject();
            CCDICT_FOREACH(pElemDic, pElem2) {
                const char* key = pElem2->getStrKey();
                CCInteger* value = (CCInteger* )pElem2->getObject();
                dic->setObject(CCInteger::create(value->getValue()), key);
            }
            _myClothesTemp->setObject(dic, key);
        }else{
            CCInteger* value = (CCInteger* )pElem->getObject();
            _myClothesTemp->setObject(CCInteger::create(value->getValue()), key);
        }
    }
    _myClothesTemp->retain();
}

CCDictionary* ClothesComp::dress(){
    return _dress;
}

CCDictionary* ClothesComp::MyClothesTemp(){
    return _myClothesTemp;
}

bool ClothesComp::is_owned(int part, int cloth_id) {
    CCString* str_id = CCString::createWithFormat("%d", part);
    bool is_owned = this->is_owned(str_id->getCString(), cloth_id);
    return is_owned;
}

bool ClothesComp::is_owned(const char* part, int cloth_id) {
    CCArray* part_clothes = (CCArray* )_clothes->objectForKey(part);
    for (int i = 0; i < part_clothes->count(); i++) {
        CCInteger* cloth_Integer = (CCInteger* )part_clothes->objectAtIndex(i);
        if (cloth_Integer->getValue() == cloth_id) {
            return true;
        }
    }
    return false;
}

void ClothesComp::print_dress(){
    CCDictElement* pElem = NULL;
    CCDICT_FOREACH(_myClothesTemp, pElem) {
        const char* key = pElem->getStrKey();
        CCInteger* value = (CCInteger* )pElem->getObject();
        CCLOG("_myClothesTemp part<%s> := %d", key, value->getValue());
    }
}
