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
    
    otherday = json["O"].asBool();
    coin = json["C"].asInt();
    energy = json["E"].asInt();
    mail = json["I"].asInt();
    message = json["M"].asInt();
    paper = json["P"].asInt();
    
    this->print_news();
}

void NewsComp::print_news() {
    CCLOG("====== News ======");
    CCLOG("Is other day: %d", otherday);
    CCLOG("C: %d", energy);
    CCLOG("Energy: %d", energy);
    CCLOG("Mail: %d", mail);
    CCLOG("Message: %d", message);
    CCLOG("Paper: %d", paper);
}