//
//  PlayerComp.cpp
//  tiegao
//
//  Created by mac on 16/5/21.
//
//

#include "PlayerComp.h"

PlayerComp::~PlayerComp() {
}

bool PlayerComp::init() {
    
    return true;
}

void PlayerComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("PlayerComp::init_with_json() json object error.");
        return;
    }
    
    update = json["update"].asInt64();
    energy = json["energy"].asInt();
    coin = json["coin"].asInt();
    gold = json["gold"].asInt();
    diam = json["diam"].asInt();
    cur_mission = json["cur_mission"].asInt();
    next_mission = json["next_mission"].asInt();
    phase = json["phase"].asInt();
    rating = json["rating"].asInt();
    left = json["left"].asInt();
    energyBuyTimes = json["eb_times"].asInt();
}

