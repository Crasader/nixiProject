//
//  SigninComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "SigninComp.h"
#include "AppUtil.h"
#include "ConfigManager.h"

#pragma mark - Export API

CCArray* SigninComp::signin7_template() {
    return CONFIG->signin7_template();
}

SigninState SigninComp::fetch_signin7_state(int id) {
    CCInteger* state = (CCInteger*)_signin7Info->objectAtIndex(id - 1);
    return (SigninState)state->getValue();
}

#pragma mark - Inner API

SigninComp::~SigninComp() {
    CC_SAFE_DELETE(_signin7Info);
}

bool SigninComp::init() {
    _signin7Info = NULL;

    return true;
}

void SigninComp::parse_signin7_info(Value json) {
    if (json.type() == nullValue) {
        CCLOG("SigninComp::parse_signin7_info() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(_signin7Info);
    _signin7Info = AppUtil::array_with_json(json);
    _signin7Info->retain();
}