//
//  PurchaseComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "PurchaseComp.h"
#include "AppUtil.h"

#pragma mark - Export

bool PurchaseComp::has_init_products() {
    return (_products != NULL);
}

CCArray* PurchaseComp::products() {
    return _products;
}

bool PurchaseComp::has_purchased(const char *product_id) {
    bool rtn = false;
    CCObject* pObj = _deals->objectForKey(product_id);
    if (pObj != NULL) {
        CCInteger* value = (CCInteger* )pObj;
        if (value->getValue() > 0) {
            rtn = true;
        }
    }
    
    return rtn;
}

void PurchaseComp::print_all_products() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_products, pObj) {
        ProductItem* item = (ProductItem* )pObj;
        item->print_self();
    }
}

#pragma mark - Inherit

// ------------------------ ProductItem --------------------------

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

// ------------------------ MonthlyCardItem --------------------------

MonthlyCardItem::~MonthlyCardItem() {
}

bool MonthlyCardItem::init() {
    return true;
}

void MonthlyCardItem::init_with_json(const char* id, Value json) {
    if (!json.isObject()) {
        CCLOG("MonthlyCardItem::init_with_json() json object error.");
        return;
    }
    
    _cardID = string(id);
    _status = json["status"].asInt();
    _daysRest = json["left"].asInt();
}

void MonthlyCardItem::print_self() {
    CCLOG("====== MonthlyCardItem: < %s > ======", getCardID().c_str());
    CCLOG("    status = %d", getStatus());
    CCLOG("    days rest = %d", getDaysRest());
}

// ------------------------ PurchaseComp --------------------------

PurchaseComp::~PurchaseComp() {
    CC_SAFE_DELETE(_products);
    CC_SAFE_DELETE(_deals);
}

bool PurchaseComp::init() {
    _products = NULL;
    _deals = NULL;

    return true;
}

void PurchaseComp::init_products(Value json) {
    if (json.type() == nullValue) {
        CCLOG("PurchaseComp::init_products() json object error.");
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

void PurchaseComp::init_purchase(CSJson::Value json) {
    /*
     {
     "last": 0,
     "eb_times": 0,
     "eb_limit": 3,
     "deals": {
     
     },
     "deadline": 0,
     "ce_times": 0,
     "ce_limit": 3
     }
     */
    setEnergyBoughtTimes(json["eb_times"].asInt());
    setEnergyBuyLimit(json["eb_limit"].asInt());
    setCoinExchangedTimes(json["ce_times"].asInt());
    setCoinExchangeLimit(json["ce_limit"].asInt());
    
    setEnergyBoughtCost(json["energy_cost"].asInt());
    setEnergyBoughtGain(json["energy_gain"].asInt());
    setCoinExchangeCost(json["coin_cost"].asInt());
    setCoinExchangeGain(json["coin_gain"].asInt());
    
    CCDictionary* deals = AppUtil::dictionary_with_json(json["deals"]);
    setDeals(deals);
    
    CSJson::Value cards = json["cards"];
    if (cards.type() != CSJson::nullValue) {
        MonthlyCardItem* card1 = MonthlyCardItem::create();
        card1->init_with_json("1", cards["1"]);
        setMonthlyCard1(card1);
        card1->print_self();
        
        MonthlyCardItem* card2 = MonthlyCardItem::create();
        card2->init_with_json("2", cards["2"]);
        setMonthlyCard2(card2);
        card2->print_self();
    }
}

