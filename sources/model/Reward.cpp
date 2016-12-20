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
    
    if (json.isMember("coin")) {
        coin = json["coin"].asInt();
    }
    else {
        coin = 0;
    }
    if (json.isMember("diam")) {
        diam = json["diam"].asInt();
    }
    else {
        diam = 0;
    }
    if (json.isMember("energy")) {
        energy = json["energy"].asInt();
    }
    else {
        energy = 0;
    }
}

void Reward::print_self() {
    CCLOG("    Reward:");
    CCLOG("        coin = %d", coin);
    CCLOG("        diam = %d", diam);
    CCLOG("        energy = %d", energy);
}

