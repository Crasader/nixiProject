//
//  CoffersComp.cpp
//  tiegao
//
//  Created by mac on 16/7/11.
//
//

#include "CoffersComp.h"

CoffersComp::~CoffersComp() {
}

bool CoffersComp::init() {
    profit = 0;
    top = 0;
    return true;
}

void CoffersComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("CoffersComp::init_with_json() json object error.");
        return;
    }
    
    profit = json["profit"].asInt();
    top = json["top"].asInt();
}
