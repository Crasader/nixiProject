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
    if (json.type() == nullValue) {
        CCLOG("NewsComp::init_with_json() json object error.");
        return;
    }
    
    otherday = json["O"].asBool();
    coin = json["C"].asInt();
    energy1 = json["EL1"].asInt();
    energy2 = json["EL2"].asInt();
    mail = json["I"].asInt();
    message = json["M"].asInt();
    paper = json["P"].asInt();
    purchaseAchievement = json["PA"].asInt();
    signin7 = json["S7"].asInt();
    signin30 = json["S30"].asInt();
    dailyShareCount = json["SC"].asInt();
}

void NewsComp::print_news() {
    CCLOG("====== News ======");
    CCLOG("Is other day: %d", otherday);
    CCLOG("C: %d", coin);
    CCLOG("Energy1: %d", energy1);
    CCLOG("Energy2: %d", energy2);
    CCLOG("Mail: %d", mail);
    CCLOG("Message: %d", message);
    CCLOG("Paper: %d", paper);
    CCLOG("dailyShareCount: %d", dailyShareCount);
}