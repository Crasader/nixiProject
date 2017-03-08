//
//  FlashSaleComp.cpp
//  tiegao
//
//  Created by ilakkey on 2017/3/8.
//
//

#include "FlashSaleComp.h"


FlashSaleInfo::~FlashSaleInfo() {
    CC_SAFE_DELETE(_clothes);
}

void FlashSaleInfo::config(CSJson::Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    this->setId(json["id"].asInt());
    this->setPhase(json["phase"].asInt());
    this->setType(json["type"].asInt());
    this->setDeadline(json["deadline"].asInt64());
    this->setOldPrice(json["old_price"].asInt());
    this->setDiscount(json["discount"].asFloat());
    this->setNowPrice(json["now_price"].asInt());
    
    CSJson::Value clothes = json["clothes"];
    int count = clothes.size();
    CCArray* arr = CCArray::createWithCapacity(count);
    for (int i = 0; i < count; i++) {
        arr->addObject(CCInteger::create(clothes[i].asInt()));
    }
    this->setClothes(arr);
}
