//
//  SigninComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "SigninComp.h"
#include "ConfigManager.h"

SigninComp::~SigninComp() {
}

bool SigninComp::init() {

    return true;
}

void SigninComp::parse_signin7_info(Value json) {
    if (json.type() == nullValue) {
        CCLOG("SigninComp::parse_signin7_info() json object error.");
        return;
    }
    
    CCArray* arr = CONFIG->signin7_template();
    CONFIG->test_signin7();
}