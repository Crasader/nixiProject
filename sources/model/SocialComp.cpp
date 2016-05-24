//
//  SocialComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "SocialComp.h"

SocialComp::~SocialComp() {
}

bool SocialComp::init() {
    
    return true;
}

void SocialComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("LoginComp::init_with_json() json object error.");
        return;
    }
    
}