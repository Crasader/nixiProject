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
    CC_SAFE_DELETE(_ornaments);
}

bool ShowComp::init() {
    _ondress = NULL;
    _ornaments = NULL;
    isadd = 0;
    
    return true;
}

void ShowComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("ShowComp::init_with_json() json is not object.");
        return;
    }
    
    _nickname = json["nickname"].asString();
    _id = json["id"].asString();
    _collected = json["collected"].asInt();
    _competition = json["competition"].asInt();
    _ranking = json["ranking"].asInt();
    _reset = json["reset"].asInt();
    
    {
        CC_SAFE_RELEASE(_ondress);
        _ondress = AppUtil::dictionary_with_json(json["ondress"]);
        _ondress->retain();
        
        CC_SAFE_RELEASE(_ornaments);
        _ornaments = AppUtil::dictionary_with_json(json["ornaments"]);
        _ornaments->retain();
        
        _ondress->setObject(_ornaments, "7");
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

int ShowComp::competition() {
    return _competition;
}

int ShowComp::ranking() {
    return _ranking;
}

string ShowComp::getShowID(){
    return _id;
}

int ShowComp::resetCost(){
    return _reset;
}

