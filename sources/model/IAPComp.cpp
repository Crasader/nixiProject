//
//  IAPComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "IAPComp.h"
#include "AppUtil.h"

IAPComp::~IAPComp() {
    CC_SAFE_DELETE(_products);
}

bool IAPComp::init() {

    return true;
}

void IAPComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("IAPComp::init_with_json() json object error.");
        return;
    }
    
    CCDictionary* dic = AppUtil::dictionary_with_json(json);
    CCArray* keys = dic->allKeys();
    AppUtil::sort_string_array(keys);
    CCArray* arr = CCArray::createWithCapacity(keys->count());
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(keys, pObj) {
        CCString* key = (CCString* )pObj;
        CCLOG("key = %s", key->getCString());
        arr->addObject(dic->objectForKey(key->getCString()));
    }
    
    CC_SAFE_RELEASE(_products);
    _products = arr;
    _products->retain();
    CCLOG("There are %d products!~", _products->count());
}
