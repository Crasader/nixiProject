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

bool TrystComp::isOngoing() {
    return false;
}

bool TrystComp::hasInitAchvTemplate() {
    return mysteryAchvTemplate != nullptr;
}

CCArray* TrystComp::fetchAchvTemplate(const char *category) {
    return dynamic_cast<CCArray*>(mysteryAchvTemplate->objectForKey(category));
}

int TrystComp::userRatingOfCategory(const char* category) {
    CCDictionary* categoryInfo = dynamic_cast<CCDictionary*>(mysteryUserdata->objectForKey(category));
    CCInteger* value = dynamic_cast<CCInteger*>(categoryInfo->objectForKey("rating"));
    return value->getValue();
}

int TrystComp::userAchvStateOfCategory(const char *category, const char *achvId) {
    CCDictionary* categoryInfo = dynamic_cast<CCDictionary*>(mysteryUserdata->objectForKey(category));
    CCDictionary* achv = dynamic_cast<CCDictionary*>(categoryInfo->objectForKey("achv"));
    CCInteger* state = dynamic_cast<CCInteger*>(achv->objectForKey(achvId));
    return state->getValue();
}

// Import
TrystComp::~TrystComp() {
    CC_SAFE_DELETE(mysteryAchvTemplate);
    CC_SAFE_DELETE(mysteryUserdata);
}

bool TrystComp::init() {
    mysteryAchvTemplate = nullptr;
    mysteryUserdata = nullptr;
    
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
    
    CC_SAFE_RELEASE(mysteryAchvTemplate);
    mysteryAchvTemplate = dict;
    mysteryAchvTemplate->retain();
}

void TrystComp::update_user_data(Value json) {
    if (json.type() == nullValue) {
        CCLOG("<ERROR> TrystComp::update_user_data() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(mysteryUserdata);
    mysteryUserdata = AppUtil::dictionary_with_json(json);
    mysteryUserdata->retain();
}

// Inner

