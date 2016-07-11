//
//  Reward.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "Reward.h"

Reward::~Reward() {
}

bool Reward::init() {
    coin = 0;
    diam = 0;
    energy = 0;
    return true;
}

void Reward::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("Reward::init_with_json() json object error.");
        return;
    }
    
    coin = json["coin"].asInt();
    diam = json["diam"].asInt();
    energy = json["energy"].asInt();
}

void Reward::print_self() {
    CCLOG("    Reward:");
    CCLOG("        coin = %d", coin);
    CCLOG("        diam = %d", diam);
    CCLOG("        energy = %d", energy);
}

