//
//  MessageComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "MessageComp.h"

MessageComp::~MessageComp() {
}

bool MessageComp::init() {
    
    return true;
}

void MessageComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("MessageComp::init_with_json() json object error.");
        return;
    }
    
}