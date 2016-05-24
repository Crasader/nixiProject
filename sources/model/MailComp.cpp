//
//  MailComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "MailComp.h"

MailComp::~MailComp() {
}

bool MailComp::init() {

    return true;
}

void MailComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("LoginComp::init_with_json() json object error.");
        return;
    }
    
}