//
//  NewsComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "NewsComp.h"

NewsComp::~NewsComp() {
}

bool NewsComp::init() {
    
    return true;
}

void NewsComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("NewsComp::init_with_json() json object error.");
        return;
    }
    
    _otherday = json["O"].asBool();
    _energy = json["E"].asInt();
    _mail = json["I"].asInt();
    _message = json["M"].asInt();
    _paper = json["P"].asInt();
}