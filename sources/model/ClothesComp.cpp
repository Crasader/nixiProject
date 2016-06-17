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
}

bool ClothesComp::init() {

    return true;
}

void ClothesComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("ClothesComp::init_with_json() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(_clothes);
    _clothes = AppUtil::dictionary_with_json(json);
    _clothes->retain();
}

void ClothesComp::init_clothestemp(CSJson::Value json) {
    if (!json.isObject()) {
        CCLOG("ClothesComp::init_clothestemp() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(_myClothesTemp);
    _myClothesTemp = AppUtil::dictionary_with_json(json);
    _myClothesTemp->retain();
    this->print_dress();
}

CCDictionary* ClothesComp::MyClothesTemp(){
    return _myClothesTemp;
}

void ClothesComp::print_dress(){
    CCDictElement* pElem = NULL;
    CCDICT_FOREACH(_myClothesTemp, pElem) {
        const char* key = pElem->getStrKey();
        CCInteger* value = (CCInteger* )pElem->getObject();
        CCLOG("_myClothesTemp part<%s> := %d", key, value->getValue());
    }
}
