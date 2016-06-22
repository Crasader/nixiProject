//
//  SigninComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "SigninComp.h"

SigninComp::~SigninComp() {
}

bool SigninComp::init() {

    return true;
}

void SigninComp::init_with_json(Value json) {
    if (json.type() == nullValue && !json.isObject()) {
        CCLOG("SigninComp::init_with_json() json object error.");
        return;
    }
    
}