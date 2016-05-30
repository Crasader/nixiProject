//
//  Rewards.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "Rewards.h"

Rewards::~Rewards() {
}

bool Rewards::init() {
    coin = 0;
    diam = 0;
    energy = 0;
    return true;
}

void Rewards::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("Rewards::init_with_json() json object error.");
        return;
    }
    
    coin = json["coin"].asInt();
    diam = json["diam"].asInt();
    energy = json["energy"].asInt();
}

void Rewards::print_self() {
    CCLOG("    Rewards:");
    CCLOG("        coin = %d", coin);
    CCLOG("        diam = %d", diam);
    CCLOG("        energy = %d", energy);
}

