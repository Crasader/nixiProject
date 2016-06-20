//
//  ShowComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "ShowComp.h"
#include "AppUtil.h"

ShowComp::~ShowComp() {
    CC_SAFE_DELETE(_ondress);
}

bool ShowComp::init() {
    _ondress = NULL;
    
    return true;
}

void ShowComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("ShowComp::init_with_json() json is not object.");
        return;
    }
    
    _nickname = json["nickname"].asString();
    _collected = json["collected"].asInt();
    {
        CC_SAFE_RELEASE(_ondress);
        _ondress = AppUtil::dictionary_with_json(json["ondress"]);
        _ondress->retain();
        this->print_dress();
    }
}

void ShowComp::print_dress() {
    CCLOG("My name is [%s].", _nickname.c_str());
    CCDictElement* pElem = NULL;
    CCDICT_FOREACH(_ondress, pElem) {
        const char* key = pElem->getStrKey();
        CCInteger* value = (CCInteger* )pElem->getObject();
        CCLOG("part<%s> := %d", key, value->getValue());
    }
}

CCDictionary* ShowComp::ondress(){
    return _ondress;
}

const char* ShowComp::nickname() {
    return _nickname.c_str();
}

int ShowComp::collected() {
    return _collected;
}

