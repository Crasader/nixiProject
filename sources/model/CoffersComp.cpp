//
//  CoffersComp.cpp
//  tiegao
//
//  Created by mac on 16/7/11.
//
//

#include "CoffersComp.h"
#include "AppUtil.h"

#pragma mark - Export

void CoffersComp::produce() {
    collected += 1;
}

void CoffersComp::reset_collected() {
    collected = 0;
}

#pragma mark -

CoffersComp::~CoffersComp() {
    CC_SAFE_DELETE(_companyTemplate);
}

bool CoffersComp::init() {
    profit = 0;
    top = 0;
    collected = 0;
    _hasInitTemplate = false;
    return true;
}

void CoffersComp::init_company_template(Value json) {
    if (json.type() == nullValue) {
        CCLOG("CoffersComp::init_company_template() json object error.");
        return;
    }
    
    CCDictionary* dic = AppUtil::dictionary_with_json(json);
    CCArray* allKeys = dic->allKeys();
    AppUtil::sort_string_array(allKeys);
    CCObject* pObj = NULL;
    CCArray* arr = CCArray::create();
    CCARRAY_FOREACH(allKeys, pObj) {
        CCString* key = (CCString*)pObj;
        arr->addObject(dic->objectForKey(key->getCString()));
    }
    
    this->setCompanyTemplate(arr);
    this->setHasInitTemplate(true);
}

void CoffersComp::replace_user_data(Value json) {
    if (json.type() == nullValue) {
        CCLOG("CoffersComp::replace_user_data() json object error.");
        return;
    }
    
    profit = json["profit"].asInt();
    top = json["top"].asInt();
    this->setCompanyRewardRecord(AppUtil::array_with_json(json["record"]));
}

