//
//  MysteryComp.cpp
//  tiegao
//
//  Created by mac on 16/10/22.
//
//

#include "MysteryComp.h"
#include "AppUtil.h"

// Export
CCArray* MysteryComp::fetchTemplate(const char *category) {
    return dynamic_cast<CCArray*>(mysteryTemplate);
}

// Import
MysteryComp::~MysteryComp() {
    CC_SAFE_DELETE(mysteryUserdata);
    CC_SAFE_DELETE(mysteryUserdata);
}

bool MysteryComp::init() {
    mysteryTemplate = nullptr;
    mysteryUserdata = nullptr;
    
    return true;
}

void MysteryComp::init_template(Value json) {
    if (json.type() == nullValue) {
        CCLOG("<ERROR> MysteryComp::init_template() json object error.");
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
    
    CC_SAFE_RELEASE(mysteryTemplate);
    mysteryTemplate = dict;
    mysteryTemplate->retain();
}

void MysteryComp::update_user_data(Value json) {
    if (json.type() == nullValue) {
        CCLOG("<ERROR> MysteryComp::update_user_data() json object error.");
        return;
    }
    
}

// Inner

