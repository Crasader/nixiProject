//
//  TrystComp.cpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#include "TrystComp.h"
#include "AppUtil.h"

// Export
TrystUserdata::~TrystUserdata() {}

TrystUserdata* TrystUserdata::create(CSJson::Value json) {
    TrystUserdata* rtn = new TrystUserdata();
    if (rtn) {
        rtn->status = static_cast<TrystStatus>(json["status"].asInt());
        rtn->curTrystId = json["id"].asString();
        rtn->timeSpan = json["seconds_span"].asInt();
        rtn->leftTime = json["left_time"].asInt();
    }
    
    return rtn;
}

// -----------------------------------------------------
// -----------------------------------------------------

// Export

bool TrystComp::isOngoing() {
    return false;
}

bool TrystComp::hasInitAchvTemplate() {
    return trystTemplate != nullptr;
}

CCArray* TrystComp::fetchAchvTemplate(const char *category) {
    return dynamic_cast<CCArray*>(trystTemplate->objectForKey(category));
}


// Import
TrystComp::~TrystComp() {
    CC_SAFE_DELETE(trystTemplate);
    CC_SAFE_DELETE(userData);
}

bool TrystComp::init() {
    trystTemplate = nullptr;
    userData = nullptr;
    
    return true;
}

void TrystComp::init_template(Value json) {
    if (json.type() == nullValue) {
        CCLOG("<ERROR> TrystComp::init_template() json object error.");
        return;
    }
    
    int size = json.size();
    CCArray* arrTemp = NULL;
    CCDictionary* dict = CCDictionary::create();
    for (int i = 0; i < size; i++) {
        string category = json[i]["category"].asString();
        if (dict->objectForKey(category) == NULL) {
            dict->setObject(CCArray::create(), category);
        }
        
        arrTemp = dynamic_cast<CCArray*>(dict->objectForKey(category));
        CCDictionary* item = AppUtil::dictionary_with_json(json[i]);
        arrTemp->addObject(item);
    }
    
    CC_SAFE_RELEASE(trystTemplate);
    trystTemplate = dict;
    trystTemplate->retain();
}

void TrystComp::update_user_data(Value json) {
    if (json.type() == nullValue) {
        CCLOG("<ERROR> TrystComp::update_user_data() json object error.");
        return;
    }
    
    TrystUserdata* userData = TrystUserdata::create(json);
    this->setUserData(userData);
}

// Inner

