//
//  HomeComp.cpp
//  tiegao
//
//  Created by mac on 16/8/5.
//
//

#include "HomeComp.h"
#include "AppUtil.h"

#pragma mark - Export

bool HomeComp::has_init_house_template() {
    return (_houseTemplate != NULL);
}

#pragma mark - Inport

void HomeComp::init_house_template(Value json) {
    if (json.type() == nullValue) {
        CCLOG("HomeComp::init_house_template() - error.");
        return;
    }
    
    CCDictionary* dic = AppUtil::dictionary_with_json(json);
//    CCArray* keys = dic->allKeys();
//    AppUtil::sort_string_array(keys);
    CCArray* arr = CCArray::create();
    int count = dic->count();
    for (int i = 1; i <= count; i++) {
        CCString* key = CCString::createWithFormat("%d", i);
        arr->addObject(dic->objectForKey(key->getCString()));
    }
    
    this->setHouseTemplate(arr);
}

void HomeComp::replace_home_info(Value json) {
    if (json.type() == nullValue) {
        CCLOG("HomeComp::replace_home_info() - error.");
        return;
    }
    
    this->setCurHouse(json["cur_house"].asString());
    this->setHouseUser(AppUtil::array_with_json(json["houses"]));
    this->setScores(AppUtil::dictionary_with_json(json["scores"]));
}

#pragma mark - Supper

HomeComp::~HomeComp() {
    CC_SAFE_DELETE(_houseTemplate);
    CC_SAFE_DELETE(_houseUser);
    CC_SAFE_DELETE(_scores);
}

bool HomeComp::init() {
    _houseTemplate = NULL;
    _houseUser = NULL;
    
    return true;
}

#pragma mark - Inner

