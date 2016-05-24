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
    if (!json.isObject()) {
        CCLOG("LoginComp::init_with_json() json object error.");
        return;
    }
    
    _nickname = json["nickname"].asString();
    _collected = json["collected"].asInt();
    AppUtil::setNewObj(_ondress, AppUtil::dictionary_with_json(json["clothes"]));
}