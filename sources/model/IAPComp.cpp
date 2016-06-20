//
//  IAPComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "IAPComp.h"
#include "AppUtil.h"

ProductItem::~ProductItem() {
}

bool ProductItem::init() {
    return true;
}

void ProductItem::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("ProductItem::init_with_json() json object error.");
        return;
    }
    
    id = json["id"].asString();
    diam = json["diam"].asInt();
    money = json["money"].asInt();
}

void ProductItem::print_self() {
    CCLOG("====== ProductItem: < %s > ======", id.c_str());
    CCLOG("    diam = %d", diam);
    CCLOG("    money = %d", money);
}

// --------------------------------------------------

IAPComp::~IAPComp() {
    CC_SAFE_DELETE(_products);
}

bool IAPComp::init() {
    _products = NULL;

    return true;
}

void IAPComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("IAPComp::init_with_json() json object error.");
        return;
    }
    
    CCDictionary* dic = CCDictionary::create();
    std::vector<std::string> keys = json.getMemberNames();
    for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
        std::string key = (std::string)* iterator;
        CSJson::Value value = json.get(key, json.jsonNull);
        if (value != value.jsonNull) {
            ProductItem* item = ProductItem::create();
            item->init_with_json(value);
            dic->setObject(item, key);
        }
    }
    
    CCArray* dic_keys = dic->allKeys();
    AppUtil::sort_string_array(dic_keys);
    CCArray* arr = CCArray::createWithCapacity(dic_keys->count());
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(dic_keys, pObj) {
        CCString* key = (CCString* )pObj;
        CCLOG("key = %s", key->getCString());
        arr->addObject(dic->objectForKey(key->getCString()));
    }
    CC_SAFE_RELEASE(_products);
    _products = arr;
    _products->retain();
    
    this->print_all_products();
}

bool IAPComp::has_init_products() {
    return (_products != NULL);
}

CCArray* IAPComp::products() {
    return _products;
}

void IAPComp::print_all_products() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_products, pObj) {
        ProductItem* item = (ProductItem* )pObj;
        item->print_self();
    }
}

