//
//  IAPComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "IAPComp.h"

IAPComp::~IAPComp() {
}

bool IAPComp::init() {

    return true;
}

void IAPComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("IAPComp::init_with_json() json object error.");
        return;
    }
    
}
